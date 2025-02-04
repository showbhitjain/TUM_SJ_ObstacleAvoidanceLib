//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeAllIneqConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "removeAllIneqConstr.h"
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
void removeAllIneqConstr(d_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                    // iFirst
      -1,                    // iLast
      1,                     // lineNo
      1,                     // colNo
      "",                    // aName
      "removeAllIneqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/removeAllIneqConstr.p", // pName
      0                                    // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      1,                     // iFirst
      6,                     // iLast
      1,                     // lineNo
      1,                     // colNo
      "",                    // aName
      "removeAllIneqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/removeAllIneqConstr.p", // pName
      0                                    // checkKind
  };
  int idxEndIneq;
  int idxStartIneq;
  int idxStartIneq_tmp;
  idxStartIneq_tmp = obj.nWConstr[0] + obj.nWConstr[1];
  idxStartIneq = idxStartIneq_tmp + 1;
  idxEndIneq = obj.nActiveConstr;
  if ((idxStartIneq_tmp + 1 <= obj.nActiveConstr) &&
      (obj.nActiveConstr > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx_global{idxStartIneq}; idx_global <= idxEndIneq; idx_global++) {
    int i;
    int i1;
    i = obj.Wid.size(0);
    if ((idx_global < 1) || (idx_global > i)) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    i = obj.Wlocalidx.size(0);
    if (idx_global > i) {
      rtDynamicBoundsError(idx_global, 1, i, w_emlrtBCI);
    }
    i = obj.Wid[idx_global - 1];
    if ((i < 1) || (i > 6)) {
      rtDynamicBoundsError(obj.Wid[idx_global - 1], 1, 6, x_emlrtBCI);
    }
    i1 = obj.isActiveConstr.size(0);
    i = (obj.isActiveIdx[i - 1] + obj.Wlocalidx[idx_global - 1]) - 1;
    if ((i < 1) || (i > i1)) {
      rtDynamicBoundsError(i, 1, i1, w_emlrtBCI);
    }
    obj.isActiveConstr[i - 1] = false;
  }
  obj.nWConstr[2] = 0;
  obj.nWConstr[3] = 0;
  obj.nWConstr[4] = 0;
  obj.nActiveConstr = idxStartIneq_tmp;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for removeAllIneqConstr.cpp
//
// [EOF]
//
