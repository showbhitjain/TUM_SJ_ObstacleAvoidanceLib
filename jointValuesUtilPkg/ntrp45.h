//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ntrp45.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

#ifndef NTRP45_H
#define NTRP45_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void ntrp45(const double t[3], double t0, const array<double, 1U> &b_y0,
            double h, const array<double, 2U> &f, array<double, 2U> &y);

}

#endif
//
// File trailer for ntrp45.h
//
// [EOF]
//
