//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: isDeltaXTooSmall.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "isDeltaXTooSmall.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &xCurrent
//                const array<double, 1U> &delta_x
//                int nVar
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
boolean_T isDeltaXTooSmall(const array<double, 1U> &xCurrent,
                           const array<double, 1U> &delta_x, int nVar)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                 // iFirst
      -1,                 // iLast
      1,                  // lineNo
      1,                  // colNo
      "",                 // aName
      "isDeltaXTooSmall", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+stopping/isDeltaXTooSmall.p", // pName
      0                               // checkKind
  };
  int idx;
  boolean_T exitg1;
  boolean_T tf;
  tf = true;
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= nVar - 1)) {
    if ((idx + 1 < 1) || (idx + 1 > xCurrent.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, xCurrent.size(0), w_emlrtBCI);
    }
    if ((idx + 1 < 1) || (idx + 1 > delta_x.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, delta_x.size(0), w_emlrtBCI);
    }
    if (1.0E-6 * std::fmax(1.0, std::abs(xCurrent[idx])) <=
        std::abs(delta_x[idx])) {
      tf = false;
      exitg1 = true;
    } else {
      idx++;
    }
  }
  return tf;
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for isDeltaXTooSmall.cpp
//
// [EOF]
//
