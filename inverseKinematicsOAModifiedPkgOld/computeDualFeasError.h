//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeDualFeasError.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef COMPUTEDUALFEASERROR_H
#define COMPUTEDUALFEASERROR_H

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
boolean_T computeDualFeasError(int nVar, const array<double, 1U> &gradLag,
                               double &val);

boolean_T computeDualFeasError(int nVar, const array<double, 2U> &gradLag,
                               double &val);

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeDualFeasError.h
//
// [EOF]
//
