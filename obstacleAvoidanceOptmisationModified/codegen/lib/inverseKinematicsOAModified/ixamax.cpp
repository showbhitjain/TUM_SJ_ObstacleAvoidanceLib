//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ixamax.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "ixamax.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int n
//                const array<double, 1U> &x
// Return Type  : int
//
namespace coder {
namespace internal {
namespace blas {
int ixamax(int n, const array<double, 1U> &x)
{
  int idxmax;
  if (n < 1) {
    idxmax = 0;
  } else {
    idxmax = 1;
    if (n > 1) {
      double smax;
      smax = std::abs(x[0]);
      if (n > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{2}; k <= n; k++) {
        double s;
        s = std::abs(x[k - 1]);
        if (s > smax) {
          idxmax = k;
          smax = s;
        }
      }
    }
  }
  return idxmax;
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for ixamax.cpp
//
// [EOF]
//
