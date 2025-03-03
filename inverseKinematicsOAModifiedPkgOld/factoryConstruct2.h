//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factoryConstruct2.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef FACTORYCONSTRUCT2_H
#define FACTORYCONSTRUCT2_H

// Include Files
#include "rtwtypes.h"
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
void factoryConstruct(int mIneqMax, int mEqMax, int nVar, int nVarMax,
                      int mConstrMax, d_struct_T &obj);

}
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for factoryConstruct2.h
//
// [EOF]
//
