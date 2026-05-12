# Interaction and Execution

## Purpose

This subsystem covers how an operator interacts with the multi-robot system and how planned work is executed on the robots.

The current interaction path in the repository is the MTSP GUI planner. It launches the navigation stack, lets the user click goals on the shared map, runs the MTSP solver, visualizes routes, and executes them through Nav2.

This page shows the interaction and execution subsystem as a whole. Mapping, localization, and exploration details belong to [Perception and Mapping](Perception-and-Mapping), while robot navigation stack bringup belongs partly to [Motion Planning and Control](Motion-Planning-and-Control).

## Main files

These files are the main sources of truth for the interaction and execution subsystem.

| File | Role |
| --- | --- |
| `src/smart_factory_ui/smart_factory_ui/mtsp_planner_gui.py` | Current MTSP GUI. Contains the Qt window, ROS node, map canvas, solver launch flow, and route execution logic. |
| `src/smart_factory_mtsp_solver/src/mtsp_solver_node.cpp` | MTSP solver node. Reads robot starts and goals from parameters, runs the genetic algorithm, and publishes best-so-far routes on `mtsp_best_solution`. |
| `src/smart_factory_bringup/launch/multi_robot_nav2_bringup.launch.py` | Launches the navigation stack used by the GUI in either SLAM or AMCL mode. |

## Before launching

The current MTSP GUI assumes the same general setup as the navigation and mapping stack:

- In simulation, launch Gazebo first.
- On real robots, launch each TurtleBot3 under the correct namespace before opening the GUI.
- Robot namespaces and start poses must match `src/smart_factory_bringup/params/general_settings.yaml`.
- The current GUI only knows the hardcoded namespaces `tb1`, `tb2`, `tb3`, and `tb4`.
- Nav2 action servers such as `/<robot>/navigate_to_pose` must exist before route execution can succeed.
- In AMCL mode, have a saved map YAML ready before starting the nav stack from the GUI.
- The current GUI does not expose a `use_sim_time` control. It launches `multi_robot_nav2_bringup.launch.py` without overriding that launch argument.

For simulation, launch Gazebo first:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_gazebo_bringup.launch.py use_sim_time:=true
```

For real robots, launch each robot under the same namespace used in `general_settings.yaml`. The full hardware setup flow is described on [Home](Home).

## Launch the subsystem

### Current MTSP GUI workflow

Run the current operator GUI from the workstation:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run smart_factory_ui mtsp_planner_gui
```

This is the main interaction workflow in the current codebase.

Inside the GUI:

- Choose `Load Existing Map (AMCL)` to localize against a saved map.
- Choose `Use SLAM + Explore` to build a live map and use exploration.
- Click `Start Nav Stack` to launch `multi_robot_nav2_bringup.launch.py`.
- Add MTSP goals by clicking on the map in `goal` mode.
- Use `move_robot` mode to send a one-off manual Nav2 goal to the selected robot.
- Use `Run MTSP` to launch the solver and wait for a best route set.
- Use `Start Execution` to send the ordered goals to each robot through Nav2.
- `Pause`, `Resume`, and `Emergency Stop` control active GUI-owned execution.
- `Dump Run Config` shows the generated solver parameter block.
- `Refresh Map` redraws the current map display.

### Expected outcome

After the GUI and nav stack start successfully:

- The MTSP GUI window opens on the operator workstation.
- A shared map appears in the map canvas after `/map` becomes available.
- Robots with valid TF data appear on the map and can be selected for manual moves.
- In AMCL mode, robots localize against the selected saved map.
- In SLAM mode, the merged map grows as robots explore the environment.
- After `Run MTSP`, planned routes appear on the map and solver progress is shown in the GUI.
- After `Start Execution`, robots begin visiting their assigned goals through Nav2.

Screenshot placeholders:

- TODO: Add MTSP GUI screenshot with main controls labeled.
- TODO: Add screenshot showing planned MTSP routes on the shared map.
- TODO: Add screenshot showing active execution or paused execution state.

### AMCL mode

Use this when localizing against an existing map:

- Set the GUI mode to `Load Existing Map (AMCL)`.
- Choose the saved map YAML file.
- Start the nav stack from the GUI.
- Optionally use `Set AMCL Poses From Current` to publish `/<robot>/initialpose` estimates based on the live TF poses currently seen by the GUI.

### SLAM mode

Use this when building a new map:

- Set the GUI mode to `Use SLAM + Explore`.
- Start the nav stack from the GUI.
- The GUI launches the SLAM, map merge, and explorer stack through `multi_robot_nav2_bringup.launch.py use_slam:=true`.
- After launch, the explorer remains paused until the user presses `Resume`.
- When the merged `/map` becomes available, use it for goal selection and MTSP planning.
- Use `Save Map` to request a saved map from `/map_saver/save_map`.

## Important parameters and controls

### Navigation mode controls

| Control or parameter | Values | Used by | Meaning |
| --- | --- | --- | --- |
| GUI mode | `Load Existing Map (AMCL)` / `Use SLAM + Explore` | `mtsp_planner_gui.py` | Chooses whether the GUI launches AMCL mode or SLAM mode. |
| `map` | Map YAML path | AMCL mode | Existing map loaded by Nav2 map server. |
| `use_slam` | `true` / `false` | `multi_robot_nav2_bringup.launch.py` | Chooses SLAM or localization mode. |
| `use_sim_time` | `true` / `false` | Nav stack | This is a bringup launch argument, but the current GUI does not expose it directly. |
| `enable_rviz` | `true` / `false` | Bringup | Controls per-robot RViz launch. |

### Map interaction and action controls

| Control | Meaning |
| --- | --- |
| `goal` click mode | Adds MTSP goal points to the current planning problem. |
| `move_robot` click mode | Sends a manual `NavigateToPose` goal to the selected robot. |
| `Robot` selector | Chooses which robot receives a manual move target. |
| `Clear Goals` | Removes all currently selected MTSP goals from the GUI. |
| `Clear Move Targets` | Clears the displayed manual move target markers. |
| `Start Nav Stack` | Launches the Nav2 bringup process from the GUI. |
| `Stop Nav Stack` | Sends a stop signal to the nav bringup process launched by the GUI. |
| `Pause` | Pauses GUI-controlled execution and requests explorer pause. |
| `Resume` | Resumes GUI-controlled execution and requests explorer resume. |
| `Emergency Stop` | Cancels GUI-controlled goals and clears the active MTSP execution state. |
| `Dump Run Config` | Writes the current solver config to the summary pane. |
| `Refresh Map` | Rebuilds the displayed map image from the latest `/map`. |

### MTSP run parameters

| Parameter | Current GUI value | Meaning |
| --- | --- | --- |
| `population_size` | `500` | Genetic algorithm population size. |
| `generations` | `1500` | Number of solver generations to run. |
| `mutation_rate` | `0.15` | Mutation rate used during evolution. |
| `seed` | `42` | Random seed for solver reproducibility. |
| `generation_delay_ms` | `0` | Optional delay between generations for slower live visualization. |
| `publish_progress` | `true` | Publishes best-so-far solutions to `mtsp_best_solution`. |
| `publish_generation_delay` | `1.0` | Publishes progress every generation with the current GUI config. |
| `distance_backend` | `nav2` | Uses Nav2 `ComputePathToPose` path lengths instead of straight-line distance. |
| `planner_action_name` | `/<first-planned-robot>/compute_path_to_pose` | Nav2 planner action used when building MTSP path costs. |
| `global_frame` | `map` | Frame used for MTSP solver path requests. |
| `planner_id` | `""` | Empty value lets the solver fall back to the planner plugin default. |
| `planner_server_timeout_ms` | `5000` | Timeout while waiting for the planner action server. |
| `planner_result_timeout_ms` | `10000` | Timeout while waiting for a path result. |

## Execution behavior

### MTSP GUI execution flow

The current MTSP GUI follows this execution flow:

1. The GUI subscribes to the merged `/map`, the `mtsp_best_solution` progress topic, and shared or namespaced TF topics.
2. The GUI builds the MTSP problem from live robot TF positions and user-clicked goal points.
3. `Run MTSP` writes a temporary parameter file and launches `mtsp_solver_node` as a separate ROS 2 process.
4. The solver publishes best routes as JSON on `mtsp_best_solution`.
5. The GUI displays the current best generation, total cost, and ordered routes on top of the map.
6. `Start Execution` sends each assigned goal to `/<robot>/navigate_to_pose` in order.
7. When one goal succeeds, the GUI sends the next goal for that robot until the route is complete.

Important current behavior:

- The current GUI uses live TF robot positions as MTSP starts. Manually clicked move targets are for manual motion commands, not MTSP start-state definition.
- Only robots with live TF positions are included in the MTSP run configuration.
- Manual move commands and MTSP execution both use Nav2 `NavigateToPose`.
- The GUI writes a temporary solver parameter file in `/tmp` and launches the solver as a separate process.
- The GUI also writes a temporary solver log file and reports solver failures from that log when possible.
- `Pause` cancels active GUI-controlled goals and keeps MTSP progress so execution can resume later.
- `Resume` restarts paused MTSP execution and also resumes explorer behavior if the SLAM explorer stack is active.
- `Emergency Stop` cancels GUI-controlled goals and clears the current MTSP execution state.
- The planner cost backend is currently `nav2`, and the solver uses the first planned robot's `ComputePathToPose` action name when generating path costs.

## How to run or test this subsystem independently

Use this procedure to test the interaction and execution subsystem without documenting every other subsystem in detail:

1. Start Gazebo first for a simulation test, or launch each real robot under the correct namespace.
2. Start the MTSP GUI from the operator workstation.
3. Choose either `Load Existing Map (AMCL)` or `Use SLAM + Explore`.
4. Click `Start Nav Stack` and wait for the map and robot TF data to appear in the GUI.
5. Add one or more goal points on the map in `goal` mode.
6. Click `Run MTSP` and confirm the GUI shows a route set and solver progress.
7. Click `Start Execution` and confirm robots begin moving toward assigned goals.
8. Optionally test `Pause`, `Resume`, `Emergency Stop`, and `Save Map` to verify control actions behave as expected.

Minimum success checks:

- The GUI receives `/map` updates.
- At least one robot pose is visible from TF.
- `mtsp_best_solution` updates after `Run MTSP`.
- `/<robot>/navigate_to_pose` accepts goals during manual movement or MTSP execution.

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/map` | Input | Shared occupancy grid shown in the MTSP GUI. |
| `mtsp_best_solution` | Input to GUI / output from solver | Best-so-far MTSP solution encoded as JSON in `std_msgs/String`. |
| `/tf`, `/tf_static` | Input | Shared TF used to recover robot poses. |
| `/<robot>/tf`, `/<robot>/tf_static` | Input | Namespaced TF used by the MTSP GUI. |
| `/<robot>/navigate_to_pose` | Output action | Used for manual moves and MTSP execution. |
| `/<robot>/initialpose` | Output | AMCL initial pose estimates published from the GUI. |
| `/map_saver/save_map` | Service client | Saves a merged map in SLAM mode. |
| `/multi_robot_explorer/start` | Service client | Starts or resumes frontier exploration from the GUI. |
| `/multi_robot_explorer/stop` | Service client | Pauses exploration from the GUI. |
| `/multi_robot_explorer/return_home` | Service client | Requests idle robots return home after a map save. |
| `/multi_robot_explorer/status` | Service client | Used by the GUI to check explorer readiness. |

## Assumptions and limitations

- The MTSP GUI is the current operator workflow and the main UI path to document.
- Route execution depends on Nav2 action servers already being available for the robot namespaces in use.
- Namespaces, start poses, and TF layout must match `general_settings.yaml` for the overall interaction flow to behave correctly.
- The current GUI only supports the hardcoded robot namespaces `tb1`, `tb2`, `tb3`, and `tb4`.
- The GUI does not expose a direct `use_sim_time` control for the nav stack launch.
- Only robots with live TF poses are included in a generated MTSP problem.
- This page does not replace the detailed setup instructions on [Home](Home) for workspace installation or real-robot preparation.

## Troubleshooting & FAQs

### The GUI opens but no map appears

- Confirm the nav stack actually started from the GUI.
- Check that `/map` is being published.
- In AMCL mode, confirm the selected map YAML file exists and loads correctly.
- In SLAM mode, wait for the merged map to be produced after SLAM and map merging start.

### `Run MTSP` does not produce a route

- Confirm at least one robot has a live TF pose in the GUI.
- Confirm at least one goal point was added in `goal` mode.
- Check whether the solver process reported an error in the GUI summary or solver log.
- Confirm the planner action server such as `/<robot>/compute_path_to_pose` is available.

### `Start Execution` does not move the robots

- Confirm Nav2 action servers such as `/<robot>/navigate_to_pose` are available.
- Confirm the robot namespaces in the running system match `general_settings.yaml`.
- Check whether the planned robots still have valid TF poses.
- Try a manual `move_robot` command first to confirm Nav2 goal execution works outside MTSP execution.

### AMCL mode starts but robots do not localize correctly

- Confirm the selected saved map matches the real or simulated environment.
- Confirm robot starting poses and namespaces match `general_settings.yaml`.
- Use `Set AMCL Poses From Current` if live TF data is available and an updated initial estimate is needed.

### SLAM mode starts but exploration does not continue

- Confirm the nav stack process launched successfully from the GUI.
- Check whether the explorer services under `/multi_robot_explorer/*` are available.
- Press `Resume` because the explorer remains paused after launch until the user resumes it.
