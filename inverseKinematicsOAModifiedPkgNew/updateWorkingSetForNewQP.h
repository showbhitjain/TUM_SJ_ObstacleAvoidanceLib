//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: updateWorkingSetForNewQP.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef UPDATEWORKINGSETFORNEWQP_H
#define UPDATEWORKINGSETFORNEWQP_H

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
namespace internal {
void updateWorkingSetForNewQP(const array<double, 1U> &xk,
                              d_struct_T &WorkingSet, int mIneq,
                              const array<double, 1U> &cIneq, int mEq,
                              const double cEq_data[], int cEq_size, int mLB,
                              const array<double, 2U> &lb, int mUB,
                              const array<double, 2U> &ub, int mFixed);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for updateWorkingSetForNewQP.h
//
// [EOF]
//
