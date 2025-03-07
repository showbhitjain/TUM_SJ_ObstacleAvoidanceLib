//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ixfun.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "ixfun.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &a
//                const array<double, 1U> &b
//                array<double, 1U> &c
// Return Type  : void
//
namespace coder {
namespace internal {
void expand_max(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c)
{
  int csz_idx_0;
  int u1;
  csz_idx_0 = a.size(0);
  u1 = b.size(0);
  if (csz_idx_0 <= u1) {
    u1 = csz_idx_0;
  }
  if (b.size(0) == 1) {
    csz_idx_0 = a.size(0);
  } else if (a.size(0) == 1) {
    csz_idx_0 = b.size(0);
  } else {
    csz_idx_0 = u1;
  }
  c.set_size(csz_idx_0);
  if (csz_idx_0 != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a.size(0) != 1);
    b1 = (b.size(0) != 1);
    u1 = csz_idx_0 - 1;
    if (static_cast<int>(csz_idx_0 < 200)) {
      for (int k{0}; k <= u1; k++) {
        c[k] = std::fmax(a[b_b * k], b[b1 * k]);
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= u1; k++) {
        c[k] = std::fmax(a[b_b * k], b[b1 * k]);
      }
    }
  }
}

//
// Arguments    : const array<double, 1U> &a
//                const array<double, 1U> &b
//                array<double, 1U> &c
// Return Type  : void
//
void expand_min(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c)
{
  int csz_idx_0;
  int u1;
  csz_idx_0 = a.size(0);
  u1 = b.size(0);
  if (csz_idx_0 <= u1) {
    u1 = csz_idx_0;
  }
  if (b.size(0) == 1) {
    csz_idx_0 = a.size(0);
  } else if (a.size(0) == 1) {
    csz_idx_0 = b.size(0);
  } else {
    csz_idx_0 = u1;
  }
  c.set_size(csz_idx_0);
  if (csz_idx_0 != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a.size(0) != 1);
    b1 = (b.size(0) != 1);
    u1 = csz_idx_0 - 1;
    if (static_cast<int>(csz_idx_0 < 200)) {
      for (int k{0}; k <= u1; k++) {
        c[k] = std::fmin(a[b_b * k], b[b1 * k]);
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4> omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= u1; k++) {
        c[k] = std::fmin(a[b_b * k], b[b1 * k]);
      }
    }
  }
}

} // namespace internal
} // namespace coder

//
// File trailer for ixfun.cpp
//
// [EOF]
//
