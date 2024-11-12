//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeComplError.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
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
double computeComplError(const array<double, 1U> &xCurrent,
                         const array<int, 1U> &finiteLB, int mLB,
                         const array<double, 1U> &lb,
                         const array<int, 1U> &finiteUB, int mUB,
                         const array<double, 1U> &ub,
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
