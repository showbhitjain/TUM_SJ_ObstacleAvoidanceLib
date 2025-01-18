//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeComplError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "computeComplError.h"
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
  double nlpComplError;
  int mLinIneq;
  nlpComplError = 0.0;
  mLinIneq = fscales_lineq_constraint.size(0);
  if ((mIneq + mLB) + mUB > 0) {
    double lbDelta;
    double lbLambda;
    int ubOffset;
    for (int idx{0}; idx < mLinIneq; idx++) {
      lbDelta = lambda[(iL0 + idx) - 1];
      nlpComplError = std::fmax(
          nlpComplError, std::fmin(std::abs(cIneq[idx] * lbDelta),
                                   std::fmin(std::abs(cIneq[idx]), lbDelta)));
    }
    mLinIneq = (iL0 + mIneq) - 1;
    ubOffset = mLinIneq + mLB;
    for (int idx{0}; idx < mLB; idx++) {
      lbDelta = xCurrent[finiteLB[idx] - 1] - lb[finiteLB[idx] - 1];
      lbLambda = lambda[mLinIneq + idx];
      nlpComplError = std::fmax(
          nlpComplError, std::fmin(std::abs(lbDelta * lbLambda),
                                   std::fmin(std::abs(lbDelta), lbLambda)));
    }
    for (int idx{0}; idx < mUB; idx++) {
      lbDelta = ub[finiteUB[idx] - 1] - xCurrent[finiteUB[idx] - 1];
      lbLambda = lambda[ubOffset + idx];
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
