//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsMatlabObstacleAvoidance.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef INVERSEKINEMATICSMATLABOBSTACLEAVOIDANCE_H
#define INVERSEKINEMATICSMATLABOBSTACLEAVOIDANCE_H

// Include Files
#include "inverseKinematicsOA_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class inverseKinematicsMatlabObstacleAvoidance {
public:
  inverseKinematicsMatlabObstacleAvoidance();
  ~inverseKinematicsMatlabObstacleAvoidance();
  void inverseKinematicsOA(
      const coder::array<double, 1U> &joint_values,
      const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
      const coder::array<double, 1U> &jointminvalues,
      const coder::array<double, 1U> &jointmaxvalues,
      const coder::array<double, 1U> &joint_min_vel,
      const coder::array<double, 1U> &joint_max_vel,
      const coder::array<double, 2U> &J_g, const coder::array<double, 1U> &b_g,
      const coder::array<double, 2U> &jointVelocityWeightMatrix,
      struct0_T *config, coder::array<double, 1U> &optimal_joint_velocity,
      double *Exit_Flag);
  inverseKinematicsOAStackData *getStackData();

private:
  inverseKinematicsOAPersistentData pd_;
  inverseKinematicsOAStackData SD_;
};

// Function Declarations
double inverseKinematicsOA_anonFcn1(
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    const struct0_T &config, const coder::array<double, 1U> &q_vel);

#endif
//
// File trailer for inverseKinematicsMatlabObstacleAvoidance.h
//
// [EOF]
//
