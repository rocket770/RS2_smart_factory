import numpy as np
from PyQt5.QtWidgets import (
    QWidget,
    QLabel,
    QLineEdit,
    QPushButton,
    QVBoxLayout,
    QHBoxLayout,
    QGridLayout,
    QTextEdit,
    QGroupBox,
    QMessageBox,
)
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt


class Dashboard(QWidget):
    def __init__(self, ros_node):
        super().__init__()
        self.ros_node = ros_node
        self.pending_futures = []

        self.setWindowTitle("Smart Factory Task GUI")
        self.setMinimumSize(900, 700)

        main_layout = QHBoxLayout()

        # --- Left panel: tasks + logs ---
        left_panel = QVBoxLayout()

        title = QLabel("Multi-Robot Task Assignment GUI")
        title.setStyleSheet("font-size: 18px; font-weight: bold;")
        left_panel.addWidget(title)

        left_panel.addWidget(self._build_task_group("Task 1", 1))
        left_panel.addWidget(self._build_task_group("Task 2", 2))

        button_row = QHBoxLayout()
        self.send_button = QPushButton("Send Tasks")
        self.send_button.clicked.connect(self.send_tasks)
        button_row.addWidget(self.send_button)

        self.clear_button = QPushButton("Clear")
        self.clear_button.clicked.connect(self.clear_fields)
        button_row.addWidget(self.clear_button)
        left_panel.addLayout(button_row)

        self.log_box = QTextEdit()
        self.log_box.setReadOnly(True)
        self.log_box.setPlaceholderText("Logs / responses will appear here...")
        left_panel.addWidget(self.log_box)

        # --- Right panel: map display ---
        right_panel = QVBoxLayout()

        map_title = QLabel("Merged Map")
        map_title.setStyleSheet("font-size: 14px; font-weight: bold;")
        map_title.setAlignment(Qt.AlignCenter)
        right_panel.addWidget(map_title)

        self.map_label = QLabel()
        self.map_label.setMinimumSize(400, 400)
        self.map_label.setAlignment(Qt.AlignCenter)
        self.map_label.setStyleSheet("background-color: #222; border: 1px solid #555;")
        self.map_label.setText("Waiting for map...")
        right_panel.addWidget(self.map_label)

        self.coord_label = QLabel("Map coordinates: hover not yet implemented")
        self.coord_label.setAlignment(Qt.AlignCenter)
        right_panel.addWidget(self.coord_label)

        main_layout.addLayout(left_panel, stretch=1)
        main_layout.addLayout(right_panel, stretch=1)

        self.setLayout(main_layout)

    def _build_task_group(self, title, idx):
        group = QGroupBox(title)
        layout = QGridLayout()

        pickup_x = QLineEdit()
        pickup_y = QLineEdit()
        dropoff_x = QLineEdit()
        dropoff_y = QLineEdit()

        setattr(self, f"pickup_x_{idx}", pickup_x)
        setattr(self, f"pickup_y_{idx}", pickup_y)
        setattr(self, f"dropoff_x_{idx}", dropoff_x)
        setattr(self, f"dropoff_y_{idx}", dropoff_y)

        layout.addWidget(QLabel("Pickup X"), 0, 0)
        layout.addWidget(pickup_x, 0, 1)
        layout.addWidget(QLabel("Pickup Y"), 0, 2)
        layout.addWidget(pickup_y, 0, 3)

        layout.addWidget(QLabel("Dropoff X"), 1, 0)
        layout.addWidget(dropoff_x, 1, 1)
        layout.addWidget(QLabel("Dropoff Y"), 1, 2)
        layout.addWidget(dropoff_y, 1, 3)

        group.setLayout(layout)
        return group

    def _read_task(self, idx):
        px_text = getattr(self, f"pickup_x_{idx}").text().strip()
        py_text = getattr(self, f"pickup_y_{idx}").text().strip()
        dx_text = getattr(self, f"dropoff_x_{idx}").text().strip()
        dy_text = getattr(self, f"dropoff_y_{idx}").text().strip()

        all_empty = not any([px_text, py_text, dx_text, dy_text])
        if all_empty:
            return None

        try:
            px = float(px_text)
            py = float(py_text)
            dx = float(dx_text)
            dy = float(dy_text)
            return (px, py), (dx, dy)
        except ValueError:
            raise ValueError(f"Task {idx}: all coordinate fields must be valid numbers.")

    def send_tasks(self):
        sent_any = False

        for idx in [1, 2]:
            try:
                result = self._read_task(idx)
            except ValueError as e:
                QMessageBox.warning(self, "Invalid Input", str(e))
                return

            if result is None:
                self.log(f"Task {idx}: skipped (no coordinates entered).")
                continue

            pickup, dropoff = result
            label = f"Task {idx}"
            self.log(f"Sending {label}...")
            fut = self.ros_node.send_task(pickup, dropoff)
            if fut is not None:
                self.pending_futures.append((label, fut))
                sent_any = True
            else:
                self.log(f"{label}: service unavailable, not sent.")

        if not sent_any:
            QMessageBox.warning(self, "No Tasks", "Please fill in at least one task.")

    def process_futures(self):
        remaining = []
        for name, fut in self.pending_futures:
            if fut.done():
                try:
                    resp = fut.result()
                    if resp is not None:
                        self.log(
                            f"{name}: accepted={resp.accepted}, "
                            f"task_id={resp.task_id}, msg={resp.message}"
                        )
                    else:
                        self.log(f"{name}: no response")
                except Exception as e:
                    self.log(f"{name}: error: {e}")
            else:
                remaining.append((name, fut))
        self.pending_futures = remaining

    def update_map(self):
        map_msg = self.ros_node.latest_map
        if map_msg is None:
            return

        width = map_msg.info.width
        height = map_msg.info.height
        data = np.array(map_msg.data, dtype=np.int8).reshape((height, width))

        img = np.zeros((height, width), dtype=np.uint8)
        img[data == -1] = 128
        img[data == 0] = 255
        img[data == 100] = 0
        img = np.flipud(img)
        img = np.rot90(img, k=1) 

        img = np.ascontiguousarray(img)
        self._current_map_img = img

        new_height, new_width = img.shape
        qimg = QImage(
            self._current_map_img.data,
            new_width,
            new_height,
            new_width,
            QImage.Format_Grayscale8
        )
        
        pixmap = QPixmap.fromImage(qimg)
        self.map_label.setPixmap(
            pixmap.scaled(
                self.map_label.width(),
                self.map_label.height(),
                Qt.KeepAspectRatio,
                Qt.SmoothTransformation,
            )
        )

    def clear_fields(self):
        for idx in [1, 2]:
            getattr(self, f"pickup_x_{idx}").clear()
            getattr(self, f"pickup_y_{idx}").clear()
            getattr(self, f"dropoff_x_{idx}").clear()
            getattr(self, f"dropoff_y_{idx}").clear()
        self.log("Cleared all fields.")

    def log(self, message):
        self.log_box.append(message)
