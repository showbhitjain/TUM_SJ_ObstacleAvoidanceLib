//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeMeritFcn.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "computeMeritFcn.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : double obj_penaltyParam
//                double fval
//                const array<double, 1U> &Cineq_workspace
//                int mIneq
//                const double Ceq_workspace_data[]
//                int mEq
//                boolean_T evalWellDefined
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace MeritFunction {
double computeMeritFcn(double obj_penaltyParam, double fval,
                       const array<double, 1U> &Cineq_workspace, int mIneq,
                       const double Ceq_workspace_data[], int mEq,
                       boolean_T evalWellDefined)
{
  double val;
  if (evalWellDefined) {
    double constrViolationEq;
    double constrViolationIneq;
    int idx;
    constrViolationEq = 0.0;
    if (mEq >= 1) {
      if (mEq > 2147483646) {
        check_forloop_overflow_error();
      }
      idx = static_cast<unsigned char>(mEq);
      for (int k{0}; k < idx; k++) {
        constrViolationEq += std::abs(Ceq_workspace_data[k]);
      }
    }
    constrViolationIneq = 0.0;
    if (mIneq > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < mIneq; idx++) {
      double d;
      if ((idx + 1 < 1) || (idx + 1 > Cineq_workspace.size(0))) {
        rtDynamicBoundsError(idx + 1, 1, Cineq_workspace.size(0), b_emlrtBCI);
      }
      d = Cineq_workspace[idx];
      if (d > 0.0) {
        if (idx + 1 > Cineq_workspace.size(0)) {
          rtDynamicBoundsError(idx + 1, 1, Cineq_workspace.size(0), b_emlrtBCI);
        }
        constrViolationIneq += d;
      }
    }
    val = fval + obj_penaltyParam * (constrViolationEq + constrViolationIneq);
  } else {
    val = rtInf;
  }
  return val;
}

} // namespace MeritFunction
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeMeritFcn.cpp
//
// [EOF]
//
