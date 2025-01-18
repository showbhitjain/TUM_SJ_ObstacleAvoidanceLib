//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOA_internal_types1.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef INVERSEKINEMATICSOA_INTERNAL_TYPES1_H
#define INVERSEKINEMATICSOA_INTERNAL_TYPES1_H

// Include Files
#include "inverseKinematicsOA_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
struct c_struct_T {
  coder::array<double, 2U> jacobi;
  double xd_eff_vel[6];
  coder::array<double, 1U> starting_joint_vel;
  coder::array<double, 1U> joint_values;
  coder::array<double, 2U> jointVelocityWeightMatrix;
  struct0_T config;
};

#endif
//
// File trailer for inverseKinematicsOA_internal_types1.h
//
// [EOF]
//
