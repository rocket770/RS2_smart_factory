# AMCL

## What it does

AMCL mode localizes each robot against an existing saved map. One `amcl` node runs inside each robot namespace, so every robot estimates its own pose in the shared `map` frame.

## How it works

1. `multi_robot_nav2_bringup.launch.py use_slam:=false` starts a shared `/map_server`.
2. The map server loads the map YAML passed with `map:=...`.
3. For each robot in `general_settings.yaml`, `nav2_bringup/localization_launch.py` starts `/<robot>/amcl`.
4. The launch rewrites placeholders in `nav2_slam_params.yaml` so AMCL uses the correct namespaced frames and scan topic.
5. Initial pose values come from `general_settings.yaml`, unless `initial_pose_x`, `initial_pose_y`, or `initial_pose_yaw` are set for that robot.

## Launch only AMCL

The combined subsystem launch is the recommended way to test AMCL:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_nav2_bringup.launch.py \
  use_slam:=false \
  use_sim_time:=false \
  map:=/path/to/factory_merged.yaml
```

For one robot's localization launch file directly:

```bash
ros2 launch smart_factory_bringup nav2_bringup/localization_launch.py \
  namespace:=tb1 \
  map:=/path/to/factory_merged.yaml \
  map_server:=true \
  use_sim_time:=false \
  params_file:=$(ros2 pkg prefix smart_factory_bringup)/share/smart_factory_bringup/params/nav2_slam_params.yaml \
  initial_pose_x:=0.0 \
  initial_pose_y:=0.0 \
  initial_pose_yaw:=0.0
```

Use `use_sim_time:=true` in Gazebo.

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/map` | Input | Shared map from map server. |
| `/<robot>/scan` | Input | Lidar scan used for localization. |
| `/<robot>/odom` and `/<robot>/base_footprint` | Input frames | Robot odometry and base frames. |
| `map` localization TF | Output | Transform connecting the robot pose into the shared map frame. |
| `/<robot>/particle_cloud` | Output | AMCL particle cloud. |
| `/<robot>/initialpose` | Input | Optional pose reset topic. |

## Key parameters

| Parameter | File | Purpose |
| --- | --- | --- |
| `map` | launch argument | Map YAML loaded by map server. |
| `base_frame_id` | `nav2_slam_params.yaml` placeholder | Rewritten to `<namespace>/base_footprint`. |
| `odom_frame_id` | `nav2_slam_params.yaml` placeholder | Rewritten to `<namespace>/odom`. |
| `scan_topic` | `nav2_slam_params.yaml` placeholder | Rewritten to `/<namespace>/scan`. |
| `initial_pose.x`, `initial_pose.y`, `initial_pose.yaw` | `general_settings.yaml` through launch args | Robot's starting estimate in the shared map. |
| `tf_broadcast` | `nav2_slam_params.yaml` | Allows AMCL to publish localization TF. |
| `qos_overrides` | `nav2_slam_params.yaml` | Sets map and scan subscription reliability. |

## Assumptions

- The saved map matches the physical or simulated environment.
- Robot starting poses in `general_settings.yaml` are close enough for AMCL to converge.
- AMCL mode does not run SLAM Toolbox, map merging, or frontier exploration.
