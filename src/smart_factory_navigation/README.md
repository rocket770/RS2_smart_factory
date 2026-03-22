# smart_factory_navigation V1

Dynamic multi-robot frontier exploration coordinator for:
- merged global `/map`
- per-robot namespaced TF buses (`/tbX/tf`, `/tbX/tf_static`)
- per-robot Nav2 `NavigateToPose` action servers (`/tbX/navigate_to_pose`)

## Features
- dynamic robot discovery from namespaces matching `^tb[0-9]+$`
- subscribes to each robot's namespaced TF topics
- reconstructs `map -> base_link` or `map -> base_footprint` in 2D from that robot's TF tree
- detects frontier clusters on merged `/map`
- assigns unique frontiers to idle robots
- runtime control services: start, pause, stop, status
- RViz MarkerArray debug output for frontiers, reservations, robot states, and assignment lines
- starts paused by default

## Install
Copy this package into your workspace `src/` and build:

```bash
colcon build --packages-select smart_factory_navigation
source install/setup.bash
```

## Launch
```bash
ros2 launch smart_factory_navigation multi_robot_explorer.launch.py
```

## Services
```bash
ros2 service call /multi_robot_explorer/start std_srvs/srv/Trigger {}
ros2 service call /multi_robot_explorer/pause std_srvs/srv/Trigger {}
ros2 service call /multi_robot_explorer/stop std_srvs/srv/Trigger {}
ros2 service call /multi_robot_explorer/status std_srvs/srv/Trigger {}
```

## Notes
- V1 uses Euclidean distance for robot-frontier scoring, not full path cost.
- V1 assumes planar robots and uses yaw only from TF.
- Frontier detection uses free cells adjacent to unknown space, and rejects frontiers adjacent to occupied cells.
- If your merged map frame is not named `map`, adjust the code or remap accordingly.

## RViz markers
Add a `MarkerArray` display and point it to:

```bash
/multi_robot_explorer/markers
```

What you will see:
- cyan arrows: robot poses
- green spheres: currently available frontier centroids
- faint blue spheres: all detected frontier centroids
- orange cylinders and lines: assigned goals and robot-to-goal links
- red translucent cylinders: blacklisted/cooldown goal regions

This should make it much easier to tell whether the node is actually finding useful frontiers or just rediscovering a goal near the robot.


V1.4 updates:
- smarter frontier goal selection uses a real frontier cell instead of centroid-only targeting
- adaptive minimum goal distance so startup still works on tiny known maps
- nearby frontier-region merge to reduce duplicate rediscovery
