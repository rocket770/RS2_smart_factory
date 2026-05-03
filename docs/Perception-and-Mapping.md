# Perception and Mapping

## Purpose

The perception and mapping subsystem builds the shared factory map used by the rest of the system. In SLAM mode it runs one SLAM Toolbox instance per robot, merges the per-robot maps into `/map`, and uses frontier exploration to expand the map. In AMCL mode it runs one AMCL instance per robot so each namespace localizes independently against the same shared map.

This subsystem does not own task planning, the GUI, MTSP, or low-level motion control. It provides map data, localization transforms, map-saving services, and exploration-control services that those layers use.

## Packages and nodes

### `smart_factory_bringup`

| File / node | Used in | Role |
| --- | --- | --- |
| `launch/multi_robot_nav2_bringup.launch.py` | SLAM and AMCL | Top-level mapping/localization bringup. Reads `params/general_settings.yaml`, launches per-robot Nav2, and switches between SLAM and AMCL with `use_slam`. |
| `launch/multi_robot_slam_toolbox.launch.py` | SLAM | Starts one `slam_toolbox` node per robot namespace and starts `map_saver`. |
| `launch/multi_robot_gazebo_bringup.launch.py` | Simulation | Spawns the robots listed in `general_settings.yaml` into Gazebo at their configured poses. |
| `launch/namespaced_robot.launch.py` | Real robots | Starts the real TurtleBot3 driver, lidar driver, robot state publisher, and a static `map -> <namespace>/map` anchor using `x_pose`, `y_pose`, and `z_pose`. |
| `params/general_settings.yaml` | SLAM and AMCL | Defines robot names and start poses. These names must match launch namespaces such as `tb1` and `tb2`. |
| `params/slam_toolbox_dynamic_overlay.yaml` | SLAM | Overrides SLAM Toolbox parameters for the real TurtleBot3/LDS-01 setup. |
| `params/nav2_slam_params.yaml` | AMCL and Nav2 support | Contains AMCL, map server, map saver, costmap, scan, and frame settings. |
| `map/factory_merged.yaml` | AMCL default | Default saved map used when an explicit `map:=...` is not provided. |

### `slam_toolbox`

| Node | Inputs | Outputs | Notes |
| --- | --- | --- | --- |
| `/<robot>/slam_toolbox` | `/<robot>/scan`, TF from `<robot>/odom` to `<robot>/base_footprint` | `/<robot>/map`, `/<robot>/map_metadata`, `/<robot>/map_updates`, SLAM TF | Launched once per robot only when `use_slam:=true`. The current launch uses best-effort QoS overrides for scan and per-robot map topics. |

Important settings come from `slam_toolbox_dynamic_overlay.yaml`: `map_update_interval`, `resolution`, laser range limits, scan matching, loop closing, and occupancy thresholds.

### `merge_map`

| File / node | Inputs | Outputs | Role |
| --- | --- | --- | --- |
| `launch/merge_map_launch.py` | `use_sim_time` launch argument | `merge_map` node | Starts the map-merging node in SLAM mode. |
| `merge_map/merge_map.py` | OccupancyGrid topics matching `^/tb\d+/map$` | `/map` | Discovers per-robot map topics, merges them into one shared map, and republishes with transient-local QoS. This also works for a single robot; the single input map is republished as the shared `/map`. |
| `config/map_merge_params.yaml` | Node parameters | n/a | Configures topic regex, output topic, QoS reliability, merged frame, and merge conflict behavior. |

Current configured merge behavior:

| Parameter | Current value | Meaning |
| --- | --- | --- |
| `map_topic_regex` | `^/tb\d+/map$` | Merge maps from robot namespaces like `/tb1/map` and `/tb2/map`. |
| `publish_topic` | `/map` | Publish the shared merged map. |
| `input_reliability` | `best_effort` | Match SLAM map publishers. |
| `output_reliability` | `reliable` | Publish the merged map reliably for consumers. |
| `merged_frame_id` | `map` | The merged map frame is the shared `map` frame. |
| `conflict_policy` | `prefer_free` | If maps disagree, prefer free space over occupied space. |
| `overwrite_known_cells` | `false` | Do not blindly overwrite known cells. |

The node also supports manual `map_offsets.<topic>` parameters in code, but the current config does not set custom offsets.

How map merging works:

1. The node periodically scans the ROS graph for `nav_msgs/msg/OccupancyGrid` topics matching `map_topic_regex`.
2. When it finds a matching topic, such as `/tb1/map`, it subscribes and stores the latest map from that robot.
3. Each input map is transformed by its configured offset `[x, y, yaw]`. With the current config, those offsets default to zero.
4. The node computes the world-space bounds of every input map and creates one larger output grid that can contain them all.
5. It copies each input map cell into the output grid using cell centers and the selected resolution.
6. If two maps write to the same output cell, `resolve_cell()` chooses the final value using `conflict_policy`, `occupied_threshold`, and `unknown_value`.
7. The merged map is published on `/map` with frame id `map`.

The conflict handling is intentionally conservative. Unknown cells do not overwrite known cells. Occupied-vs-free conflicts are resolved by the configured `conflict_policy`; the current config uses `prefer_free`, which helps clear stale obstacles when a robot later observes free space. If only one robot map is available, the same pipeline still runs and produces `/map` from that single input.

### `nav2_map_server` and `nav2_amcl`

| Node | Used in | Inputs | Outputs |
| --- | --- | --- | --- |
| `/map_server` | AMCL mode | `map:=...` YAML file | `/map`, `/map_metadata` |
| `/<robot>/amcl` | AMCL mode | `/map`, `/<robot>/scan`, initial pose | Localization TF with `tf_broadcast: true` |
| `/map_saver` | SLAM mode | `/map` | `/map_saver/save_map` service |

AMCL is launched from `nav2_bringup/localization_launch.py` only when `use_slam:=false`. The launch file rewrites placeholders in `nav2_slam_params.yaml` so each robot gets:

- `base_frame_id: <namespace>/base_footprint`
- `odom_frame_id: <namespace>/odom`
- `scan_topic: /<namespace>/scan`
- `initial_pose.x`, `initial_pose.y`, and `initial_pose.yaw` from `general_settings.yaml`

### `smart_factory_navigation`

| File / node | Inputs | Outputs / services | Role |
| --- | --- | --- | --- |
| `launch/multi_robot_explorer.launch.py` | Launch parameters | `multi_robot_explorer` node | Starts frontier exploration in SLAM mode. |
| `multi_robot_explorer.py` | `/map`, `/tf`, `/tf_static`, `/<robot>/tf`, `/<robot>/tf_static` | `/multi_robot_explorer/start`, `/pause`, `/stop`, `/return_home`, `/status`; optional `~/markers` | Detects frontiers on the merged map and assigns exploration goals to discovered robots. |

The explorer discovers robot namespaces from ROS topics using `robot_namespace_regex`, default `^tb[0-9]+$`. It starts paused by default so the GUI can decide when exploration begins.

The explorer sends goals to `/<robot>/navigate_to_pose`. This is why Nav2 is required even when documenting perception and mapping: the explorer decides which frontier should be explored, but Nav2 provides the action server that actually plans and drives the robot to that frontier.

How frontier exploration works:

1. The explorer subscribes to the shared `/map`.
2. It discovers robot namespaces from active ROS topics, then listens to shared and namespaced TF topics to estimate each robot's current pose in the map frame.
3. It scans the occupancy grid for frontier cells. A frontier cell is free space next to unknown space, without an occupied neighbor.
4. Nearby frontier cells are grouped into clusters. Small clusters are ignored using `frontier_min_cluster_size`.
5. Nearby clusters can be merged using `frontier_merge_radius_m`.
6. The explorer filters out frontiers that are already reserved, recently failed, or too close to another active goal.
7. For each idle robot, it scores candidate frontiers using distance, frontier size, and a home-pose bias.
8. Selected goals are sent to the robot's `/<robot>/navigate_to_pose` action server in the shared `map` frame.

The explorer exposes simple services for the GUI or terminal:

| Service | Purpose |
| --- | --- |
| `/multi_robot_explorer/start` | Starts assigning frontier goals and captures each robot's current pose as its home pose when available. |
| `/multi_robot_explorer/pause` | Stops assigning new goals. Existing goals continue unless `cancel_goals_on_pause` is enabled. |
| `/multi_robot_explorer/stop` | Pauses exploration and clears active assignments. |
| `/multi_robot_explorer/return_home` | Pauses exploration, clears active assignments, and asks idle robots to return to their captured home poses. |
| `/multi_robot_explorer/status` | Reports whether exploration is paused, whether a map is available, and each discovered robot's state. |

## Inputs and outputs

| Mode | Inputs | Outputs |
| --- | --- | --- |
| SLAM | `/<robot>/scan`, `/<robot>/odom`, robot TF, robot namespaces and poses from `general_settings.yaml` | `/<robot>/map`, merged `/map`, `/map_saver/save_map`, explorer services, optional explorer RViz markers |
| AMCL | Existing map YAML, `/<robot>/scan`, `/<robot>/odom`, initial poses from `general_settings.yaml` | `/map`, per-robot AMCL localization TF, localized robot poses for downstream navigation |

## How the pieces fit together

### SLAM mode

1. Each robot must publish namespaced scan, odometry, and TF data.
2. `multi_robot_slam_toolbox.launch.py` starts one SLAM Toolbox node per robot.
3. Each SLAM Toolbox node publishes a local map such as `/tb1/map`.
4. `merge_map` discovers map topics matching `^/tb\d+/map$` and publishes the shared map on `/map`. With multiple robots this is a merged map; with one robot it is still the shared map output.
5. `multi_robot_explorer` reads `/map`, finds frontiers, and exposes start/pause/stop/status services.
6. When exploration is started, it assigns frontier goals to each robot through `/<robot>/navigate_to_pose`.
7. `map_saver` can save the merged `/map` through `/map_saver/save_map`.

### AMCL mode

1. A saved map YAML is loaded by `/map_server`.
2. `localization_launch.py` starts AMCL inside each robot namespace, so AMCL is also per robot.
3. AMCL uses the robot's namespaced scan and odometry frames, plus the initial pose from `general_settings.yaml`.
4. AMCL publishes the localization transform needed to place each robot in the shared `map` frame.

## TF and starting pose behavior

| Run type | SLAM mode | AMCL mode |
| --- | --- | --- |
| Simulation | `multi_robot_gazebo_bringup.launch.py` spawns robots at the poses in `general_settings.yaml`. When `use_slam:=true` and `use_sim_time:=true`, `multi_robot_nav2_bringup.launch.py` also publishes a zero `map -> <robot>/map` static transform for each robot. | Gazebo spawn poses and AMCL initial poses both come from `general_settings.yaml`; AMCL localizes each robot against the shared `/map`. |
| Real robots | Each robot runs `namespaced_robot.launch.py` with `namespace`, `x_pose`, and `y_pose`. That launch publishes `map -> <namespace>/map` using the provided starting pose, so one robot can be `(0.0, 0.0)` and the others can be relative offsets in meters. | AMCL initial poses are taken from `general_settings.yaml`. The map server publishes `/map`, and AMCL uses each robot's scan/odom frames to connect the robot into the shared map frame. |

The key rule is that launch arguments for real robots and entries in `general_settings.yaml` must match. If `tb2` is launched with `x_pose:=2.5 y_pose:=0.5`, then `tb2` should have the same pose in `general_settings.yaml`.

## Run or test independently

The subsystem is normally started from the GUI, but it can be tested from the terminal by launching the bringup stack directly. Nav2 is included because AMCL is part of Nav2 and the frontier explorer sends goals to Nav2's `NavigateToPose` action servers.

### SLAM with simulation

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
export TURTLEBOT3_MODEL=waffle
ros2 launch smart_factory_bringup multi_robot_gazebo_bringup.launch.py use_sim_time:=true
```

In another terminal:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py use_slam:=true use_sim_time:=true
```

Useful checks:

```bash
ros2 topic list | grep -E '/tb[0-9]+/map|/map$'
ros2 service call /multi_robot_explorer/status std_srvs/srv/Trigger {}
ros2 service call /multi_robot_explorer/start std_srvs/srv/Trigger {}
```

### SLAM with real robots

Start each robot first:

```bash
ros2 launch multi_robot_bringup namespaced_robot.launch.py \
  namespace:=tb1 \
  x_pose:=0.0 \
  y_pose:=0.0
```

```bash
ros2 launch multi_robot_bringup namespaced_robot.launch.py \
  namespace:=tb2 \
  x_pose:=2.5 \
  y_pose:=0.5
```

Then start the mapping/localization bringup on the operator workstation:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py use_slam:=true use_sim_time:=false
```

### AMCL with an existing map

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py \
  use_slam:=false \
  use_sim_time:=false \
  map:=/path/to/factory_merged.yaml
```

Use `use_sim_time:=true` for a Gazebo AMCL test.

## Configurable settings

| Setting | File | Why it matters |
| --- | --- | --- |
| Robot list and poses | `smart_factory_bringup/params/general_settings.yaml` | Controls robot namespaces, simulation spawn poses, AMCL initial poses, and expected real-robot starting offsets. |
| SLAM scan/map tuning | `smart_factory_bringup/params/slam_toolbox_dynamic_overlay.yaml` | Controls SLAM update rate, resolution, scan range, scan matching, and loop closure behavior. |
| Map merge behavior | `merge_map/config/map_merge_params.yaml` | Controls which robot map topics are merged, output topic, QoS, frame id, and conflict policy. |
| Explorer behavior | `smart_factory_navigation/launch/multi_robot_explorer.launch.py` | Controls map topic, namespace regex, frontier size/separation, goal tolerance, stale robot timeout, blacklist timeout, and marker output. |
| AMCL and map server settings | `smart_factory_bringup/params/nav2_slam_params.yaml` | Controls AMCL frames, scan topic, initial pose placeholders, map topic, map saver, and map QoS. |
| Real robot namespace launch | `smart_factory_bringup/launch/namespaced_robot.launch.py` | Controls lidar port, OpenCR USB port, robot namespace, starting pose, and real-robot map anchor transform. |

## Known limitations and assumptions

- Robot namespaces are expected to match the `tb<number>` pattern unless `robot_namespace_regex` and `map_topic_regex` are changed together.
- `general_settings.yaml` must match the number of robots actually launched.
- Real-robot starting poses are manual. The system assumes the operator places the robots at the configured relative offsets before starting.
- `merge_map` currently uses configured/manual map offsets, not TF lookup, when merging occupancy grids.
- Frontier exploration requires Nav2 action servers at `/<robot>/navigate_to_pose`.
- The explorer starts paused by default; the GUI or a service call must start it.
- AMCL mode requires a saved map whose frame and scale match the real or simulated environment.
- Lidar topics use best-effort QoS in the current launch configuration.
