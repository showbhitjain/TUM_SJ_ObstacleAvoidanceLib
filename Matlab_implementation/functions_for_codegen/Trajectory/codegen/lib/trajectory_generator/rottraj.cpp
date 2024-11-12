//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rottraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "rottraj.h"
#include "quaternion.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const quaternion R0
//                const quaternion RF
//                const ::coder::array<double, 2U> &t
//                const ::coder::array<double, 2U> &varargin_2
//                b_quaternion &R
//                ::coder::array<double, 2U> &omega
//                ::coder::array<double, 2U> &alpha
// Return Type  : void
//
namespace TrajectoryGeneration {
namespace coder {
void rottraj(const quaternion R0, const quaternion RF,
             const ::coder::array<double, 2U> &t,
             const ::coder::array<double, 2U> &varargin_2, b_quaternion &R,
             ::coder::array<double, 2U> &omega,
             ::coder::array<double, 2U> &alpha)
{
  quaternion W_tmp;
  quaternion b_o;
  quaternion b_q;
  quaternion b_y;
  quaternion pn;
  quaternion pnCorrected;
  quaternion q;
  quaternion q1n;
  quaternion q2n;
  quaternion qdCalc;
  quaternion qn;
  quaternion qnCorrected;
  quaternion rhs;
  ::coder::array<double, 2U> b_varargin_2;
  ::coder::array<double, 2U> s;
  ::coder::array<double, 2U> timeScaling;
  ::coder::array<boolean_T, 2U> r;
  double d;
  double n;
  int end_tmp;
  int i;
  int input_sizes_idx_0_tmp;
  int loop_ub;
  int sizes_idx_0;
  signed char input_sizes_idx_0;
  boolean_T empty_non_axis_sizes;
  s.set_size(1, varargin_2.size(1));
  loop_ub = varargin_2.size(1);
  r.set_size(1, varargin_2.size(1));
  for (i = 0; i < loop_ub; i++) {
    d = varargin_2[3 * i];
    s[i] = d;
    r[i] = ((d > 1.0) && (d < 1.0000000149011612));
  }
  end_tmp = r.size(1) - 1;
  for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
    if (r[loop_ub]) {
      s[loop_ub] = 1.0;
    }
  }
  r.set_size(1, varargin_2.size(1));
  loop_ub = varargin_2.size(1);
  for (i = 0; i < loop_ub; i++) {
    d = varargin_2[3 * i];
    r[i] = ((d < 0.0) && (d > -1.4901161193847656E-8));
  }
  for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
    if (r[loop_ub]) {
      s[loop_ub] = 0.0;
    }
  }
  if (s.size(1) != 0) {
    end_tmp = s.size(1);
  } else if (varargin_2.size(1) != 0) {
    end_tmp = varargin_2.size(1);
  } else {
    end_tmp = 0;
  }
  empty_non_axis_sizes = (end_tmp == 0);
  if (empty_non_axis_sizes || (s.size(1) != 0)) {
    input_sizes_idx_0 = 1;
  } else {
    input_sizes_idx_0 = 0;
  }
  if (empty_non_axis_sizes || (varargin_2.size(1) != 0)) {
    sizes_idx_0 = 2;
  } else {
    sizes_idx_0 = 0;
  }
  input_sizes_idx_0_tmp = input_sizes_idx_0;
  b_varargin_2.set_size(2, varargin_2.size(1));
  loop_ub = varargin_2.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_varargin_2[2 * i] = varargin_2[3 * i + 1];
    b_varargin_2[2 * i + 1] = varargin_2[3 * i + 2];
  }
  timeScaling.set_size(input_sizes_idx_0 + sizes_idx_0, end_tmp);
  for (i = 0; i < end_tmp; i++) {
    for (loop_ub = 0; loop_ub < input_sizes_idx_0_tmp; loop_ub++) {
      timeScaling[timeScaling.size(0) * i] = s[input_sizes_idx_0 * i];
    }
    for (loop_ub = 0; loop_ub < sizes_idx_0; loop_ub++) {
      timeScaling[(loop_ub + input_sizes_idx_0) + timeScaling.size(0) * i] =
          b_varargin_2[loop_ub + sizes_idx_0 * i];
    }
  }
  omega.set_size(3, t.size(1));
  alpha.set_size(3, t.size(1));
  R.a.set_size(t.size(1));
  loop_ub = t.size(1);
  R.b.set_size(t.size(1));
  R.c.set_size(t.size(1));
  R.d.set_size(t.size(1));
  for (i = 0; i < loop_ub; i++) {
    R.a[i] = 1.0;
    R.b[i] = 0.0;
    R.c[i] = 0.0;
    R.d[i] = 0.0;
  }
  pn = R0;
  n = std::sqrt(((pn.a * pn.a + pn.b * pn.b) + pn.c * pn.c) + pn.d * pn.d);
  pn.a /= n;
  pn.b /= n;
  pn.c /= n;
  pn.d /= n;
  qn = RF;
  n = std::sqrt(((qn.a * qn.a + qn.b * qn.b) + qn.c * qn.c) + qn.d * qn.d);
  qn.a /= n;
  qn.b /= n;
  qn.c /= n;
  qn.d /= n;
  pnCorrected = pn.slerp(qn);
  qnCorrected = pn.quaternionBase_slerp(qn);
  i = t.size(1);
  if (t.size(1) - 1 >= 0) {
    n = std::sqrt(((pn.a * pn.a + pn.b * pn.b) + pn.c * pn.c) + pn.d * pn.d);
    q.a = pn.a / n;
    q.b = pn.b / n;
    q.c = pn.c / n;
    q.d = pn.d / n;
    n = std::sqrt(((qn.a * qn.a + qn.b * qn.b) + qn.c * qn.c) + qn.d * qn.d);
    b_q.a = qn.a / n;
    b_q.b = qn.b / n;
    b_q.c = qn.c / n;
    b_q.d = qn.d / n;
  }
  for (loop_ub = 0; loop_ub < i; loop_ub++) {
    double sinv;
    double xa;
    double xc;
    double y;
    xa = timeScaling[timeScaling.size(0) * loop_ub];
    qn.init();
    q1n.a = ((q.a * qn.a - q.b * qn.b) - q.c * qn.c) - q.d * qn.d;
    q1n.b = ((q.a * qn.b + q.b * qn.a) + q.c * qn.d) - q.d * qn.c;
    q1n.c = ((q.a * qn.c - q.b * qn.d) + q.c * qn.a) + q.d * qn.b;
    q1n.d = ((q.a * qn.d + q.b * qn.c) - q.c * qn.b) + q.d * qn.a;
    q2n.a = ((b_q.a * qn.a - b_q.b * qn.b) - b_q.c * qn.c) - b_q.d * qn.d;
    q2n.b = ((b_q.a * qn.b + b_q.b * qn.a) + b_q.c * qn.d) - b_q.d * qn.c;
    q2n.c = ((b_q.a * qn.c - b_q.b * qn.d) + b_q.c * qn.a) + b_q.d * qn.b;
    q2n.d = ((b_q.a * qn.d + b_q.b * qn.c) - b_q.c * qn.b) + b_q.d * qn.a;
    n = ((q1n.a * q2n.a + q1n.b * q2n.b) + q1n.c * q2n.c) + q1n.d * q2n.d;
    if (n < 0.0) {
      q2n.a = -q2n.a;
      q2n.b = -q2n.b;
      q2n.c = -q2n.c;
      q2n.d = -q2n.d;
      n = -n;
    }
    if (n > 1.0) {
      n = 1.0;
    }
    n = std::acos(n);
    sinv = 1.0 / std::sin(n);
    y = std::sin((1.0 - xa) * n);
    n = std::sin(xa * n);
    rhs.a = sinv * (y * q1n.a + n * q2n.a);
    rhs.b = sinv * (y * q1n.b + n * q2n.b);
    rhs.c = sinv * (y * q1n.c + n * q2n.c);
    rhs.d = sinv * (y * q1n.d + n * q2n.d);
    if (std::isinf(sinv)) {
      c_quaternion o;
      o.a.size[0] = 1;
      o.a.size[1] = 1;
      o.a.data[0] = pn.a;
      o.b.size[0] = 1;
      o.b.size[1] = 1;
      o.b.data[0] = pn.b;
      o.c.size[0] = 1;
      o.c.size[1] = 1;
      o.c.data[0] = pn.c;
      o.d.size[0] = 1;
      o.d.size[1] = 1;
      o.d.data[0] = pn.d;
      rhs.parenAssign(o);
    }
    n = std::sqrt(((rhs.a * rhs.a + rhs.b * rhs.b) + rhs.c * rhs.c) +
                  rhs.d * rhs.d);
    rhs.a /= n;
    rhs.b /= n;
    rhs.c /= n;
    rhs.d /= n;
    R.a[loop_ub] = rhs.a;
    R.b[loop_ub] = rhs.b;
    R.c[loop_ub] = rhs.c;
    R.d[loop_ub] = rhs.d;
    n = ((pnCorrected.a * qnCorrected.a - -pnCorrected.b * qnCorrected.b) -
         -pnCorrected.c * qnCorrected.c) -
        -pnCorrected.d * qnCorrected.d;
    qn.a = n;
    xa = ((pnCorrected.a * qnCorrected.b + -pnCorrected.b * qnCorrected.a) +
          -pnCorrected.c * qnCorrected.d) -
         -pnCorrected.d * qnCorrected.c;
    qn.b = xa;
    sinv = ((pnCorrected.a * qnCorrected.c - -pnCorrected.b * qnCorrected.d) +
            -pnCorrected.c * qnCorrected.a) +
           -pnCorrected.d * qnCorrected.b;
    qn.c = sinv;
    xc = ((pnCorrected.a * qnCorrected.d + -pnCorrected.b * qnCorrected.c) -
          -pnCorrected.c * qnCorrected.b) +
         -pnCorrected.d * qnCorrected.a;
    qn.d = xc;
    qn.b_log();
    d = timeScaling[timeScaling.size(0) * loop_ub + 1];
    qdCalc.a =
        d * (((rhs.a * qn.a - rhs.b * qn.b) - rhs.c * qn.c) - rhs.d * qn.d);
    qdCalc.b =
        d * (((rhs.a * qn.b + rhs.b * qn.a) + rhs.c * qn.d) - rhs.d * qn.c);
    qdCalc.c =
        d * (((rhs.a * qn.c - rhs.b * qn.d) + rhs.c * qn.a) + rhs.d * qn.b);
    qdCalc.d =
        d * (((rhs.a * qn.d + rhs.b * qn.c) - rhs.c * qn.b) + rhs.d * qn.a);
    W_tmp.a = 2.0 * qdCalc.a;
    W_tmp.b = 2.0 * qdCalc.b;
    W_tmp.c = 2.0 * qdCalc.c;
    W_tmp.d = 2.0 * qdCalc.d;
    omega[3 * loop_ub] =
        ((W_tmp.a * -rhs.b + W_tmp.b * rhs.a) + W_tmp.c * -rhs.d) -
        W_tmp.d * -rhs.c;
    omega[3 * loop_ub + 1] =
        ((W_tmp.a * -rhs.c - W_tmp.b * -rhs.d) + W_tmp.c * rhs.a) +
        W_tmp.d * -rhs.b;
    omega[3 * loop_ub + 2] =
        ((W_tmp.a * -rhs.d + W_tmp.b * -rhs.c) - W_tmp.c * -rhs.b) +
        W_tmp.d * rhs.a;
    qn.a = n;
    qn.b = xa;
    qn.c = sinv;
    qn.d = xc;
    qn.b_log();
    b_y.a = n;
    b_y.b = xa;
    b_y.c = sinv;
    b_y.d = xc;
    b_y.b_log();
    b_o.a = ((rhs.a * b_y.a - rhs.b * b_y.b) - rhs.c * b_y.c) - rhs.d * b_y.d;
    b_o.b = ((rhs.a * b_y.b + rhs.b * b_y.a) + rhs.c * b_y.d) - rhs.d * b_y.c;
    b_o.c = ((rhs.a * b_y.c - rhs.b * b_y.d) + rhs.c * b_y.a) + rhs.d * b_y.b;
    b_o.d = ((rhs.a * b_y.d + rhs.b * b_y.c) - rhs.c * b_y.b) + rhs.d * b_y.a;
    b_y.a = n;
    b_y.b = xa;
    b_y.c = sinv;
    b_y.d = xc;
    b_y.b_log();
    y = d * d;
    d = timeScaling[timeScaling.size(0) * loop_ub + 2];
    xa = 2.0 *
         (y * (((b_o.a * b_y.a - b_o.b * b_y.b) - b_o.c * b_y.c) -
               b_o.d * b_y.d) +
          d * (((rhs.a * qn.a - rhs.b * qn.b) - rhs.c * qn.c) - rhs.d * qn.d));
    sinv =
        2.0 *
        (y * (((b_o.a * b_y.b + b_o.b * b_y.a) + b_o.c * b_y.d) -
              b_o.d * b_y.c) +
         d * (((rhs.a * qn.b + rhs.b * qn.a) + rhs.c * qn.d) - rhs.d * qn.c));
    xc = 2.0 *
         (y * (((b_o.a * b_y.c - b_o.b * b_y.d) + b_o.c * b_y.a) +
               b_o.d * b_y.b) +
          d * (((rhs.a * qn.c - rhs.b * qn.d) + rhs.c * qn.a) + rhs.d * qn.b));
    n = 2.0 *
        (y * (((b_o.a * b_y.d + b_o.b * b_y.c) - b_o.c * b_y.b) +
              b_o.d * b_y.a) +
         d * (((rhs.a * qn.d + rhs.b * qn.c) - rhs.c * qn.b) + rhs.d * qn.a));
    qdCalc.b = -qdCalc.b;
    qdCalc.c = -qdCalc.c;
    qdCalc.d = -qdCalc.d;
    alpha[3 * loop_ub] =
        (((xa * -rhs.b + sinv * rhs.a) + xc * -rhs.d) - n * -rhs.c) -
        (((W_tmp.a * qdCalc.b + W_tmp.b * qdCalc.a) + W_tmp.c * qdCalc.d) -
         W_tmp.d * qdCalc.c);
    alpha[3 * loop_ub + 1] =
        (((xa * -rhs.c - sinv * -rhs.d) + xc * rhs.a) + n * -rhs.b) -
        (((W_tmp.a * qdCalc.c - W_tmp.b * qdCalc.d) + W_tmp.c * qdCalc.a) +
         W_tmp.d * qdCalc.b);
    alpha[3 * loop_ub + 2] =
        (((xa * -rhs.d + sinv * -rhs.c) - xc * -rhs.b) + n * rhs.a) -
        (((W_tmp.a * qdCalc.d + W_tmp.b * qdCalc.c) - W_tmp.c * qdCalc.b) +
         W_tmp.d * qdCalc.a);
  }
}

} // namespace coder
} // namespace TrajectoryGeneration

//
// File trailer for rottraj.cpp
//
// [EOF]
//
