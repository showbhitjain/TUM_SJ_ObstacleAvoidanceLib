//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: squareQ_appendCol.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "squareQ_appendCol.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
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
//                const array<double, 1U> &vec
//                int iv0
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace QRManager {
void squareQ_appendCol(e_struct_T &obj, const array<double, 1U> &vec, int iv0)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "squareQ_appendCol", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+QRManager/"
      "squareQ_appendCol.p", // pName
      0                      // checkKind
  };
  double c;
  double s;
  double temp;
  int Qk0;
  int b_iy;
  int iac;
  int iy;
  int iyend;
  int lda;
  iyend = obj.mrows;
  Qk0 = obj.ncols + 1;
  if (iyend <= Qk0) {
    Qk0 = iyend;
  }
  obj.minRowCol = Qk0;
  iy = obj.ldq * obj.ncols;
  lda = obj.ldq;
  if (obj.mrows != 0) {
    boolean_T overflow;
    iyend = iy + obj.mrows;
    if ((iy + 1 <= iyend) && (iyend > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (b_iy = iy + 1; b_iy <= iyend; b_iy++) {
      obj.QR[b_iy - 1] = 0.0;
    }
    Qk0 = obj.ldq * (obj.mrows - 1) + 1;
    if ((obj.ldq == 0) || ((obj.ldq > 0) && (Qk0 < 1)) ||
        ((obj.ldq < 0) && (Qk0 > 1))) {
      overflow = false;
    } else if (obj.ldq > 0) {
      overflow = (Qk0 > MAX_int32_T - obj.ldq);
    } else {
      overflow = (Qk0 < MIN_int32_T - obj.ldq);
    }
    if (obj.ldq == 0) {
      m_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (iac = 1; lda < 0 ? iac >= Qk0 : iac <= Qk0; iac += lda) {
      c = 0.0;
      iyend = (iac + obj.mrows) - 1;
      if ((iac <= iyend) && (iyend > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idx{iac}; idx <= iyend; idx++) {
        c += obj.Q[idx - 1] * vec[((iv0 + idx) - iac) - 1];
      }
      obj.QR[iy] = obj.QR[iy] + c;
      iy++;
    }
  }
  obj.ncols++;
  iac = obj.jpvt.size(0);
  if ((obj.ncols < 1) || (obj.ncols > iac)) {
    rtDynamicBoundsError(obj.ncols, 1, iac, w_emlrtBCI);
  }
  iac = obj.ncols - 1;
  obj.jpvt[iac] = obj.ncols;
  for (int idx{obj.mrows}; idx > obj.ncols; idx--) {
    Qk0 = obj.QR.size(0) * obj.QR.size(1);
    iyend = idx + obj.ldq * iac;
    if ((iyend < 1) || (iyend > Qk0)) {
      rtDynamicBoundsError(iyend, 1, Qk0, w_emlrtBCI);
    }
    temp = obj.QR[iyend - 1];
    Qk0 = obj.QR.size(0) * obj.QR.size(1);
    if ((iyend - 1 < 1) || (iyend - 1 > Qk0)) {
      rtDynamicBoundsError(iyend - 1, 1, Qk0, w_emlrtBCI);
    }
    c = internal::blas::xrotg(&obj.QR[iyend - 2], temp, s);
    Qk0 = obj.QR.size(0) * obj.QR.size(1);
    if (iyend > Qk0) {
      rtDynamicBoundsError(iyend, 1, Qk0, w_emlrtBCI);
    }
    obj.QR[iyend - 1] = temp;
    Qk0 = obj.ldq * (idx - 2);
    iyend = obj.mrows;
    if (obj.mrows >= 1) {
      b_iy = obj.ldq + Qk0;
      if (obj.mrows > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < iyend; k++) {
        lda = b_iy + k;
        iy = Qk0 + k;
        temp = c * obj.Q[iy] + s * obj.Q[lda];
        obj.Q[lda] = c * obj.Q[lda] - s * obj.Q[iy];
        obj.Q[iy] = temp;
      }
    }
  }
}

} // namespace QRManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for squareQ_appendCol.cpp
//
// [EOF]
//
