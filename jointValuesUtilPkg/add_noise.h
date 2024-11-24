//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: add_noise.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

#ifndef ADD_NOISE_H
#define ADD_NOISE_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void add_noise(const coder::array<double, 1U> &joint_vector,
                      double noise_min, double noise_max,
                      coder::array<double, 1U> &noisy_joint_vector);

#endif
//
// File trailer for add_noise.h
//
// [EOF]
//
