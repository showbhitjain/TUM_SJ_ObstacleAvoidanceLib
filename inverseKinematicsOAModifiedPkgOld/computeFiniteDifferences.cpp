//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFiniteDifferences.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "computeFiniteDifferences.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : l_struct_T &obj
//                double fCurrent
//                array<double, 1U> &xk
//                array<double, 1U> &gradf
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                const array<double, 1U>
//                &runTimeOptions_FiniteDifferenceStepSize const array<double,
//                1U> &runTimeOptions_TypicalX
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
boolean_T computeFiniteDifferences(
    l_struct_T &obj, double fCurrent, array<double, 1U> &xk,
    array<double, 1U> &gradf, const array<double, 2U> &lb,
    const array<double, 2U> &ub,
    const array<double, 1U> &runTimeOptions_FiniteDifferenceStepSize,
    const array<double, 1U> &runTimeOptions_TypicalX)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "ForwardStepSize", // fName
      "/usr/local/MATLAB/R2023b/toolbox/shared/optimlib/+optim/+coder/+utils/"
      "+FiniteDifferences/+internal/ForwardStepSize.p", // pName
      0                                                 // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                          // iFirst
      -1,                          // iLast
      1,                           // lineNo
      1,                           // colNo
      "",                          // aName
      "computeForwardDifferences", // fName
      "/usr/local/MATLAB/R2023b/toolbox/shared/optimlib/+optim/+coder/+utils/"
      "+FiniteDifferences/+internal/computeForwardDifferences.p", // pName
      0                                                           // checkKind
  };
  int idx;
  boolean_T evalOK;
  boolean_T exitg1;
  evalOK = true;
  obj.numEvals = 0;
  if (obj.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= obj.nVar - 1)) {
    double deltaX;
    double lbDiff;
    double ubDiff;
    int i;
    boolean_T b_guard1;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T modifiedStep;
    modifiedStep = false;
    i = xk.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    if ((idx + 1 < 1) ||
        (idx + 1 > runTimeOptions_FiniteDifferenceStepSize.size(0))) {
      rtDynamicBoundsError(idx + 1, 1,
                           runTimeOptions_FiniteDifferenceStepSize.size(0),
                           w_emlrtBCI);
    }
    if ((idx + 1 < 1) || (idx + 1 > runTimeOptions_TypicalX.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, runTimeOptions_TypicalX.size(0),
                           w_emlrtBCI);
    }
    deltaX = 1.4901161193847656E-8 *
             (1.0 - 2.0 * static_cast<double>(xk[idx] < 0.0)) *
             std::fmax(std::abs(xk[idx]), 1.0);
    i = obj.hasLB.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
    }
    guard1 = false;
    guard2 = false;
    if (obj.hasLB[idx]) {
      guard2 = true;
    } else {
      i = obj.hasUB.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      if (obj.hasUB[idx]) {
        guard2 = true;
      }
    }
    if (guard2) {
      i = obj.hasLB.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      if (obj.hasLB[idx]) {
        i = obj.hasUB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
        }
        if (obj.hasUB[idx]) {
          if ((idx + 1 < 1) || (idx + 1 > lb.size(0))) {
            rtDynamicBoundsError(idx + 1, 1, lb.size(0), x_emlrtBCI);
          }
          if ((idx + 1 < 1) || (idx + 1 > ub.size(0))) {
            rtDynamicBoundsError(idx + 1, 1, ub.size(0), x_emlrtBCI);
          }
          lbDiff = deltaX;
          if ((lb[idx] != ub[idx]) && (xk[idx] >= lb[idx]) &&
              (xk[idx] <= ub[idx])) {
            ubDiff = xk[idx] + deltaX;
            if ((ubDiff > ub[idx]) || (ubDiff < lb[idx])) {
              lbDiff = -deltaX;
              modifiedStep = true;
              ubDiff = xk[idx] - deltaX;
              if ((ubDiff > ub[idx]) || (ubDiff < lb[idx])) {
                lbDiff = xk[idx] - lb[idx];
                ubDiff = ub[idx] - xk[idx];
                if (lbDiff <= ubDiff) {
                  lbDiff = -lbDiff;
                } else {
                  lbDiff = ubDiff;
                }
              }
            }
          }
          deltaX = lbDiff;
        } else {
          guard1 = true;
        }
      } else {
        guard1 = true;
      }
    }
    if (guard1) {
      i = obj.hasUB.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      if (obj.hasUB[idx]) {
        if ((idx + 1 < 1) || (idx + 1 > ub.size(0))) {
          rtDynamicBoundsError(idx + 1, 1, ub.size(0), x_emlrtBCI);
        }
        if ((xk[idx] <= ub[idx]) && (xk[idx] + deltaX > ub[idx])) {
          deltaX = -deltaX;
          modifiedStep = true;
        }
      } else {
        if ((idx + 1 < 1) || (idx + 1 > lb.size(0))) {
          rtDynamicBoundsError(idx + 1, 1, lb.size(0), x_emlrtBCI);
        }
        if ((xk[idx] >= lb[idx]) && (xk[idx] + deltaX < lb[idx])) {
          deltaX = -deltaX;
          modifiedStep = true;
        }
      }
    }
    lbDiff = xk[idx];
    xk[idx] = lbDiff + deltaX;
    ubDiff = inverseKinematicsOAModified_anonFcn1(
        obj.objfun.workspace.jacobi, obj.objfun.workspace.xd_eff_vel,
        obj.objfun.workspace.starting_joint_vel,
        obj.objfun.workspace.joint_values,
        obj.objfun.workspace.jointVelocityWeightMatrix,
        obj.objfun.workspace.config, obj.objfun.workspace.slackIsUsed, xk);
    evalOK = ((!std::isinf(ubDiff)) && (!std::isnan(ubDiff)));
    if (evalOK) {
      xk[idx] = lbDiff;
    }
    obj.f_1 = ubDiff;
    obj.numEvals++;
    b_guard1 = false;
    if (!evalOK) {
      if (!modifiedStep) {
        deltaX = -deltaX;
        i = obj.hasLB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
        }
        if (obj.hasLB[idx]) {
          if ((idx + 1 < 1) || (idx + 1 > lb.size(0))) {
            rtDynamicBoundsError(idx + 1, 1, lb.size(0), x_emlrtBCI);
          }
          ubDiff = xk[idx] + deltaX;
          if (ubDiff >= lb[idx]) {
            i = obj.hasUB.size(0);
            if ((idx + 1 < 1) || (idx + 1 > i)) {
              rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
            }
            if (obj.hasUB[idx]) {
              if ((idx + 1 < 1) || (idx + 1 > ub.size(0))) {
                rtDynamicBoundsError(idx + 1, 1, ub.size(0), x_emlrtBCI);
              }
              if (ubDiff <= ub[idx]) {
                modifiedStep = true;
              } else {
                modifiedStep = false;
              }
            } else {
              modifiedStep = false;
            }
          } else {
            modifiedStep = false;
          }
        } else {
          modifiedStep = false;
        }
        if ((!obj.hasBounds) || modifiedStep) {
          lbDiff = xk[idx];
          xk[idx] = lbDiff + deltaX;
          ubDiff = inverseKinematicsOAModified_anonFcn1(
              obj.objfun.workspace.jacobi, obj.objfun.workspace.xd_eff_vel,
              obj.objfun.workspace.starting_joint_vel,
              obj.objfun.workspace.joint_values,
              obj.objfun.workspace.jointVelocityWeightMatrix,
              obj.objfun.workspace.config, obj.objfun.workspace.slackIsUsed,
              xk);
          evalOK = ((!std::isinf(ubDiff)) && (!std::isnan(ubDiff)));
          if (evalOK) {
            xk[idx] = lbDiff;
          }
          obj.f_1 = ubDiff;
          obj.numEvals++;
        }
      }
      if (!evalOK) {
        exitg1 = true;
      } else {
        b_guard1 = true;
      }
    } else {
      b_guard1 = true;
    }
    if (b_guard1) {
      i = gradf.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      gradf[idx] = (obj.f_1 - fCurrent) / deltaX;
      idx++;
    }
  }
  return evalOK;
}

} // namespace FiniteDifferences
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeFiniteDifferences.cpp
//
// [EOF]
//
