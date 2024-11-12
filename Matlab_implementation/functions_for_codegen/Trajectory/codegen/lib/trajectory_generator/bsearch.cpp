//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: bsearch.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "bsearch.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const ::coder::array<double, 2U> &x
//                double xi
// Return Type  : int
//
namespace TrajectoryGeneration {
namespace coder {
namespace internal {
int b_bsearch(const ::coder::array<double, 2U> &x, double xi)
{
  int high_i;
  int low_ip1;
  int n;
  high_i = x.size(1);
  n = 1;
  low_ip1 = 2;
  while (high_i > low_ip1) {
    int mid_i;
    mid_i = (n >> 1) + (high_i >> 1);
    if (((n & 1) == 1) && ((high_i & 1) == 1)) {
      mid_i++;
    }
    if (xi >= x[mid_i - 1]) {
      n = mid_i;
      low_ip1 = mid_i + 1;
    } else {
      high_i = mid_i;
    }
  }
  return n;
}

} // namespace internal
} // namespace coder
} // namespace TrajectoryGeneration

//
// File trailer for bsearch.cpp
//
// [EOF]
//
