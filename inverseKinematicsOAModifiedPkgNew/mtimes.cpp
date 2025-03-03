//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mtimes.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "mtimes.h"
#include "eml_int_forloop_overflow_check.h"
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
  if (B.size(1) > 2147483646) {
    check_forloop_overflow_error();
  }
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

//
// Arguments    : const array<double, 1U> &A
//                const array<double, 2U> &B
//                array<double, 2U> &C
// Return Type  : void
//
void mtimes(const array<double, 1U> &A, const array<double, 2U> &B,
            array<double, 2U> &C)
{
  int inner;
  int nc;
  inner = A.size(0);
  nc = B.size(1);
  C.set_size(1, B.size(1));
  if (B.size(1) > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int j{0}; j < nc; j++) {
    int boffset;
    boffset = j * B.size(0);
    C[j] = 0.0;
    if (inner > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int k{0}; k < inner; k++) {
      C[j] = C[j] + A[k] * B[boffset + k];
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
