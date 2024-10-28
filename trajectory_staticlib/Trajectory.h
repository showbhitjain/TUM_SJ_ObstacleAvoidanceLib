//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: Trajectory.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
namespace trajectoryGeneration {
    class Trajectory {
    public:
        Trajectory();

        ~Trajectory();

        void orientationTrajectory(const ::coder::array<double, 2U> &orientations,
                                   const ::coder::array<double, 2U> &waypointTimes,
                                   double ts, const ::coder::array<char, 2U> &trajType,
                                   ::coder::array<double, 2U> &desired_quaternions,
                                   ::coder::array<double, 2U> &desired_angular_velocity,
                                   ::coder::array<double, 2U> &desired_angular_accel);

        void positionTrajectory(const ::coder::array<double, 2U> &waypoints,
                                const ::coder::array<double, 2U> &waypointTimes,
                                double ts, const ::coder::array<char, 2U> &trajType,
                                ::coder::array<double, 2U> &waypointVels,
                                ::coder::array<double, 2U> &waypointAccels,
                                ::coder::array<double, 2U> &xd,
                                ::coder::array<double, 2U> &xd_vel,
                                ::coder::array<double, 2U> &xdd);
    };

} // namespace trajectoryGeneration

#endif
//
// File trailer for Trajectory.h
//
// [EOF]
//
