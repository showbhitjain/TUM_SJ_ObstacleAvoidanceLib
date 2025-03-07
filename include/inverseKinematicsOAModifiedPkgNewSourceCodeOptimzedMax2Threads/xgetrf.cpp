//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgetrf.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "xgetrf.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : double A[36]
//                int ipiv[6]
// Return Type  : void
//
namespace coder {
namespace internal {
namespace lapack {
void xgetrf(double A[36], int ipiv[6])
{
  ipiv[0] = 1;
  ipiv[1] = 2;
  ipiv[2] = 3;
  ipiv[3] = 4;
  ipiv[4] = 5;
  ipiv[5] = 6;
  for (int j{0}; j < 5; j++) {
    double smax;
    int a;
    int b_tmp;
    int i;
    int jA;
    int jp1j;
    int mmj_tmp;
    mmj_tmp = 4 - j;
    b_tmp = j * 7;
    jp1j = b_tmp + 2;
    jA = 6 - j;
    a = 0;
    smax = std::abs(A[b_tmp]);
    for (int k{2}; k <= jA; k++) {
      double s;
      s = std::abs(A[(b_tmp + k) - 1]);
      if (s > smax) {
        a = k - 1;
        smax = s;
      }
    }
    if (A[b_tmp + a] != 0.0) {
      if (a != 0) {
        jA = j + a;
        ipiv[j] = jA + 1;
        smax = A[j];
        A[j] = A[jA];
        A[jA] = smax;
        smax = A[j + 6];
        A[j + 6] = A[jA + 6];
        A[jA + 6] = smax;
        smax = A[j + 12];
        A[j + 12] = A[jA + 12];
        A[jA + 12] = smax;
        smax = A[j + 18];
        A[j + 18] = A[jA + 18];
        A[jA + 18] = smax;
        smax = A[j + 24];
        A[j + 24] = A[jA + 24];
        A[jA + 24] = smax;
        smax = A[j + 30];
        A[j + 30] = A[jA + 30];
        A[jA + 30] = smax;
      }
      i = (b_tmp - j) + 6;
      for (jA = jp1j; jA <= i; jA++) {
        A[jA - 1] /= A[b_tmp];
      }
    }
    jA = b_tmp;
    for (int k{0}; k <= mmj_tmp; k++) {
      smax = A[(b_tmp + k * 6) + 6];
      if (smax != 0.0) {
        i = jA + 8;
        a = (jA - j) + 12;
        for (jp1j = i; jp1j <= a; jp1j++) {
          A[jp1j - 1] += A[((b_tmp + jp1j) - jA) - 7] * -smax;
        }
      }
      jA += 6;
    }
  }
}

} // namespace lapack
} // namespace internal
} // namespace coder

//
// File trailer for xgetrf.cpp
//
// [EOF]
//
