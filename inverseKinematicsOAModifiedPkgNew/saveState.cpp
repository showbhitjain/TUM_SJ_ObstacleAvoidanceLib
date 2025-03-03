//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: saveState.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "saveState.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : j_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace TrialState {
void saveState(j_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,          // iFirst
      -1,          // iLast
      1,           // lineNo
      1,           // colNo
      "",          // aName
      "saveState", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+TrialState/saveState.p", // pName
      0                          // checkKind
  };
  array<double, 1U> y;
  int i;
  int n;
  int nVar;
  obj.sqpFval_old = obj.sqpFval;
  nVar = obj.xstarsqp.size(0) - 1;
  if (obj.xstarsqp.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k <= nVar; k++) {
    obj.xstarsqp_old[k] = obj.xstarsqp[k];
  }
  if (obj.xstarsqp.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k <= nVar; k++) {
    i = obj.grad.size(0);
    if (k + 1 > i) {
      rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
    }
    i = obj.grad_old.size(0);
    if (k + 1 > i) {
      rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
    }
    obj.grad_old[k] = obj.grad[k];
  }
  n = obj.mIneq;
  if (obj.mIneq > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k < n; k++) {
    obj.cIneq_old[k] = obj.cIneq[k];
  }
  n = obj.mEq;
  y.set_size(obj.cEq_old.size[0]);
  nVar = obj.cEq_old.size[0];
  for (i = 0; i < nVar; i++) {
    y[i] = obj.cEq_old.data[i];
  }
  if (obj.mEq > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k < n; k++) {
    y[k] = obj.cEq.data[k];
  }
  obj.cEq_old.size[0] = y.size(0);
  nVar = y.size(0);
  for (i = 0; i < nVar; i++) {
    obj.cEq_old.data[i] = y[i];
  }
}

} // namespace TrialState
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for saveState.cpp
//
// [EOF]
//
