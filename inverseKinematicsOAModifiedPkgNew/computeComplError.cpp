//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeComplError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "computeComplError.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &fscales_lineq_constraint
//                const array<double, 1U> &xCurrent
//                int mIneq
//                const array<double, 1U> &cIneq
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<double, 2U> &lb
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 2U> &ub
//                const array<double, 1U> &lambda
//                int iL0
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
double computeComplError(const array<double, 1U> &fscales_lineq_constraint,
                         const array<double, 1U> &xCurrent, int mIneq,
                         const array<double, 1U> &cIneq,
                         const array<int, 1U> &finiteLB, int mLB,
                         const array<double, 2U> &lb,
                         const array<int, 1U> &finiteUB, int mUB,
                         const array<double, 2U> &ub,
                         const array<double, 1U> &lambda, int iL0)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "computeComplError", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+stopping/computeComplError.p", // pName
      0                                // checkKind
  };
  double nlpComplError;
  int mLinIneq;
  nlpComplError = 0.0;
  mLinIneq = fscales_lineq_constraint.size(0);
  if ((mIneq + mLB) + mUB > 0) {
    double lbDelta;
    double lbLambda;
    int i;
    int ubOffset;
    if (fscales_lineq_constraint.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mLinIneq; idx++) {
      boolean_T b;
      if (idx + 1 > fscales_lineq_constraint.size(0)) {
        rtDynamicBoundsError(idx + 1, 1, fscales_lineq_constraint.size(0),
                             w_emlrtBCI);
      }
      if (idx + 1 > cIneq.size(0)) {
        rtDynamicBoundsError(idx + 1, 1, cIneq.size(0), w_emlrtBCI);
      }
      i = iL0 + idx;
      b = ((i < 1) || (i > lambda.size(0)));
      if (b) {
        rtDynamicBoundsError(i, 1, lambda.size(0), w_emlrtBCI);
      }
      lbDelta = lambda[i - 1];
      if (idx + 1 > cIneq.size(0)) {
        rtDynamicBoundsError(idx + 1, 1, cIneq.size(0), w_emlrtBCI);
      }
      nlpComplError = std::fmax(
          nlpComplError, std::fmin(std::abs(cIneq[idx] * lbDelta),
                                   std::fmin(std::abs(cIneq[idx]), lbDelta)));
    }
    mLinIneq = iL0 + mIneq;
    ubOffset = mLinIneq + mLB;
    if (mLB > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mLB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > finiteLB.size(0))) {
        rtDynamicBoundsError(idx + 1, 1, finiteLB.size(0), w_emlrtBCI);
      }
      if ((finiteLB[idx] < 1) || (finiteLB[idx] > xCurrent.size(0))) {
        rtDynamicBoundsError(finiteLB[idx], 1, xCurrent.size(0), w_emlrtBCI);
      }
      if ((finiteLB[idx] < 1) || (finiteLB[idx] > lb.size(0))) {
        rtDynamicBoundsError(finiteLB[idx], 1, lb.size(0), w_emlrtBCI);
      }
      lbDelta = xCurrent[finiteLB[idx] - 1] - lb[finiteLB[idx] - 1];
      i = mLinIneq + idx;
      if ((i < 1) || (i > lambda.size(0))) {
        rtDynamicBoundsError(i, 1, lambda.size(0), w_emlrtBCI);
      }
      lbLambda = lambda[i - 1];
      nlpComplError = std::fmax(
          nlpComplError, std::fmin(std::abs(lbDelta * lbLambda),
                                   std::fmin(std::abs(lbDelta), lbLambda)));
    }
    if (mUB > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mUB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > finiteUB.size(0))) {
        rtDynamicBoundsError(idx + 1, 1, finiteUB.size(0), w_emlrtBCI);
      }
      if ((finiteUB[idx] < 1) || (finiteUB[idx] > ub.size(0))) {
        rtDynamicBoundsError(finiteUB[idx], 1, ub.size(0), w_emlrtBCI);
      }
      if ((finiteUB[idx] < 1) || (finiteUB[idx] > xCurrent.size(0))) {
        rtDynamicBoundsError(finiteUB[idx], 1, xCurrent.size(0), w_emlrtBCI);
      }
      lbDelta = ub[finiteUB[idx] - 1] - xCurrent[finiteUB[idx] - 1];
      i = ubOffset + idx;
      if ((i < 1) || (i > lambda.size(0))) {
        rtDynamicBoundsError(i, 1, lambda.size(0), w_emlrtBCI);
      }
      lbLambda = lambda[i - 1];
      nlpComplError = std::fmax(
          nlpComplError, std::fmin(std::abs(lbDelta * lbLambda),
                                   std::fmin(std::abs(lbDelta), lbLambda)));
    }
  }
  return nlpComplError;
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeComplError.cpp
//
// [EOF]
//
