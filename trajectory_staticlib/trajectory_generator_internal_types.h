//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: trajectory_generator_internal_types.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef TRAJECTORY_GENERATOR_INTERNAL_TYPES_H
#define TRAJECTORY_GENERATOR_INTERNAL_TYPES_H

// Include Files
#include "rtwtypes.h"
#include "trajectory_generator_types.h"
#include "coder_array.h"

// Type Definitions
namespace trajectoryGeneration {
struct struct_T {
  double breaks[6];
  double coefs[15];
};

struct b_struct_T {
  double breaks[4];
  double coefs[12];
};

struct c_struct_T {
  double breaks[4];
  double coefs[18];
};

struct d_struct_T {
  ::coder::array<double, 2U> breaks;
  ::coder::array<double, 3U> coefs;
};

} // namespace trajectoryGeneration

#endif
//
// File trailer for trajectory_generator_internal_types.h
//
// [EOF]
//
