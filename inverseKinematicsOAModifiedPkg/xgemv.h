//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemv.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
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
void b_xgemv(int m, int n, const array<double, 1U> &A, int lda,
             const array<double, 1U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 2U> &y);

void xgemv(int m, int n, const array<double, 2U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, int ix0, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y);

void xgemv(int m, int n, const array<double, 2U> &A, int lda,
           const array<double, 1U> &x, array<double, 2U> &y);

void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, int ix0, array<double, 2U> &y);

} // namespace blas
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xgemv.h
//
// [EOF]
//
