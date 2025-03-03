//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factor.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef FACTOR_H
#define FACTOR_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct f_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace CholManager {
void factor(f_struct_T &obj, const array<double, 2U> &A, int ndims, int ldA);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for factor.h
//
// [EOF]
//
