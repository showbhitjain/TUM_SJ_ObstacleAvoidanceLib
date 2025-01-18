//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mtimes.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &B
//                array<double, 2U> &C
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void mtimes(const array<double, 2U> &B, array<double, 2U> &C)
{
  static const signed char iv[36]{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                                  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
  int n;
  n = B.size(1);
  C.set_size(6, B.size(1));
  for (int j{0}; j < n; j++) {
    int coffset_tmp;
    coffset_tmp = j * 6;
    for (int i{0}; i < 6; i++) {
      double s;
      s = 0.0;
      for (int k{0}; k < 6; k++) {
        s += static_cast<double>(iv[k * 6 + i]) * B[coffset_tmp + k];
      }
      C[coffset_tmp + i] = s;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for mtimes.cpp
//
// [EOF]
//
