//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quaternion.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "quaternion.h"
#include "div.h"
#include "log.h"
#include "rt_nonfinite.h"
#include "coder_bounded_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
namespace TrajectoryGeneration {
namespace coder {
void quaternion::b_log()
{
  double qnorm;
  double vnorm;
  double vscale_data;
  int b_tmp_size[2];
  int c_tmp_size[2];
  int tmp_size[2];
  int vscale_size[2];
  int k;
  int trueCount;
  boolean_T b_b;
  boolean_T nz;
  vnorm = std::sqrt((b * b + c * c) + d * d);
  qnorm = std::sqrt(a * a + vnorm * vnorm);
  nz = (vnorm != 0.0);
  vscale_size[0] = 1;
  trueCount = 0;
  if (nz) {
    trueCount = 1;
  }
  vscale_size[1] = trueCount;
  if (nz) {
    vscale_data = a / qnorm;
  }
  for (k = 0; k < trueCount; k++) {
    vscale_data = std::acos(vscale_data);
  }
  k = 0;
  if (nz) {
    k = 1;
  }
  if (trueCount == k) {
    k = trueCount - 1;
    vscale_size[0] = 1;
    for (trueCount = 0; trueCount <= k; trueCount++) {
      vscale_data /= vnorm;
    }
  } else {
    binary_expand_op_3((double *)&vscale_data, vscale_size, vnorm, k);
  }
  a = std::log(qnorm);
  vnorm = b;
  trueCount = 0;
  if (nz) {
    trueCount = 1;
  }
  tmp_size[0] = 1;
  tmp_size[1] = trueCount;
  if (trueCount == vscale_size[1]) {
    if (vscale_size[1] - 1 >= 0) {
      vnorm = b * vscale_data;
    }
  } else {
    binary_expand_op_2(vnorm, this, tmp_size, (const double *)&vscale_data,
                       vscale_size);
  }
  b_b = !nz;
  if (b_b) {
    vnorm = 0.0;
  }
  b = vnorm;
  vnorm = c;
  trueCount = 0;
  if (nz) {
    trueCount = 1;
  }
  b_tmp_size[0] = 1;
  b_tmp_size[1] = trueCount;
  if (trueCount == vscale_size[1]) {
    if (vscale_size[1] - 1 >= 0) {
      vnorm = c * vscale_data;
    }
  } else {
    binary_expand_op_1(vnorm, this, b_tmp_size, (const double *)&vscale_data,
                       vscale_size);
  }
  if (b_b) {
    vnorm = 0.0;
  }
  c = vnorm;
  vnorm = d;
  trueCount = 0;
  if (nz) {
    trueCount = 1;
  }
  c_tmp_size[0] = 1;
  c_tmp_size[1] = trueCount;
  if (trueCount == vscale_size[1]) {
    if (vscale_size[1] - 1 >= 0) {
      vnorm = d * vscale_data;
    }
  } else {
    binary_expand_op(vnorm, this, c_tmp_size, (const double *)&vscale_data,
                     vscale_size);
  }
  if (b_b) {
    vnorm = 0.0;
  }
  d = vnorm;
}

//
// Arguments    : void
// Return Type  : void
//
void quaternion::init()
{
  a = 1.0;
  b = 0.0;
  c = 0.0;
  d = 0.0;
}

//
// Arguments    : const c_quaternion rhs
// Return Type  : void
//
void quaternion::parenAssign(const c_quaternion rhs)
{
  a = rhs.a.data[0];
  b = rhs.b.data[0];
  c = rhs.c.data[0];
  d = rhs.d.data[0];
}

//
// Arguments    : const quaternion q2
// Return Type  : quaternion
//
quaternion quaternion::quaternionBase_slerp(const quaternion q2) const
{
  quaternion b_q;
  quaternion q;
  quaternion q1n;
  quaternion q1n_tmp;
  quaternion q2n;
  quaternion qo;
  double n;
  double sinv;
  double sinv_tmp;
  q1n_tmp.init();
  n = std::sqrt(((a * a + b * b) + c * c) + d * d);
  q.a = a / n;
  q.b = b / n;
  q.c = c / n;
  q.d = d / n;
  q1n.a =
      ((q.a * q1n_tmp.a - q.b * q1n_tmp.b) - q.c * q1n_tmp.c) - q.d * q1n_tmp.d;
  q1n.b =
      ((q.a * q1n_tmp.b + q.b * q1n_tmp.a) + q.c * q1n_tmp.d) - q.d * q1n_tmp.c;
  q1n.c =
      ((q.a * q1n_tmp.c - q.b * q1n_tmp.d) + q.c * q1n_tmp.a) + q.d * q1n_tmp.b;
  q1n.d =
      ((q.a * q1n_tmp.d + q.b * q1n_tmp.c) - q.c * q1n_tmp.b) + q.d * q1n_tmp.a;
  n = std::sqrt(((q2.a * q2.a + q2.b * q2.b) + q2.c * q2.c) + q2.d * q2.d);
  b_q.a = q2.a / n;
  b_q.b = q2.b / n;
  b_q.c = q2.c / n;
  b_q.d = q2.d / n;
  q2n.a = ((b_q.a * q1n_tmp.a - b_q.b * q1n_tmp.b) - b_q.c * q1n_tmp.c) -
          b_q.d * q1n_tmp.d;
  q2n.b = ((b_q.a * q1n_tmp.b + b_q.b * q1n_tmp.a) + b_q.c * q1n_tmp.d) -
          b_q.d * q1n_tmp.c;
  q2n.c = ((b_q.a * q1n_tmp.c - b_q.b * q1n_tmp.d) + b_q.c * q1n_tmp.a) +
          b_q.d * q1n_tmp.b;
  q2n.d = ((b_q.a * q1n_tmp.d + b_q.b * q1n_tmp.c) - b_q.c * q1n_tmp.b) +
          b_q.d * q1n_tmp.a;
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
  sinv_tmp = std::sin(n);
  sinv = 1.0 / sinv_tmp;
  n = std::sin(0.0 * n);
  qo.a = sinv * (n * q1n.a + sinv_tmp * q2n.a);
  qo.b = sinv * (n * q1n.b + sinv_tmp * q2n.b);
  qo.c = sinv * (n * q1n.c + sinv_tmp * q2n.c);
  qo.d = sinv * (n * q1n.d + sinv_tmp * q2n.d);
  if (std::isinf(sinv)) {
    c_quaternion o;
    o.a.size[0] = 1;
    o.a.size[1] = 1;
    o.a.data[0] = a;
    o.b.size[0] = 1;
    o.b.size[1] = 1;
    o.b.data[0] = b;
    o.c.size[0] = 1;
    o.c.size[1] = 1;
    o.c.data[0] = c;
    o.d.size[0] = 1;
    o.d.size[1] = 1;
    o.d.data[0] = d;
    qo.parenAssign(o);
  }
  n = std::sqrt(((qo.a * qo.a + qo.b * qo.b) + qo.c * qo.c) + qo.d * qo.d);
  qo.a /= n;
  qo.b /= n;
  qo.c /= n;
  qo.d /= n;
  return qo;
}

//
// Arguments    : const quaternion q2
// Return Type  : quaternion
//
quaternion quaternion::slerp(const quaternion q2) const
{
  quaternion b_q;
  quaternion q;
  quaternion q1n;
  quaternion q1n_tmp;
  quaternion q2n;
  quaternion qo;
  double n;
  double sinv;
  double sinv_tmp;
  q1n_tmp.init();
  n = std::sqrt(((a * a + b * b) + c * c) + d * d);
  q.a = a / n;
  q.b = b / n;
  q.c = c / n;
  q.d = d / n;
  q1n.a =
      ((q.a * q1n_tmp.a - q.b * q1n_tmp.b) - q.c * q1n_tmp.c) - q.d * q1n_tmp.d;
  q1n.b =
      ((q.a * q1n_tmp.b + q.b * q1n_tmp.a) + q.c * q1n_tmp.d) - q.d * q1n_tmp.c;
  q1n.c =
      ((q.a * q1n_tmp.c - q.b * q1n_tmp.d) + q.c * q1n_tmp.a) + q.d * q1n_tmp.b;
  q1n.d =
      ((q.a * q1n_tmp.d + q.b * q1n_tmp.c) - q.c * q1n_tmp.b) + q.d * q1n_tmp.a;
  n = std::sqrt(((q2.a * q2.a + q2.b * q2.b) + q2.c * q2.c) + q2.d * q2.d);
  b_q.a = q2.a / n;
  b_q.b = q2.b / n;
  b_q.c = q2.c / n;
  b_q.d = q2.d / n;
  q2n.a = ((b_q.a * q1n_tmp.a - b_q.b * q1n_tmp.b) - b_q.c * q1n_tmp.c) -
          b_q.d * q1n_tmp.d;
  q2n.b = ((b_q.a * q1n_tmp.b + b_q.b * q1n_tmp.a) + b_q.c * q1n_tmp.d) -
          b_q.d * q1n_tmp.c;
  q2n.c = ((b_q.a * q1n_tmp.c - b_q.b * q1n_tmp.d) + b_q.c * q1n_tmp.a) +
          b_q.d * q1n_tmp.b;
  q2n.d = ((b_q.a * q1n_tmp.d + b_q.b * q1n_tmp.c) - b_q.c * q1n_tmp.b) +
          b_q.d * q1n_tmp.a;
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
  sinv_tmp = std::sin(n);
  sinv = 1.0 / sinv_tmp;
  n = std::sin(0.0 * n);
  qo.a = sinv * (sinv_tmp * q1n.a + n * q2n.a);
  qo.b = sinv * (sinv_tmp * q1n.b + n * q2n.b);
  qo.c = sinv * (sinv_tmp * q1n.c + n * q2n.c);
  qo.d = sinv * (sinv_tmp * q1n.d + n * q2n.d);
  if (std::isinf(sinv)) {
    c_quaternion o;
    o.a.size[0] = 1;
    o.a.size[1] = 1;
    o.a.data[0] = a;
    o.b.size[0] = 1;
    o.b.size[1] = 1;
    o.b.data[0] = b;
    o.c.size[0] = 1;
    o.c.size[1] = 1;
    o.c.data[0] = c;
    o.d.size[0] = 1;
    o.d.size[1] = 1;
    o.d.data[0] = d;
    qo.parenAssign(o);
  }
  n = std::sqrt(((qo.a * qo.a + qo.b * qo.b) + qo.c * qo.c) + qo.d * qo.d);
  qo.a /= n;
  qo.b /= n;
  qo.c /= n;
  qo.d /= n;
  return qo;
}

} // namespace coder
} // namespace TrajectoryGeneration

//
// File trailer for quaternion.cpp
//
// [EOF]
//
