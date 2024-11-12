//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematics_internal_types1.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef INVERSEKINEMATICS_INTERNAL_TYPES1_H
#define INVERSEKINEMATICS_INTERNAL_TYPES1_H

// Include Files
#include "inverseKinematics_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
struct c_struct_T {
  coder::array<double, 2U> jacobi;
  coder::bounded_array<double, 6U, 1U> xd_eff_vel;
  coder::array<double, 1U> starting_joint_vel;
  coder::array<double, 1U> joint_values;
  struct0_T config;
};

#endif
//
// File trailer for inverseKinematics_internal_types1.h
//
// [EOF]
//
