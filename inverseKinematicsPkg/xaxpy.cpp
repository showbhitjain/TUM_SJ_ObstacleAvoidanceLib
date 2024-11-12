//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xaxpy.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "xaxpy.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int n
//                double a
//                int ix0
//                array<double, 2U> &y
//                int iy0
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void xaxpy(int n, double a, int ix0, array<double, 2U> &y, int iy0)
{
  if ((n >= 1) && (!(a == 0.0))) {
    int i;
    i = n - 1;
    for (int k{0}; k <= i; k++) {
      int i1;
      i1 = (iy0 + k) - 1;
      y[i1] = y[i1] + a * y[(ix0 + k) - 1];
    }
  }
}

//
// Arguments    : int n
//                double a
//                const array<double, 2U> &x
//                int ix0
//                double y_data[]
//                int iy0
// Return Type  : void
//
void xaxpy(int n, double a, const array<double, 2U> &x, int ix0,
           double y_data[], int iy0)
{
  if ((n >= 1) && (!(a == 0.0))) {
    int i;
    i = n - 1;
    for (int k{0}; k <= i; k++) {
      int i1;
      i1 = (iy0 + k) - 1;
      y_data[i1] += a * x[(ix0 + k) - 1];
    }
  }
}

//
// Arguments    : int n
//                double a
//                const double x_data[]
//                int ix0
//                array<double, 2U> &y
//                int iy0
// Return Type  : void
//
void xaxpy(int n, double a, const double x_data[], int ix0,
           array<double, 2U> &y, int iy0)
{
  if ((n >= 1) && (!(a == 0.0))) {
    int i;
    i = n - 1;
    for (int k{0}; k <= i; k++) {
      int i1;
      i1 = (iy0 + k) - 1;
      y[i1] = y[i1] + a * x_data[(ix0 + k) - 1];
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xaxpy.cpp
//
// [EOF]
//
