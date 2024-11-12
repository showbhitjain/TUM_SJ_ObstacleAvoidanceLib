//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: step.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef STEP_H
#define STEP_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct h_struct_T;

struct struct_T;

struct g_struct_T;

struct i_struct_T;

struct d_struct_T;

struct e_struct_T;

struct f_struct_T;

struct j_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
boolean_T b_step(int &STEP_TYPE, array<double, 2U> &Hessian,
                 const array<double, 1U> &lb, const array<double, 1U> &ub,
                 h_struct_T &b_TrialState, struct_T &MeritFunction,
                 g_struct_T &memspace, i_struct_T &WorkingSet,
                 d_struct_T &b_QRManager, e_struct_T &b_CholManager,
                 f_struct_T &QPObjective, j_struct_T &qpoptions);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for step.h
//
// [EOF]
//
