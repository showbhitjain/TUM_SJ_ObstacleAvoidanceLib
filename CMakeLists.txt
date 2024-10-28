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
find_package(Eigen3 REQUIRED)
find_package(AndreiUtils REQUIRED COMPONENTS eigen json python)
find_package(PerceptionData REQUIRED)

set(PATH "${CMAKE_CURRENT_SOURCE_DIR}/trajectory_staticlib")
message("Path: ${CMAKE_CURRENT_SOURCE_DIR}")
#include_directories()
include_directories(include private_include ${PATH})


set(LIB_NAME ${PROJECT_TARGET})
add_library(${LIB_NAME}
        src/Robot.cpp
        src/Joints.cpp
        src/utils.cpp
        src/cartesianTrajetory.cpp
        include/cartesianTrajetory.h
        private_include/privateUtils.hpp


)

set(LIBRARIES_SOURCES ${AndreiUtils_LIBRARY} dqrobotics dqrobotics-interface-vrep pthread Eigen3::Eigen)

target_link_libraries(${LIB_NAME} ${AndreiUtils_LIBRARY} dqrobotics dqrobotics-interface-vrep pthread Eigen3::Eigen ${PATH}/trajectory_generator.a)
target_compile_options(${LIB_NAME} PUBLIC ${PCL_OPTIMIZATION_FLAGS})

target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(TUM_SJ_main main.cpp)
target_link_libraries(TUM_SJ_main ${LIB_NAME})
target_link_libraries(TUM_SJ_ObstacleAvoidanceLib ${LIBRARIES_SOURCES})

set(ALL_INSTALL_TARGETS ${LIB_NAME} ${ALL_INSTALL_TARGETS})



