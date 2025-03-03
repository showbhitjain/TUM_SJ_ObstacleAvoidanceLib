//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: addAeqConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "addAeqConstr.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int idx_local
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void addAeqConstr(d_struct_T &obj, int idx_local)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,             // iFirst
      -1,             // iLast
      1,              // lineNo
      1,              // colNo
      "",             // aName
      "addAeqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/addAeqConstr.p", // pName
      0                             // checkKind
  };
  int totalEq;
  totalEq = (obj.nWConstr[0] + obj.nWConstr[1]) + 1;
  if ((obj.nActiveConstr == totalEq - 1) && (idx_local > obj.nWConstr[1])) {
    int i;
    int i1;
    int iAeq0;
    int iAw0;
    obj.nWConstr[1]++;
    i = obj.isActiveConstr.size(0);
    i1 = (obj.isActiveIdx[1] + idx_local) - 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, q_emlrtBCI);
    }
    obj.isActiveConstr[i1 - 1] = true;
    obj.nActiveConstr++;
    i = obj.Wid.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i, q_emlrtBCI);
    }
    i = obj.nActiveConstr - 1;
    obj.Wid[i] = 2;
    i1 = obj.Wlocalidx.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i1, q_emlrtBCI);
    }
    obj.Wlocalidx[i] = idx_local;
    iAeq0 = obj.ldA * (idx_local - 1);
    iAw0 = obj.ldA * i;
    totalEq = obj.nVar;
    if (obj.nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < totalEq; idx++) {
      int i2;
      int i3;
      i1 = obj.Aeq.size(0);
      i2 = (iAeq0 + idx) + 1;
      if ((i2 < 1) || (i2 > i1)) {
        rtDynamicBoundsError(i2, 1, i1, w_emlrtBCI);
      }
      i1 = obj.ATwset.size(0);
      i3 = (iAw0 + idx) + 1;
      if ((i3 < 1) || (i3 > i1)) {
        rtDynamicBoundsError(i3, 1, i1, w_emlrtBCI);
      }
      obj.ATwset[i3 - 1] = obj.Aeq[i2 - 1];
    }
    i1 = obj.beq.size[0];
    if ((idx_local < 1) || (idx_local > i1)) {
      rtDynamicBoundsError(idx_local, 1, i1, w_emlrtBCI);
    }
    i1 = obj.bwset.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i1, w_emlrtBCI);
    }
    obj.bwset[i] = obj.beq.data[idx_local - 1];
  } else {
    int b_tmp;
    int i;
    int i1;
    int i2;
    int iAeq0;
    int iAw0;
    obj.nActiveConstr++;
    i = obj.Wid.size(0);
    if ((totalEq < 1) || (totalEq > i)) {
      rtDynamicBoundsError(totalEq, 1, i, j_emlrtBCI);
    }
    i = obj.Wid.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i, j_emlrtBCI);
    }
    i = obj.nActiveConstr - 1;
    obj.Wid[i] = obj.Wid[totalEq - 1];
    i1 = obj.Wlocalidx.size(0);
    if (totalEq > i1) {
      rtDynamicBoundsError(totalEq, 1, i1, j_emlrtBCI);
    }
    i1 = obj.Wlocalidx.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i1, j_emlrtBCI);
    }
    obj.Wlocalidx[i] = obj.Wlocalidx[totalEq - 1];
    b_tmp = obj.nVar;
    if (obj.nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b_tmp; idx++) {
      int i3;
      i1 = obj.ATwset.size(0);
      i2 = (idx + obj.ldA * (totalEq - 1)) + 1;
      if ((i2 < 1) || (i2 > i1)) {
        rtDynamicBoundsError(i2, 1, i1, j_emlrtBCI);
      }
      i1 = obj.ATwset.size(0);
      i3 = (idx + obj.ldA * i) + 1;
      if ((i3 < 1) || (i3 > i1)) {
        rtDynamicBoundsError(i3, 1, i1, j_emlrtBCI);
      }
      obj.ATwset[i3 - 1] = obj.ATwset[i2 - 1];
    }
    i1 = obj.bwset.size(0);
    if (totalEq > i1) {
      rtDynamicBoundsError(totalEq, 1, i1, j_emlrtBCI);
    }
    i1 = obj.bwset.size(0);
    if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
      rtDynamicBoundsError(obj.nActiveConstr, 1, i1, j_emlrtBCI);
    }
    obj.bwset[i] = obj.bwset[totalEq - 1];
    obj.nWConstr[1]++;
    i = obj.isActiveConstr.size(0);
    i1 = (obj.isActiveIdx[1] + idx_local) - 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    obj.isActiveConstr[i1 - 1] = true;
    i = obj.Wid.size(0);
    if (totalEq > i) {
      rtDynamicBoundsError(totalEq, 1, i, w_emlrtBCI);
    }
    obj.Wid[totalEq - 1] = 2;
    i = obj.Wlocalidx.size(0);
    if (totalEq > i) {
      rtDynamicBoundsError(totalEq, 1, i, w_emlrtBCI);
    }
    obj.Wlocalidx[totalEq - 1] = idx_local;
    iAeq0 = obj.ldA * (idx_local - 1);
    iAw0 = obj.ldA * (totalEq - 1);
    if (obj.nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b_tmp; idx++) {
      i = obj.Aeq.size(0);
      i1 = (iAeq0 + idx) + 1;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      i = obj.ATwset.size(0);
      i2 = (iAw0 + idx) + 1;
      if ((i2 < 1) || (i2 > i)) {
        rtDynamicBoundsError(i2, 1, i, w_emlrtBCI);
      }
      obj.ATwset[i2 - 1] = obj.Aeq[i1 - 1];
    }
    i = obj.beq.size[0];
    if ((idx_local < 1) || (idx_local > i)) {
      rtDynamicBoundsError(idx_local, 1, i, w_emlrtBCI);
    }
    i = obj.bwset.size(0);
    if (totalEq > i) {
      rtDynamicBoundsError(totalEq, 1, i, w_emlrtBCI);
    }
    obj.bwset[totalEq - 1] = obj.beq.data[idx_local - 1];
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for addAeqConstr.cpp
//
// [EOF]
//
