//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: IndexOfDependentEq_.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef INDEXOFDEPENDENTEQ__H
#define INDEXOFDEPENDENTEQ__H

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
void IndexOfDependentEq_(array<int, 1U> &depIdx, int mFixed, int nDep,
                         e_struct_T &qrmanager, int mRows, int nCols);

}
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for IndexOfDependentEq_.h
//
// [EOF]
//
