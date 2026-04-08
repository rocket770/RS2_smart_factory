#!/usr/bin/env python3
import json
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

import numpy as np
import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data

from std_msgs.msg import String
from nav_msgs.msg import OccupancyGrid
from geometry_msgs.msg import PoseStamped, PoseWithCovarianceStamped, TransformStamped
from tf2_msgs.msg import TFMessage

from PyQt5.QtCore import Qt, QTimer, QRectF, QPointF
from PyQt5.QtGui import QColor, QPen, QBrush, QPainter, QImage, QPixmap
from PyQt5.QtWidgets import (
    QApplication,
    QDoubleSpinBox,
    QFormLayout,
    QGridLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QListWidget,
    QListWidgetItem,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QSpinBox,
    QSplitter,
    QTextEdit,
    QVBoxLayout,
    QWidget,
    QComboBox,
)


# -----------------------------
# Data models
# -----------------------------

@dataclass
class WorldPoint:
    x: float
    y: float


@dataclass
class MtspResult:
    robot_starts: List[Tuple[float, float]]
    goals: List[Tuple[float, float]]
    routes: List[List[int]]
    generation: int
    cost: float


# -----------------------------
# ROS node
# -----------------------------

class MtspPlannerGuiNode(Node):
    def __init__(self):
        super().__init__("mtsp_planner_gui")

        self.latest_map: Optional[OccupancyGrid] = None
        self.latest_result: Optional[MtspResult] = None
        self.robot_positions: Dict[str, Tuple[float, float]] = {}

        self.robot_namespaces = ["tb1", "tb2", "tb3", "tb4"]

        self.create_subscription(OccupancyGrid, "/map", self._on_map, 10)
        self.create_subscription(String, "mtsp_best_solution", self._on_result, 10)
        self.create_subscription(TFMessage, "/tf", self._on_tf, qos_profile_sensor_data)
        self.create_subscription(TFMessage, "/tf_static", self._on_tf, qos_profile_sensor_data)

        # If each robot really publishes its own namespaced tf topics, subscribe to those too.
        self.tf_ns_subs = []
        for ns in self.robot_namespaces:
            self.tf_ns_subs.append(
                self.create_subscription(
                    TFMessage,
                    f"/{ns}/tf",
                    self._on_tf,
                    qos_profile_sensor_data,
                )
            )
            self.tf_ns_subs.append(
                self.create_subscription(
                    TFMessage,
                    f"/{ns}/tf_static",
                    self._on_tf,
                    qos_profile_sensor_data,
                )
            )

    def _on_map(self, msg: OccupancyGrid):
        self.latest_map = msg

    def _on_result(self, msg: String):
        try:
            data = json.loads(msg.data)
            self.latest_result = MtspResult(
                robot_starts=[tuple(p) for p in data["robot_starts"]],
                goals=[tuple(p) for p in data["goals"]],
                routes=[list(r) for r in data["routes"]],
                generation=int(data["generation"]),
                cost=float(data["cost"]),
            )
        except Exception as exc:
            self.get_logger().error(f"Failed to parse MTSP result: {exc}")

    def _on_tf(self, msg: TFMessage):
        for t in msg.transforms:
            robot_name = self._robot_name_from_transform(t)
            if robot_name is None:
                continue
            self.robot_positions[robot_name] = (
                float(t.transform.translation.x),
                float(t.transform.translation.y),
            )

    def _robot_name_from_transform(self, t: TransformStamped) -> Optional[str]:
        child = t.child_frame_id.lstrip("/")
        frame = t.header.frame_id.lstrip("/")

        # Expected examples:
        # tb1/base_link, tb1/base_footprint, base_link (on /tb1/tf topic)
        for ns in self.robot_namespaces:
            if child.startswith(f"{ns}/") and child.endswith(("base_link", "base_footprint")):
                return ns
            if frame.startswith(f"{ns}/") and child.endswith(("base_link", "base_footprint")):
                return ns

        # If message arrived on namespaced topic, child might be just base_link.
        if child in ("base_link", "base_footprint"):
            # Ambiguous on shared /tf, so do not overwrite unless only one possible robot.
            # Keep this conservative.
            return None

        return None


# -----------------------------
# Map canvas
# -----------------------------

class MapCanvas(QLabel):
    ROBOT_COLORS = [
        QColor("#ff7f0e"),
        QColor("#2ca02c"),
        QColor("#d62728"),
        QColor("#9467bd"),
        QColor("#8c564b"),
        QColor("#1f77b4"),
        QColor("#e377c2"),
        QColor("#7f7f7f"),
    ]

    def __init__(self, ros_node: MtspPlannerGuiNode, log_fn):
        super().__init__()
        self.ros_node = ros_node
        self.log = log_fn

        self.setMinimumSize(700, 700)
        self.setAlignment(Qt.AlignCenter)
        self.setStyleSheet("background-color: #222; border: 1px solid #555;")
        self.setText("Waiting for merged /map...")

        self.display_mode = "goal"  # goal | robot_start
        self.selected_robot_namespace = "tb1"

        self.goal_points: List[WorldPoint] = []
        self.robot_starts: Dict[str, WorldPoint] = {}

        self._base_qimage: Optional[QImage] = None
        self._scaled_pixmap: Optional[QPixmap] = None
        self._drawn_pixmap: Optional[QPixmap] = None
        self._last_target_rect: Optional[QRectF] = None

    def set_display_mode(self, mode: str):
        self.display_mode = mode

    def set_selected_robot_namespace(self, ns: str):
        self.selected_robot_namespace = ns

    def clear_goals(self):
        self.goal_points.clear()
        self.redraw()

    def clear_robot_starts(self):
        self.robot_starts.clear()
        self.redraw()

    def refresh_map(self):
        map_msg = self.ros_node.latest_map
        if map_msg is None:
            return

        width = map_msg.info.width
        height = map_msg.info.height
        data = np.array(map_msg.data, dtype=np.int8).reshape((height, width))

        img = np.zeros((height, width), dtype=np.uint8)
        img[data == -1] = 128
        img[data == 0] = 255
        img[data > 50] = 0

        img = np.flipud(img)
        img = np.ascontiguousarray(img)

        self._base_qimage = QImage(
            img.data,
            width,
            height,
            width,
            QImage.Format_Grayscale8,
        ).copy()
        self.redraw()

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.redraw()

    def mousePressEvent(self, event):
        if event.button() != Qt.LeftButton:
            return

        world = self._widget_click_to_world(event.pos())
        if world is None:
            return

        if self.display_mode == "goal":
            self.goal_points.append(WorldPoint(*world))
            self.log(f"Added goal at ({world[0]:.2f}, {world[1]:.2f})")
        else:
            self.robot_starts[self.selected_robot_namespace] = WorldPoint(*world)
            self.log(
                f"Set {self.selected_robot_namespace} start to ({world[0]:.2f}, {world[1]:.2f})"
            )

        self.redraw()

    def redraw(self):
        if self._base_qimage is None:
            return

        base = QPixmap.fromImage(self._base_qimage)
        scaled = base.scaled(self.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        canvas = QPixmap(scaled)
        canvas.fill(Qt.transparent)

        painter = QPainter(canvas)
        painter.drawPixmap(0, 0, scaled)
        target_rect = self._centered_target_rect(scaled)
        self._last_target_rect = target_rect

        # Draw user-selected goals
        for idx, point in enumerate(self.goal_points):
            px, py = self._world_to_widget(point.x, point.y)
            if px is None:
                continue
            painter.setPen(QPen(QColor("#00bfff"), 2))
            painter.drawLine(int(px - 6), int(py - 6), int(px + 6), int(py + 6))
            painter.drawLine(int(px - 6), int(py + 6), int(px + 6), int(py - 6))
            painter.drawText(int(px + 8), int(py - 8), f"G{idx}")

        # Draw selected robot start points
        for i, ns in enumerate(sorted(self.robot_starts.keys())):
            point = self.robot_starts[ns]
            px, py = self._world_to_widget(point.x, point.y)
            if px is None:
                continue
            color = self.ROBOT_COLORS[i % len(self.ROBOT_COLORS)]
            painter.setPen(QPen(color, 2))
            painter.setBrush(QBrush(color))
            painter.drawEllipse(QPointF(px, py), 7, 7)
            painter.drawText(int(px + 8), int(py - 8), f"{ns} start")

        # Draw live robot positions from TF
        ns_list = self.ros_node.robot_namespaces
        for i, ns in enumerate(ns_list):
            pos = self.ros_node.robot_positions.get(ns)
            if pos is None:
                continue
            px, py = self._world_to_widget(pos[0], pos[1])
            if px is None:
                continue
            color = self.ROBOT_COLORS[i % len(self.ROBOT_COLORS)]
            painter.setPen(QPen(color, 2))
            painter.setBrush(Qt.NoBrush)
            painter.drawEllipse(QPointF(px, py), 10, 10)
            painter.drawText(int(px + 10), int(py + 12), ns)

        # Draw last MTSP result if present
        result = self.ros_node.latest_result
        if result is not None:
            for robot_idx, route in enumerate(result.routes):
                color = self.ROBOT_COLORS[robot_idx % len(self.ROBOT_COLORS)]
                painter.setPen(QPen(color, 2))

                if robot_idx >= len(result.robot_starts):
                    continue

                sx, sy = result.robot_starts[robot_idx]
                prev = (sx, sy)
                for order_idx, goal_idx in enumerate(route):
                    if goal_idx >= len(result.goals):
                        continue
                    gx, gy = result.goals[goal_idx]
                    p1 = self._world_to_widget(prev[0], prev[1])
                    p2 = self._world_to_widget(gx, gy)
                    if p1[0] is not None and p2[0] is not None:
                        painter.drawLine(int(p1[0]), int(p1[1]), int(p2[0]), int(p2[1]))
                        painter.drawText(int(p2[0] + 8), int(p2[1] + 12), str(order_idx))
                    prev = (gx, gy)

        painter.end()
        self.setPixmap(canvas)
        self._drawn_pixmap = canvas

    def _centered_target_rect(self, scaled_pixmap: QPixmap) -> QRectF:
        x = (self.width() - scaled_pixmap.width()) / 2.0
        y = (self.height() - scaled_pixmap.height()) / 2.0
        return QRectF(x, y, scaled_pixmap.width(), scaled_pixmap.height())

    def _widget_click_to_world(self, pos) -> Optional[Tuple[float, float]]:
        map_msg = self.ros_node.latest_map
        if map_msg is None or self._base_qimage is None:
            return None

        target = self._centered_target_rect(
            QPixmap.fromImage(self._base_qimage).scaled(self.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        )
        if not target.contains(pos):
            return None

        rel_x = (pos.x() - target.x()) / target.width()
        rel_y = (pos.y() - target.y()) / target.height()

        width = map_msg.info.width
        height = map_msg.info.height
        pixel_x = rel_x * width
        pixel_y_flipped = rel_y * height
        pixel_y = height - pixel_y_flipped

        origin_x = map_msg.info.origin.position.x
        origin_y = map_msg.info.origin.position.y
        resolution = map_msg.info.resolution

        world_x = origin_x + pixel_x * resolution
        world_y = origin_y + pixel_y * resolution
        return world_x, world_y

    def _world_to_widget(self, world_x: float, world_y: float) -> Tuple[Optional[float], Optional[float]]:
        map_msg = self.ros_node.latest_map
        if map_msg is None or self._base_qimage is None:
            return None, None

        scaled = QPixmap.fromImage(self._base_qimage).scaled(self.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        target = self._centered_target_rect(scaled)

        origin_x = map_msg.info.origin.position.x
        origin_y = map_msg.info.origin.position.y
        resolution = map_msg.info.resolution
        width = map_msg.info.width
        height = map_msg.info.height

        pixel_x = (world_x - origin_x) / resolution
        pixel_y = (world_y - origin_y) / resolution
        pixel_y_flipped = height - pixel_y

        rel_x = pixel_x / width
        rel_y = pixel_y_flipped / height

        px = target.x() + rel_x * target.width()
        py = target.y() + rel_y * target.height()
        return px, py


# -----------------------------
# Main window
# -----------------------------

class MainWindow(QMainWindow):
    def __init__(self, ros_node: MtspPlannerGuiNode):
        super().__init__()
        self.ros_node = ros_node

        self.setWindowTitle("MTSP Planner GUI")
        self.resize(1400, 900)

        root = QWidget()
        self.setCentralWidget(root)
        root_layout = QHBoxLayout(root)

        splitter = QSplitter(Qt.Horizontal)
        root_layout.addWidget(splitter)
        # Create map canvas first because left-side controls connect to it
        self.map_canvas = MapCanvas(self.ros_node, self.log)

        # Left panel
        left = QWidget()
        left_layout = QVBoxLayout(left)

        title = QLabel("Multi-Robot MTSP Planner")
        title.setStyleSheet("font-size: 18px; font-weight: bold;")
        left_layout.addWidget(title)

        left_layout.addWidget(self._build_mode_group())
        left_layout.addWidget(self._build_robot_start_group())
        left_layout.addWidget(self._build_hyperparam_group())
        left_layout.addWidget(self._build_actions_group())
        left_layout.addWidget(self._build_summary_group())

        self.log_box = QTextEdit()
        self.log_box.setReadOnly(True)
        self.log_box.setPlaceholderText("Logs / actions / run status...")
        left_layout.addWidget(self.log_box, stretch=1)

        # Right panel
        right = QWidget()
        right_layout = QVBoxLayout(right)
        right_layout.addWidget(self.map_canvas, stretch=1)

        # Right panel
        right = QWidget()
        right_layout = QVBoxLayout(right)
        self.map_canvas = MapCanvas(self.ros_node, self.log)
        right_layout.addWidget(self.map_canvas, stretch=1)

        self.result_label = QLabel("No MTSP result yet.")
        self.result_label.setAlignment(Qt.AlignCenter)
        right_layout.addWidget(self.result_label)

        splitter.addWidget(left)
        splitter.addWidget(right)
        splitter.setSizes([420, 980])

        self._qt_timer = QTimer(self)
        self._qt_timer.timeout.connect(self._on_timer)
        self._qt_timer.start(100)

    def _build_mode_group(self):
        group = QGroupBox("Map Interaction")
        layout = QVBoxLayout(group)

        row = QHBoxLayout()
        self.mode_combo = QComboBox()
        self.mode_combo.addItems(["goal", "robot_start"])
        self.mode_combo.currentTextChanged.connect(self.map_canvas.set_display_mode)
        row.addWidget(QLabel("Click mode:"))
        row.addWidget(self.mode_combo)

        self.robot_ns_combo = QComboBox()
        self.robot_ns_combo.addItems(self.ros_node.robot_namespaces)
        self.robot_ns_combo.currentTextChanged.connect(self.map_canvas.set_selected_robot_namespace)
        row.addWidget(QLabel("Robot:"))
        row.addWidget(self.robot_ns_combo)
        layout.addLayout(row)

        buttons = QHBoxLayout()
        clear_goals_btn = QPushButton("Clear Goals")
        clear_goals_btn.clicked.connect(self.map_canvas.clear_goals)
        buttons.addWidget(clear_goals_btn)

        clear_starts_btn = QPushButton("Clear Robot Starts")
        clear_starts_btn.clicked.connect(self.map_canvas.clear_robot_starts)
        buttons.addWidget(clear_starts_btn)
        layout.addLayout(buttons)

        return group

    def _build_robot_start_group(self):
        group = QGroupBox("Selected Points")
        layout = QVBoxLayout(group)

        self.goal_list = QListWidget()
        self.goal_list.setMinimumHeight(130)
        layout.addWidget(QLabel("Goals"))
        layout.addWidget(self.goal_list)

        self.start_list = QListWidget()
        self.start_list.setMinimumHeight(120)
        layout.addWidget(QLabel("Robot starts"))
        layout.addWidget(self.start_list)

        return group

    def _build_hyperparam_group(self):
        group = QGroupBox("MTSP Hyperparameters")
        form = QFormLayout(group)

        self.population_size = QSpinBox()
        self.population_size.setRange(2, 100000)
        self.population_size.setValue(500)

        self.generations = QSpinBox()
        self.generations.setRange(1, 100000)
        self.generations.setValue(1500)

        self.mutation_rate = QDoubleSpinBox()
        self.mutation_rate.setRange(0.0, 1.0)
        self.mutation_rate.setSingleStep(0.01)
        self.mutation_rate.setDecimals(3)
        self.mutation_rate.setValue(0.15)

        self.seed = QSpinBox()
        self.seed.setRange(0, 2_147_483_647)
        self.seed.setValue(42)

        self.generation_delay_ms = QSpinBox()
        self.generation_delay_ms.setRange(0, 10000)
        self.generation_delay_ms.setValue(0)

        form.addRow("Population size", self.population_size)
        form.addRow("Generations", self.generations)
        form.addRow("Mutation rate", self.mutation_rate)
        form.addRow("Seed", self.seed)
        form.addRow("Generation delay (ms)", self.generation_delay_ms)

        return group

    def _build_actions_group(self):
        group = QGroupBox("Actions")
        layout = QGridLayout(group)

        self.run_btn = QPushButton("Run MTSP")
        self.run_btn.clicked.connect(self.on_run_clicked)
        layout.addWidget(self.run_btn, 0, 0)

        self.start_btn = QPushButton("Start Execution")
        self.start_btn.clicked.connect(self.on_start_clicked)
        layout.addWidget(self.start_btn, 0, 1)

        self.export_btn = QPushButton("Dump Run Config")
        self.export_btn.clicked.connect(self.on_dump_clicked)
        layout.addWidget(self.export_btn, 1, 0)

        self.refresh_btn = QPushButton("Refresh Map")
        self.refresh_btn.clicked.connect(self.map_canvas.refresh_map)
        layout.addWidget(self.refresh_btn, 1, 1)

        return group

    def _build_summary_group(self):
        group = QGroupBox("Run Summary")
        layout = QVBoxLayout(group)
        self.summary_box = QTextEdit()
        self.summary_box.setReadOnly(True)
        self.summary_box.setMaximumHeight(180)
        layout.addWidget(self.summary_box)
        return group

    def _on_timer(self):
        rclpy.spin_once(self.ros_node, timeout_sec=0.0)
        self.map_canvas.refresh_map()
        self.map_canvas.redraw()
        self._refresh_lists()
        self._refresh_result_summary()

    def _refresh_lists(self):
        self.goal_list.clear()
        for i, p in enumerate(self.map_canvas.goal_points):
            self.goal_list.addItem(QListWidgetItem(f"G{i}: ({p.x:.2f}, {p.y:.2f})"))

        self.start_list.clear()
        for ns in sorted(self.map_canvas.robot_starts.keys()):
            p = self.map_canvas.robot_starts[ns]
            self.start_list.addItem(QListWidgetItem(f"{ns}: ({p.x:.2f}, {p.y:.2f})"))

    def _refresh_result_summary(self):
        result = self.ros_node.latest_result
        if result is None:
            return

        self.result_label.setText(
            f"Generation {result.generation} | Best Cost = {result.cost:.3f}"
        )

        lines = [
            f"generation: {result.generation}",
            f"cost: {result.cost:.3f}",
            f"robots: {len(result.robot_starts)}",
            f"goals: {len(result.goals)}",
            "",
        ]
        for i, route in enumerate(result.routes):
            lines.append(f"robot {i}: {route}")
        self.summary_box.setPlainText("\n".join(lines))

    def on_run_clicked(self):
        if not self.map_canvas.goal_points:
            QMessageBox.warning(self, "No Goals", "Add at least one goal on the map.")
            return

        if not self.map_canvas.robot_starts:
            QMessageBox.warning(self, "No Robot Starts", "Set at least one robot start on the map.")
            return

        config = self.build_run_config()
        self.summary_box.setPlainText(json.dumps(config, indent=2))
        self.log("Run requested.")
        self.log("Placeholder: this is where you would call the solver service/action/process.")

        # Suggested integration options:
        # 1. Call a dedicated ROS2 service on the MTSP solver node.
        # 2. Publish a request message to a planner topic.
        # 3. Spawn the existing solver executable with a temp YAML file.
        # 4. Refactor the solver into a library and invoke directly in-process.

    def on_start_clicked(self):
        self.log("Start requested.")
        self.log("Placeholder: publish per-robot ordered goals one by one here.")

    def on_dump_clicked(self):
        config = self.build_run_config()
        self.summary_box.setPlainText(json.dumps(config, indent=2))
        self.log("Dumped current run config to summary pane.")

    def build_run_config(self) -> dict:
        robot_names = sorted(self.map_canvas.robot_starts.keys())
        robot_starts_flat = []
        for ns in robot_names:
            p = self.map_canvas.robot_starts[ns]
            robot_starts_flat.extend([round(p.x, 4), round(p.y, 4)])

        goals_flat = []
        for p in self.map_canvas.goal_points:
            goals_flat.extend([round(p.x, 4), round(p.y, 4)])

        return {
            "mtsp_solver_node": {
                "ros__parameters": {
                    "robot_namespaces": robot_names,
                    "robot_starts": robot_starts_flat,
                    "goals": goals_flat,
                    "population_size": self.population_size.value(),
                    "generations": self.generations.value(),
                    "mutation_rate": self.mutation_rate.value(),
                    "seed": self.seed.value(),
                    "publish_progress": True,
                    "generation_delay_ms": self.generation_delay_ms.value(),
                }
            }
        }

    def log(self, message: str):
        self.log_box.append(message)


# -----------------------------
# Entrypoint
# -----------------------------

def main():
    rclpy.init()
    ros_node = MtspPlannerGuiNode()

    app = QApplication([])
    window = MainWindow(ros_node)
    window.show()

    try:
        app.exec_()
    finally:
        ros_node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
