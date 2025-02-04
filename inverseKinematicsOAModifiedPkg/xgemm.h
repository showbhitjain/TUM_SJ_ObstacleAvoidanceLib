//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemm.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef XGEMM_H
#define XGEMM_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace blas {
void xgemm(int m, int n, int k, const array<double, 2U> &A, int lda,
           const array<double, 2U> &B, int ib0, int ldb, array<double, 2U> &C,
           int ldc);

void xgemm(int m, int n, int k, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &B, int ldb, array<double, 2U> &C, int ldc);

void xgemm(int m, int k, const array<double, 2U> &A, int lda,
           const array<double, 2U> &B, int ldb, array<double, 2U> &C, int ldc);

} // namespace blas
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xgemm.h
//
// [EOF]
//
