//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ixfun.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

#ifndef IXFUN_H
#define IXFUN_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
void expand_max(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c);

void expand_min(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c);

} // namespace internal
} // namespace coder

#endif
//
// File trailer for ixfun.h
//
// [EOF]
//
