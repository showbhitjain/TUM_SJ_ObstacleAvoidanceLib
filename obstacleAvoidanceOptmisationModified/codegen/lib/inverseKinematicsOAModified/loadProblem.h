//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: loadProblem.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef LOADPROBLEM_H
#define LOADPROBLEM_H

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
namespace qpactiveset {
namespace WorkingSet {
void loadProblem(d_struct_T &obj, int mIneq, int mLinIneq,
                 const array<double, 2U> &Aineq, int mEq, int mLinEq,
                 const array<double, 2U> &Aeq, int mLB, int mUB, int mFixed,
                 int mConstrMax);

}
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for loadProblem.h
//
// [EOF]
//
