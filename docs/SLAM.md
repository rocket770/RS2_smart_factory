# SLAM

## What it does

SLAM mode runs one `slam_toolbox` node per robot namespace. Each robot builds its own local occupancy grid from its lidar and odometry, then publishes that map under its namespace, for example `/tb1/map`.

SLAM does not create the final shared map by itself. The per-robot maps are consumed by [Map Merging](Map-Merging.md), which publishes the shared `/map`.

## How it works

1. `multi_robot_slam_toolbox.launch.py` reads robot names from `smart_factory_bringup/params/general_settings.yaml`.
2. For each robot, it starts `slam_toolbox` as `/<robot>/slam_toolbox`.
3. Each instance uses:
   - map frame: `<robot>/map`
   - odom frame: `<robot>/odom`
   - base frame: `<robot>/base_footprint`
   - scan topic: `/<robot>/scan`
4. The launch loads the default SLAM Toolbox `mapper_params_online_async.yaml` and then overlays project settings from `slam_toolbox_dynamic_overlay.yaml`.
5. A shared `map_saver` node is also started so the merged `/map` can be saved.

The overlay is tuned for real TurtleBot3 Waffle Pi robots with LDS-01 lidar. The main idea is to keep map updates responsive while limiting the scan range and requiring stronger scan matches than the default simulation-focused settings.

## Launch only SLAM

Start simulation or real robots first so `/<robot>/scan`, `/<robot>/odom`, and TF are available.

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_slam_toolbox.launch.py use_sim_time:=false
```

Use `use_sim_time:=true` in Gazebo.

This launches SLAM Toolbox and map saver only. It does not launch map merging, exploration, or AMCL.

## TF anchoring in SLAM mode

In SLAM mode, each robot has its own local map frame, such as `tb1/map` or `tb2/map`. A static transform anchors each local robot map into the shared `map` frame so the maps can be interpreted together.

The TF structure is the same idea in simulation and on real robots; the difference is where the static transform comes from:

| Run type | Static transform source | Behavior |
| --- | --- | --- |
| Simulation | `multi_robot_nav2_bringup.launch.py` | When `use_slam:=true` and `use_sim_time:=true`, it publishes a zero `map -> <robot>/map` static transform for each robot. Gazebo already spawned the robots at the configured starting poses. |
| Real robots | `namespaced_robot.launch.py` | Each robot publishes `map -> <namespace>/map` using its `x_pose`, `y_pose`, and `z_pose` launch arguments. These should match `general_settings.yaml`. |

`use_sim_time` changes timestamps and clock source, not the basic TF design. The important TF difference is whether SLAM is running with local per-robot map frames or AMCL is localizing directly against a saved shared map.

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/<robot>/scan` | Input | Lidar scan used by SLAM Toolbox. |
| `/<robot>/odom` to `/<robot>/base_footprint` TF | Input | Robot motion estimate used for scan matching. |
| `/<robot>/map` | Output | Per-robot occupancy grid. |
| `/<robot>/map_metadata` | Output | Metadata for the per-robot map. |
| `/<robot>/map_updates` | Output | Incremental map updates. |
| `/map_saver/save_map` | Service | Saves the current map received by map saver. |

## Key parameters

| Parameter | File | Purpose |
| --- | --- | --- |
| `map_update_interval` | `slam_toolbox_dynamic_overlay.yaml` | How often SLAM updates the map. |
| `resolution` | `slam_toolbox_dynamic_overlay.yaml` | Occupancy grid resolution. |
| `min_laser_range`, `max_laser_range` | `slam_toolbox_dynamic_overlay.yaml` | Useful lidar range limits. |
| `minimum_travel_distance`, `minimum_travel_heading` | `slam_toolbox_dynamic_overlay.yaml` | Minimum robot motion before processing updates. |
| `use_scan_matching` | `slam_toolbox_dynamic_overlay.yaml` | Enables scan matching. |
| `do_loop_closing` | `slam_toolbox_dynamic_overlay.yaml` | Enables loop closure. |
| `scan_buffer_size` | `slam_toolbox_dynamic_overlay.yaml` | Number of recent scans kept for matching and loop closure. |
| `link_match_minimum_response_fine` | `slam_toolbox_dynamic_overlay.yaml` | Minimum scan-match quality for linking scans locally. |
| `loop_match_minimum_response_coarse`, `loop_match_minimum_response_fine` | `slam_toolbox_dynamic_overlay.yaml` | Minimum match quality for accepting loop closures. |
| `loop_search_maximum_distance` | `slam_toolbox_dynamic_overlay.yaml` | Maximum distance for searching loop-closure candidates. |
| `correlation_search_space_dimension` | `slam_toolbox_dynamic_overlay.yaml` | Search window for scan matching. |
| `occupancy_threshold` | `slam_toolbox_dynamic_overlay.yaml` | Threshold used when converting scan evidence into occupied map cells. |
| `qos_overrides` | `multi_robot_slam_toolbox.launch.py` | Uses best-effort QoS for scan and per-robot map topics. |

Parameter notes:

- `map_update_interval: 0.5` updates the map frequently enough for slow robot movement and interactive exploration.
- `resolution: 0.05` means each map cell represents 5 cm.
- `min_laser_range: 0.12` and `max_laser_range: 3.2` keep SLAM focused on the useful part of the LDS-01 scan and avoid noisy far-range readings.
- `minimum_travel_distance: 0.04` and `minimum_travel_heading: 0.04` allow map updates during slow driving without fusing every tiny encoder jitter.
- `use_scan_matching: true` lets SLAM align scans against the current map instead of trusting odometry alone.
- `do_loop_closing: true` enables closed-loop correction. When a robot revisits an area, SLAM Toolbox can detect the match and reduce accumulated drift in the map.
- The loop-closure response thresholds are stricter than loose defaults so real hardware noise is less likely to create a false loop closure.

## Assumptions

- Robot namespaces follow the names in `general_settings.yaml`.
- Each robot publishes a namespaced lidar topic and odometry TF.
- The final shared map is produced by map merging, not directly by SLAM Toolbox.
