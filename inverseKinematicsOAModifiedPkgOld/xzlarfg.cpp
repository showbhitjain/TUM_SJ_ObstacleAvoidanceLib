//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzlarfg.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "xzlarfg.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double b;
  double y;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (std::isnan(b)) {
    y = rtNaN;
  } else {
    y = a * 1.4142135623730951;
  }
  return y;
}

//
// Arguments    : int n
//                double &alpha1
//                array<double, 2U> &x
//                int ix0
// Return Type  : double
//
namespace coder {
namespace internal {
namespace reflapack {
double xzlarfg(int n, double &alpha1, array<double, 2U> &x, int ix0)
{
  double tau;
  tau = 0.0;
  if (n > 0) {
    double xnorm;
    xnorm = blas::xnrm2(n - 1, x, ix0);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(alpha1, xnorm);
      if (alpha1 >= 0.0) {
        xnorm = -xnorm;
      }
      if (std::abs(xnorm) < 1.0020841800044864E-292) {
        int b_tmp;
        int knt;
        boolean_T overflow_tmp;
        knt = 0;
        b_tmp = (ix0 + n) - 2;
        overflow_tmp = ((ix0 <= b_tmp) && (b_tmp > 2147483646));
        do {
          knt++;
          if (overflow_tmp) {
            check_forloop_overflow_error();
          }
          for (int k{ix0}; k <= b_tmp; k++) {
            x[k - 1] = 9.9792015476736E+291 * x[k - 1];
          }
          xnorm *= 9.9792015476736E+291;
          alpha1 *= 9.9792015476736E+291;
        } while ((std::abs(xnorm) < 1.0020841800044864E-292) && (knt < 20));
        xnorm = blas::xnrm2(n - 1, x, ix0);
        xnorm = rt_hypotd_snf(alpha1, xnorm);
        if (alpha1 >= 0.0) {
          xnorm = -xnorm;
        }
        tau = (xnorm - alpha1) / xnorm;
        alpha1 = 1.0 / (alpha1 - xnorm);
        for (int k{ix0}; k <= b_tmp; k++) {
          x[k - 1] = alpha1 * x[k - 1];
        }
        for (int k{0}; k < knt; k++) {
          xnorm *= 1.0020841800044864E-292;
        }
        alpha1 = xnorm;
      } else {
        int knt;
        tau = (xnorm - alpha1) / xnorm;
        alpha1 = 1.0 / (alpha1 - xnorm);
        knt = (ix0 + n) - 2;
        if ((ix0 <= knt) && (knt > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int k{ix0}; k <= knt; k++) {
          x[k - 1] = alpha1 * x[k - 1];
        }
        alpha1 = xnorm;
      }
    }
  }
  return tau;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

//
// File trailer for xzlarfg.cpp
//
// [EOF]
//
