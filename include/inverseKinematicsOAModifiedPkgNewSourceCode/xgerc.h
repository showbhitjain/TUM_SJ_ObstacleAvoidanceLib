//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgerc.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

#ifndef XGERC_H
#define XGERC_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace blas {
void xgerc(int m, int n, double alpha1, int ix0, const array<double, 1U> &y,
           array<double, 2U> &A, int ia0, int lda);

}
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xgerc.h
//
// [EOF]
//
