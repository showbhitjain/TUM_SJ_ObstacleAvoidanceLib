//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factoryConstruct1.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "factoryConstruct1.h"
#include "anonymous_function.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const anonymous_function &objfun
//                int nVar
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                l_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
void factoryConstruct(const anonymous_function &objfun, int nVar,
                      const array<double, 2U> &lb, const array<double, 2U> &ub,
                      l_struct_T &obj)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "hasFiniteBounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/shared/optimlib/+optim/+coder/+utils/"
      "hasFiniteBounds.p", // pName
      0                    // checkKind
  };
  int idx;
  boolean_T b;
  obj.objfun = objfun;
  obj.f_1 = 0.0;
  obj.f_2 = 0.0;
  obj.nVar = nVar;
  obj.mIneq = 0;
  obj.mEq = 0;
  obj.numEvals = 0;
  obj.SpecifyObjectiveGradient = false;
  obj.SpecifyConstraintGradient = false;
  obj.isEmptyNonlcon = true;
  obj.hasLB.set_size(nVar);
  obj.hasUB.set_size(nVar);
  obj.FiniteDifferenceType = 0;
  b = false;
  idx = 1;
  switch (static_cast<unsigned int>(ub.size(0) == 0) << 1 |
          static_cast<unsigned int>(lb.size(0) == 0)) {
  case 0U: {
    double d;
    while ((!b) && (idx <= nVar)) {
      if ((idx < 1) || (idx > lb.size(0))) {
        rtDynamicBoundsError(idx, 1, lb.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      d = lb[idx - 1];
      obj.hasLB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > ub.size(0)) {
        rtDynamicBoundsError(idx, 1, ub.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      d = ub[idx - 1];
      obj.hasUB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      if (obj.hasLB[idx - 1]) {
        b = true;
      } else {
        if (idx > obj.hasUB.size(0)) {
          rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
        }
        if (obj.hasUB[idx - 1]) {
          b = true;
        }
      }
      idx++;
    }
    while (idx <= nVar) {
      if ((idx < 1) || (idx > lb.size(0))) {
        rtDynamicBoundsError(idx, 1, lb.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      d = lb[idx - 1];
      obj.hasLB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > ub.size(0)) {
        rtDynamicBoundsError(idx, 1, ub.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      d = ub[idx - 1];
      obj.hasUB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      idx++;
    }
  } break;
  case 1U: {
    double d;
    while ((!b) && (idx <= nVar)) {
      if ((idx < 1) || (idx > obj.hasLB.size(0))) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      obj.hasLB[idx - 1] = false;
      if (idx > ub.size(0)) {
        rtDynamicBoundsError(idx, 1, ub.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      d = ub[idx - 1];
      obj.hasUB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      b = obj.hasUB[idx - 1];
      idx++;
    }
    while (idx <= nVar) {
      if ((idx < 1) || (idx > obj.hasLB.size(0))) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      obj.hasLB[idx - 1] = false;
      if (idx > ub.size(0)) {
        rtDynamicBoundsError(idx, 1, ub.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      d = ub[idx - 1];
      obj.hasUB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      idx++;
    }
  } break;
  case 2U: {
    double d;
    while ((!b) && (idx <= nVar)) {
      if ((idx < 1) || (idx > lb.size(0))) {
        rtDynamicBoundsError(idx, 1, lb.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      d = lb[idx - 1];
      obj.hasLB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      obj.hasUB[idx - 1] = false;
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      b = obj.hasLB[idx - 1];
      idx++;
    }
    while (idx <= nVar) {
      if ((idx < 1) || (idx > lb.size(0))) {
        rtDynamicBoundsError(idx, 1, lb.size(0), w_emlrtBCI);
      }
      if (idx > obj.hasLB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      d = lb[idx - 1];
      obj.hasLB[idx - 1] = ((!std::isinf(d)) && (!std::isnan(d)));
      if (idx > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      obj.hasUB[idx - 1] = false;
      idx++;
    }
  } break;
  default:
    if (nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < nVar; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > obj.hasLB.size(0))) {
        rtDynamicBoundsError(idx + 1, 1, obj.hasLB.size(0), w_emlrtBCI);
      }
      obj.hasLB[idx] = false;
      if (idx + 1 > obj.hasUB.size(0)) {
        rtDynamicBoundsError(idx + 1, 1, obj.hasUB.size(0), w_emlrtBCI);
      }
      obj.hasUB[idx] = false;
    }
    break;
  }
  obj.hasBounds = b;
}

} // namespace FiniteDifferences
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factoryConstruct1.cpp
//
// [EOF]
//
