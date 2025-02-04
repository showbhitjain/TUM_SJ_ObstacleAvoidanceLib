//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: modifyOverheadRegularized_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "modifyOverheadRegularized_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void modifyOverheadRegularized_(d_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                           // iFirst
      -1,                           // iLast
      1,                            // lineNo
      1,                            // colNo
      "",                           // aName
      "modifyOverheadRegularized_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/modifyOverheadRegularized_.p", // pName
      0                                           // checkKind
  };
  int a;
  int b;
  int b_tmp;
  int colOffsetATw;
  int colOffsetAeq;
  int colOffsetAineq;
  int i;
  int i1;
  int mEq;
  int mIneq;
  int offsetEq1_tmp;
  int offsetEq2;
  int offsetIneq_tmp;
  mIneq = obj.sizes[2];
  mEq = obj.sizes[1];
  offsetIneq_tmp = obj.nVarOrig + 1;
  offsetEq1_tmp = obj.nVarOrig + obj.sizes[2];
  offsetEq2 = offsetEq1_tmp + obj.sizes[1];
  b = obj.sizes[0];
  if (obj.sizes[0] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx_col{0}; idx_col < b; idx_col++) {
    colOffsetATw = obj.ldA * idx_col;
    colOffsetAeq = obj.nVar;
    if ((offsetIneq_tmp <= obj.nVar) && (obj.nVar > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{offsetIneq_tmp}; idx_row <= colOffsetAeq; idx_row++) {
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
  }
  if (obj.sizes[2] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx_col{0}; idx_col < mIneq; idx_col++) {
    colOffsetAineq = obj.ldA * idx_col;
    b_tmp = offsetIneq_tmp + idx_col;
    b = b_tmp - 1;
    if ((offsetIneq_tmp <= b_tmp - 1) && (b_tmp - 1 > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{offsetIneq_tmp}; idx_row <= b; idx_row++) {
      i = obj.Aineq.size(0);
      i1 = idx_row + colOffsetAineq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aineq[i1 - 1] = 0.0;
    }
    i = obj.Aineq.size(0);
    i1 = b_tmp + colOffsetAineq;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.Aineq[i1 - 1] = -1.0;
    a = b_tmp + 1;
    b = obj.nVar;
    if ((b_tmp + 1 <= obj.nVar) && (obj.nVar > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{a}; idx_row <= b; idx_row++) {
      i = obj.Aineq.size(0);
      i1 = idx_row + colOffsetAineq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aineq[i1 - 1] = 0.0;
    }
  }
  if (obj.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx_col{0}; idx_col < mEq; idx_col++) {
    colOffsetAeq = obj.ldA * idx_col;
    colOffsetATw = colOffsetAeq + obj.ldA * (obj.isActiveIdx[1] - 1);
    if ((offsetIneq_tmp <= offsetEq1_tmp) && (offsetEq1_tmp > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{offsetIneq_tmp}; idx_row <= offsetEq1_tmp; idx_row++) {
      i = obj.Aeq.size(0);
      i1 = idx_row + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aeq[i1 - 1] = 0.0;
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    a = offsetEq1_tmp + 1;
    b_tmp = offsetEq1_tmp + idx_col;
    if ((offsetEq1_tmp + 1 <= b_tmp) && (b_tmp > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{a}; idx_row <= b_tmp; idx_row++) {
      i = obj.Aeq.size(0);
      i1 = idx_row + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aeq[i1 - 1] = 0.0;
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    i = obj.Aeq.size(0);
    i1 = (b_tmp + colOffsetAeq) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.Aeq[i1 - 1] = -1.0;
    i = obj.ATwset.size(0);
    i1 = (b_tmp + colOffsetATw) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = -1.0;
    a = b_tmp + 2;
    if ((b_tmp + 2 <= offsetEq2) && (offsetEq2 > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{a}; idx_row <= offsetEq2; idx_row++) {
      i = obj.Aeq.size(0);
      i1 = idx_row + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aeq[i1 - 1] = 0.0;
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    a = offsetEq2 + 1;
    b_tmp = offsetEq2 + idx_col;
    if ((offsetEq2 + 1 <= b_tmp) && (b_tmp > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{a}; idx_row <= b_tmp; idx_row++) {
      i = obj.Aeq.size(0);
      i1 = idx_row + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aeq[i1 - 1] = 0.0;
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    i = obj.Aeq.size(0);
    i1 = (b_tmp + colOffsetAeq) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.Aeq[i1 - 1] = 1.0;
    i = obj.ATwset.size(0);
    i1 = (b_tmp + colOffsetATw) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 1.0;
    a = b_tmp + 2;
    b = obj.nVar;
    if ((b_tmp + 2 <= obj.nVar) && (obj.nVar > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx_row{a}; idx_row <= b; idx_row++) {
      i = obj.Aeq.size(0);
      i1 = idx_row + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.Aeq[i1 - 1] = 0.0;
      i = obj.ATwset.size(0);
      i1 = idx_row + colOffsetATw;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
  }
  colOffsetAineq = obj.nVarOrig;
  a = obj.sizesNormal[3] + 1;
  b = obj.sizesRegularized[3];
  if ((obj.sizesNormal[3] + 1 <= obj.sizesRegularized[3]) &&
      (obj.sizesRegularized[3] > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (colOffsetAeq = a; colOffsetAeq <= b; colOffsetAeq++) {
    colOffsetAineq++;
    i = obj.indexLB.size(0);
    if ((colOffsetAeq < 1) || (colOffsetAeq > i)) {
      rtDynamicBoundsError(colOffsetAeq, 1, i, w_emlrtBCI);
    }
    obj.indexLB[colOffsetAeq - 1] = colOffsetAineq;
  }
  if (obj.nWConstr[4] > 0) {
    b = obj.sizesRegularized[4];
    if (obj.sizesRegularized[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (colOffsetAeq = 0; colOffsetAeq < b; colOffsetAeq++) {
      i = obj.isActiveConstr.size(0);
      i1 = obj.isActiveIdx[4] + colOffsetAeq;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      i = obj.isActiveConstr.size(0);
      colOffsetAineq = (obj.isActiveIdxRegularized[4] + colOffsetAeq) + 1;
      if ((colOffsetAineq < 1) || (colOffsetAineq > i)) {
        rtDynamicBoundsError(colOffsetAineq, 1, i, w_emlrtBCI);
      }
      obj.isActiveConstr[colOffsetAineq - 1] = obj.isActiveConstr[i1 - 1];
    }
  }
  a = obj.isActiveIdx[4];
  b = obj.isActiveIdxRegularized[4] - 1;
  if ((obj.isActiveIdx[4] <= obj.isActiveIdxRegularized[4] - 1) &&
      (obj.isActiveIdxRegularized[4] - 1 > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (colOffsetAeq = a; colOffsetAeq <= b; colOffsetAeq++) {
    i = obj.isActiveConstr.size(0);
    if ((colOffsetAeq < 1) || (colOffsetAeq > i)) {
      rtDynamicBoundsError(colOffsetAeq, 1, i, w_emlrtBCI);
    }
    obj.isActiveConstr[colOffsetAeq - 1] = false;
  }
  b = offsetEq1_tmp + (obj.sizes[1] << 1);
  if ((offsetIneq_tmp <= b) && (b > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (colOffsetAeq = offsetIneq_tmp; colOffsetAeq <= b; colOffsetAeq++) {
    i = obj.lb.size(0);
    if ((colOffsetAeq < 1) || (colOffsetAeq > i)) {
      rtDynamicBoundsError(colOffsetAeq, 1, i, w_emlrtBCI);
    }
    obj.lb[colOffsetAeq - 1] = 0.0;
  }
  colOffsetAineq = obj.isActiveIdx[2];
  b = obj.nActiveConstr;
  if ((obj.isActiveIdx[2] <= obj.nActiveConstr) &&
      (obj.nActiveConstr > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx_col{colOffsetAineq}; idx_col <= b; idx_col++) {
    colOffsetATw = obj.ldA * (idx_col - 1);
    i = obj.Wid.size(0);
    if ((idx_col < 1) || (idx_col > i)) {
      rtDynamicBoundsError(idx_col, 1, i, w_emlrtBCI);
    }
    if (obj.Wid[idx_col - 1] == 3) {
      i = obj.Wlocalidx.size(0);
      if (idx_col > i) {
        rtDynamicBoundsError(idx_col, 1, i, w_emlrtBCI);
      }
      b_tmp = offsetIneq_tmp + obj.Wlocalidx[idx_col - 1];
      colOffsetAeq = b_tmp - 2;
      if ((offsetIneq_tmp <= b_tmp - 2) && (b_tmp - 2 > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idx_row{offsetIneq_tmp}; idx_row <= colOffsetAeq; idx_row++) {
        i = obj.ATwset.size(0);
        i1 = idx_row + colOffsetATw;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        obj.ATwset[i1 - 1] = 0.0;
      }
      i = obj.ATwset.size(0);
      i1 = (b_tmp + colOffsetATw) - 1;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = -1.0;
      colOffsetAeq = obj.nVar;
      if ((b_tmp <= obj.nVar) && (obj.nVar > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idx_row{b_tmp}; idx_row <= colOffsetAeq; idx_row++) {
        i = obj.ATwset.size(0);
        i1 = idx_row + colOffsetATw;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        obj.ATwset[i1 - 1] = 0.0;
      }
    } else {
      colOffsetAeq = obj.nVar;
      if ((offsetIneq_tmp <= obj.nVar) && (obj.nVar > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idx_row{offsetIneq_tmp}; idx_row <= colOffsetAeq; idx_row++) {
        i = obj.ATwset.size(0);
        i1 = idx_row + colOffsetATw;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        obj.ATwset[i1 - 1] = 0.0;
      }
    }
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for modifyOverheadRegularized_.cpp
//
// [EOF]
//
