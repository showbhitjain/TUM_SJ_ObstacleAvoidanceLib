//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFiniteDifferences.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef COMPUTEFINITEDIFFERENCES_H
#define COMPUTEFINITEDIFFERENCES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct k_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
boolean_T computeFiniteDifferences(k_struct_T &obj, double fCurrent,
                                   array<double, 1U> &xk,
                                   array<double, 1U> &gradf,
                                   const array<double, 1U> &lb,
                                   const array<double, 1U> &ub);

}
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeFiniteDifferences.h
//
// [EOF]
//
