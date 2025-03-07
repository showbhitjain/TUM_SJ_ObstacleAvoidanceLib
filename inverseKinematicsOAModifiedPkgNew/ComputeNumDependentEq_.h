//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ComputeNumDependentEq_.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

#ifndef COMPUTENUMDEPENDENTEQ__H
#define COMPUTENUMDEPENDENTEQ__H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct e_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
int ComputeNumDependentEq_(e_struct_T &qrmanager, const array<double, 1U> &beqf,
                           int mConstr, int nVar);

}
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for ComputeNumDependentEq_.h
//
// [EOF]
//
