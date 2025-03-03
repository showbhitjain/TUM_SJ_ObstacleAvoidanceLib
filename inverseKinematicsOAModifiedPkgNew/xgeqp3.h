//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgeqp3.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef XGEQP3_H
#define XGEQP3_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace lapack {
void xgeqp3(array<double, 2U> &A, int m, int n, array<int, 1U> &jpvt,
            array<double, 1U> &tau);

}
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xgeqp3.h
//
// [EOF]
//
