//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: cubicpolytraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

// Include Files
#include "cubicpolytraj.h"
#include "find.h"
#include "ppval.h"
#include "rt_nonfinite.h"
#include "trajectory_generator_internal_types.h"
#include "trajectory_generator_rtwutil.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const double timePoints[2]
//                const ::coder::array<double, 2U> &t
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
// Return Type  : void
//
namespace trajectoryGeneration {
namespace coder {
void cubicpolytraj(const double timePoints[2],
                   const ::coder::array<double, 2U> &t,
                   ::coder::array<double, 2U> &q,
                   ::coder::array<double, 2U> &qd,
                   ::coder::array<double, 2U> &qdd)
{
  ::coder::array<double, 2U> r;
  ::coder::array<boolean_T, 2U> b_t;
  b_struct_T b_expl_temp;
  b_struct_T c_expl_temp;
  b_struct_T expl_temp;
  double dCoeffs[12];
  double coefsWithFlatStart[8];
  double newSegmentCoeffs[4];
  double dt;
  int tGreaterThanTfIdx_size[2];
  int i;
  int tGreaterThanTfIdx_data;
  q.set_size(1, t.size(1));
  tGreaterThanTfIdx_data = t.size(1);
  qd.set_size(1, t.size(1));
  qdd.set_size(1, t.size(1));
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    q[i] = 0.0;
    qd[i] = 0.0;
    qdd[i] = 0.0;
  }
  double d;
  double d1;
  double y_idx_0;
  dt = timePoints[1] - timePoints[0];
  y_idx_0 = dt * 0.0;
  d = dt * dt;
  d1 = rt_powd_snf(dt, 3.0);
  expl_temp.breaks[2] = 3.0 / d * (1.0 - y_idx_0) + -1.0 / dt * 0.0;
  d = -2.0 / d1 * (1.0 - y_idx_0) + 1.0 / d * 0.0;
  expl_temp.breaks[1] = expl_temp.breaks[2];
  coefsWithFlatStart[0] = 0.0;
  coefsWithFlatStart[1] = d;
  coefsWithFlatStart[2] = 0.0;
  coefsWithFlatStart[3] = expl_temp.breaks[1];
  coefsWithFlatStart[4] = 0.0;
  coefsWithFlatStart[5] = 0.0;
  coefsWithFlatStart[6] = d * 0.0 + expl_temp.breaks[1] * 0.0;
  coefsWithFlatStart[7] = 0.0;
  newSegmentCoeffs[0] = 0.0;
  newSegmentCoeffs[1] = 0.0;
  newSegmentCoeffs[2] = 0.0;
  newSegmentCoeffs[3] = ((d1 * d + rt_powd_snf(dt, 2.0) * expl_temp.breaks[1]) +
                         rt_powd_snf(dt, 1.0) * 0.0) +
                        rt_powd_snf(dt, 0.0) * 0.0;
  std::memset(&expl_temp.coefs[0], 0, 12U * sizeof(double));
  for (i = 0; i < 4; i++) {
    tGreaterThanTfIdx_data = i << 1;
    expl_temp.coefs[3 * i] = coefsWithFlatStart[tGreaterThanTfIdx_data];
    expl_temp.coefs[3 * i + 1] = coefsWithFlatStart[tGreaterThanTfIdx_data + 1];
    expl_temp.coefs[3 * i + 2] = newSegmentCoeffs[i];
  }
  expl_temp.breaks[0] = timePoints[0] - 1.0;
  expl_temp.breaks[1] = timePoints[0];
  expl_temp.breaks[2] = timePoints[1];
  expl_temp.breaks[3] = timePoints[1] + 1.0;
  std::copy(&expl_temp.coefs[0], &expl_temp.coefs[12], &b_expl_temp.coefs[0]);
  b_expl_temp.breaks[0] = timePoints[0] - 1.0;
  b_expl_temp.breaks[1] = timePoints[0];
  b_expl_temp.breaks[2] = timePoints[1];
  b_expl_temp.breaks[3] = timePoints[1] + 1.0;
  ppval(b_expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  dt = 0.01;
  b_t.set_size(1, t.size(1));
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    q[i] = r[i];
    b_t[i] = (t[i] > expl_temp.breaks[2]);
  }
  eml_find(b_t, (int *)&tGreaterThanTfIdx_data, tGreaterThanTfIdx_size);
  if (tGreaterThanTfIdx_size[1] != 0) {
    dt = std::fmin((t[tGreaterThanTfIdx_data - 1] - timePoints[1]) / 2.0, 0.01);
  }
  expl_temp.breaks[2] = timePoints[1] + dt;
  std::memset(&dCoeffs[0], 0, 12U * sizeof(double));
  for (int b_i{0}; b_i < 3; b_i++) {
    tGreaterThanTfIdx_data = 3 * (b_i + 1);
    dCoeffs[tGreaterThanTfIdx_data] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i];
    dCoeffs[tGreaterThanTfIdx_data + 1] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i + 1];
    dCoeffs[tGreaterThanTfIdx_data + 2] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i + 2];
  }
  std::copy(&dCoeffs[0], &dCoeffs[12], &c_expl_temp.coefs[0]);
  c_expl_temp.breaks[0] = timePoints[0] - 1.0;
  c_expl_temp.breaks[1] = timePoints[0];
  c_expl_temp.breaks[2] = expl_temp.breaks[2];
  c_expl_temp.breaks[3] = timePoints[1] + 1.0;
  ppval(c_expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qd[i] = r[i];
  }
  std::memset(&expl_temp.coefs[0], 0, 12U * sizeof(double));
  for (int b_i{0}; b_i < 3; b_i++) {
    i = 3 * (b_i + 1);
    expl_temp.coefs[i] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i];
    expl_temp.coefs[i + 1] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i + 1];
    expl_temp.coefs[i + 2] =
        ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i + 2];
  }
  ppval(expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qdd[i] = r[i];
  }
}

} // namespace coder
} // namespace trajectoryGeneration

//
// File trailer for cubicpolytraj.cpp
//
// [EOF]
//
