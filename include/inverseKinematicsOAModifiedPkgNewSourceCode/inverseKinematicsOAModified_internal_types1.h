//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_internal_types1.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

#ifndef INVERSEKINEMATICSOAMODIFIED_INTERNAL_TYPES1_H
#define INVERSEKINEMATICSOAMODIFIED_INTERNAL_TYPES1_H

// Include Files
#include "inverseKinematicsOAModified_types.h"
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
  boolean_T slackIsUsed;
};

#endif
//
// File trailer for inverseKinematicsOAModified_internal_types1.h
//
// [EOF]
//
