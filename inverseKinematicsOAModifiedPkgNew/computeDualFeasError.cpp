//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeDualFeasError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "computeDualFeasError.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo emlrtBCI{
    -1,                     // iFirst
    -1,                     // iLast
    1,                      // lineNo
    1,                      // colNo
    "",                     // aName
    "computeDualFeasError", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
    "+stopping/computeDualFeasError.p", // pName
    0                                   // checkKind
};

// Function Definitions
//
// Arguments    : int nVar
//                const array<double, 1U> &gradLag
//                double &val
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
boolean_T computeDualFeasError(int nVar, const array<double, 1U> &gradLag,
                               double &val)
{
  int idx;
  boolean_T exitg1;
  boolean_T gradOK;
  gradOK = true;
  val = 0.0;
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= nVar - 1)) {
    boolean_T b;
    b = ((idx + 1 < 1) || (idx + 1 > gradLag.size(0)));
    if (b) {
      rtDynamicBoundsError(idx + 1, 1, gradLag.size(0), emlrtBCI);
    }
    gradOK = ((!std::isinf(gradLag[idx])) && (!std::isnan(gradLag[idx])));
    if (!gradOK) {
      exitg1 = true;
    } else {
      val = std::fmax(val, std::abs(gradLag[idx]));
      idx++;
    }
  }
  return gradOK;
}

//
// Arguments    : int nVar
//                const array<double, 2U> &gradLag
//                double &val
// Return Type  : boolean_T
//
boolean_T computeDualFeasError(int nVar, const array<double, 2U> &gradLag,
                               double &val)
{
  int idx;
  boolean_T exitg1;
  boolean_T gradOK;
  gradOK = true;
  val = 0.0;
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= nVar - 1)) {
    int i;
    i = gradLag.size(0) * gradLag.size(1);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, emlrtBCI);
    }
    gradOK = ((!std::isinf(gradLag[idx])) && (!std::isnan(gradLag[idx])));
    if (!gradOK) {
      exitg1 = true;
    } else {
      val = std::fmax(val, std::abs(gradLag[idx]));
      idx++;
    }
  }
  return gradOK;
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeDualFeasError.cpp
//
// [EOF]
//
