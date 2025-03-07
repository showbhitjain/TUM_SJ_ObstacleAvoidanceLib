//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeComplError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "computeComplError.h"
#include "inverseKinematicsOAModified_data.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
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
//                const array<double, 1U> &lb
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &ub
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
                         const array<double, 1U> &lb,
                         const array<int, 1U> &finiteUB, int mUB,
                         const array<double, 1U> &ub,
                         const array<double, 1U> &lambda, int iL0)
{
  double b_nlpComplErrorPrime;
  double lbDelta;
  double lbLambda;
  double nlpComplError;
  double nlpComplErrorPrime;
  double x;
  int idx;
  int mLinIneq;
  int ubOffset;
  nlpComplError = 0.0;
  mLinIneq = fscales_lineq_constraint.size(0);
  if ((mIneq + mLB) + mUB > 0) {
    if (static_cast<int>(fscales_lineq_constraint.size(0) < 200)) {
      for (idx = 0; idx < mLinIneq; idx++) {
        lbLambda = lambda[(iL0 + idx) - 1];
        nlpComplError =
            std::fmax(nlpComplError,
                      std::fmin(std::abs(cIneq[idx] * lbLambda),
                                std::fmin(std::abs(cIneq[idx]), lbLambda)));
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(lbDelta, lbLambda, x)
      {
        lbDelta = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mLinIneq; idx++) {
          lbLambda = lambda[(iL0 + idx) - 1];
          x = cIneq[idx] * lbLambda;
          lbDelta = std::fmax(
              lbDelta, std::fmin(std::abs(x),
                                 std::fmin(std::abs(cIneq[idx]), lbLambda)));
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          nlpComplError = std::fmax(nlpComplError, lbDelta);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
    mLinIneq = (iL0 + mIneq) - 1;
    ubOffset = mLinIneq + mLB;
    if (static_cast<int>(mLB < 200)) {
      for (idx = 0; idx < mLB; idx++) {
        lbDelta = xCurrent[finiteLB[idx] - 1] - lb[finiteLB[idx] - 1];
        lbLambda = lambda[mLinIneq + idx];
        nlpComplError = std::fmax(
            nlpComplError, std::fmin(std::abs(lbDelta * lbLambda),
                                     std::fmin(std::abs(lbDelta), lbLambda)));
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(nlpComplErrorPrime, lbDelta, \
                                                  lbLambda)
      {
        nlpComplErrorPrime = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mLB; idx++) {
          lbDelta = xCurrent[finiteLB[idx] - 1] - lb[finiteLB[idx] - 1];
          lbLambda = lambda[mLinIneq + idx];
          nlpComplErrorPrime =
              std::fmax(nlpComplErrorPrime,
                        std::fmin(std::abs(lbDelta * lbLambda),
                                  std::fmin(std::abs(lbDelta), lbLambda)));
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          nlpComplError = std::fmax(nlpComplError, nlpComplErrorPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
    if (static_cast<int>(mUB < 200)) {
      for (idx = 0; idx < mUB; idx++) {
        lbDelta = ub[finiteUB[idx] - 1] - xCurrent[finiteUB[idx] - 1];
        lbLambda = lambda[ubOffset + idx];
        nlpComplError = std::fmax(
            nlpComplError, std::fmin(std::abs(lbDelta * lbLambda),
                                     std::fmin(std::abs(lbDelta), lbLambda)));
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(b_nlpComplErrorPrime,        \
                                                  lbDelta, lbLambda)
      {
        b_nlpComplErrorPrime = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mUB; idx++) {
          lbDelta = ub[finiteUB[idx] - 1] - xCurrent[finiteUB[idx] - 1];
          lbLambda = lambda[ubOffset + idx];
          b_nlpComplErrorPrime =
              std::fmax(b_nlpComplErrorPrime,
                        std::fmin(std::abs(lbDelta * lbLambda),
                                  std::fmin(std::abs(lbDelta), lbLambda)));
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          nlpComplError = std::fmax(nlpComplError, b_nlpComplErrorPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
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
