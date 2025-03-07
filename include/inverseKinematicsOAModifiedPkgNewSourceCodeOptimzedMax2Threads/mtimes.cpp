//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mtimes.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &A
//                const array<double, 2U> &B
//                array<double, 2U> &C
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void mtimes(const array<double, 1U> &A, const array<double, 2U> &B,
            array<double, 2U> &C)
{
  int boffset;
  int inner;
  int k;
  int nc;
  inner = A.size(0);
  nc = B.size(1);
  C.set_size(1, B.size(1));
  if (static_cast<int>(A.size(0) * B.size(1) < 200)) {
    for (int j{0}; j < nc; j++) {
      boffset = j * B.size(0);
      C[j] = 0.0;
      for (k = 0; k < inner; k++) {
        C[j] = C[j] + A[k] * B[boffset + k];
      }
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(k, boffset)

    for (int j = 0; j < nc; j++) {
      boffset = j * B.size(0);
      C[j] = 0.0;
      for (k = 0; k < inner; k++) {
        C[j] = C[j] + A[k] * B[boffset + k];
      }
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
