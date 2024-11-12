//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factoryConstruct.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef FACTORYCONSTRUCT_H
#define FACTORYCONSTRUCT_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct h_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace TrialState {
void factoryConstruct(int nVarMax, int mConstrMax, int mEq,
                      const array<double, 1U> &x0, h_struct_T &obj);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for factoryConstruct.h
//
// [EOF]
//
