//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGradLag.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef COMPUTEGRADLAG_H
#define COMPUTEGRADLAG_H

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
void computeGradLag(array<double, 1U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda);

void computeGradLag(array<double, 2U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda);

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeGradLag.h
//
// [EOF]
//
