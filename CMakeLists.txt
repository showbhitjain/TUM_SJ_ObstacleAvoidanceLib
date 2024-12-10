cmake_minimum_required(VERSION 3.27)
project(TUM_SJ_ObstacleAvoidanceLib)

set(PROJECT_VERSION_STRING "1.0.0")
set(PROJECT_TARGET ${PROJECT_NAME})
set(PROJECT_INCLUDE_PREFIX ${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif ()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASE)
if (${CMAKE_BUILD_TYPE_LOWERCASE} MATCHES release)
    SET(CMAKE_CXX_FLAGS "-O2")
endif ()

option(WITH_PCL_OPTIMIZATION_FLAGS "Use optimization flags used by PCL to compile a compatible interface to EIGEN types" ON)
set(PCL_OPTIMIZATION_FLAGS)
if (${WITH_PCL_OPTIMIZATION_FLAGS})
    find_package(PCL REQUIRED COMPONENTS common)
    get_target_property(OUTPUT pcl_common INTERFACE_COMPILE_OPTIONS)
    set(PCL_OPTIMIZATION_FLAGS ${OUTPUT})
    message(STATUS "PCL optimization flags = ${PCL_OPTIMIZATION_FLAGS}")
endif ()

add_definitions(-DIL_STD)
add_definitions(-DNON_MATLAB_PARSING)
add_definitions(-DMAX_EXT_API_CONNECTIONS=255)
add_definitions(-DDO_NOT_USE_SHARED_MEMORY)
find_package(Eigen REQUIRED)
find_package(AndreiUtils REQUIRED COMPONENTS eigen json python)
find_package(PerceptionData REQUIRED)
find_package(Franka REQUIRED)

set(PATHTRAJECTORY "${CMAKE_CURRENT_SOURCE_DIR}/trajectory_staticlib")
set(PATHIK "${CMAKE_CURRENT_SOURCE_DIR}/inverseKinematicsPkg")
set(PATHUTILSMATLAB "${CMAKE_CURRENT_SOURCE_DIR}/jointValuesUtilPkg")

include_directories(include private_include ${PATHTRAJECTORY} ${PATHIK} ${PATHUTILSMATLAB})

message(STATUS "Including Inverse Kinematics Package from ${PATHIK}")





set(LIB_NAME ${PROJECT_TARGET})
add_library(${LIB_NAME}
        src/Robot.cpp
        src/Joints.cpp
        src/utils.cpp
        src/cartesianTrajetory.cpp
        src/inverseKinematics.cpp
        privateSrc/privateUtils.cpp

        src/utilsJointValuesMatlab.cpp

        src/ObstacleAvoidanceUtils.cpp

        src/Obstacles.cpp

        src/controllerFranka.cpp
        src/examples_common.cpp


)

set(LIBRARIES_SOURCES ${AndreiUtils_LIBRARY} dqrobotics dqrobotics-interface-vrep pthread Eigen3::Eigen
        ${PATHTRAJECTORY}/trajectory_generator.a
        ${PATHIK}/inverseKinematics.a ${PATHUTILSMATLAB}/utilsMatlab.a Franka::Franka)


target_compile_options(${LIB_NAME} PUBLIC ${PCL_OPTIMIZATION_FLAGS})

target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(TUM_SJ_main src/main.cpp)
add_executable(ControllerExperiments src/frankaControllerExperiments.cpp)

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    target_link_libraries(TUM_SJ_ObstacleAvoidanceLib ${LIBRARIES_SOURCES} OpenMP::OpenMP_CXX)
    target_link_libraries(TUM_SJ_main ${LIB_NAME})
    target_link_libraries(ControllerExperiments ${LIB_NAME})
endif ()


set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})



