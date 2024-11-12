//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_exit.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef TEST_EXIT_H
#define TEST_EXIT_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct struct_T;

struct i_struct_T;

struct h_struct_T;

struct b_struct_T;

struct g_struct_T;

struct d_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void b_test_exit(b_struct_T &Flags, g_struct_T &memspace,
                 struct_T &MeritFunction, const i_struct_T &WorkingSet,
                 h_struct_T &b_TrialState, d_struct_T &b_QRManager,
                 const array<double, 1U> &lb, const array<double, 1U> &ub,
                 int runTimeOptions_MaxFunctionEvaluations);

boolean_T test_exit(struct_T &MeritFunction, const i_struct_T &WorkingSet,
                    h_struct_T &b_TrialState, const array<double, 1U> &lb,
                    const array<double, 1U> &ub,
                    int runTimeOptions_MaxFunctionEvaluations,
                    boolean_T &Flags_fevalOK, boolean_T &Flags_done,
                    boolean_T &Flags_stepAccepted,
                    boolean_T &Flags_failedLineSearch, int &Flags_stepType);

} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for test_exit.h
//
// [EOF]
//
