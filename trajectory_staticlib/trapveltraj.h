//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: trapveltraj.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef TRAPVELTRAJ_H
#define TRAPVELTRAJ_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace trajectoryGeneration {
namespace coder {
void trapveltraj(double numSamples, double varargin_2, double varargin_4,
                 ::coder::array<double, 2U> &q, ::coder::array<double, 2U> &qd,
                 ::coder::array<double, 2U> &qdd);

void trapveltraj(const ::coder::array<double, 2U> &wayPoints, double numSamples,
                 const ::coder::array<double, 2U> &varargin_2,
                 const ::coder::array<double, 2U> &varargin_4,
                 ::coder::array<double, 2U> &q, ::coder::array<double, 2U> &qd,
                 ::coder::array<double, 2U> &qdd);

} // namespace coder
} // namespace trajectoryGeneration

#endif
//
// File trailer for trapveltraj.h
//
// [EOF]
//
