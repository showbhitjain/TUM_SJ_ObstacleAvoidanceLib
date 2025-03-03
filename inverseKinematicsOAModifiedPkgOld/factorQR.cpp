//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factorQR.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "factorQR.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xzgeqp3.h"
#include "coder_array.h"
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
  int k;
  int minmana;
  boolean_T guard1;
  k = mrows * ncols;
  guard1 = false;
  if (k > 0) {
    if (ncols > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < ncols; idx++) {
      ix0 = ldA * idx;
      minmana = obj.ldq * idx;
      if (mrows > 2147483646) {
        check_forloop_overflow_error();
      }
      for (k = 0; k < mrows; k++) {
        obj.QR[minmana + k] = A[ix0 + k];
      }
    }
    guard1 = true;
  } else if (k == 0) {
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
    if (ncols > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < ncols; idx++) {
      k = obj.jpvt.size(0);
      if ((idx + 1 < 1) || (idx + 1 > k)) {
        rtDynamicBoundsError(idx + 1, 1, k, m_emlrtBCI);
      }
      obj.jpvt[idx] = idx + 1;
    }
    if (mrows <= ncols) {
      k = mrows;
    } else {
      k = ncols;
    }
    obj.minRowCol = k;
    ix0 = obj.QR.size(0);
    minmana = obj.QR.size(1);
    if (ix0 <= minmana) {
      minmana = ix0;
    }
    obj.tau.set_size(minmana);
    for (ix0 = 0; ix0 < minmana; ix0++) {
      obj.tau[ix0] = 0.0;
    }
    if (k >= 1) {
      internal::reflapack::qrf(obj.QR, mrows, ncols, k, obj.tau);
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
