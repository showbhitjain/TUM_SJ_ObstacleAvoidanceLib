//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: Trajectory.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class Trajectory {
public:
  Trajectory();
  ~Trajectory();
  void cartesian_trajectory(const coder::array<double, 2U> &waypoints,
                            const coder::array<double, 2U> &waypointTimes,
                            double ts, const char trajType_data[],
                            const int trajType_size[2],
                            coder::array<double, 2U> &waypointVels,
                            coder::array<double, 2U> &waypointAccels,
                            coder::array<double, 2U> &xd,
                            coder::array<double, 2U> &xd_vel,
                            coder::array<double, 2U> &xdd);
};

#endif
//
// File trailer for Trajectory.h
//
// [EOF]
//
