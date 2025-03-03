//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGradLag.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "computeGradLag.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo f_emlrtBCI{
    -1,               // iFirst
    -1,               // iLast
    1,                // lineNo
    1,                // colNo
    "",               // aName
    "computeGradLag", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
    "+stopping/computeGradLag.p", // pName
    0                             // checkKind
};

// Function Definitions
//
// Arguments    : array<double, 1U> &workspace
//                int ldA
//                int nVar
//                const array<double, 1U> &grad
//                int mIneq
//                const array<double, 1U> &AineqTrans
//                int mEq
//                const array<double, 1U> &AeqTrans
//                const array<int, 1U> &finiteFixed
//                int mFixed
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &lambda
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
void computeGradLag(array<double, 1U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int i;
  int iL0;
  boolean_T b;
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (iL0 = 0; iL0 < nVar; iL0++) {
    if ((iL0 + 1 < 1) || (iL0 + 1 > grad.size(0))) {
      rtDynamicBoundsError(iL0 + 1, 1, grad.size(0), f_emlrtBCI);
    }
    i = workspace.size(0);
    if (iL0 + 1 > i) {
      rtDynamicBoundsError(iL0 + 1, 1, i, f_emlrtBCI);
    }
    workspace[iL0] = grad[iL0];
  }
  if (mFixed > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mFixed; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteFixed.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteFixed.size(0), f_emlrtBCI);
    }
    i = workspace.size(0);
    b = ((finiteFixed[idx] < 1) || (finiteFixed[idx] > i));
    if (b) {
      rtDynamicBoundsError(finiteFixed[idx], 1, i, f_emlrtBCI);
    }
    if ((idx + 1 < 1) || (idx + 1 > lambda.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteFixed[idx] - 1] =
        workspace[finiteFixed[idx] - 1] + lambda[idx];
  }
  ::coder::internal::blas::xgemv(nVar, mEq, AeqTrans, ldA, lambda, mFixed + 1,
                                 workspace);
  iL0 = (mFixed + mEq) + 1;
  ::coder::internal::blas::xgemv(nVar, mIneq, AineqTrans, ldA, lambda, iL0,
                                 workspace);
  iL0 += mIneq;
  if (mLB > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mLB; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteLB.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteLB.size(0), f_emlrtBCI);
    }
    i = workspace.size(0);
    b = ((finiteLB[idx] < 1) || (finiteLB[idx] > i));
    if (b) {
      rtDynamicBoundsError(finiteLB[idx], 1, i, f_emlrtBCI);
    }
    i = iL0 + idx;
    if ((i < 1) || (i > lambda.size(0))) {
      rtDynamicBoundsError(i, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteLB[idx] - 1] = workspace[finiteLB[idx] - 1] - lambda[i - 1];
  }
  if (mLB - 1 >= 0) {
    iL0 += mLB;
  }
  if (mUB > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mUB; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteUB.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteUB.size(0), f_emlrtBCI);
    }
    i = workspace.size(0);
    b = ((finiteUB[idx] < 1) || (finiteUB[idx] > i));
    if (b) {
      rtDynamicBoundsError(finiteUB[idx], 1, i, f_emlrtBCI);
    }
    i = iL0 + idx;
    if ((i < 1) || (i > lambda.size(0))) {
      rtDynamicBoundsError(i, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteUB[idx] - 1] = workspace[finiteUB[idx] - 1] + lambda[i - 1];
  }
}

//
// Arguments    : array<double, 2U> &workspace
//                int ldA
//                int nVar
//                const array<double, 1U> &grad
//                int mIneq
//                const array<double, 1U> &AineqTrans
//                int mEq
//                const array<double, 1U> &AeqTrans
//                const array<int, 1U> &finiteFixed
//                int mFixed
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &lambda
// Return Type  : void
//
void computeGradLag(array<double, 2U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int iL0;
  int workspace_tmp;
  boolean_T b;
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (iL0 = 0; iL0 < nVar; iL0++) {
    workspace_tmp = workspace.size(0) * workspace.size(1);
    if ((iL0 + 1 < 1) || (iL0 + 1 > grad.size(0))) {
      rtDynamicBoundsError(iL0 + 1, 1, grad.size(0), f_emlrtBCI);
    }
    if (iL0 + 1 > workspace_tmp) {
      rtDynamicBoundsError(iL0 + 1, 1, workspace_tmp, f_emlrtBCI);
    }
    workspace[iL0] = grad[iL0];
  }
  if (mFixed > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mFixed; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteFixed.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteFixed.size(0), f_emlrtBCI);
    }
    workspace_tmp = workspace.size(0) * workspace.size(1);
    b = ((finiteFixed[idx] < 1) || (finiteFixed[idx] > workspace_tmp));
    if (b) {
      rtDynamicBoundsError(finiteFixed[idx], 1, workspace_tmp, f_emlrtBCI);
    }
    if ((idx + 1 < 1) || (idx + 1 > lambda.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteFixed[idx] - 1] =
        workspace[finiteFixed[idx] - 1] + lambda[idx];
  }
  ::coder::internal::blas::xgemv(nVar, mEq, AeqTrans, ldA, lambda, mFixed + 1,
                                 workspace);
  iL0 = (mFixed + mEq) + 1;
  ::coder::internal::blas::xgemv(nVar, mIneq, AineqTrans, ldA, lambda, iL0,
                                 workspace);
  iL0 += mIneq;
  if (mLB > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mLB; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteLB.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteLB.size(0), f_emlrtBCI);
    }
    workspace_tmp = workspace.size(0) * workspace.size(1);
    b = ((finiteLB[idx] < 1) || (finiteLB[idx] > workspace_tmp));
    if (b) {
      rtDynamicBoundsError(finiteLB[idx], 1, workspace_tmp, f_emlrtBCI);
    }
    workspace_tmp = iL0 + idx;
    if ((workspace_tmp < 1) || (workspace_tmp > lambda.size(0))) {
      rtDynamicBoundsError(workspace_tmp, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteLB[idx] - 1] =
        workspace[finiteLB[idx] - 1] - lambda[workspace_tmp - 1];
  }
  if (mLB - 1 >= 0) {
    iL0 += mLB;
  }
  if (mUB > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mUB; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > finiteUB.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, finiteUB.size(0), f_emlrtBCI);
    }
    workspace_tmp = workspace.size(0) * workspace.size(1);
    b = ((finiteUB[idx] < 1) || (finiteUB[idx] > workspace_tmp));
    if (b) {
      rtDynamicBoundsError(finiteUB[idx], 1, workspace_tmp, f_emlrtBCI);
    }
    workspace_tmp = iL0 + idx;
    if ((workspace_tmp < 1) || (workspace_tmp > lambda.size(0))) {
      rtDynamicBoundsError(workspace_tmp, 1, lambda.size(0), f_emlrtBCI);
    }
    workspace[finiteUB[idx] - 1] =
        workspace[finiteUB[idx] - 1] + lambda[workspace_tmp - 1];
  }
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeGradLag.cpp
//
// [EOF]
//
