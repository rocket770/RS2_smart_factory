# Perception and Mapping

## Purpose

This subsystem builds and provides the shared factory map. It supports two operating modes:

- SLAM mode: one SLAM Toolbox instance runs per robot, robot maps are combined into `/map`, and frontier exploration can expand the map.
- AMCL mode: one AMCL instance runs per robot, all robots localize against the same saved `/map`.

This page shows how to run the perception and mapping subsystem as a whole. It relies on Nav2 because AMCL is a Nav2 component and frontier exploration sends goals to Nav2 action servers, but task planning, MTSP, and GUI behavior are outside this subsystem.

To launch a single component instead of the full subsystem, use that component page:

- [SLAM](SLAM)
- [AMCL](AMCL)
- [Map Merger](Map-Merger)
- [Map Saving](Map-Saving)
- [SLAM Explorer](SLAM-Explorer)

## Main bringup files

These files are in the `smart_factory_bringup` package.

| File | Role |
| --- | --- |
| `launch/multi_robot_gazebo_bringup.launch.py` | Starts the simulation robots from `params/general_settings.yaml`. |
| `launch/namespaced_robot.launch.py` | Starts one real robot under a namespace and publishes its starting map anchor. |
| `launch/multi_robot_nav2_bringup.launch.py` | Starts the combined mapping/localization stack. Use `use_slam:=true` for SLAM, `use_slam:=false` for AMCL. |
| `params/general_settings.yaml` | Defines robot namespaces and starting poses. |
| `params/nav2_slam_params.yaml` | Shared Nav2/AMCL/map server/map saver parameter file. |
| `params/slam_toolbox_dynamic_overlay.yaml` | SLAM Toolbox overlay tuned for the real TurtleBot3/LDS-01 setup. |

## Before launching

For simulation, launch Gazebo first:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_gazebo_bringup.launch.py use_sim_time:=true
```

For real robots, launch each robot with `namespaced_robot.launch.py`. The full real-robot setup is on [Home](Home), but the important rule is that the namespace and starting pose must match `general_settings.yaml`.

Example:

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

## Launch the subsystem

Run this from the operator workstation after simulation or real robots are already publishing namespaced scan, odometry, and TF data.

### SLAM mode

Use this when building a new map:

```bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py \
  use_slam:=true \
  use_sim_time:=true
```

For real robots:

```bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py \
  use_slam:=true \
  use_sim_time:=false
```

This launches per-robot SLAM Toolbox, map merging, map saving, frontier exploration, and the Nav2 pieces required for exploration goals.

### AMCL mode

Use this when localizing against an existing saved map:

```bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py \
  use_slam:=false \
  use_sim_time:=false \
  map:=/path/to/factory_merged.yaml
```

For a Gazebo AMCL test, set `use_sim_time:=true`.

If `map:=...` is omitted, the launch file defaults to:

```text
smart_factory_bringup/map/factory_merged.yaml
```

## Important launch parameters

| Parameter | Values | Used by | Meaning |
| --- | --- | --- | --- |
| `use_slam` | `true` / `false` | `multi_robot_nav2_bringup.launch.py` | Chooses SLAM mode or AMCL mode. |
| `use_sim_time` | `true` / `false` | All launched nodes | Use `true` in Gazebo and `false` on real robots. |
| `map` | Map YAML path | AMCL mode | Existing map loaded by map server. |
| `nav_params_file` | YAML path | Nav2/AMCL | Defaults to `params/nav2_slam_params.yaml`. |
| `enable_rviz` | `true` / `false` | Bringup | Enables per-robot RViz launch. |
| `rviz_config_file` | RViz config path | Bringup | Defaults to `rviz/multi_nav2_default_view.rviz`. |

Common combinations:

| Run | `use_slam` | `use_sim_time` |
| --- | --- | --- |
| Simulation SLAM | `true` | `true` |
| Real-robot SLAM | `true` | `false` |
| Simulation AMCL | `false` | `true` |
| Real-robot AMCL | `false` | `false` |

## TF behavior by mode

In SLAM mode, each robot owns a local map frame such as `tb1/map`; static transforms anchor those local map frames into the shared `map` frame. In AMCL mode, a saved shared map already exists, so AMCL estimates each robot's pose relative to that shared `map` frame.

| Run type | SLAM mode | AMCL mode |
| --- | --- | --- |
| Simulation | Gazebo spawns robots at `general_settings.yaml` poses. When `use_slam:=true` and `use_sim_time:=true`, `multi_robot_nav2_bringup.launch.py` publishes a zero `map -> <robot>/map` static transform for each robot. | Gazebo poses and AMCL initial poses come from `general_settings.yaml`; AMCL connects each namespaced robot into the shared `map` frame. |
| Real robots | `namespaced_robot.launch.py` publishes `map -> <namespace>/map` using the robot's `x_pose`, `y_pose`, and `z_pose` launch args. This anchors each robot's local SLAM map relative to the shared map frame. | AMCL initial poses come from `general_settings.yaml`; the shared map comes from map server, and AMCL publishes each robot's localization transform. |

The practical rule is simple: launch arguments on real robots and entries in `general_settings.yaml` must describe the same robot layout.

## Perception and mapping troubleshooting

These checks are specific to the mapping stack: namespaced scans and odometry, per-robot SLAM maps, the merged `/map`, map saving, and explorer robot discovery. Nav2 costmap or planner failures can appear nearby in the logs, but start here only when the symptom affects mapping, map merge, saved maps, or frontier discovery.

| Symptom | Likely mapping cause | What it affects | Quick check | Fix | Image |
| --- | --- | --- | --- | --- | --- |
| Robot topics from real TurtleBots do not appear on the workstation. | `ROS_DOMAIN_ID` differs between the robot and workstation, or ROS 2 was not sourced in one terminal. | `merge_map` cannot discover `/tbN/map`; explorer cannot discover robot namespaces. | `python3 tools/perception_mapping/check_ros_domain.py` | Set the same `ROS_DOMAIN_ID` on every robot and the workstation, then relaunch the affected terminals. | TODO |
| A TurtleBot is connected, but mapping looks frozen, robot poses jump, or logs say data is from the future/past. | The TurtleBot system date/time differs from the operator workstation. ROS 2 messages, TF, scans, and SLAM updates are timestamped with that bad clock. | SLAM Toolbox can reject scans, TF lookups can fail, explorer pose data can become stale, and the merged `/map` may stop updating for that robot. | Run `date -u` or `python3 tools/perception_mapping/check_time_sync.py` on the TurtleBot and workstation, then compare the UTC time. | Fix the TurtleBot date/time with NTP or manual time sync, confirm both machines show the same UTC time, then relaunch the robot bringup and mapping stack. | TODO |
| SLAM maps or explorer robot poses look stale, and logs mention old data, future data, extrapolation, or message filter drops. | Robot and workstation clocks are not synchronized. This is common when a TurtleBot date is wrong. | SLAM and explorer receive TF or scan messages with timestamps that do not line up. | `python3 tools/perception_mapping/check_time_sync.py` | Enable NTP/time sync on every machine and relaunch mapping after clocks agree. | TODO |
| `/map` appears late, costmaps take a long time to fill, or map updates pause during real-robot runs. | ROS 2 traffic is delayed or dropped on the network. | Per-robot maps may arrive late; map merge and explorer startup can lag. | `python3 tools/perception_mapping/check_ros_network_logs.py` | Wait for discovery to settle, keep robots on the same reliable network, reduce extra RViz/topic load, and restart only if topics never recover. | TODO |
| Merged map is rotated, doubled, or badly misaligned even though each robot's local map looks usable. | Real robots did not start with the same heading. `namespaced_robot.launch.py` anchors `map -> <namespace>/map` with x/y/z only, so yaw is effectively assumed to match. | The shared `/map` is wrong even if `/tb1/map` and `/tb2/map` individually look reasonable. | Check physical start angles before launch and compare `general_settings.yaml` poses. | Start all robots facing the same direction and keep `namespace`, `x_pose`, and `y_pose` consistent with `general_settings.yaml`. | TODO: phone photo |
| A robot map exists but is not included in `/map`. | The map topic name does not match `map_topic_regex`, currently `^/tb\d+/map$`. | `merge_map` ignores that robot's occupancy grid. | `ros2 topic list | sort | rg '^/tb[0-9]+/map$|^/map$'` | Use `tb1`, `tb2`, etc. namespaces or update `map_topic_regex` intentionally. | TODO |
| Logs say a queue is full, but `/tbN/map` and `/map` continue updating. | A subscriber is falling behind briefly during high topic load. | Usually none; it is normally safe to ignore if mapping continues. | Check that `/map` still publishes: `ros2 topic echo /map --once --qos-durability transient_local` | Ignore occasional messages. If constant, reduce RViz displays or other high-rate subscribers. | TODO |
| Map saving returns `false` or produces no files. | The output directory does not exist, the path is not writable, or no merged `/map` has arrived yet. | Saved YAML/image is missing or invalid for later AMCL. | `python3 tools/perception_mapping/check_map_file.py /home/nick/maps/factory_merged.yaml` after saving | Create the directory, use an absolute writable path prefix, confirm `/map_saver/save_map` exists, and wait for `/map` before saving. | TODO |

## Diagnostic scripts

The helper scripts are intentionally small and standalone:

| Script | Use |
| --- | --- |
| `tools/perception_mapping/check_ros_domain.py` | Prints local `ROS_DOMAIN_ID`, checks whether `ros2 topic list` is available, and summarizes visible mapping topics. |
| `tools/perception_mapping/check_time_sync.py` | Prints local UTC time, optional `timedatectl` sync state, and scans ROS logs for timestamp/TF symptoms. Run it on each machine and compare the UTC line. |
| `tools/perception_mapping/check_ros_network_logs.py` | Scans ROS logs for DDS, discovery, socket, transport, timeout, and queue-pressure hints. |
| `tools/perception_mapping/check_map_file.py` | Checks that a saved map YAML and its image exist and that PGM/PNG dimensions can be read. |
