from typing import Dict, List, Optional, Tuple

import numpy as np
from PyQt5.QtCore import QPointF, QRectF, Qt
from PyQt5.QtGui import QBrush, QColor, QImage, QPainter, QPen, QPixmap
from PyQt5.QtWidgets import QLabel

from .mtsp_models import WorldPoint
from .mtsp_ros_node import MtspPlannerGuiNode


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

    def __init__(self, ros_node: MtspPlannerGuiNode, log_fn, manual_move_callback):
        super().__init__()
        self.ros_node = ros_node
        self.log = log_fn
        self.manual_move_callback = manual_move_callback

        self.setMinimumSize(700, 700)
        self.setAlignment(Qt.AlignCenter)
        self.setStyleSheet("background-color: #222; border: 1px solid #555;")
        self.setText("Waiting for merged /map...")

        self.display_mode = "goal"  # goal | move_robot
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

        img = np.rot90(np.flipud(img), 1)
        img = np.ascontiguousarray(img)
        display_height, display_width = img.shape

        self._base_qimage = QImage(
            img.data,
            display_width,
            display_height,
            display_width,
            QImage.Format_Grayscale8,
        ).copy()
        self.redraw()

    def clear_map(self, message: str = "Waiting for merged /map..."):
        self._base_qimage = None
        self._scaled_pixmap = None
        self._drawn_pixmap = None
        self._last_target_rect = None
        self.clear()
        self.setText(message)

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
                f"Set manual move target for {self.selected_robot_namespace} to ({world[0]:.2f}, {world[1]:.2f})"
            )
            self.manual_move_callback(self.selected_robot_namespace, WorldPoint(*world))

        self.redraw()

    def redraw(self):
        if self._base_qimage is None:
            self.setText("Waiting for merged /map...")
            return

        base = QPixmap.fromImage(self._base_qimage)
        scaled = base.scaled(self.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        canvas = QPixmap(self.size())
        canvas.fill(QColor("#222"))

        painter = QPainter(canvas)
        target_rect = self._centered_target_rect(scaled)
        self._last_target_rect = target_rect
        painter.drawPixmap(int(target_rect.x()), int(target_rect.y()), scaled)

        # Draw user-selected goals
        for idx, point in enumerate(self.goal_points):
            px, py = self._world_to_widget(point.x, point.y)
            if px is None:
                continue
            painter.setPen(QPen(QColor("#00bfff"), 2))
            painter.drawLine(int(px - 6), int(py - 6), int(px + 6), int(py + 6))
            painter.drawLine(int(px - 6), int(py + 6), int(px + 6), int(py - 6))
            painter.drawText(int(px + 8), int(py - 8), f"G{idx}")

        # Draw manual move targets
        for i, ns in enumerate(sorted(self.robot_starts.keys())):
            point = self.robot_starts[ns]
            px, py = self._world_to_widget(point.x, point.y)
            if px is None:
                continue
            color = self.ROBOT_COLORS[i % len(self.ROBOT_COLORS)]
            painter.setPen(QPen(color, 2))
            painter.setBrush(QBrush(color))
            painter.drawEllipse(QPointF(px, py), 7, 7)
            painter.drawText(int(px + 8), int(py - 8), f"{ns} target")

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
        self.setText("")
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
        rotated_x = rel_x * height
        rotated_y = rel_y * width
        pixel_x = width - rotated_y
        pixel_y = height - rotated_x

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

        rotated_x = height - pixel_y
        rotated_y = width - pixel_x

        rel_x = rotated_x / height
        rel_y = rotated_y / width

        px = target.x() + rel_x * target.width()
        py = target.y() + rel_y * target.height()
        return px, py
