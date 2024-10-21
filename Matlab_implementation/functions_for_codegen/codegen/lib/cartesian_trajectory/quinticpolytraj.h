//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quinticpolytraj.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

#ifndef QUINTICPOLYTRAJ_H
#define QUINTICPOLYTRAJ_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void quinticpolytraj(const array<double, 2U> &wayPoints,
                     array<double, 2U> &timePoints, const array<double, 2U> &t,
                     const array<double, 2U> &varargin_2,
                     const array<double, 2U> &varargin_4, array<double, 2U> &q,
                     array<double, 2U> &qd, array<double, 2U> &qdd);

}

#endif
//
// File trailer for quinticpolytraj.h
//
// [EOF]
//
