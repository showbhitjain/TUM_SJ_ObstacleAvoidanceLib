//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computePrimalFeasError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "computePrimalFeasError.h"
#include "inverseKinematicsOAModified_data.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &x
//                int mLinIneq
//                const array<double, 1U> &cIneq
//                int mLinEq
//                const double cEq_data[]
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<double, 1U> &lb
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &ub
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
double computePrimalFeasError(const array<double, 1U> &x, int mLinIneq,
                              const array<double, 1U> &cIneq, int mLinEq,
                              const double cEq_data[],
                              const array<int, 1U> &finiteLB, int mLB,
                              const array<double, 1U> &lb,
                              const array<int, 1U> &finiteUB, int mUB,
                              const array<double, 1U> &ub)
{
  double b_feasErrorPrime;
  double c_feasErrorPrime;
  double d_feasErrorPrime;
  double feasError;
  double feasErrorPrime;
  int idx;
  feasError = 0.0;
  if (static_cast<int>(mLinEq < 200)) {
    for (idx = 0; idx < mLinEq; idx++) {
      feasError = std::fmax(feasError, std::abs(cEq_data[idx]));
    }
  } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(feasErrorPrime)
    {
      feasErrorPrime = rtMinusInf;
#pragma omp for nowait
      for (idx = 0; idx < mLinEq; idx++) {
        feasErrorPrime = std::fmax(feasErrorPrime, std::abs(cEq_data[idx]));
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        feasError = std::fmax(feasError, feasErrorPrime);
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  if (static_cast<int>(mLinIneq < 200)) {
    for (idx = 0; idx < mLinIneq; idx++) {
      feasError = std::fmax(feasError, cIneq[idx]);
    }
  } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(b_feasErrorPrime)
    {
      b_feasErrorPrime = rtMinusInf;
#pragma omp for nowait
      for (idx = 0; idx < mLinIneq; idx++) {
        b_feasErrorPrime = std::fmax(b_feasErrorPrime, cIneq[idx]);
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        feasError = std::fmax(feasError, b_feasErrorPrime);
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  if (static_cast<int>(mLB < 200)) {
    for (idx = 0; idx < mLB; idx++) {
      feasError =
          std::fmax(feasError, lb[finiteLB[idx] - 1] - x[finiteLB[idx] - 1]);
    }
  } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(c_feasErrorPrime)
    {
      c_feasErrorPrime = rtMinusInf;
#pragma omp for nowait
      for (idx = 0; idx < mLB; idx++) {
        c_feasErrorPrime = std::fmax(
            c_feasErrorPrime, lb[finiteLB[idx] - 1] - x[finiteLB[idx] - 1]);
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        feasError = std::fmax(feasError, c_feasErrorPrime);
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  if (static_cast<int>(mUB < 200)) {
    for (idx = 0; idx < mUB; idx++) {
      feasError =
          std::fmax(feasError, x[finiteUB[idx] - 1] - ub[finiteUB[idx] - 1]);
    }
  } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(d_feasErrorPrime)
    {
      d_feasErrorPrime = rtMinusInf;
#pragma omp for nowait
      for (idx = 0; idx < mUB; idx++) {
        d_feasErrorPrime = std::fmax(
            d_feasErrorPrime, x[finiteUB[idx] - 1] - ub[finiteUB[idx] - 1]);
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        feasError = std::fmax(feasError, d_feasErrorPrime);
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  return feasError;
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computePrimalFeasError.cpp
//
// [EOF]
//
