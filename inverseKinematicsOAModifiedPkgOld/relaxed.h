//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: relaxed.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef RELAXED_H
#define RELAXED_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct j_struct_T;

struct struct_T;

struct h_struct_T;

struct d_struct_T;

struct e_struct_T;

struct f_struct_T;

struct g_struct_T;

struct k_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void b_relaxed(const array<double, 2U> &Hessian, const array<double, 1U> &grad,
               j_struct_T &b_TrialState, struct_T &b_MeritFunction,
               h_struct_T &memspace, d_struct_T &WorkingSet,
               e_struct_T &b_QRManager, f_struct_T &b_CholManager,
               g_struct_T &QPObjective, k_struct_T &qpoptions);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for relaxed.h
//
// [EOF]
//
