//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: deleteColMoveEnd.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "deleteColMoveEnd.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xrotg.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : e_struct_T &obj
//                int idx
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace QRManager {
void deleteColMoveEnd(e_struct_T &obj, int idx)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                 // iFirst
      -1,                 // iLast
      1,                  // lineNo
      1,                  // colNo
      "",                 // aName
      "deleteColMoveEnd", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+QRManager/"
      "deleteColMoveEnd.p", // pName
      0                     // checkKind
  };
  double s;
  int b_i;
  int i;
  if (obj.usedPivoting) {
    boolean_T exitg1;
    i = 1;
    exitg1 = false;
    while ((!exitg1) && (i <= obj.ncols)) {
      b_i = obj.jpvt.size(0);
      if ((i < 1) || (i > b_i)) {
        rtDynamicBoundsError(i, 1, b_i, w_emlrtBCI);
      }
      if (obj.jpvt[i - 1] != idx) {
        i++;
      } else {
        exitg1 = true;
      }
    }
    idx = i;
  }
  if (idx >= obj.ncols) {
    obj.ncols--;
  } else {
    int i1;
    int ix;
    int k;
    int temp_tmp;
    b_i = obj.jpvt.size(0);
    if ((obj.ncols < 1) || (obj.ncols > b_i)) {
      rtDynamicBoundsError(obj.ncols, 1, b_i, w_emlrtBCI);
    }
    b_i = obj.jpvt.size(0);
    if ((idx < 1) || (idx > b_i)) {
      rtDynamicBoundsError(idx, 1, b_i, w_emlrtBCI);
    }
    b_i = obj.ncols - 1;
    obj.jpvt[idx - 1] = obj.jpvt[b_i];
    i = obj.minRowCol;
    if (obj.minRowCol > 2147483646) {
      check_forloop_overflow_error();
    }
    for (k = 0; k < i; k++) {
      i1 = obj.QR.size(0) * obj.QR.size(1);
      temp_tmp = (k + obj.ldq * b_i) + 1;
      if ((temp_tmp < 1) || (temp_tmp > i1)) {
        rtDynamicBoundsError(temp_tmp, 1, i1, w_emlrtBCI);
      }
      i1 = obj.QR.size(0) * obj.QR.size(1);
      ix = (k + obj.ldq * (idx - 1)) + 1;
      if ((ix < 1) || (ix > i1)) {
        rtDynamicBoundsError(ix, 1, i1, w_emlrtBCI);
      }
      obj.QR[ix - 1] = obj.QR[temp_tmp - 1];
    }
    obj.ncols = b_i;
    ix = obj.mrows;
    i = obj.ncols;
    if (ix <= i) {
      i = ix;
    }
    obj.minRowCol = i;
    if (idx < obj.mrows) {
      double c;
      double temp;
      int endIdx;
      int idxRotGCol;
      int n;
      ix = obj.mrows - 1;
      endIdx = obj.ncols;
      if (ix <= endIdx) {
        endIdx = ix;
      }
      k = endIdx;
      idxRotGCol = obj.ldq * (idx - 1);
      while (k >= idx) {
        b_i = obj.QR.size(0) * obj.QR.size(1);
        i1 = k + idxRotGCol;
        if ((i1 + 1 < 1) || (i1 + 1 > b_i)) {
          rtDynamicBoundsError(i1 + 1, 1, b_i, w_emlrtBCI);
        }
        temp = obj.QR[i1];
        b_i = obj.QR.size(0) * obj.QR.size(1);
        if ((i1 < 1) || (i1 > b_i)) {
          rtDynamicBoundsError(i1, 1, b_i, w_emlrtBCI);
        }
        c = internal::blas::xrotg(&obj.QR[i1 - 1], temp, s);
        b_i = obj.QR.size(0) * obj.QR.size(1);
        if ((i1 + 1 < 1) || (i1 + 1 > b_i)) {
          rtDynamicBoundsError(i1 + 1, 1, b_i, w_emlrtBCI);
        }
        obj.QR[i1] = temp;
        b_i = obj.QR.size(0) * obj.QR.size(1);
        i1 = obj.ldq * (k - 1);
        temp_tmp = (k + i1) + 1;
        if ((temp_tmp < 1) || (temp_tmp > b_i)) {
          rtDynamicBoundsError(temp_tmp, 1, b_i, w_emlrtBCI);
        }
        obj.QR[temp_tmp - 1] = 0.0;
        i = k + obj.ldq * idx;
        n = obj.ncols - idx;
        if (n >= 1) {
          ix = i - 1;
          if (n > 2147483646) {
            check_forloop_overflow_error();
          }
          for (int b_k{0}; b_k < n; b_k++) {
            temp = c * obj.QR[ix] + s * obj.QR[i];
            obj.QR[i] = c * obj.QR[i] - s * obj.QR[ix];
            obj.QR[ix] = temp;
            i += obj.ldq;
            ix += obj.ldq;
          }
        }
        i = obj.ldq + i1;
        n = obj.mrows;
        if (obj.mrows > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int b_k{0}; b_k < n; b_k++) {
          temp_tmp = i + b_k;
          ix = i1 + b_k;
          temp = c * obj.Q[ix] + s * obj.Q[temp_tmp];
          obj.Q[temp_tmp] = c * obj.Q[temp_tmp] - s * obj.Q[ix];
          obj.Q[ix] = temp;
        }
        k--;
      }
      b_i = idx + 1;
      for (k = b_i; k <= endIdx; k++) {
        idxRotGCol = obj.ldq * (k - 1);
        i1 = obj.QR.size(0) * obj.QR.size(1);
        temp_tmp = k + idxRotGCol;
        if ((temp_tmp + 1 < 1) || (temp_tmp + 1 > i1)) {
          rtDynamicBoundsError(temp_tmp + 1, 1, i1, w_emlrtBCI);
        }
        temp = obj.QR[temp_tmp];
        i1 = obj.QR.size(0) * obj.QR.size(1);
        if ((temp_tmp < 1) || (temp_tmp > i1)) {
          rtDynamicBoundsError(temp_tmp, 1, i1, w_emlrtBCI);
        }
        c = internal::blas::xrotg(&obj.QR[temp_tmp - 1], temp, s);
        i1 = obj.QR.size(0) * obj.QR.size(1);
        if ((temp_tmp + 1 < 1) || (temp_tmp + 1 > i1)) {
          rtDynamicBoundsError(temp_tmp + 1, 1, i1, w_emlrtBCI);
        }
        obj.QR[temp_tmp] = temp;
        i = k * (obj.ldq + 1);
        n = obj.ncols - k;
        if (n >= 1) {
          ix = i - 1;
          if (n > 2147483646) {
            check_forloop_overflow_error();
          }
          for (int b_k{0}; b_k < n; b_k++) {
            temp = c * obj.QR[ix] + s * obj.QR[i];
            obj.QR[i] = c * obj.QR[i] - s * obj.QR[ix];
            obj.QR[ix] = temp;
            i += obj.ldq;
            ix += obj.ldq;
          }
        }
        i = obj.ldq + idxRotGCol;
        n = obj.mrows;
        if (obj.mrows > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int b_k{0}; b_k < n; b_k++) {
          temp_tmp = i + b_k;
          ix = idxRotGCol + b_k;
          temp = c * obj.Q[ix] + s * obj.Q[temp_tmp];
          obj.Q[temp_tmp] = c * obj.Q[temp_tmp] - s * obj.Q[ix];
          obj.Q[ix] = temp;
        }
      }
    }
  }
}

} // namespace QRManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for deleteColMoveEnd.cpp
//
// [EOF]
//
