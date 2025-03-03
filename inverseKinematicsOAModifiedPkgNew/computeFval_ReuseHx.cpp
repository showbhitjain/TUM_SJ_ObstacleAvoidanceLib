//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFval_ReuseHx.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "computeFval_ReuseHx.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const g_struct_T &obj
//                array<double, 2U> &workspace
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
double computeFval_ReuseHx(const g_struct_T &obj, array<double, 2U> &workspace,
                           const array<double, 1U> &f,
                           const array<double, 1U> &x)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                    // iFirst
      -1,                    // iLast
      1,                     // lineNo
      1,                     // colNo
      "",                    // aName
      "computeFval_ReuseHx", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+Objective/computeFval_ReuseHx.p", // pName
      0                                   // checkKind
  };
  double val;
  switch (obj.objtype) {
  case 5:
    if ((obj.nvar < 1) || (obj.nvar > x.size(0))) {
      rtDynamicBoundsError(obj.nvar, 1, x.size(0), w_emlrtBCI);
    }
    val = obj.gammaScalar * x[obj.nvar - 1];
    break;
  case 3: {
    if (obj.hasLinear) {
      int ixlast;
      int k;
      ixlast = obj.nvar;
      if (obj.nvar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int i{0}; i < ixlast; i++) {
        int idx;
        k = workspace.size(0) * workspace.size(1);
        idx = obj.Hx.size(0);
        if ((i + 1 < 1) || (i + 1 > idx)) {
          rtDynamicBoundsError(i + 1, 1, idx, w_emlrtBCI);
        }
        if (i + 1 > f.size(0)) {
          rtDynamicBoundsError(i + 1, 1, f.size(0), w_emlrtBCI);
        }
        if (i + 1 > k) {
          rtDynamicBoundsError(i + 1, 1, k, w_emlrtBCI);
        }
        workspace[i] = 0.5 * obj.Hx[i] + f[i];
      }
      val = 0.0;
      if (obj.nvar >= 1) {
        ixlast = obj.nvar;
        if (obj.nvar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (k = 0; k < ixlast; k++) {
          val += x[k] * workspace[k];
        }
      }
    } else {
      val = 0.0;
      if (obj.nvar >= 1) {
        int ixlast;
        ixlast = obj.nvar;
        if (obj.nvar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int k{0}; k < ixlast; k++) {
          val += x[k] * obj.Hx[k];
        }
      }
      val *= 0.5;
    }
  } break;
  default: {
    int maxRegVar_tmp;
    maxRegVar_tmp = obj.maxVar - 1;
    if (obj.hasLinear) {
      int idx;
      int ixlast;
      int k;
      ixlast = obj.nvar;
      if (obj.nvar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int i{0}; i < ixlast; i++) {
        k = workspace.size(0) * workspace.size(1);
        if ((i + 1 < 1) || (i + 1 > f.size(0))) {
          rtDynamicBoundsError(i + 1, 1, f.size(0), w_emlrtBCI);
        }
        if (i + 1 > k) {
          rtDynamicBoundsError(i + 1, 1, k, w_emlrtBCI);
        }
        workspace[i] = f[i];
      }
      ixlast = (obj.maxVar - obj.nvar) - 1;
      if (ixlast > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int i{0}; i < ixlast; i++) {
        k = workspace.size(0) * workspace.size(1);
        idx = (obj.nvar + i) + 1;
        if ((idx < 1) || (idx > k)) {
          rtDynamicBoundsError(idx, 1, k, w_emlrtBCI);
        }
        workspace[idx - 1] = obj.rho;
      }
      if (maxRegVar_tmp > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int i{0}; i < maxRegVar_tmp; i++) {
        k = workspace.size(0) * workspace.size(1);
        if ((i + 1 < 1) || (i + 1 > k)) {
          rtDynamicBoundsError(i + 1, 1, k, w_emlrtBCI);
        }
        idx = obj.Hx.size(0);
        if (i + 1 > idx) {
          rtDynamicBoundsError(i + 1, 1, idx, w_emlrtBCI);
        }
        if (i + 1 > k) {
          rtDynamicBoundsError(i + 1, 1, k, w_emlrtBCI);
        }
        workspace[i] = workspace[i] + 0.5 * obj.Hx[i];
      }
      val = 0.0;
      if (maxRegVar_tmp >= 1) {
        ixlast = obj.maxVar;
        for (k = 0; k <= ixlast - 2; k++) {
          val += x[k] * workspace[k];
        }
      }
    } else {
      int ixlast;
      val = 0.0;
      if (maxRegVar_tmp >= 1) {
        ixlast = obj.maxVar;
        if (maxRegVar_tmp > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int k{0}; k <= ixlast - 2; k++) {
          val += x[k] * obj.Hx[k];
        }
      }
      val *= 0.5;
      ixlast = obj.nvar + 1;
      for (int idx{ixlast}; idx <= maxRegVar_tmp; idx++) {
        if ((idx < 1) || (idx > x.size(0))) {
          rtDynamicBoundsError(idx, 1, x.size(0), w_emlrtBCI);
        }
        val += x[idx - 1] * obj.rho;
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
// File trailer for computeFval_ReuseHx.cpp
//
// [EOF]
//
