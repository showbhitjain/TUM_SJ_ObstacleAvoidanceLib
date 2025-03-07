//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemv.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

#ifndef XGEMV_H
#define XGEMV_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace blas {
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 2U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y);

} // namespace blas
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xgemv.h
//
// [EOF]
//
