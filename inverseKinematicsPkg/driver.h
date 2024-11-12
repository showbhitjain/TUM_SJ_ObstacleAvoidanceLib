//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef DRIVER_H
#define DRIVER_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct h_struct_T;

struct struct_T;

namespace coder {
namespace internal {
class i_stickyStruct;

}
} // namespace coder
struct k_struct_T;

struct g_struct_T;

struct i_struct_T;

struct d_struct_T;

struct e_struct_T;

struct f_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void driver(array<double, 2U> &Hessian, const double beq_data[],
            const array<double, 1U> &lb, const array<double, 1U> &ub,
            h_struct_T &b_TrialState, struct_T &MeritFunction,
            const ::coder::internal::i_stickyStruct &FcnEvaluator,
            k_struct_T &FiniteDifferences, g_struct_T &memspace,
            i_struct_T &WorkingSet, d_struct_T &b_QRManager,
            e_struct_T &b_CholManager, f_struct_T &QPObjective,
            int runTimeOptions_MaxFunctionEvaluations);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for driver.h
//
// [EOF]
//
