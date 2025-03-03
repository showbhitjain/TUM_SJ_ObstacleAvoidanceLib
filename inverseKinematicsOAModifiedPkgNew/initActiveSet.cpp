//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: initActiveSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "initActiveSet.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
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
void initActiveSet(d_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,              // iFirst
      -1,              // iLast
      1,               // lineNo
      1,               // colNo
      "",              // aName
      "initActiveSet", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/initActiveSet.p", // pName
      0                              // checkKind
  };
  int i;
  int i1;
  int iAeq0;
  int idxFillStart;
  int idx_global;
  int nWFixed;
  setProblemType(obj, 3);
  idxFillStart = obj.isActiveIdx[2];
  iAeq0 = obj.mConstrMax;
  if ((obj.isActiveIdx[2] <= obj.mConstrMax) && (obj.mConstrMax > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (idx_global = idxFillStart; idx_global <= iAeq0; idx_global++) {
    i = obj.isActiveConstr.size(0);
    if ((idx_global < 1) || (idx_global > i)) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    obj.isActiveConstr[idx_global - 1] = false;
  }
  obj.nWConstr[0] = obj.sizes[0];
  obj.nWConstr[1] = obj.sizes[1];
  obj.nWConstr[2] = 0;
  obj.nWConstr[3] = 0;
  obj.nWConstr[4] = 0;
  obj.nActiveConstr = obj.nWConstr[0] + obj.nWConstr[1];
  nWFixed = obj.sizes[0];
  if (obj.sizes[0] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx_local{0}; idx_local < nWFixed; idx_local++) {
    i = obj.Wid.size(0);
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    obj.Wid[idx_local] = 1;
    i = obj.Wlocalidx.size(0);
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    obj.Wlocalidx[idx_local] = idx_local + 1;
    i = obj.isActiveConstr.size(0);
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    obj.isActiveConstr[idx_local] = true;
    i = obj.indexFixed.size(0);
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    idxFillStart = obj.ldA * idx_local;
    iAeq0 = obj.indexFixed[idx_local];
    if (obj.indexFixed[idx_local] - 1 > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int b_i{0}; b_i <= iAeq0 - 2; b_i++) {
      i = obj.ATwset.size(0);
      i1 = (b_i + idxFillStart) + 1;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    i = obj.ATwset.size(0);
    i1 = obj.indexFixed[idx_local] + idxFillStart;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 1.0;
    idx_global = obj.indexFixed[idx_local] + 1;
    iAeq0 = obj.nVar;
    if ((obj.indexFixed[idx_local] + 1 <= obj.nVar) &&
        (obj.nVar > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int b_i{idx_global}; b_i <= iAeq0; b_i++) {
      i = obj.ATwset.size(0);
      i1 = b_i + idxFillStart;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i1 - 1] = 0.0;
    }
    i = obj.ub.size(0);
    if ((obj.indexFixed[idx_local] < 1) || (obj.indexFixed[idx_local] > i)) {
      rtDynamicBoundsError(obj.indexFixed[idx_local], 1, i, w_emlrtBCI);
    }
    i = obj.bwset.size(0);
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    obj.bwset[idx_local] = obj.ub[obj.indexFixed[idx_local] - 1];
  }
  idxFillStart = obj.sizes[1];
  if (obj.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx_local{0}; idx_local < idxFillStart; idx_local++) {
    int iATw0;
    idx_global = (nWFixed + idx_local) + 1;
    i = obj.Wid.size(0);
    if ((idx_global < 1) || (idx_global > i)) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    obj.Wid[idx_global - 1] = 2;
    i = obj.Wlocalidx.size(0);
    if (idx_global > i) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    obj.Wlocalidx[idx_global - 1] = idx_local + 1;
    i = obj.isActiveConstr.size(0);
    if (idx_global > i) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    obj.isActiveConstr[idx_global - 1] = true;
    iAeq0 = obj.ldA * idx_local + 1;
    iATw0 = obj.ldA * (idx_global - 1) + 1;
    i = obj.nVar - 1;
    for (int b_i{0}; b_i <= i; b_i++) {
      int i2;
      int i3;
      i1 = obj.Aeq.size(0);
      i2 = iAeq0 + b_i;
      if ((i2 < 1) || (i2 > i1)) {
        rtDynamicBoundsError(i2, 1, i1, w_emlrtBCI);
      }
      i1 = obj.ATwset.size(0);
      i3 = iATw0 + b_i;
      if ((i3 < 1) || (i3 > i1)) {
        rtDynamicBoundsError(i3, 1, i1, w_emlrtBCI);
      }
      obj.ATwset[i3 - 1] = obj.Aeq[i2 - 1];
    }
    i = obj.beq.size[0];
    if ((idx_local + 1 < 1) || (idx_local + 1 > i)) {
      rtDynamicBoundsError(idx_local + 1, 1, i, w_emlrtBCI);
    }
    i = obj.bwset.size(0);
    if (idx_global > i) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    obj.bwset[idx_global - 1] = obj.beq.data[idx_local];
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for initActiveSet.cpp
//
// [EOF]
//
