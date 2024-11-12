//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemv.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "xgemv.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 2U> &x
//                array<double, 1U> &y
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int i;
    int iy;
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    i = lda * (n - 1) + 1;
    for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
      double c;
      int i1;
      c = 0.0;
      i1 = (iac + m) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 2U> &x
//                int ix0
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int i;
    int iy;
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    i = lda * (n - 1) + 1;
    for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
      double c;
      int i1;
      c = 0.0;
      i1 = (iac + m) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        c += A[ia - 1] * x[((ix0 + ia) - iac) - 1];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int i;
    int iy;
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    i = lda * (n - 1) + 1;
    for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
      double c;
      int i1;
      c = 0.0;
      i1 = (iac + m) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xgemv.cpp
//
// [EOF]
//
