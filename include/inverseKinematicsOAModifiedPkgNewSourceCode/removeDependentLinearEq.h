//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeDependentLinearEq.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

#ifndef REMOVEDEPENDENTLINEAREQ_H
#define REMOVEDEPENDENTLINEAREQ_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct h_struct_T;

struct j_struct_T;

struct d_struct_T;

struct e_struct_T;

struct g_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace internal {
int removeDependentLinearEq(const array<double, 2U> &Aeq, double beq_data[],
                            const int &beq_size, const array<double, 1U> &bnd,
                            array<int, 1U> &idxArray, h_struct_T &memspace,
                            j_struct_T &b_TrialState, d_struct_T &WorkingSet,
                            e_struct_T &b_QRManager, g_struct_T &QPObjective);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for removeDependentLinearEq.h
//
// [EOF]
//
