//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFval.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "computeFval.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "linearForm_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const g_struct_T &obj
//                array<double, 2U> &workspace
//                const array<double, 2U> &H
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
double computeFval(const g_struct_T &obj, array<double, 2U> &workspace,
                   const array<double, 2U> &H, const array<double, 1U> &f,
                   const array<double, 1U> &x)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,               // iFirst
      -1,               // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "linearFormReg_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+Objective/linearFormReg_.p", // pName
      0                              // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,            // iFirst
      -1,            // iLast
      1,             // lineNo
      1,             // colNo
      "",            // aName
      "computeFval", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+Objective/computeFval.p", // pName
      0                           // checkKind
  };
  double val;
  switch (obj.objtype) {
  case 5:
    if ((obj.nvar < 1) || (obj.nvar > x.size(0))) {
      rtDynamicBoundsError(obj.nvar, 1, x.size(0), x_emlrtBCI);
    }
    val = obj.gammaScalar * x[obj.nvar - 1];
    break;
  case 3: {
    linearForm_(obj.hasLinear, obj.nvar, workspace, H, f, x);
    val = 0.0;
    if (obj.nvar >= 1) {
      int ixlast;
      ixlast = obj.nvar;
      if (obj.nvar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int b_tmp{0}; b_tmp < ixlast; b_tmp++) {
        val += x[b_tmp] * workspace[b_tmp];
      }
    }
  } break;
  default: {
    int b_tmp;
    int ixlast;
    linearForm_(obj.hasLinear, obj.nvar, workspace, H, f, x);
    ixlast = obj.nvar + 1;
    b_tmp = obj.maxVar - 1;
    if ((ixlast <= b_tmp) && (b_tmp > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx{ixlast}; idx <= b_tmp; idx++) {
      int workspace_tmp;
      workspace_tmp = workspace.size(0) * workspace.size(1);
      if ((idx < 1) || (idx > x.size(0))) {
        rtDynamicBoundsError(idx, 1, x.size(0), w_emlrtBCI);
      }
      if (idx > workspace_tmp) {
        rtDynamicBoundsError(idx, 1, workspace_tmp, w_emlrtBCI);
      }
      workspace[idx - 1] = 0.5 * obj.beta * x[idx - 1] + obj.rho;
    }
    val = 0.0;
    if (b_tmp >= 1) {
      ixlast = obj.maxVar;
      for (b_tmp = 0; b_tmp <= ixlast - 2; b_tmp++) {
        val += x[b_tmp] * workspace[b_tmp];
      }
    }
  } break;
  }
  return val;
}

} // namespace Objective
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeFval.cpp
//
// [EOF]
//
