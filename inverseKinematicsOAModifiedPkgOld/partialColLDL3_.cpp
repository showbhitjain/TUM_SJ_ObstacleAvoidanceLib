//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: partialColLDL3_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "partialColLDL3_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : f_struct_T &obj
//                int LD_offset
//                int NColsRemain
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace DynamicRegCholManager {
void partialColLDL3_(f_struct_T &obj, int LD_offset, int NColsRemain)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      1,                 // iFirst
      1,                 // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "partialColLDL3_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
      "+DynamicRegCholManager/partialColLDL3_.p", // pName
      3                                           // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "partialColLDL3_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
      "+DynamicRegCholManager/partialColLDL3_.p", // pName
      0                                           // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      1,                 // iFirst
      1,                 // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "partialColLDL3_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
      "+DynamicRegCholManager/partialColLDL3_.p", // pName
      0                                           // checkKind
  };
  int LD_diagOffset;
  int LD_diagOffset_tmp;
  int LDimSizeP1;
  int b;
  int b_b;
  int b_tmp;
  int i;
  int iac;
  int ix;
  int lda;
  int offsetColK;
  int subRows;
  LDimSizeP1 = obj.ldm + 1;
  b = NColsRemain - 1;
  for (int k{0}; k < 48; k++) {
    double y;
    subRows = (NColsRemain - k) - 1;
    LD_diagOffset_tmp = LDimSizeP1 * k;
    LD_diagOffset = LD_offset + LD_diagOffset_tmp;
    for (int idx{0}; idx <= subRows; idx++) {
      i = (LD_diagOffset_tmp + idx) + 1;
      if ((i < 1) || (i > 1)) {
        rtDynamicBoundsError(i, 1, 1, w_emlrtBCI);
      }
      i = obj.FMat.size(0) * obj.FMat.size(1);
      lda = LD_diagOffset + idx;
      if ((lda < 1) || (lda > i)) {
        rtDynamicBoundsError(lda, 1, i, x_emlrtBCI);
      }
      obj.workspace_ = obj.FMat[lda - 1];
    }
    offsetColK = obj.ldm * k + 1;
    for (int idx{0}; idx <= b; idx++) {
      i = offsetColK + idx;
      if ((i < 1) || (i > 1)) {
        rtDynamicBoundsError(i, 1, 1, y_emlrtBCI);
      }
      if ((idx + 1 < 1) || (idx + 1 > 1)) {
        rtDynamicBoundsError(idx + 1, 1, 1, w_emlrtBCI);
      }
      obj.workspace2_ = obj.workspace_;
    }
    lda = obj.ldm;
    y = obj.workspace2_;
    if ((NColsRemain != 0) && (k != 0)) {
      boolean_T overflow;
      ix = LD_offset + k;
      b_tmp = obj.ldm * (k - 1) + 1;
      if ((obj.ldm == 0) || ((obj.ldm > 0) && (b_tmp < 1)) ||
          ((obj.ldm < 0) && (b_tmp > 1))) {
        overflow = false;
      } else if (obj.ldm > 0) {
        overflow = (b_tmp > MAX_int32_T - obj.ldm);
      } else {
        overflow = (b_tmp < MIN_int32_T - obj.ldm);
      }
      if (obj.ldm == 0) {
        m_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      }
      if (overflow) {
        check_forloop_overflow_error();
      }
      for (iac = 1; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
        b_b = (iac + NColsRemain) - 1;
        if ((iac <= b_b) && (b_b > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int idx{iac}; idx <= b_b; idx++) {
          y += obj.workspace_ * -obj.FMat[ix - 1];
        }
        ix += obj.ldm;
      }
    }
    obj.workspace2_ = y;
    for (int idx{0}; idx <= b; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > 1)) {
        rtDynamicBoundsError(idx + 1, 1, 1, y_emlrtBCI);
      }
      i = offsetColK + idx;
      if ((i < 1) || (i > 1)) {
        rtDynamicBoundsError(i, 1, 1, w_emlrtBCI);
      }
      obj.workspace_ = y;
    }
    for (int idx{0}; idx <= subRows; idx++) {
      i = (LD_diagOffset_tmp + idx) + 1;
      if ((i < 1) || (i > 1)) {
        rtDynamicBoundsError(i, 1, 1, y_emlrtBCI);
      }
      i = obj.FMat.size(0) * obj.FMat.size(1);
      lda = LD_diagOffset + idx;
      if ((lda < 1) || (lda > i)) {
        rtDynamicBoundsError(lda, 1, i, x_emlrtBCI);
      }
      obj.FMat[lda - 1] = obj.workspace_;
    }
    i = obj.FMat.size(0) * obj.FMat.size(1);
    if ((LD_diagOffset < 1) || (LD_diagOffset > i)) {
      rtDynamicBoundsError(LD_diagOffset, 1, i, x_emlrtBCI);
    }
    y = obj.FMat[LD_diagOffset - 1];
    if (std::abs(y) <= obj.regTol_) {
      if (LD_diagOffset > i) {
        rtDynamicBoundsError(LD_diagOffset, 1, i, x_emlrtBCI);
      }
      obj.FMat[LD_diagOffset - 1] = y;
    }
    if (subRows > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < subRows; idx++) {
      lda = (LD_diagOffset + idx) + 1;
      if ((lda < 1) || (lda > i)) {
        rtDynamicBoundsError(lda, 1, i, x_emlrtBCI);
      }
      if (LD_diagOffset > i) {
        rtDynamicBoundsError(LD_diagOffset, 1, i, x_emlrtBCI);
      }
      obj.FMat[lda - 1] = obj.FMat[lda - 1] / obj.FMat[LD_diagOffset - 1];
    }
  }
  if (NColsRemain - 1 > 2147483599) {
    check_forloop_overflow_error();
  }
  for (int j{48}; j <= b; j += 48) {
    int c_b;
    int iy0;
    int m;
    int u1_tmp;
    u1_tmp = NColsRemain - j;
    if (u1_tmp >= 48) {
      LD_diagOffset = 48;
    } else {
      LD_diagOffset = u1_tmp;
    }
    b_tmp = j + LD_diagOffset;
    b_b = b_tmp - 1;
    if ((j <= b_tmp - 1) && (b_tmp - 1 > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int k{j}; k <= b_b; k++) {
      m = b_tmp - k;
      iy0 = (LD_offset + LDimSizeP1 * k) - 1;
      for (int idx{0}; idx < 48; idx++) {
        if (idx + 1 > 1) {
          rtDynamicBoundsError(idx + 1, 1, 1, w_emlrtBCI);
        }
        i = obj.FMat.size(0) * obj.FMat.size(1);
        lda = (LD_offset + k) + idx * obj.ldm;
        if ((lda < 1) || (lda > i)) {
          rtDynamicBoundsError(lda, 1, i, x_emlrtBCI);
        }
        obj.workspace2_ = obj.FMat[lda - 1];
      }
      ix = k + 1;
      lda = obj.ldm;
      if (m != 0) {
        c_b = (k + obj.ldm * 47) + 1;
        if ((k + 1 <= c_b) && (c_b > MAX_int32_T - obj.ldm)) {
          check_forloop_overflow_error();
        }
        for (iac = ix; lda < 0 ? iac >= c_b : iac <= c_b; iac += lda) {
          offsetColK = (iac + m) - 1;
          if ((iac <= offsetColK) && (offsetColK > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (int idx{iac}; idx <= offsetColK; idx++) {
            i = (iy0 + idx) - iac;
            obj.FMat[i] = obj.FMat[i] + obj.workspace_ * -obj.workspace2_;
          }
        }
      }
    }
    if (b_tmp < NColsRemain) {
      m = u1_tmp - LD_diagOffset;
      iac = ((LD_offset + LD_diagOffset) + LDimSizeP1 * j) - 1;
      i = LD_diagOffset - 1;
      for (int idx{0}; idx < 48; idx++) {
        ix = idx * obj.ldm;
        offsetColK = (LD_offset + j) + ix;
        for (subRows = 0; subRows <= i; subRows++) {
          lda = (ix + subRows) + 1;
          if ((lda < 1) || (lda > 1)) {
            rtDynamicBoundsError(lda, 1, 1, w_emlrtBCI);
          }
          lda = obj.FMat.size(0) * obj.FMat.size(1);
          LD_diagOffset_tmp = offsetColK + subRows;
          if ((LD_diagOffset_tmp < 1) || (LD_diagOffset_tmp > lda)) {
            rtDynamicBoundsError(LD_diagOffset_tmp, 1, lda, x_emlrtBCI);
          }
          obj.workspace2_ = obj.FMat[LD_diagOffset_tmp - 1];
        }
      }
      subRows = obj.ldm;
      if ((m != 0) && (LD_diagOffset != 0)) {
        ix = iac + obj.ldm * (LD_diagOffset - 1);
        if ((iac <= ix) && (ix > MAX_int32_T - obj.ldm)) {
          check_forloop_overflow_error();
        }
        for (LD_diagOffset = iac;
             subRows < 0 ? LD_diagOffset >= ix : LD_diagOffset <= ix;
             LD_diagOffset += subRows) {
          b_b = LD_diagOffset + m;
          if ((LD_diagOffset + 1 <= b_b) && (b_b > 2147483646)) {
            check_forloop_overflow_error();
          }
        }
        offsetColK = 0;
        for (LD_diagOffset = iac;
             subRows < 0 ? LD_diagOffset >= ix : LD_diagOffset <= ix;
             LD_diagOffset += subRows) {
          offsetColK++;
          b_b = offsetColK + subRows * 47;
          if ((offsetColK <= b_b) && (b_b > MAX_int32_T - subRows)) {
            check_forloop_overflow_error();
          }
          for (u1_tmp = offsetColK; subRows < 0 ? u1_tmp >= b_b : u1_tmp <= b_b;
               u1_tmp += subRows) {
            LD_diagOffset_tmp = LD_diagOffset + 1;
            c_b = LD_diagOffset + m;
            if ((LD_diagOffset + 1 <= c_b) && (c_b > 2147483646)) {
              check_forloop_overflow_error();
            }
            for (iy0 = LD_diagOffset_tmp; iy0 <= c_b; iy0++) {
              obj.FMat[iy0 - 1] =
                  obj.FMat[iy0 - 1] + -obj.workspace2_ * obj.workspace_;
            }
          }
        }
      }
    }
  }
}

} // namespace DynamicRegCholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for partialColLDL3_.cpp
//
// [EOF]
//
