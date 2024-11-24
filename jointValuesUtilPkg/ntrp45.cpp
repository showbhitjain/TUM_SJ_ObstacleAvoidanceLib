//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ntrp45.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

// Include Files
#include "ntrp45.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const double t[3]
//                double t0
//                const array<double, 1U> &b_y0
//                double h
//                const array<double, 2U> &f
//                array<double, 2U> &y
// Return Type  : void
//
namespace coder {
void ntrp45(const double t[3], double t0, const array<double, 1U> &b_y0,
            double h, const array<double, 2U> &f, array<double, 2U> &y)
{
  static const double b[7]{-2.859375,
                           0.0,
                           4.0431266846361185,
                           -3.90625,
                           2.7939268867924527,
                           -1.5714285714285714,
                           1.5};
  static const double b_b[7]{3.0833333333333335,
                             0.0,
                             -6.2893081761006293,
                             10.416666666666666,
                             -6.8773584905660377,
                             3.6666666666666665,
                             -4.0};
  static const double c_b[7]{-1.1328125,
                             0.0,
                             2.6954177897574123,
                             -5.859375,
                             3.7610554245283021,
                             -1.9642857142857142,
                             2.5};
  array<double, 1U> fhBI1;
  array<double, 1U> fhBI2;
  array<double, 1U> fhBI3;
  array<double, 1U> fhBI4;
  double b_y[7];
  double s;
  int loop_ub;
  int neq;
  neq = b_y0.size(0);
  fhBI1.set_size(f.size(0));
  loop_ub = f.size(0);
  for (int i{0}; i < loop_ub; i++) {
    fhBI1[i] = f[i] * h;
  }
  for (int i{0}; i < 7; i++) {
    b_y[i] = h * b[i];
  }
  loop_ub = f.size(0);
  fhBI2.set_size(f.size(0));
  for (int i{0}; i < loop_ub; i++) {
    s = 0.0;
    for (int k{0}; k < 7; k++) {
      s += f[k * f.size(0) + i] * b_y[k];
    }
    fhBI2[i] = s;
  }
  for (int i{0}; i < 7; i++) {
    b_y[i] = h * b_b[i];
  }
  loop_ub = f.size(0);
  fhBI3.set_size(f.size(0));
  for (int i{0}; i < loop_ub; i++) {
    s = 0.0;
    for (int k{0}; k < 7; k++) {
      s += f[k * f.size(0) + i] * b_y[k];
    }
    fhBI3[i] = s;
  }
  for (int i{0}; i < 7; i++) {
    b_y[i] = h * c_b[i];
  }
  loop_ub = f.size(0);
  fhBI4.set_size(f.size(0));
  for (int i{0}; i < loop_ub; i++) {
    s = 0.0;
    for (int k{0}; k < 7; k++) {
      s += f[k * f.size(0) + i] * b_y[k];
    }
    fhBI4[i] = s;
  }
  y.set_size(b_y0.size(0), 3);
  for (loop_ub = 0; loop_ub < 3; loop_ub++) {
    s = (t[loop_ub] - t0) / h;
    for (int k{0}; k < neq; k++) {
      y[k + y.size(0) * loop_ub] =
          (((fhBI4[k] * s + fhBI3[k]) * s + fhBI2[k]) * s + fhBI1[k]) * s +
          b_y0[k];
    }
  }
}

} // namespace coder

//
// File trailer for ntrp45.cpp
//
// [EOF]
//
