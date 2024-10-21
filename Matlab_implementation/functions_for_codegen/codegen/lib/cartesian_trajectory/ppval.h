//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ppval.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

#ifndef PPVAL_H
#define PPVAL_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct struct_T;

// Function Declarations
namespace coder {
void b_ppval(const array<double, 2U> &pp_breaks,
             const array<double, 3U> &pp_coefs, const array<double, 2U> &x,
             array<double, 2U> &v);

void ppval(const struct_T &pp, const array<double, 2U> &x,
           array<double, 2U> &v);

void ppval(const array<double, 2U> &pp_breaks,
           const array<double, 3U> &pp_coefs, const array<double, 2U> &x,
           array<double, 2U> &v);

} // namespace coder

#endif
//
// File trailer for ppval.h
//
// [EOF]
//
