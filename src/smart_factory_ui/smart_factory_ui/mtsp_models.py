from dataclasses import dataclass
from typing import List, Tuple


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
