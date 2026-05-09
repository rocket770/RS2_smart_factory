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
