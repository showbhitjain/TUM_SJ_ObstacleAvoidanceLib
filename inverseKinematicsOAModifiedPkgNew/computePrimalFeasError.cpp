//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computePrimalFeasError.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "computePrimalFeasError.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
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
//                const array<double, 2U> &lb
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 2U> &ub
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
                              const array<double, 2U> &lb,
                              const array<int, 1U> &finiteUB, int mUB,
                              const array<double, 2U> &ub)
{
  double feasError;
  feasError = 0.0;
  for (int idx{0}; idx < mLinEq; idx++) {
    feasError = std::fmax(feasError, std::abs(cEq_data[idx]));
  }
  for (int idx{0}; idx < mLinIneq; idx++) {
    feasError = std::fmax(feasError, cIneq[idx]);
  }
  for (int idx{0}; idx < mLB; idx++) {
    feasError =
        std::fmax(feasError, lb[finiteLB[idx] - 1] - x[finiteLB[idx] - 1]);
  }
  for (int idx{0}; idx < mUB; idx++) {
    feasError =
        std::fmax(feasError, x[finiteUB[idx] - 1] - ub[finiteUB[idx] - 1]);
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
