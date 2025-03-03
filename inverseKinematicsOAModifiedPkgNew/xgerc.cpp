//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgerc.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "xgerc.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int m
//                int n
//                double alpha1
//                int ix0
//                const array<double, 1U> &y
//                array<double, 2U> &A
//                int ia0
//                int lda
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void xgerc(int m, int n, double alpha1, int ix0, const array<double, 1U> &y,
           array<double, 2U> &A, int ia0, int lda)
{
  if (!(alpha1 == 0.0)) {
    int jA;
    jA = ia0;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int j{0}; j < n; j++) {
      if (y[j] != 0.0) {
        double temp;
        int b;
        temp = y[j] * alpha1;
        b = (m + jA) - 1;
        if ((jA <= b) && (b > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int ijA{jA}; ijA <= b; ijA++) {
          A[ijA - 1] = A[ijA - 1] + A[((ix0 + ijA) - jA) - 1] * temp;
        }
      }
      jA += lda;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xgerc.cpp
//
// [EOF]
//
