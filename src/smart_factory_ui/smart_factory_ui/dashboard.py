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
    QSpinBox,
)


class Dashboard(QWidget):
    def __init__(self, ros_node):
        super().__init__()
        self.ros_node = ros_node
        self.pending_futures = []

        self.setWindowTitle("Smart Factory Task GUI")
        self.setMinimumSize(720, 500)

        main_layout = QVBoxLayout()

        title = QLabel("Multi-Robot Task Assignment GUI")
        title.setStyleSheet("font-size: 18px; font-weight: bold;")
        main_layout.addWidget(title)

        main_layout.addWidget(self._build_task_group("Task 1", 1))
        main_layout.addWidget(self._build_task_group("Task 2", 2))

        button_row = QHBoxLayout()

        self.send_button = QPushButton("Send Both Tasks")
        self.send_button.clicked.connect(self.send_both_tasks)
        button_row.addWidget(self.send_button)

        self.clear_button = QPushButton("Clear")
        self.clear_button.clicked.connect(self.clear_fields)
        button_row.addWidget(self.clear_button)

        main_layout.addLayout(button_row)

        self.log_box = QTextEdit()
        self.log_box.setReadOnly(True)
        self.log_box.setPlaceholderText("Logs / responses will appear here...")
        main_layout.addWidget(self.log_box)

        self.setLayout(main_layout)

    def _build_task_group(self, title, idx):
        group = QGroupBox(title)
        layout = QGridLayout()

        pickup_x = QLineEdit()
        pickup_y = QLineEdit()
        dropoff_x = QLineEdit()
        dropoff_y = QLineEdit()
        priority = QSpinBox()
        priority.setMinimum(0)
        priority.setMaximum(100)
        priority.setValue(0)

        setattr(self, f"pickup_x_{idx}", pickup_x)
        setattr(self, f"pickup_y_{idx}", pickup_y)
        setattr(self, f"dropoff_x_{idx}", dropoff_x)
        setattr(self, f"dropoff_y_{idx}", dropoff_y)
        setattr(self, f"priority_{idx}", priority)

        layout.addWidget(QLabel("Pickup X"), 0, 0)
        layout.addWidget(pickup_x, 0, 1)
        layout.addWidget(QLabel("Pickup Y"), 0, 2)
        layout.addWidget(pickup_y, 0, 3)

        layout.addWidget(QLabel("Dropoff X"), 1, 0)
        layout.addWidget(dropoff_x, 1, 1)
        layout.addWidget(QLabel("Dropoff Y"), 1, 2)
        layout.addWidget(dropoff_y, 1, 3)

        layout.addWidget(QLabel("Priority"), 2, 0)
        layout.addWidget(priority, 2, 1)

        group.setLayout(layout)
        return group

    def _read_task(self, idx):
        try:
            px = float(getattr(self, f"pickup_x_{idx}").text())
            py = float(getattr(self, f"pickup_y_{idx}").text())
            dx = float(getattr(self, f"dropoff_x_{idx}").text())
            dy = float(getattr(self, f"dropoff_y_{idx}").text())
            pr = int(getattr(self, f"priority_{idx}").value())
            return (px, py), (dx, dy), pr
        except ValueError:
            raise ValueError(f"Task {idx}: all coordinate fields must be valid numbers.")

    def send_both_tasks(self):
        try:
            pickup1, dropoff1, pr1 = self._read_task(1)
            pickup2, dropoff2, pr2 = self._read_task(2)
        except ValueError as e:
            QMessageBox.warning(self, "Invalid Input", str(e))
            return

        for label, pickup, dropoff, pr in [
            ("Task 1", pickup1, dropoff1, pr1),
            ("Task 2", pickup2, dropoff2, pr2),
        ]:
            self.log(f"Sending {label}...")
            fut = self.ros_node.send_task(pickup, dropoff, priority=pr)
            if fut is not None:
                self.pending_futures.append((label, fut))
            else:
                self.log(f"{label}: service unavailable, not sent.")

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

    def clear_fields(self):
        for idx in [1, 2]:
            getattr(self, f"pickup_x_{idx}").clear()
            getattr(self, f"pickup_y_{idx}").clear()
            getattr(self, f"dropoff_x_{idx}").clear()
            getattr(self, f"dropoff_y_{idx}").clear()
            getattr(self, f"priority_{idx}").setValue(0)
        self.log("Cleared all fields.")

    def log(self, message):
        self.log_box.append(message)