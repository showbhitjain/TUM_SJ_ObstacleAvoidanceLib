//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzlarf.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "xzlarf.h"
#include "rt_nonfinite.h"
#include "xgerc.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
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
namespace coder {
namespace internal {
namespace reflapack {
void xzlarf(int m, int n, int iv0, double tau, array<double, 2U> &C, int ic0,
            int ldc, array<double, 1U> &work)
{
  int i;
  int ia;
  int lastc;
  int lastv;
  if (tau != 0.0) {
    boolean_T exitg2;
    lastv = m;
    i = iv0 + m;
    while ((lastv > 0) && (C[i - 2] == 0.0)) {
      lastv--;
      i--;
    }
    lastc = n;
    exitg2 = false;
    while ((!exitg2) && (lastc > 0)) {
      int exitg1;
      i = ic0 + (lastc - 1) * ldc;
      ia = i;
      do {
        exitg1 = 0;
        if (ia <= (i + lastv) - 1) {
          if (C[ia - 1] != 0.0) {
            exitg1 = 1;
          } else {
            ia++;
          }
        } else {
          lastc--;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
  } else {
    lastv = 0;
    lastc = 0;
  }
  if (lastv > 0) {
    if (lastc != 0) {
      int b_i;
      if (static_cast<int>(lastc < 200)) {
        for (int iy{0}; iy < lastc; iy++) {
          work[iy] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int iy = 0; iy < lastc; iy++) {
          work[iy] = 0.0;
        }
      }
      i = 0;
      b_i = ic0 + ldc * (lastc - 1);
      for (int iac{ic0}; ldc < 0 ? iac >= b_i : iac <= b_i; iac += ldc) {
        double c;
        int i1;
        c = 0.0;
        i1 = (iac + lastv) - 1;
        for (ia = iac; ia <= i1; ia++) {
          c += C[ia - 1] * C[((iv0 + ia) - iac) - 1];
        }
        work[i] = work[i] + c;
        i++;
      }
    }
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
