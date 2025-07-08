# 🦾 Obstacle Avoidance for Serial Chain Manipulators (Master Thesis)

This repository presents my Master’s thesis work on obstacle avoidance algorithms and robot modeling for serial-chain robotic manipulators. The algorithms are implemented in modern C++ and validated on the **Franka Emika Panda** platform.

> ⚠️ **Note**: The `master` branch depends on private repositories owned by <u> [**M.Sc. Andrei Costinescu**](https://www.linkedin.com/in/andrei-costinescu/)</u>, for computer vision and obstacle detection. As a result, **dynamic obstacle avoidance cannot be tested independently** by external users.

The `master-shobhit` branch includes standalone modules for static obstacle avoidance and self-collision avoidance, which can be tested offline using MATLAB or Python-based visualization tools developed for this project.


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

| Dependency                  | Description                                             | Installation / Link                                                                               |
|-----------------------------|---------------------------------------------------------|---------------------------------------------------------------------------------------------------|
| **Eigen**                   | Linear algebra library                                  | [eigen.tuxfamily.org](https://eigen.tuxfamily.org)                                                |
| **Franka Emika Library**    | Robot SDK for Franka Emika Panda                        | [frankaemika.github.io](https://frankaemika.github.io/docs/index.html)                            |
| **FCL** (Flexible Collision Library) | Collision checking library                     | [github.com/flexible-collision-library/fcl](https://github.com/flexible-collision-library/fcl)    |
| **libccd**                  | Collision detection (used with FCL)                     | [github.com/danfis/libccd](https://github.com/danfis/libccd)                                      |
| **PCL** (Point Cloud Library)       | Only `common` module required for Eigen compatibility | `sudo apt install libpcl-dev` (Terminal)                                                    |
| **OpenMP**                  | For parallel computing support                          | `sudo apt install libomp-dev` (Terminal)                                                          |
| **pthread**                 | POSIX threads (usually preinstalled on Linux)           | —                                                                                                 |
| **AndreiUtils**             | Utilities for `eigen`, `json`, and `python`             | [bitbucket.org/andreicostinescu/andreiutils](https://bitbucket.org/andreicostinescu/andreiutils/src/main/) |



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
find_package(TUM_SJ_ObstacleAvoidanceLib REQUIRED PATHS "/<your custom install path>/lib/cmake/TUM_SJ_ObstacleAvoidanceLib")
message("ObstacleAvoidance include dirs are ${TUM_SJ_ObstacleAvoidanceLib_INCLUDE}")
message("ObstacleAvoidance libraries are ${TUM_SJ_ObstacleAvoidanceLib_LIBRARY}")

add_executable(TestObstacleAvoidance main.cpp)
target_link_libraries(TestObstacleAvoidance TUM_SJ_ObstacleAvoidanceLib::TUM_SJ_ObstacleAvoidanceLib)
```


There's no need for including the include directories of `TUM_SJ_ObstacleAvoidanceLib` explicitly; by linking to `${TUM_SJ_ObstacleAvoidanceLib}` the include directories are automatically set correctly. There is no need to mention required path in find_package if the library was installed by default to `/usr/local`

             
   
---

[//]: # ()
[//]: # (## 🔧 Tech Stack)

[//]: # ()
[//]: # (- **C++17** with modular architecture)

[//]: # (- **CMake** for building and dependency management)

[//]: # (- **MATLAB / Python** for simulation and visualization)

[//]: # (- **Eigen**, **nlohmann/json**, and other standard libraries)

---

##  Examplen on how to run the test:


<u>[Configuration Files](./config/configDemo)</u> in configDemo contain all the configuration parameters that can be set for different examples. The `.json`files can be editted with desired configuration parameters to set the experimental parameters.
In  **[Obstacles.json](./config/configDemo/Obstacles.json)**
file all obstacles can be manually registered. Only three primitive shapes are considered for representing obstacles and their registration as explained in Master thesis in Geometry chapter.
**[pandaRobot.json](./config/configDemo/pandaRobot.json)** containes robot related information. The important parameters for obstacle avoidance experiments are
"distanceActivate": d_{2} outer threshold distance when obstacle avoidance gets activated  ,
"distanceStop": {d_1} inner distance threhold (if distance is less than this, then it is a collision between robot link and obstacle),
"distanceBuffer": 0.005,
"smootheningCoefficient": smmothening coefficient k value from  obstacle avoidance equation in order to set distance threhold d_0 for deceleration in critical zone,
"radiusLinks": radius of all a segments and d segments used for robot modeling as shown in robot modeling in master,
"radiusJoints": radius of all spheres used to represent starting of a and d link segments,
"percentagePartialLinks": [50,50,50,50,50,50,50,50],
"finalLinkType": "Box",
"finalLinkDimensions": dimensions Box used to geometrically approximate Gripper ,
"considerFinalLinkOA": parameter to consider gripper or link from end effector to TCP for obstacle avoidance,
"considerBaseToFirstJoint": parameter to consider robot link from base to first joint for obstacle avoidance and proximity calculations,
"robotLinkAsLineSwept": false,
"selfCollision": true,
"convergenceBetaDynamicObstacleAvoidance": β from complete obstacle avoidance equation

