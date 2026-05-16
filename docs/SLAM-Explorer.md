# SLAM Explorer

## What it does

The `multi_robot_explorer` node finds frontiers on the shared `/map` and assigns exploration goals to available robots. It is used only in SLAM mode.

## How it works

The explorer runs two loops: a discovery loop and a planning loop.

Discovery loop:

1. It scans active ROS topics and extracts namespaces matching `robot_namespace_regex`, for example `tb1` or `tb2`.
2. For each discovered namespace, it creates a TF store for that robot and a Nav2 action client for `/<robot>/navigate_to_pose`.
3. The TF store listens to `/tf`, `/tf_static`
4. It tries to resolve each robot pose in the shared map frame using `base_frame`
5. If a robot has a recent pose and its `NavigateToPose` action server is ready, it is marked available. If pose data becomes stale for longer than `robot_stale_timeout_sec`, the robot is made unavailable and any assignment is released.

Planning loop:

1. If the node is paused or no `/map` has arrived yet, it does nothing.
2. It scans the occupancy grid for frontier cells. A frontier cell must be free space, must touch unknown space, and must not touch an occupied cell.
3. Frontier cells are grown into clusters using BFS over 8-neighbor connectivity (all neighbouring cells including diagonals)
4. Clusters smaller than `frontier_min_cluster_size` are ignored.
5. Nearby clusters are merged using `frontier_merge_radius_m`.
6. Frontiers too close to active goals or recently blacklisted goals are filtered out.
7. Idle robots are paired with candidate frontiers. For each robot/frontier pair, the node picks a usable goal cell from that frontier and computes a score.
8. Lower scores are preferred. The score mainly uses distance, then subtracts a size bonus for larger frontiers and applies a home-bias penalty so robots tend to explore areas closer to their own starting region.
9. The global assignment step chooses robot/frontier pairs while preventing two robots from taking the same frontier or goals closer than `min_frontier_separation_m`.
10. Selected goals are sent as `NavigateToPose` goals in the shared `map` frame.

Goal lifecycle:

1. When `/multi_robot_explorer/start` is called, the node unpauses and records each robot's current pose as its home pose when available.
2. If a goal is accepted, the robot stays busy until the action result returns or the assignment times out.
3. Reached frontier goals enter a short cooldown so they are not immediately reused.
4. Failed or rejected goals are blacklisted for `blacklist_timeout_sec`.
5. If no usable frontiers remain, idle robots with a captured home pose are sent home.

The node starts paused by default. Starting exploration captures each robot's current pose as its home pose when available.

## Launch only exploring

Start map publishing and Nav2 action servers first. The explorer needs `/map`, robot TF, and `/<robot>/navigate_to_pose`.

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_navigation multi_robot_explorer.launch.py \
  map_topic:=/map \
  use_sim_time:=false
```

Use `use_sim_time:=true` in Gazebo.

Start it with:

```bash
ros2 service call /multi_robot_explorer/start std_srvs/srv/Trigger {}
```

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/map` | Input | Shared occupancy grid. |
| `/tf`, `/tf_static` | Input | Shared TF tree. |
| `/<robot>/tf`, `/<robot>/tf_static` | Input | Namespaced TF tree. |
| `/<robot>/navigate_to_pose` | Output action | Frontier goal sent to Nav2. |
| `~/markers` | Output | Optional RViz debug markers. |

## Services

| Service | Purpose |
| --- | --- |
| `/multi_robot_explorer/start` | Unpauses exploration and captures home poses. |
| `/multi_robot_explorer/pause` | Stops assigning new goals. |
| `/multi_robot_explorer/stop` | Pauses and clears active assignments. |
| `/multi_robot_explorer/return_home` | Sends idle robots back to captured home poses. |
| `/multi_robot_explorer/status` | Reports map readiness, pause state, and robot states. |

## Key parameters

| Parameter | Default | Purpose |
| --- | --- | --- |
| `map_topic` | `/map` | Occupancy grid to explore. |
| `robot_namespace_regex` | `^tb[0-9]+$` | Discovers robot namespaces. |
| `base_frame` | `base_link` | Preferred base frame. |
| `fallback_base_frame` | `base_footprint` | Backup base frame. |
| `start_paused` | `true` | Prevents automatic goal assignment at startup. |
| `planning_period_sec` | `1.0` | Frontier planning rate. |
| `discovery_period_sec` | `2.0` | Robot discovery rate. |
| `reservation_radius_m` | `1.5` | Keeps robots from selecting nearby goals. |
| `frontier_min_cluster_size` | `8` | Ignores small frontier clusters. |
| `min_frontier_separation_m` | `1.5` | Separates chosen frontiers. |
| `goal_tolerance_m` | `0.75` | Distance threshold for considering a goal reached. |
| `robot_stale_timeout_sec` | `5.0` | Marks robots unavailable when pose data is stale. |
| `assignment_timeout_sec` | `30.0` | Releases slow assignments. |
| `blacklist_timeout_sec` | `20.0` | Avoids recently failed goals. |
| `cancel_goals_on_pause` | `false` | Controls whether pausing cancels active goals. |
| `enable_rviz_markers` | `true` | Publishes debug markers. |

## Assumptions

- Exploration needs Nav2, because it sends `NavigateToPose` goals.
- Robot namespaces match `robot_namespace_regex`.
- A shared `/map` is already being published.
