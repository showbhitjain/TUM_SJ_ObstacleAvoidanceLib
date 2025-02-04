//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xpotrf.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "xpotrf.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int n
//                array<double, 2U> &A
//                int lda
// Return Type  : int
//
namespace coder {
namespace internal {
namespace lapack {
int xpotrf(int n, array<double, 2U> &A, int lda)
{
  int info;
  int j;
  boolean_T exitg1;
  info = 0;
  if (n > 2147483646) {
    check_forloop_overflow_error();
  }
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= n - 1)) {
    double c;
    double ssq;
    int idxA1j;
    int idxAjj_tmp;
    int nmj;
    idxA1j = j * lda;
    idxAjj_tmp = idxA1j + j;
    ssq = 0.0;
    if (j >= 1) {
      if (j > 2147483646) {
        check_forloop_overflow_error();
      }
      for (nmj = 0; nmj < j; nmj++) {
        c = A[idxA1j + nmj];
        ssq += c * c;
      }
    }
    ssq = A[idxAjj_tmp] - ssq;
    if (ssq > 0.0) {
      ssq = std::sqrt(ssq);
      A[idxAjj_tmp] = ssq;
      if (j + 1 < n) {
        int b;
        int ia0;
        int idxAjjp1;
        nmj = (n - j) - 2;
        ia0 = (idxA1j + lda) + 1;
        idxAjjp1 = (idxAjj_tmp + lda) + 1;
        if (j > 0) {
          if ((idxA1j + 1 <= idxAjj_tmp) && (idxAjj_tmp > 2147483646)) {
            check_forloop_overflow_error();
          }
          if (nmj + 1 != 0) {
            int iy;
            iy = idxAjjp1 - 1;
            b = ia0 + lda * nmj;
            if ((ia0 <= b) && (b > MAX_int32_T - lda)) {
              check_forloop_overflow_error();
            }
            for (int iac{ia0}; lda < 0 ? iac >= b : iac <= b; iac += lda) {
              int b_b;
              c = 0.0;
              b_b = (iac + j) - 1;
              if ((iac <= b_b) && (b_b > 2147483646)) {
                check_forloop_overflow_error();
              }
              for (int ia{iac}; ia <= b_b; ia++) {
                c += A[ia - 1] * A[(idxA1j + ia) - iac];
              }
              A[iy] = A[iy] - c;
              iy += lda;
            }
          }
        }
        if ((j > 0) &&
            ((idxA1j + 1 <= idxAjj_tmp) && (idxAjj_tmp > 2147483646))) {
          check_forloop_overflow_error();
        }
        ssq = 1.0 / ssq;
        b = idxAjjp1 + lda * nmj;
        if ((idxAjjp1 <= b) && (b > MAX_int32_T - lda)) {
          check_forloop_overflow_error();
        }
        for (nmj = idxAjjp1; lda < 0 ? nmj >= b : nmj <= b; nmj += lda) {
          A[nmj - 1] = ssq * A[nmj - 1];
        }
      }
      j++;
    } else {
      A[idxAjj_tmp] = ssq;
      info = j + 1;
      exitg1 = true;
    }
  }
  return info;
}

} // namespace lapack
} // namespace internal
} // namespace coder

//
// File trailer for xpotrf.cpp
//
// [EOF]
//
