# 🦾 Obstacle Avoidance for Serial Chain Manipulators (Master Thesis)

This repository contains the full implementation of my Master’s thesis on obstacle avoidance algorithms and robot modeling for serial-chain robotic manipulators. The algorithms are developed in modern C++ and have been validated on the **Franka Emika Panda Manipulator**. While the core methods are applicable to any serial chain manipulator with revolute joints, they have so far been tested exclusively with the Franka robot. Future work may include extending compatibility to other robotic manipulators by updating configuration files and introducing additional customization features.
> ⚠️ **Note**: The `master` branch relies on private repositories owned by <u> [**M.Sc. Andrei Costinescu**](https://www.linkedin.com/in/andrei-costinescu/)</u>, for computer vision and obstacle detection. Consequently, **dynamic obstacle avoidance cannot be tested independently** by external users.

### ✅ Publicly Testable Branch: `master-shobhit`

This branch enables evaluation of:

- **Static obstacle avoidance**
- **Self-collision avoidance**

Both can be tested offline using the included visualization tools:

- [MATLAB-based visualization](./Matlab%20Visualisation)
- [Python-based visualization](./PythonVisualisation)

---

### 🚀 Real-Time Experiments on Franka Emika Panda

The real-time obstacle avoidance system was implemented in [`realTimeObstacleAvoidance.cpp`](executablesRealRobotFranka/realTimeObstacleAvoidance.cpp), which depends on the private `TaskrepresentationLib` repository owned by [**M.Sc. Andrei Costinescu**](https://www.linkedin.com/in/andrei-costinescu/).  
Due to these dependencies, the code cannot be compiled or tested externally — but you can view results below:

#### 🧪 Experiment 1 – Static Obstacle Avoidance:
Click on the image below to view the Experiement on youtube:

[![Experiment 1 – Static Obstacle Avoidance](https://img.youtube.com/vi/L97HPGtAQWk/hqdefault.jpg)](https://www.youtube.com/watch?v=L97HPGtAQWk)

#### 🧪 Experiment 2 – Dynamic Obstacle Avoidance:
Click on the image below to view the Experiement on youtube:

[![Experiment 2 – Dynamic Obstacle Avoidance](https://img.youtube.com/vi/izx8e6XRooc/0.jpg)](https://www.youtube.com/watch?v=izx8e6XRooc)


## 📄 Read the Master Thesis

To understand the complete implementation, the full master thesis document can be viewed here:

 [**Master Thesis PDF (Google Drive)**](https://drive.google.com/file/d/1uZbSs04RhBjDHDoSViISdTlmXnGgS7YP/view?usp=sharing)

> Note: The PDF is hosted externally due to GitHub file size limits.
---

## 📊 Presentation

The complete presentation—including detailed experiments—can be viewed online here:  
[**PowerPoint Presentation**](https://1drv.ms/p/c/dea76ca5570c8d33/ERC1VpDvPnpGh0Q68iaLugwBiAfvXDLN0amC-sXOPc1f3Q?e=Uk3dhX)

> The presentation is hosted on Microsoft OneDrive and can be viewed directly in the browser with full formatting and embedded content preserved.
## Feature Overview

| Feature                              | Description                                            | Testable by Public?                |
|--------------------------------------|--------------------------------------------------------|------------------------------------|
| Static Obstacle Avoidance            | Avoidance of known fixed obstacles                     | ✅ Yes (Offline Simulation)         |
| Self-Collision Avoidance             | Real-time detection of self-collision configurations   | ✅ Yes (Offline Simulation)         |
| Dynamic Obstacle Avoidance           | Real-time perception and avoidance of moving obstacles | ❌ No (Private Vision Dependencies) |
| Real-Time Proximity Computation      | Distance monitoring to nearby objects         | ✅Yes                               |
| Robot modeling with primitive shapes | Kinematic modeling with modified Denavit–Hartenberg parameters            | ✅ Yes                              |

---
## 📦 Dependencies

To build and run this project, the following libraries and tools are required:

| Dependency                  | Description                                           | Installation / Link                                                                               |
|-----------------------------|-------------------------------------------------------|---------------------------------------------------------------------------------------------------|
| **Eigen**                   | Linear algebra library                                | [eigen.tuxfamily.org](https://eigen.tuxfamily.org)                                                |
| **Franka Emika Library**    | Robot SDK for Franka Emika Panda                      | [frankaemika.github.io](https://frankaemika.github.io/docs/index.html)                            |
| **FCL** (Flexible Collision Library) | Collision checking library                            | [github.com/flexible-collision-library/fcl](https://github.com/flexible-collision-library/fcl)    |
| **libccd**                  | Collision detection (used with FCL)                   | [github.com/danfis/libccd](https://github.com/danfis/libccd)                                      |
| **PCL** (Point Cloud Library)       | Only `common` module required for Eigen compatibility | `sudo apt install libpcl-dev` (Terminal)                                                    |
| **OpenMP**                  | For parallel computing support                        | `sudo apt install libomp-dev` (Terminal)                                                          |
| **pthread**                 | POSIX threads (usually preinstalled on Linux)         | —                                                                                                 |
| **AndreiUtils**             | All Utilities that use `eigen`, `json`, and `python`    | [bitbucket.org/andreicostinescu/andreiutils](https://bitbucket.org/andreicostinescu/andreiutils/src/main/) |



---
## Building the Library

1. **Clone the repository:**
    ```
    git clone https://github.com/showbhitjain/TUM_SJ_ObstacleAvoidanceLib.git
    ```

2. **Build instructions:**
    ```
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_INSTALL_PREFIX=/your/custom/install/path  # optional
    make -j
    sudo make install
    ```

By default, the library will be installed to `/usr/local`.  
To change the installation location, a custom path can be set using `-DCMAKE_INSTALL_PREFIX`.  
For development or debugging purposes, `-DCMAKE_BUILD_TYPE=Debug` can be specified instead of `Release`.

---
## Usage in other (Cmake) Projects
In your project's CMakeLists.txt:
```
option(WITH_PCL_OPTIMIZATION_FLAGS "Use optimization flags used by PCL to compile a compatible interface to EIGEN types" ON)
set(PCL_OPTIMIZATION_FLAGS)
if (${WITH_PCL_OPTIMIZATION_FLAGS})
    find_package(PCL REQUIRED COMPONENTS common)
    get_target_property(OUTPUT pcl_common INTERFACE_COMPILE_OPTIONS)
    set(PCL_OPTIMIZATION_FLAGS ${OUTPUT})
    message(STATUS "PCL optimization flags = ${PCL_OPTIMIZATION_FLAGS}")
endif ()

find_package(AndreiUtils REQUIRED COMPONENTS eigen json python)
find_package(Franka REQUIRED)
find_package(OpenMP REQUIRED)
find_package(TUM_SJ_ObstacleAvoidanceLib REQUIRED PATHS "/<your custom install path>/lib/cmake/TUM_SJ_ObstacleAvoidanceLib")
message("ObstacleAvoidance include dirs are ${TUM_SJ_ObstacleAvoidanceLib_INCLUDE}")
message("ObstacleAvoidance libraries are ${TUM_SJ_ObstacleAvoidanceLib_LIBRARY}")

add_executable(TestObstacleAvoidance main.cpp)
target_link_libraries(TestObstacleAvoidance TUM_SJ_ObstacleAvoidanceLib::TUM_SJ_ObstacleAvoidanceLib)
```


There's no need for including the include directories of `TUM_SJ_ObstacleAvoidanceLib` explicitly; by linking to `${TUM_SJ_ObstacleAvoidanceLib}` the include directories are automatically set correctly. There is no need to mention required path in find_package if the library was installed by default to `/usr/local`. `PCL_OPTIMIZATION_FLAGS` are required to ensure compatibility with the `AndreiUtils` library, as it relies on the same Eigen alignment and optimization settings used by PCL.

             
   
---

[//]: # ()
[//]: # (## 🔧 Tech Stack)

[//]: # ()
[//]: # (- **C++17** with modular architecture)

[//]: # (- **CMake** for building and dependency management)

[//]: # (- **MATLAB / Python** for simulation and visualization)

[//]: # (- **Eigen**, **nlohmann/json**, and other standard libraries)

---

#  Running Experiments & Visualizing Results
Detailed instructions and  parameter explanations for running experiments and visualizing joint trajectories are provided in this section.
This includes usage of `.json `configuration files, and both MATLAB and Python visualization tools.

To perform experiments with different configurations, it is recommended to read the [master thesis](https://drive.google.com/file/d/1uZbSs04RhBjDHDoSViISdTlmXnGgS7YP/view?usp=sharing) and review the [presentation](https://1drv.ms/p/c/dea76ca5570c8d33/ERC1VpDvPnpGh0Q68iaLugwBiAfvXDLN0amC-sXOPc1f3Q?e=Uk3dhX) for context—especially the experimental setup.
### 🔧 Configuration Files

All runtime parameters can be set via JSON files located in:  
📁 [`./config/configDemo`](./config/configDemo)

These include robot settings, obstacle definitions, trajectory setup, and inverse kinematics configuration. A brief explanation of each file is provided below.


#### 🦿 `pandaRobot.json` — Robot Configuration

| Key                        | Description                                                                                                                                                         |
|---------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `jointPositionAtBeginning` | Initial joint angles (set before starting the trajectory)                                                                                                           |
| `distanceActivate`        | Outer activation threshold \(d2\) for obstacle avoidance                                                                                                            |
| `distanceStop`            | Inner stop threshold \(d1\) to detect collisions                                                                                                                    |
| `smootheningCoefficient`  | Smoothing coefficient k, which modulates the deceleration profile, and the distance threshold d0, which defines the activation range of the obstacle avoidance logic |
| `radiusLinks`             | Radii for all `a` and `d` segments in the robot model                                                                                                               |
| `radiusJoints`            | Radii for spherical joints at the start of each link segment                                                                                                        |
| `finalLinkDimensions`     | Dimensions of the box representing the gripper or end-effector                                                                                                      |
| `considerFinalLinkOA`     | Whether the final link (TCP to tool) is considered in obstacle avoidance                                                                                            |
| `considerBaseToFirstJoint`| Whether the base to first joint is considered in avoidance                                                                                                          |
| `selfCollision`           | Enable/disable self-collision avoidance                                                                                                                             |
| `convergenceBetaDynamicObstacleAvoidance` | \( β \)-weight for balancing dynamic vs. static obstacle avoidance                                                                                              |

---

#### 🧱 `Obstacles.json` — Obstacle Configuration

This file allows manual registration of all obstacles.  
Only three primitive shapes are supported:
- Sphere
- Box
- Cylinder

> 💡 Refer to the "Geometry" chapter of the thesis for the mathematical representation of these shapes.

---

#### 🧭 `trajectoryConfig.json` — Trajectory Setup

| Key                        | Description                                                   |
|---------------------------|---------------------------------------------------------------|
| `Waypoints`               | 3D waypoints in column-wise `[x, y, z]` format                |
| `orientations`            | Desired orientations as quaternions `[w, x, y, z]` per column |
| `preTrajectoryTime`       | Duration to move from current TCP pose to first waypoint      |
| `positionTrajectoryType`  | Interpolation method (e.g., `"cubic"`, `"quintic"`)           |
| `orientationTrajectoryType`| Timescaling type (e.g., `"cubic"`)                            |

The trajectory is generated using generated C++ Code of MATLAB's interpolation functions for position and orientation:
- [`trapveltraj`](https://www.mathworks.com/help/robotics/ref/trapveltraj.html)
- [`cubicpolytraj`](https://www.mathworks.com/help/robotics/ref/cubicpolytraj.html)
- [`quinticpolytraj`](https://www.mathworks.com/help/robotics/ref/quinticpolytraj.html)
- [`rottraj`](https://www.mathworks.com/help/robotics/ref/rottraj.html)

---
To perform Tool Center Point (TCP) trajectory interpolation based on specified waypoints and orientations, C++ code was generated from the MATLAB functions [`positionTrajectory.m`](https://github.com/showbhitjain/Matlab_implementation/blob/master-shobhit/functions_for_codegen/Trajectory/positionTrajectory.m) and [`orientationTrajectory.m`](https://github.com/showbhitjain/Matlab_implementation/blob/master-shobhit/functions_for_codegen/Trajectory/orientationTrajectory.m), available in the [Matlab_implementation](https://github.com/showbhitjain/Matlab_implementation) repository.

In this repository, the functions `positionTrajectory` and `orientationTrajectory`, implemented in [`CartesianTrajectory.cpp`](src/CartesianTrajetory.cpp), internally invoke the generated code to compute the desired TCP motion profiles.

- The `positionTrajectory` function returns:
    - Desired TCP positions
    - Desired TCP velocities
    - Desired TCP accelerations

- The `orientationTrajectory` function returns:
    - Desired TCP orientations (as quaternions)
    - Desired TCP angular velocities
    - Desired TCP angular accelerations
#### 🤖 `inverseKinematicsWithOAParameters.json` — Optimization & OA Settings

This configuration file defines objective terms and constraint settings used by the optimization-based inverse kinematics (IK) solver with obstacle avoidance.

| Key                          | Description                                                             |
|------------------------------|-------------------------------------------------------------------------|
| `applyInequalityConstraints` | Enables obstacle avoidance constraints. Set to `false` to disable OA.   |
| `applyEqualityConstraints`   | Enables task-space tracking (i.e., trajectory following).               |
| `dynamicSlack`               | Enables dynamic slack for obstacle Avoidance.                           |
| `forObstacleAvoidance`       | Specifies whether slack terms are used specifically for OA constraints. |
| `applySlack`                 | Enables slack.                                                          |
| `SlackLowerBound, SlackUpperBound`     | Limits the slack search space.                                          |
| `SlackObjectiveWeight`     | Cost multiplier for slack variables in objective.                       |
| `SlackPenaltyWeight`     | Penalizes each slack variable direction differently.                    |


>  To use dynamic slack behavior for obstacle avoidance, both `dynamicSlack: true` and `forObstacleAvoidance: true` must be set.

---


#### 🎯 Objective Terms: `ObjectiveConstraints`

These cost terms shape the optimization behavior by weighting various robot motion criteria. The available objectives (from the thesis) are:

| Objective                          | Expression                           | Description                                                     |
|-----------------------------------|--------------------------------------|-----------------------------------------------------------------|
| `useObjectiveNormInfinity`        | (1/2) · ‖q̇‖∞²             | Minimizes the largest joint velocity (infinity norm).           |
| `useObjectiveNormL2`              | (1/2) · q̇ᵀ · W · q̇            | Weighted quadratic velocity minimization.                       |
| `useObjectiveTrajectoryFollowing` | (1/2) · ‖J·q̇ − ẋ_desired‖²         | Minimizes error in end-effector velocity.                       |
| `useObjectiveJointAcceleration`   | Σ (q̇ − q̇_prev)²            | Penalizes sharp changes in joint velocity (acceleration).       |
| `useObjectiveManipulability`      | Jₘᵀ(θ) · q̇             | Maximizes manipulability         |

Each term has an associated `weight...` key in the JSON for tuning its contribution.

Example:
```json
"ObjectiveConstraints": {
  "useObjectiveNormInfinity": false,
  "weightNormInfinity": 0.1,
  "useObjectiveNormL2": true,
  "weightNormL2": 10,
  "useObjectiveJointAcceleration": true,
  "weightJointAcceleration": 10,
  "useObjectiveTrajectoryFollowing": false,
  "weightTrajectoryFollowing": 5,
  "useObjectiveManipulability": false,
  "weightManipulability": 0.1
}
```        


To solve the underlying optimization problem, the C++ code was generated from the MATLAB function file [`inverseKinematicsOAModified.m`](https://github.com/showbhitjain/Matlab_implementation/blob/master-shobhit/functions_for_codegen/obstacleAvoidanceOptmisationModified/inverseKinematicsOAModified.m) in the [Matlab_implementation](https://github.com/showbhitjain/Matlab_implementation) repository. The list of supported objective functions used in the solver can be found in [`configurableObjective.m`](https://github.com/showbhitjain/Matlab_implementation/blob/master-shobhit/functions_for_codegen/obstacleAvoidanceOptmisationModified/configurableObjective.m).

These files may be edited by the user to modify or extend the objective formulations or adjust settings for code generation. To regenerate the C++ implementation from MATLAB, the project file [`inverseKinematicsOAModified.prj`](https://github.com/showbhitjain/Matlab_implementation/blob/master-shobhit/functions_for_codegen/obstacleAvoidanceOptmisationModified/inverseKinematicsOAModified.prj) can be opened using MATLAB Coder.

In this repository, the C++ function `inverseKinematicsWithOA` defined in [`OptimizationBasedIKWithOA.cpp`](src/OptimizationBasedIKWithOA.cpp) internally calls the generated code to compute the optimal joint velocities under the specified objective and constraint settings.

### ▶️ Running the C++ Executable

After setting the configuration files, run the following C++ executable:

```bash
./ObstacleAvoidanceTest
```
This will:

- Run obstacle avoidance test file in offline mode.

- Store resulting joint positions trajectory in
     [outputDemo/robotJointPositions.csv](./outputDemo/robotJointPositions.csv)

### 📈 Visualizing the Resulting Trajectory

Two options are available for visualizing the robot’s motion:

#### Option 1: MATLAB Visualization (Recommended)
Run [visualiseOutputFileNew.m](./Matlab%20Visualisation/visualiseOutputFileNew.m) in Matlab. This script will read the generated CSV file and visualize the robot trajectory with full 3D model support of Franka Emika Panda.

Requirements:
- [Robotics Systems Toolbox](https://de.mathworks.com/help/robotics/)
- [Robot Library Data Support Package](https://de.mathworks.com/help/robotics/ug/install-robotics-system-toolbox-robot-library-data-support-package.html)

**How to Run:**
```bash
run('./Matlab Visualisation/visualiseOutputFileNew.m')
```
#### Sample Output:
Click below to view an example of the MATLAB-based visualization:

[![Static Obstacle Avoidance Matlab Visualization](https://img.youtube.com/vi/Ghk_7c2lYGY/0.jpg)](https://youtu.be/Ghk_7c2lYGY)

#### Option 2: Python Visualization
This uses a geometric model (not the full Franka CAD) and simulates the trajectory using the modeled links and spheres but is sufficient to analyze motion and obstacle interaction. Run [visualise.py](./PythonVisualisation/visualise.py) in python.

Dependencies for python visualisation are listed in [**requirements.txt**](./PythonVisualisation/requirements.txt)

#### Sample Output:
Click below to view an example of the Python-based visualization:

[![Static Obstacle Avoidance Python Visualization](https://img.youtube.com/vi/NStOrQsUJmM/0.jpg)](https://youtu.be/NStOrQsUJmM)


### Quick Summary Table 
| Task                                                                   | Tool / File                                                                     |
|------------------------------------------------------------------------| ------------------------------------------------------------------------------- |
| **1.** Configure robot, obstacles avoidance, and trajectory parameters | JSON files in [`configDemo`](./config/configDemo)                               |
| **2.** Run trajectory execution                                        | `./ObstacleAvoidanceTest` (C++ executable)                                      |
| **3.**  Output joint data                                              | [`robotJointPositions.csv`](./outputDemo/robotJointPositions.csv)               |
| **4.** MATLAB visualization                                            | [`visualiseOutputFileNew.m`](./Matlab%20Visualisation/visualiseOutputFileNew.m) |
| **4.** Python visualization                                            | [`visualise.py`](./PythonVisualisation/visualise.py)                            |


[//]: # (In  **[Obstacles.json]&#40;./config/configDemo/Obstacles.json&#41;**)

[//]: # ()
[//]: # ()
[//]: # (file all obstacles can be manually registered. Only three primitive shapes are considered for representing obstacles and their registration as explained in Master thesis in Geometry chapter.)

[//]: # (**[pandaRobot.json]&#40;./config/configDemo/pandaRobot.json&#41;** containes robot related information. "jointPositionAtBeginning": this parameter is important to set the current joint position of the robot before starting of trajectory for offline tests. The important parameters for obstacle avoidance experiments are)

[//]: # ("distanceActivate": d_{2} outer threshold distance when obstacle avoidance gets activated  ,)

[//]: # ("distanceStop": {d_1} inner distance threhold &#40;if distance is less than this, then it is a collision between robot link and obstacle&#41;,)

[//]: # ()
[//]: # ("smootheningCoefficient": smmothening coefficient k value from  obstacle avoidance equation in order to set distance threhold d_0 for deceleration in critical zone,)

[//]: # ("radiusLinks": radius of all a segments and d segments used for robot modeling as shown in robot modeling in master,)

[//]: # ("radiusJoints": radius of all spheres used to represent starting of a and d link segments,)

[//]: # ()
[//]: # ()
[//]: # ("finalLinkDimensions": dimensions of Box used to geometrically approximate Gripper ,)

[//]: # ("considerFinalLinkOA": parameter to consider gripper or link from end effector to TCP for obstacle avoidance,)

[//]: # ("considerBaseToFirstJoint": parameter to consider robot link from base to first joint for obstacle avoidance and proximity calculations,)

[//]: # ()
[//]: # ("selfCollision": for turning self collision on and off,)

[//]: # ("convergenceBetaDynamicObstacleAvoidance": β parameter from complete obstacle avoidance equation to set priority of static obstacle avoidance and dynamic obstacle avoidance)

[//]: # ()
[//]: # (In   **[trajectoryConfig.json]&#40;./config/configDemo/trajectoryConfig.json&#41;**, the TCP desired trajectory parameters can be set, where )

[//]: # (s)

[//]: # ("Waypoints": waypoints are set in column wise manner &#40;each column represents a waypoint with  x,y,z coordinates and &#41;,)

[//]: # ("orientations": ame for orientation where each orientations are defined as quaternions where each column represent desired quaternion orientation with w,x,y,z attributes.)

[//]: # ()
[//]: # (for trajectory generation C++ generated code of matlab functions such as [rottraj]&#40;https://de.mathworks.com/help/robotics/ref/rottraj.html&#41; &#40;for orientation&#41; and [trapveltraj]&#40;https://de.mathworks.com/help/robotics/ref/trapveltraj.html&#41;, [cubicpolytraj]&#40;https://de.mathworks.com/help/robotics/ref/cubicpolytraj.html&#41;, [quinticpolytraj]&#40;https://de.mathworks.com/help/robotics/ref/quinticpolytraj.html&#41; for position trajectory, was used.)

[//]: # ("preTrajectoryTime": sets the time that should be taken to get to the first waypoint from current TCP position before starting of trajectory ,)

[//]: # ("positionTrajectoryType": "cubic" sets the interpolation between waypoints and results in cubic spline interpolation,)

[//]: # ("orientationTrajectoryType" : "cubic" sets the type of timescaling,)

[//]: # ()
[//]: # (the optimization based inverse kinematics parameters can be set in **[inverseKinematicsWithOAParameters.json]&#40;./config/configDemo/inverseKinematicsWithOAParameters.json&#41;**)

[//]: # (and also which objective functions should be minimized. )

[//]: # (Here by turning off inequality constraints &#40;"applyInequalityConstraints": false&#41;, the obstacle avoidance is also turned off and only optimisation based inverse kinemtics takes place.)

[//]: # ()
[//]: # (Both "dynamicSlack": true,)

[//]: # ("forObstacleAvoidance": true, must be set to true in order to use dynamic slack feature for obstacle avoidance.)

[//]: # ()
[//]: # ()
[//]: # (Simply run the executable **ObstacleAvoidanceTest** generated from file [obstacleAvoidanceTest.cpp]&#40;./executables/obstacleAvoidanceTest.cpp&#41; can be tested after setting all the configuration parameters. Different experiments can be performed and tested for franka using this same file for different configuration parametersexecutable . The resulting joints trajectory of the robot will be stored in )

[//]: # ([outputDemo/robotJointPositions.csv]&#40;./outputDemo/robotJointPositions.csv&#41;. In order to visualise this trajectrory two options are avalaible: Python and Matlab &#40;recommended or better visualisation&#41; visualisation :)

[//]: # ()
[//]: # (For matlab visualisation these toolboxes must be installed: )

[//]: # (-[Robotics Systems Toolbox]&#40;https://de.mathworks.com/help/robotics/&#41;)

[//]: # (-[Robot Library Data Support Package]&#40;https://de.mathworks.com/help/robotics/ug/install-robotics-system-toolbox-robot-library-data-support-package.html&#41;)

[//]: # ()
[//]: # (Just run [visualiseOutputFileNew.m]&#40;./Matlab%20Visualisation/visualiseOutputFileNew.m&#41; in Matlab order to visualise joint trajectory saved in outputDemo.)

[//]: # ()
[//]: # (Python visualisation does not show the 3d model of franka emika panda but shows only the robot modeling implemented within the scope of this master thesis and is not as smooth as matlab but still good enough to visualise the trajectory.)

[//]: # ()
[//]: # (Dependencies for python visualisation are listed in [**requirements.txt**]&#40;./PythonVisualisation/requirements.txt&#41;)
