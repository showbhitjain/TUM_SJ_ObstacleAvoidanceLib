//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: cubicpolytraj.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

#ifndef CUBICPOLYTRAJ_H
#define CUBICPOLYTRAJ_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace TrajectoryGeneration {
namespace coder {
void cubicpolytraj(const double timePoints[2],
                   const ::coder::array<double, 2U> &t,
                   ::coder::array<double, 2U> &q,
                   ::coder::array<double, 2U> &qd,
                   ::coder::array<double, 2U> &qdd);

}
} // namespace TrajectoryGeneration

#endif
//
// File trailer for cubicpolytraj.h
//
// [EOF]
//
