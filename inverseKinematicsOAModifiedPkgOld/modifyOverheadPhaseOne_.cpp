//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: modifyOverheadPhaseOne_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "modifyOverheadPhaseOne_.h"
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
void modifyOverheadPhaseOne_(d_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                        // iFirst
      -1,                        // iLast
      1,                         // lineNo
      1,                         // colNo
      "",                        // aName
      "modifyOverheadPhaseOne_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/modifyOverheadPhaseOne_.p", // pName
      0                                        // checkKind
  };
  int b;
  int i;
  int idxEq;
  int idxStartIneq;
  b = obj.sizes[0];
  if (obj.sizes[0] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < b; idx++) {
    i = obj.ATwset.size(0);
    idxEq = obj.nVar + obj.ldA * idx;
    if ((idxEq < 1) || (idxEq > i)) {
      rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
    }
    obj.ATwset[idxEq - 1] = 0.0;
  }
  b = obj.sizes[1];
  if (obj.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < b; idx++) {
    idxEq = obj.nVar + obj.ldA * idx;
    i = obj.Aeq.size(0);
    if ((idxEq < 1) || (idxEq > i)) {
      rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
    }
    obj.Aeq[idxEq - 1] = 0.0;
    i = obj.ATwset.size(0);
    idxEq += obj.ldA * (obj.isActiveIdx[1] - 1);
    if ((idxEq < 1) || (idxEq > i)) {
      rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
    }
    obj.ATwset[idxEq - 1] = 0.0;
  }
  b = obj.sizes[2];
  if (obj.sizes[2] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < b; idx++) {
    i = obj.Aineq.size(0);
    idxEq = obj.nVar + obj.ldA * idx;
    if ((idxEq < 1) || (idxEq > i)) {
      rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
    }
    obj.Aineq[idxEq - 1] = -1.0;
  }
  i = obj.indexLB.size(0);
  if ((obj.sizes[3] < 1) || (obj.sizes[3] > i)) {
    rtDynamicBoundsError(obj.sizes[3], 1, i, w_emlrtBCI);
  }
  obj.indexLB[obj.sizes[3] - 1] = obj.nVar;
  i = obj.lb.size(0);
  if ((obj.nVar < 1) || (obj.nVar > i)) {
    rtDynamicBoundsError(obj.nVar, 1, i, w_emlrtBCI);
  }
  obj.lb[obj.nVar - 1] = 1.0E-5;
  idxStartIneq = obj.isActiveIdx[2];
  b = obj.nActiveConstr;
  if ((obj.isActiveIdx[2] <= obj.nActiveConstr) &&
      (obj.nActiveConstr > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx{idxStartIneq}; idx <= b; idx++) {
    i = obj.ATwset.size(0);
    idxEq = obj.nVar + obj.ldA * (idx - 1);
    if ((idxEq < 1) || (idxEq > i)) {
      rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
    }
    obj.ATwset[idxEq - 1] = -1.0;
  }
  if (obj.nWConstr[4] > 0) {
    b = obj.sizesNormal[4];
    if (obj.sizesNormal[4] + 1 > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx <= b; idx++) {
      i = obj.isActiveConstr.size(0);
      idxEq = obj.isActiveIdx[4] + idx;
      if ((idxEq < 1) || (idxEq > i)) {
        rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
      }
      obj.isActiveConstr[idxEq - 1] = false;
    }
  }
  i = obj.isActiveConstr.size(0);
  idxEq = obj.isActiveIdx[4] - 1;
  if ((idxEq < 1) || (idxEq > i)) {
    rtDynamicBoundsError(idxEq, 1, i, w_emlrtBCI);
  }
  obj.isActiveConstr[idxEq - 1] = false;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for modifyOverheadPhaseOne_.cpp
//
// [EOF]
//
