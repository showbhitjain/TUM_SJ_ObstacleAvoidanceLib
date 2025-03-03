//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: makeBoundFeasible.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef MAKEBOUNDFEASIBLE_H
#define MAKEBOUNDFEASIBLE_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct d_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void makeBoundFeasible(array<double, 1U> &xk, const d_struct_T &WorkingSet,
                       const array<double, 2U> &lb,
                       const array<double, 2U> &ub);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for makeBoundFeasible.h
//
// [EOF]
//
