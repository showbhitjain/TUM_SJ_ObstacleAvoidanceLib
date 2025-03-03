//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzlarf.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "xzlarf.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xgerc.h"
#include "coder_array.h"
#include <cstring>

// Function Declarations
namespace coder {
namespace internal {
namespace reflapack {
static int ilazlc(int m, int n, const array<double, 2U> &A, int ia0, int lda);

}
} // namespace internal
} // namespace coder

// Function Definitions
//
// Arguments    : int m
//                int n
//                const array<double, 2U> &A
//                int ia0
//                int lda
// Return Type  : int
//
namespace coder {
namespace internal {
namespace reflapack {
static int ilazlc(int m, int n, const array<double, 2U> &A, int ia0, int lda)
{
  int j;
  boolean_T exitg2;
  j = n;
  exitg2 = false;
  while ((!exitg2) && (j > 0)) {
    int colbottom;
    int coltop;
    int exitg1;
    coltop = ia0 + (j - 1) * lda;
    colbottom = (coltop + m) - 1;
    if ((coltop <= colbottom) && (colbottom > 2147483646)) {
      check_forloop_overflow_error();
    }
    do {
      exitg1 = 0;
      if (coltop <= colbottom) {
        if (A[coltop - 1] != 0.0) {
          exitg1 = 1;
        } else {
          coltop++;
        }
      } else {
        j--;
        exitg1 = 2;
      }
    } while (exitg1 == 0);
    if (exitg1 == 1) {
      exitg2 = true;
    }
  }
  return j;
}

//
// Arguments    : int m
//                int n
//                int iv0
//                double tau
//                array<double, 2U> &C
//                int ic0
//                int ldc
//                array<double, 1U> &work
// Return Type  : void
//
void xzlarf(int m, int n, int iv0, double tau, array<double, 2U> &C, int ic0,
            int ldc, array<double, 1U> &work)
{
  int lastc;
  int lastv;
  if (tau != 0.0) {
    lastv = m;
    lastc = iv0 + m;
    while ((lastv > 0) && (C[lastc - 2] == 0.0)) {
      lastv--;
      lastc--;
    }
    lastc = ilazlc(lastv, n, C, ic0, ldc);
  } else {
    lastv = 0;
    lastc = 0;
  }
  if (lastv > 0) {
    blas::xgemv(lastv, lastc, C, ic0, ldc, C, iv0, work);
    blas::xgerc(lastv, lastc, -tau, iv0, work, C, ic0, ldc);
  }
}

} // namespace reflapack
} // namespace internal
} // namespace coder

//
// File trailer for xzlarf.cpp
//
// [EOF]
//
