//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzlarf.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef XZLARF_H
#define XZLARF_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace reflapack {
void xzlarf(int m, int n, int iv0, double tau, array<double, 2U> &C, int ic0,
            int ldc, array<double, 1U> &work);

}
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xzlarf.h
//
// [EOF]
//
