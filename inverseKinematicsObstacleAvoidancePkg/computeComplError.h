//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeComplError.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef COMPUTECOMPLERROR_H
#define COMPUTECOMPLERROR_H

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
double computeComplError(const array<double, 1U> &fscales_lineq_constraint,
                         const array<double, 1U> &xCurrent, int mIneq,
                         const array<double, 1U> &cIneq,
                         const array<int, 1U> &finiteLB, int mLB,
                         const array<double, 2U> &lb,
                         const array<int, 1U> &finiteUB, int mUB,
                         const array<double, 2U> &ub,
                         const array<double, 1U> &lambda, int iL0);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeComplError.h
//
// [EOF]
//
