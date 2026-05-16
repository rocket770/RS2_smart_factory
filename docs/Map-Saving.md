# Map Saving

## What it saves

Map saving writes the merged `/map` occupancy grid to a YAML file plus an image file. Use it after SLAM and map merging have produced a usable shared map. The saved YAML can then be loaded in AMCL mode.

The save interface is `/map_saver/save_map`. It is a ROS service, not a topic.

## Normal path through the GUI

In SLAM mode, the GUI calls `/map_saver/save_map` with:

| Field | Value |
| --- | --- |
| `map_topic` | `map` |
| `image_format` | `png` |
| `map_mode` | `trinary` |
| `free_thresh` | `0.25` |
| `occupied_thresh` | `0.65` |

Use an absolute writable output prefix such as `/home/nick/maps/factory_merged`. The service writes files such as `factory_merged.yaml` and `factory_merged.png`.

## Save by calling the service

First make sure SLAM mode is running and `/map` exists:

```bash
ros2 topic echo /map --once --qos-durability transient_local
```

Create the output directory:

```bash
mkdir -p /home/nick/maps
```

Call the save service:

```bash
ros2 service call /map_saver/save_map nav2_msgs/srv/SaveMap "{map_topic: map, map_url: /home/nick/maps/factory_merged, image_format: pgm, map_mode: trinary, free_thresh: 0.25, occupied_thresh: 0.65}"
```

Check the result:

```bash
python3 tools/perception_mapping/check_map_file.py /home/nick/maps/factory_merged.yaml
```

## Run map saver by itself

Use this only when `/map` is already being published by the mapping stack and you need a standalone saver.

Terminal 1:

```bash
ros2 run nav2_map_server map_saver_server --ros-args -r __node:=map_saver -p use_sim_time:=false -p save_map_timeout:=5000.0 -p free_thresh_default:=0.25 -p occupied_thresh_default:=0.65 -p map_subscribe_transient_local:=true
```

Terminal 2:

```bash
ros2 lifecycle set /map_saver configure
```

```bash
ros2 lifecycle set /map_saver activate
```

Then call `/map_saver/save_map` using the service command above.

Use `use_sim_time:=true` only for Gazebo runs.

## Common failures

| Symptom | Cause | Fix |
| --- | --- | --- |
| Service returns `result=false`. | Output directory is missing, path is not writable, or no `/map` was received before timeout. | Create the directory, use an absolute writable prefix, and wait until `/map` publishes. |
| YAML exists but image is missing. | Save failed partway through or the image path was moved after saving. | Save again and keep the YAML next to its referenced image unless you edit the YAML image path. |
| AMCL cannot load the saved map. | YAML image path is wrong or the image file is corrupt/unsupported. | Run `check_map_file.py` on the YAML and fix the image path. |
