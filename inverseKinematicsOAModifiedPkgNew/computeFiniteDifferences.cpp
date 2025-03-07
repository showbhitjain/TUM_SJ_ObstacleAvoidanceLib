//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFiniteDifferences.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "computeFiniteDifferences.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_internal_types1.h"
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
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
boolean_T computeFiniteDifferences(l_struct_T &obj, double fCurrent,
                                   array<double, 1U> &xk,
                                   array<double, 1U> &gradf,
                                   const array<double, 2U> &lb,
                                   const array<double, 2U> &ub)
{
  int idx;
  boolean_T evalOK;
  boolean_T exitg1;
  evalOK = true;
  obj.numEvals = 0;
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= obj.nVar - 1)) {
    double deltaX;
    double lbDiff;
    double ubDiff;
    boolean_T guard1;
    boolean_T modifiedStep;
    modifiedStep = false;
    deltaX = 1.4901161193847656E-8 *
             (1.0 - 2.0 * static_cast<double>(xk[idx] < 0.0)) *
             std::fmax(std::abs(xk[idx]), 1.0);
    if (obj.hasLB[idx] || obj.hasUB[idx]) {
      if (obj.hasLB[idx] && obj.hasUB[idx]) {
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
      } else if (obj.hasUB[idx]) {
        if ((xk[idx] <= ub[idx]) && (xk[idx] + deltaX > ub[idx])) {
          deltaX = -deltaX;
          modifiedStep = true;
        }
      } else if ((xk[idx] >= lb[idx]) && (xk[idx] + deltaX < lb[idx])) {
        deltaX = -deltaX;
        modifiedStep = true;
      }
    }
    lbDiff = xk[idx];
    xk[idx] = xk[idx] + deltaX;
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
    guard1 = false;
    if (!evalOK) {
      if (!modifiedStep) {
        deltaX = -deltaX;
        if (obj.hasLB[idx]) {
          ubDiff = xk[idx] + deltaX;
          if ((ubDiff >= lb[idx]) && obj.hasUB[idx] && (ubDiff <= ub[idx])) {
            modifiedStep = true;
          } else {
            modifiedStep = false;
          }
        } else {
          modifiedStep = false;
        }
        if ((!obj.hasBounds) || modifiedStep) {
          lbDiff = xk[idx];
          xk[idx] = xk[idx] + deltaX;
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
        guard1 = true;
      }
    } else {
      guard1 = true;
    }
    if (guard1) {
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
