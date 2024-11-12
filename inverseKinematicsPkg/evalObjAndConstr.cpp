//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: evalObjAndConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "evalObjAndConstr.h"
#include "anonymous_function.h"
#include "inverseKinematicsWithConstraints.h"
#include "inverseKinematics_internal_types1.h"
#include "inverseKinematics_types.h"
#include "rt_nonfinite.h"
#include "stickyStruct.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
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
  fval = inverseKinematics_anonFcn1(
      obj.next.next.next.next.next.next.next.next.value.workspace.jacobi,
      obj.next.next.next.next.next.next.next.next.value.workspace.xd_eff_vel
          .data,
      obj.next.next.next.next.next.next.next.next.value.workspace.xd_eff_vel
          .size[0],
      obj.next.next.next.next.next.next.next.next.value.workspace
          .starting_joint_vel,
      obj.next.next.next.next.next.next.next.next.value.workspace.joint_values,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective1,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight1,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective2,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight2,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective3,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight3,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective4,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight4,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective5,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight5,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .useObjective6,
      obj.next.next.next.next.next.next.next.next.value.workspace.config
          .weight6,
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
