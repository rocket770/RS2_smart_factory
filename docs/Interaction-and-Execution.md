# Interaction and Execution

## Purpose

This subsystem covers how an operator interacts with the multi-robot system and how planned work is executed on the robots.

The current operator path in this repository is the MTSP GUI planner. It launches the navigation stack, displays the shared map, collects operator-selected goals, runs the MTSP solver, visualizes routes, and executes those routes through Nav2.

The MTSP GUI is responsible for launching and stopping the navigation stack used by the operator workflow, subscribing to the merged `/map`, solver progress, and robot TF, collecting goal points, sending manual navigation goals, launching solver runs, executing MTSP routes through Nav2, and exposing pause, resume, emergency stop, and map-save controls.

This page documents the interaction and execution layer only. Mapping, localization, and exploration details belong to [Perception and Mapping](Perception-and-Mapping), while lower-level navigation stack behavior belongs partly to [Motion Planning and Control](Motion-Planning-and-Control).

## Main files

These files are the main sources of truth for the GUI-based interaction and execution workflow.

| File | Role |
| --- | --- |
| `src/smart_factory_ui/smart_factory_ui/mtsp_planner_gui.py` | Current MTSP GUI. Contains the Qt window, ROS node, map canvas, nav-stack launch flow, solver launch flow, and route execution logic. |
| `src/smart_factory_mtsp_solver/src/mtsp_solver_node.cpp` | MTSP solver node. Reads robot starts and goals from parameters, runs the genetic algorithm, and publishes best-so-far routes on `mtsp_best_solution`. |
| `src/smart_factory_bringup/launch/multi_robot_nav2_bringup.launch.py` | Launches the navigation stack used by the GUI in either AMCL or SLAM mode. |

## Before launching

The current workflow assumes:

- In simulation, Gazebo is launched before the GUI.
- On real robots, each TurtleBot3 is already running under the correct namespace before the GUI starts.
- Robot namespaces and start poses match `src/smart_factory_bringup/params/general_settings.yaml`.
- The GUI currently only supports the hardcoded namespaces `tb1`, `tb2`, `tb3`, and `tb4`.
- Nav2 action servers such as `/<robot>/navigate_to_pose` are available before route execution can succeed.
- In AMCL mode, a saved map YAML is available before starting the nav stack.
- The GUI does not expose a direct `use_sim_time` control when launching the nav stack.

For simulation:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 launch smart_factory_bringup multi_robot_gazebo_bringup.launch.py use_sim_time:=true
```

For real robots, launch each robot under the same namespace used in `general_settings.yaml`. Full hardware setup is documented on [Home](Home).

## Launch the subsystem

Run the operator GUI from the workstation:

```bash
cd ~/smart_factory_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
ros2 run smart_factory_ui mtsp_planner_gui
```

### Current GUI workflow

The normal workflow is:

1. Choose a navigation mode:
   - `Load Existing Map (AMCL)` for localization against a saved map.
   - `Use SLAM + Explore` for live map building and frontier exploration.
2. If using AMCL mode, choose the map YAML file.
3. Click `Start Nav Stack`.
4. Wait for the shared map and live robot TF poses to appear in the GUI.
5. Add MTSP goals by clicking the map in `goal` mode.
6. Optionally use `move_robot` mode to send a one-off manual Nav2 goal to the selected robot.
7. Click `Run MTSP` to launch the solver and wait for a valid result.
8. Click `Start Execution` to send each robot through its assigned goals in order.
9. Use `Pause`, `Resume`, or `Emergency Stop` as needed during execution.

### Mode-specific behavior

#### AMCL mode

Use this mode when localizing against an existing saved map.

- The GUI launches the Nav2 stack in localization mode.
- The selected map YAML is passed to the map server.
- `Set AMCL Poses From Current` optionally publishes `/<robot>/initialpose` estimates using the live TF pose currently seen by the GUI.
- This AMCL pose publication is manual from the GUI; it is not automatically triggered by nav-stack startup.

#### SLAM mode

Use this mode when building a new map.

- The GUI launches `multi_robot_nav2_bringup.launch.py` with `use_slam:=true`.
- The SLAM, map merge, and explorer stack start together.
- The explorer remains paused after launch until the user presses `Resume`.
- `Save Map` calls `/map_saver/save_map` to request a saved merged map.

### Execution behavior

The current execution flow is:

1. The GUI subscribes to `/map`, `mtsp_best_solution`, and shared or namespaced TF topics.
2. The GUI builds the MTSP problem from live robot TF poses and operator-selected goal points.
3. `Run MTSP` writes a temporary solver parameter file in `/tmp` and launches `mtsp_solver_node` as a separate ROS 2 process.
4. The solver publishes best-so-far routes as JSON on `mtsp_best_solution`.
5. The GUI renders the current best generation, total cost, and ordered routes on the map.
6. `Start Execution` sends assigned goals to each robot through `/<robot>/navigate_to_pose`.
7. When one goal succeeds, the GUI sends the next goal for that robot until the route is complete.

Current behavior to keep in mind:

- MTSP start positions come from live TF poses, not from manually clicked move targets.
- Only robots with live TF poses are included in a generated MTSP problem.
- Manual move commands and MTSP execution both use Nav2 `NavigateToPose`.
- `Pause` cancels active GUI-controlled goals but preserves execution progress so it can resume.
- `Resume` continues paused MTSP execution and also resumes explorer behavior if the SLAM explorer stack is active.
- `Emergency Stop` cancels GUI-controlled goals and clears the current MTSP execution state.
- The planner cost backend is currently `nav2`, and the solver uses the first planned robot's `ComputePathToPose` action name when generating path costs.

### Expected outcome

After a successful run:

- the GUI window opens on the operator workstation
- a shared map appears once `/map` is available
- robots with valid TF data appear on the map
- solver progress and planned routes appear after `Run MTSP`
- robots begin visiting their assigned goals after `Start Execution`

### How to run or test this subsystem independently

Use this as the minimum subsystem-level validation flow:

1. Start Gazebo for simulation, or launch the real robots under the correct namespaces.
2. Start the MTSP GUI.
3. Select AMCL or SLAM mode and start the nav stack.
4. Confirm the GUI receives `/map` and at least one robot pose from TF.
5. Add one or more goals and run MTSP.
6. Confirm `mtsp_best_solution` updates and a route is shown in the GUI.
7. Start execution and confirm `/<robot>/navigate_to_pose` accepts goals.
8. Optionally verify `Pause`, `Resume`, `Emergency Stop`, and `Save Map`.

## Important parameters and controls

### Operator controls

| Control | Meaning |
| --- | --- |
| GUI mode | Chooses `Load Existing Map (AMCL)` or `Use SLAM + Explore`. |
| `map` path | Saved map YAML used in AMCL mode. |
| `goal` click mode | Adds MTSP goal points to the current planning problem. |
| `move_robot` click mode | Sends a manual `NavigateToPose` goal to the selected robot. |
| `Robot` selector | Chooses which robot receives a manual move target. |
| `Set AMCL Poses From Current` | Publishes `/<robot>/initialpose` estimates using current live TF poses. |
| `Clear Goals` | Removes all currently selected MTSP goals from the GUI. |
| `Clear Move Targets` | Clears the displayed manual move target markers. |
| `Start Nav Stack` | Launches the nav-stack bringup process from the GUI. |
| `Stop Nav Stack` | Stops the nav-stack process launched by the GUI. |
| `Pause` | Pauses GUI-controlled execution and requests explorer pause. |
| `Resume` | Resumes GUI-controlled execution and requests explorer resume. |
| `Emergency Stop` | Cancels GUI-controlled goals and clears active MTSP execution state. |
| `Run MTSP` | Launches the solver with the current problem and hyperparameters. |
| `Start Execution` | Executes the latest valid MTSP result through Nav2. |
| `Dump Run Config` | Shows the generated solver parameter block. |
| `Refresh Map` | Rebuilds the displayed map image from the latest `/map`. |
| `Save Map` | Requests map saving through `/map_saver/save_map` in SLAM mode. |

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

### Launch parameters

| Parameter | Values | Used by | Meaning |
| --- | --- | --- | --- |
| `use_slam` | `true` / `false` | `multi_robot_nav2_bringup.launch.py` | Chooses SLAM or localization mode. |
| `use_sim_time` | `true` / `false` | Nav stack | Bringup launch argument not exposed directly by the GUI. |
| `enable_rviz` | `true` / `false` | Bringup | Controls per-robot RViz launch. |

## Interfaces

| Interface | Direction | Meaning |
| --- | --- | --- |
| `/map` | Input | Shared occupancy grid shown in the GUI. |
| `mtsp_best_solution` | Input to GUI / output from solver | Best-so-far MTSP solution encoded as JSON in `std_msgs/String`. |
| `/tf`, `/tf_static` | Input | Shared TF used to recover robot poses. |
| `/<robot>/tf`, `/<robot>/tf_static` | Input | Namespaced TF used when present. |
| `/<robot>/navigate_to_pose` | Output action | Used for manual moves and MTSP execution. |
| `/<robot>/initialpose` | Output | AMCL initial pose estimates published from the GUI. |
| `/map_saver/save_map` | Service client | Saves a merged map in SLAM mode. |
| `/multi_robot_explorer/start` | Service client | Starts or resumes frontier exploration from the GUI. |
| `/multi_robot_explorer/stop` | Service client | Pauses exploration from the GUI. |
| `/multi_robot_explorer/return_home` | Service client | Requests idle robots return home after a map save. |
| `/multi_robot_explorer/status` | Service client | Used by the GUI to check explorer readiness. |

## Assumptions and limitations

- The MTSP GUI is the current operator workflow and the main UI path for this subsystem.
- Route execution depends on Nav2 action servers already being available for the robot namespaces in use.
- Namespaces, start poses, and TF layout must match `general_settings.yaml` for the overall GUI workflow to behave correctly.
- The current GUI only supports the hardcoded robot namespaces `tb1`, `tb2`, `tb3`, and `tb4`.
- The GUI does not expose a direct `use_sim_time` control for the nav-stack launch.
- Only robots with live TF poses are included in a generated MTSP problem.
- This page does not replace the detailed workspace installation or robot setup instructions on [Home](Home).

## Troubleshooting & FAQs

### The GUI opens but no map appears

- Confirm the nav stack started successfully from the GUI.
- Check that `/map` is being published.
- In AMCL mode, confirm the selected map YAML exists and loads correctly.
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
- Use `Set AMCL Poses From Current` if a new AMCL initial estimate is needed and live TF data is already available.

### SLAM mode starts but exploration does not continue

- Confirm the nav-stack process launched successfully from the GUI.
- Check whether the explorer services under `/multi_robot_explorer/*` are available.
- Press `Resume`, because the explorer remains paused after launch until the user resumes it.
