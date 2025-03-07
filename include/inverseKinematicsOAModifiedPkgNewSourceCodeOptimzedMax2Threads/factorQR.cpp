//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factorQR.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "factorQR.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "xzgeqp3.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : e_struct_T &obj
//                const array<double, 1U> &A
//                int mrows
//                int ncols
//                int ldA
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace QRManager {
void factorQR(e_struct_T &obj, const array<double, 1U> &A, int mrows, int ncols,
              int ldA)
{
  int ix0;
  int minmana;
  int minmn;
  boolean_T guard1;
  ix0 = mrows * ncols;
  guard1 = false;
  if (ix0 > 0) {
    for (minmn = 0; minmn < ncols; minmn++) {
      ix0 = ldA * minmn;
      minmana = obj.ldq * minmn;
      for (int k{0}; k < mrows; k++) {
        obj.QR[minmana + k] = A[ix0 + k];
      }
    }
    guard1 = true;
  } else if (ix0 == 0) {
    obj.mrows = mrows;
    obj.ncols = ncols;
    obj.minRowCol = 0;
  } else {
    guard1 = true;
  }
  if (guard1) {
    obj.usedPivoting = false;
    obj.mrows = mrows;
    obj.ncols = ncols;
    if (static_cast<int>(ncols < 200)) {
      for (int idx{0}; idx < ncols; idx++) {
        obj.jpvt[idx] = idx + 1;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = 0; idx < ncols; idx++) {
        obj.jpvt[idx] = idx + 1;
      }
    }
    if (mrows <= ncols) {
      minmn = mrows;
    } else {
      minmn = ncols;
    }
    obj.minRowCol = minmn;
    ix0 = obj.QR.size(0);
    minmana = obj.QR.size(1);
    if (ix0 <= minmana) {
      minmana = ix0;
    }
    obj.tau.set_size(minmana);
    if (static_cast<int>(minmana < 200)) {
      for (int idx{0}; idx < minmana; idx++) {
        obj.tau[idx] = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = 0; idx < minmana; idx++) {
        obj.tau[idx] = 0.0;
      }
    }
    if (minmn >= 1) {
      internal::reflapack::qrf(obj.QR, mrows, ncols, minmn, obj.tau);
    }
  }
}

} // namespace QRManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factorQR.cpp
//
// [EOF]
//
