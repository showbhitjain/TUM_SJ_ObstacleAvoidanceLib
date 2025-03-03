//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: addBoundToActiveSetMatrix_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "addBoundToActiveSetMatrix_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo v_emlrtBCI{
    -1,                           // iFirst
    -1,                           // iLast
    1,                            // lineNo
    1,                            // colNo
    "",                           // aName
    "addBoundToActiveSetMatrix_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/addBoundToActiveSetMatrix_.p", // pName
    0                                           // checkKind
};

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
void addBoundToActiveSetMatrix_(d_struct_T &obj, int idx_local)
{
  int a_tmp;
  int b;
  int colOffset;
  int i;
  int i1;
  int i2;
  obj.nWConstr[3]++;
  i = obj.isActiveConstr.size(0);
  i1 = (obj.isActiveIdx[3] + idx_local) - 1;
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
  obj.Wid[i] = 4;
  i1 = obj.Wlocalidx.size(0);
  if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
    rtDynamicBoundsError(obj.nActiveConstr, 1, i1, q_emlrtBCI);
  }
  obj.Wlocalidx[i] = idx_local;
  colOffset = obj.ldA * i;
  i1 = obj.indexLB.size(0);
  if ((idx_local < 1) || (idx_local > i1)) {
    rtDynamicBoundsError(idx_local, 1, i1, v_emlrtBCI);
  }
  i1 = obj.lb.size(0);
  i2 = obj.indexLB[idx_local - 1];
  if ((i2 < 1) || (i2 > i1)) {
    rtDynamicBoundsError(obj.indexLB[idx_local - 1], 1, i1, v_emlrtBCI);
  }
  i1 = obj.bwset.size(0);
  if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
    rtDynamicBoundsError(obj.nActiveConstr, 1, i1, v_emlrtBCI);
  }
  obj.bwset[i] = obj.lb[i2 - 1];
  for (int idx{0}; idx <= i2 - 2; idx++) {
    i = obj.ATwset.size(0);
    i1 = (idx + colOffset) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 0.0;
  }
  i = obj.ATwset.size(0);
  i1 = i2 + colOffset;
  if ((i1 < 1) || (i1 > i)) {
    rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
  }
  obj.ATwset[i1 - 1] = -1.0;
  a_tmp = i2 + 1;
  b = obj.nVar;
  if ((i2 + 1 <= obj.nVar) && (obj.nVar > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx{a_tmp}; idx <= b; idx++) {
    i = obj.ATwset.size(0);
    i1 = idx + colOffset;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 0.0;
  }
  switch (obj.probType) {
  case 3:
  case 2:
    break;
  default:
    i = obj.ATwset.size(0);
    i1 = obj.nVar + colOffset;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = -1.0;
    break;
  }
}

//
// Arguments    : d_struct_T &obj
//                int idx_local
// Return Type  : void
//
void b_addBoundToActiveSetMatrix_(d_struct_T &obj, int idx_local)
{
  int a_tmp;
  int b;
  int colOffset;
  int i;
  int i1;
  int i2;
  obj.nWConstr[4]++;
  i = obj.isActiveConstr.size(0);
  i1 = (obj.isActiveIdx[4] + idx_local) - 1;
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
  obj.Wid[i] = 5;
  i1 = obj.Wlocalidx.size(0);
  if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
    rtDynamicBoundsError(obj.nActiveConstr, 1, i1, q_emlrtBCI);
  }
  obj.Wlocalidx[i] = idx_local;
  colOffset = obj.ldA * i;
  i1 = obj.indexUB.size(0);
  if ((idx_local < 1) || (idx_local > i1)) {
    rtDynamicBoundsError(idx_local, 1, i1, v_emlrtBCI);
  }
  i1 = obj.ub.size(0);
  i2 = obj.indexUB[idx_local - 1];
  if ((i2 < 1) || (i2 > i1)) {
    rtDynamicBoundsError(obj.indexUB[idx_local - 1], 1, i1, v_emlrtBCI);
  }
  i1 = obj.bwset.size(0);
  if ((obj.nActiveConstr < 1) || (obj.nActiveConstr > i1)) {
    rtDynamicBoundsError(obj.nActiveConstr, 1, i1, v_emlrtBCI);
  }
  obj.bwset[i] = obj.ub[i2 - 1];
  for (int idx{0}; idx <= i2 - 2; idx++) {
    i = obj.ATwset.size(0);
    i1 = (idx + colOffset) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 0.0;
  }
  i = obj.ATwset.size(0);
  i1 = i2 + colOffset;
  if ((i1 < 1) || (i1 > i)) {
    rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
  }
  obj.ATwset[i1 - 1] = 1.0;
  a_tmp = i2 + 1;
  b = obj.nVar;
  if ((i2 + 1 <= obj.nVar) && (obj.nVar > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx{a_tmp}; idx <= b; idx++) {
    i = obj.ATwset.size(0);
    i1 = idx + colOffset;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = 0.0;
  }
  switch (obj.probType) {
  case 3:
  case 2:
    break;
  default:
    i = obj.ATwset.size(0);
    i1 = obj.nVar + colOffset;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, v_emlrtBCI);
    }
    obj.ATwset[i1 - 1] = -1.0;
    break;
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for addBoundToActiveSetMatrix_.cpp
//
// [EOF]
//
