//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computePrimalFeasError.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

#ifndef COMPUTEPRIMALFEASERROR_H
#define COMPUTEPRIMALFEASERROR_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
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
                              const array<double, 1U> &ub);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computePrimalFeasError.h
//
// [EOF]
//
