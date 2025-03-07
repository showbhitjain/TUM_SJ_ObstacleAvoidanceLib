//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BFGSUpdate.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "BFGSUpdate.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int nvar
//                array<double, 2U> &Bk
//                const array<double, 1U> &sk
//                array<double, 1U> &yk
//                array<double, 2U> &workspace
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
boolean_T BFGSUpdate(int nvar, array<double, 2U> &Bk,
                     const array<double, 1U> &sk, array<double, 1U> &yk,
                     array<double, 2U> &workspace)
{
  double curvatureS;
  double dotSY;
  double theta;
  int i;
  int i1;
  int ix;
  int j;
  int ldBk;
  boolean_T success;
  ldBk = Bk.size(0);
  dotSY = 0.0;
  if (nvar >= 1) {
    for (ix = 0; ix < nvar; ix++) {
      dotSY += sk[ix] * yk[ix];
    }
  }
  if (nvar != 0) {
    if (static_cast<int>(nvar < 200)) {
      for (int iy{0}; iy < nvar; iy++) {
        workspace[iy] = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int iy = 0; iy < nvar; iy++) {
        workspace[iy] = 0.0;
      }
    }
    ix = 0;
    i = Bk.size(0) * (nvar - 1) + 1;
    for (int iac{1}; ldBk < 0 ? iac >= i : iac <= i; iac += ldBk) {
      i1 = (iac + nvar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        j = ia - iac;
        workspace[j] = workspace[j] + Bk[ia - 1] * sk[ix];
      }
      ix++;
    }
  }
  curvatureS = 0.0;
  if (nvar >= 1) {
    for (ix = 0; ix < nvar; ix++) {
      curvatureS += sk[ix] * workspace[ix];
    }
  }
  if (dotSY < 0.2 * curvatureS) {
    theta = 0.8 * curvatureS / (curvatureS - dotSY);
    i = (nvar < 200);
    if (i) {
      for (int iy{0}; iy < nvar; iy++) {
        yk[iy] = theta * yk[iy];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int iy = 0; iy < nvar; iy++) {
        yk[iy] = theta * yk[iy];
      }
    }
    if ((nvar >= 1) && (!(1.0 - theta == 0.0))) {
      ix = nvar - 1;
      if (i) {
        for (int iy{0}; iy <= ix; iy++) {
          yk[iy] = yk[iy] + (1.0 - theta) * workspace[iy];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int iy = 0; iy <= ix; iy++) {
          yk[iy] = yk[iy] + (1.0 - theta) * workspace[iy];
        }
      }
    }
    dotSY = 0.0;
    if (nvar >= 1) {
      for (ix = 0; ix < nvar; ix++) {
        dotSY += sk[ix] * yk[ix];
      }
    }
  }
  if ((curvatureS > 2.2204460492503131E-16) &&
      (dotSY > 2.2204460492503131E-16)) {
    success = true;
  } else {
    success = false;
  }
  if (success) {
    curvatureS = -1.0 / curvatureS;
    if (!(curvatureS == 0.0)) {
      ix = 0;
      for (j = 0; j < nvar; j++) {
        if (workspace[j] != 0.0) {
          theta = workspace[j] * curvatureS;
          i = ix + 1;
          i1 = nvar + ix;
          for (int iac{i}; iac <= i1; iac++) {
            Bk[iac - 1] = Bk[iac - 1] + workspace[(iac - ix) - 1] * theta;
          }
        }
        ix += ldBk;
      }
    }
    curvatureS = 1.0 / dotSY;
    if (!(curvatureS == 0.0)) {
      ix = 0;
      for (j = 0; j < nvar; j++) {
        theta = yk[j];
        if (theta != 0.0) {
          theta *= curvatureS;
          i = ix + 1;
          i1 = nvar + ix;
          for (int iac{i}; iac <= i1; iac++) {
            Bk[iac - 1] = Bk[iac - 1] + yk[(iac - ix) - 1] * theta;
          }
        }
        ix += ldBk;
      }
    }
  }
  return success;
}

} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for BFGSUpdate.cpp
//
// [EOF]
//
