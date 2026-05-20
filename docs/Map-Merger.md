# Map Merger

## What it does

The `merge_map` node combines per-robot occupancy grids into one shared `/map`. It also supports a single robot: if only one matching map topic exists, that map is republished as the shared map.

## How it works

1. The node scans the ROS graph for `nav_msgs/msg/OccupancyGrid` topics matching `map_topic_regex`.
2. It subscribes to each matching topic, such as `/tb1/map`.
3. It stores the latest occupancy grid from every discovered robot.
4. It computes output bounds large enough to contain all input maps.
5. For each input map cell, it computes that cell's world position and writes its occupancy value into the matching cell in the shared output grid.
6. Cell conflicts are resolved with `unknown_value`, `occupied_threshold`, `overwrite_known_cells`, and `conflict_policy`.
7. The merged grid is published on `/map` with frame id `map`.
8. Publishing is throttled by `publish_period_sec`; incoming robot maps mark the output dirty, and the next publish timer tick sends the latest merged grid.

Unknown cells do not overwrite known cells. With the current `prefer_free` conflict policy, free observations can clear stale occupied cells when maps disagree.

## Launch only map merging

Start SLAM first so at least one `/<robot>/map` topic exists.

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch merge_map merge_map_launch.py use_sim_time:=false
```

Use `use_sim_time:=true` in Gazebo.

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/tb1/map`, `/tb2/map`, ... | Input | Per-robot maps discovered by regex. |
| `/map` | Output | Shared occupancy grid used by AMCL, exploration, map saver, and downstream consumers. |

## Key parameters

| Parameter | Current value | Purpose |
| --- | --- | --- |
| `map_topic_regex` | `^/tb\d+/map$` | Selects per-robot map topics. |
| `publish_topic` | `/map` | Shared map output topic. |
| `input_reliability` | `best_effort` | QoS for SLAM map subscriptions. |
| `output_reliability` | `reliable` | QoS for the shared map publisher. |
| `scan_period_sec` | `1.0` | How often the node scans for new map topics. |
| `publish_period_sec` | `1.0` | Minimum period between merged `/map` publications. |
| `merged_frame_id` | `map` | Frame id on the output map. |
| `unknown_value` | `-1` | OccupancyGrid value for unknown cells. |
| `occupied_threshold` | `50` | Cells at or above this value are treated as occupied. |
| `overwrite_known_cells` | `false` | Prevents blind overwrites of known cells. |
| `conflict_policy` | `prefer_free` | Resolves occupied/free conflicts. |
| `free_space_clear_radius_cells` | `0` | Optional radius for clearing nearby cells. |

Parameter notes:

- `conflict_policy` decides what happens when two input maps disagree about the same output cell. With `prefer_free`, a free cell wins over an occupied cell. This is useful when one robot has stale obstacle data but another later observes the area as clear. Other code-supported policies include `prefer_occupied` and `latest`.
- `free_space_clear_radius_cells` expands the effect of a free observation to nearby cells. At `0`, only the exact projected cell is updated. Higher values can clear small stale obstacle artifacts around free cells, but can also erase valid thin obstacles if set too aggressively.
- `overwrite_known_cells: false` means known map data is resolved through the conflict logic instead of simply replacing old values with the newest input.
- `occupied_threshold: 50` is the cutoff used by the conflict logic to decide whether a cell counts as occupied.
- `input_reliability: best_effort` matches the per-robot SLAM map publishers; `output_reliability: reliable` keeps the shared `/map` compatible with Nav2 Humble's static layer.
- `publish_period_sec` limits full-map network traffic. The node does not publish continuously when no input map has changed.

## Assumptions

- Robot map topics match the `tb<number>` namespace convention.
- The input maps are already in compatible local map frames.
