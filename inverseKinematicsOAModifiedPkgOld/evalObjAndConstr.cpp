//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: evalObjAndConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "evalObjAndConstr.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "stickyStruct.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const ::coder::internal::i_stickyStruct &obj
//                const array<double, 1U> &x
//                int &status
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace ObjNonlinEvaluator {
double evalObjAndConstr(const ::coder::internal::i_stickyStruct &obj,
                        const array<double, 1U> &x, int &status)
{
  double fval;
  boolean_T b;
  if (x.size(0) == 0) {
    d_rtErrorWithMessageID("input", emlrtRTEI.fName, emlrtRTEI.lineNo);
  }
  fval = inverseKinematicsOAModified_anonFcn1(
      obj.next.next.next.next.next.next.next.next.value.workspace.jacobi,
      obj.next.next.next.next.next.next.next.next.value.workspace.xd_eff_vel,
      obj.next.next.next.next.next.next.next.next.value.workspace
          .starting_joint_vel,
      obj.next.next.next.next.next.next.next.next.value.workspace.joint_values,
      obj.next.next.next.next.next.next.next.next.value.workspace
          .jointVelocityWeightMatrix,
      obj.next.next.next.next.next.next.next.next.value.workspace.config,
      obj.next.next.next.next.next.next.next.next.value.workspace.slackIsUsed,
      x);
  status = 1;
  b = std::isnan(fval);
  if (std::isinf(fval) || b) {
    if (b) {
      status = -3;
    } else if (fval < 0.0) {
      status = -1;
    } else {
      status = -2;
    }
  }
  if (status == 1) {
    status = 1;
  }
  return fval;
}

} // namespace ObjNonlinEvaluator
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for evalObjAndConstr.cpp
//
// [EOF]
//
