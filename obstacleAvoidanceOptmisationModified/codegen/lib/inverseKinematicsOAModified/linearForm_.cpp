//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: linearForm_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "linearForm_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : boolean_T obj_hasLinear
//                int obj_nvar
//                array<double, 2U> &workspace
//                const array<double, 2U> &H
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
void linearForm_(boolean_T obj_hasLinear, int obj_nvar,
                 array<double, 2U> &workspace, const array<double, 2U> &H,
                 const array<double, 1U> &f, const array<double, 1U> &x)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,            // iFirst
      -1,            // iLast
      1,             // lineNo
      1,             // colNo
      "",            // aName
      "linearForm_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+Objective/linearForm_.p", // pName
      0                           // checkKind
  };
  int i;
  int ix;
  ix = 0;
  if (obj_hasLinear) {
    if (obj_nvar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (i = 0; i < obj_nvar; i++) {
      ix = workspace.size(0) * workspace.size(1);
      if ((i + 1 < 1) || (i + 1 > f.size(0))) {
        rtDynamicBoundsError(i + 1, 1, f.size(0), w_emlrtBCI);
      }
      if (i + 1 > ix) {
        rtDynamicBoundsError(i + 1, 1, ix, w_emlrtBCI);
      }
      workspace[i] = f[i];
    }
    ix = 1;
  }
  if (obj_nvar != 0) {
    boolean_T overflow;
    if (ix != 1) {
      if (obj_nvar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (ix = 0; ix < obj_nvar; ix++) {
        workspace[ix] = 0.0;
      }
    }
    ix = 0;
    i = obj_nvar * (obj_nvar - 1) + 1;
    if ((obj_nvar == 0) || ((obj_nvar > 0) && (i < 1)) ||
        ((obj_nvar < 0) && (i > 1))) {
      overflow = false;
    } else if (obj_nvar > 0) {
      overflow = (i > MAX_int32_T - obj_nvar);
    } else {
      overflow = (i < MIN_int32_T - obj_nvar);
    }
    if (obj_nvar == 0) {
      m_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; obj_nvar < 0 ? iac >= i : iac <= i; iac += obj_nvar) {
      double c;
      int b;
      c = 0.5 * x[ix];
      b = (iac + obj_nvar) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        int b_i;
        b_i = ia - iac;
        workspace[b_i] = workspace[b_i] + H[ia - 1] * c;
      }
      ix++;
    }
  }
}

} // namespace Objective
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for linearForm_.cpp
//
// [EOF]
//
