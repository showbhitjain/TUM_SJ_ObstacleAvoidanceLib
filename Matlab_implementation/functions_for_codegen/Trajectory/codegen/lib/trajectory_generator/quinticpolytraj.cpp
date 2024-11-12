//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quinticpolytraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "quinticpolytraj.h"
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
// Arguments    : const ::coder::array<double, 2U> &wayPoints
//                ::coder::array<double, 2U> &timePoints
//                const ::coder::array<double, 2U> &t
//                const ::coder::array<double, 2U> &varargin_2
//                const ::coder::array<double, 2U> &varargin_4
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
// Return Type  : void
//
namespace TrajectoryGeneration {
namespace coder {
void quinticpolytraj(const ::coder::array<double, 2U> &wayPoints,
                     ::coder::array<double, 2U> &timePoints,
                     const ::coder::array<double, 2U> &t,
                     const ::coder::array<double, 2U> &varargin_2,
                     const ::coder::array<double, 2U> &varargin_4,
                     ::coder::array<double, 2U> &q,
                     ::coder::array<double, 2U> &qd,
                     ::coder::array<double, 2U> &qdd)
{
  static const signed char iv[6]{0, 0, 0, 0, 0, 1};
  ::coder::array<double, 3U> pp_coefs;
  ::coder::array<double, 2U> b_timePoints;
  ::coder::array<double, 2U> coefMat;
  ::coder::array<double, 2U> dCoeffs;
  ::coder::array<double, 2U> derivativeBreaks;
  ::coder::array<double, 2U> r;
  ::coder::array<double, 1U> valueAtEnd;
  ::coder::array<boolean_T, 2U> b_t;
  double coeffMat[18];
  double coeffVec[6];
  double d;
  double wayPoints_idx_0;
  double xtmp;
  int tGreaterThanTfIdx_size[2];
  int i;
  int i1;
  int loop_ub_tmp;
  int tGreaterThanTfIdx_data;
  b_timePoints.set_size(1, timePoints.size(1));
  tGreaterThanTfIdx_data = timePoints.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    b_timePoints[i] = timePoints[i];
  }
  timePoints.set_size(1, b_timePoints.size(1));
  tGreaterThanTfIdx_data = b_timePoints.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    timePoints[i] = b_timePoints[i];
  }
  q.set_size(3, t.size(1));
  loop_ub_tmp = 3 * t.size(1);
  qd.set_size(3, t.size(1));
  qdd.set_size(3, t.size(1));
  for (i = 0; i < loop_ub_tmp; i++) {
    q[i] = 0.0;
    qd[i] = 0.0;
    qdd[i] = 0.0;
  }
  i = (wayPoints.size(1) - 1) * 3;
  coefMat.set_size(i, 6);
  tGreaterThanTfIdx_data = i * 6;
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    coefMat[i] = 0.0;
  }
  i = wayPoints.size(1);
  for (int b_i{0}; b_i <= i - 2; b_i++) {
    double d1;
    double d2;
    double d3;
    double finalTime;
    finalTime = timePoints[b_i + 1] - timePoints[b_i];
    d = rt_powd_snf(finalTime, 3.0);
    d1 = rt_powd_snf(finalTime, 4.0);
    d2 = rt_powd_snf(finalTime, 5.0);
    d3 = finalTime * finalTime;
    for (tGreaterThanTfIdx_data = 0; tGreaterThanTfIdx_data < 3;
         tGreaterThanTfIdx_data++) {
      double dv[9];
      double y[3];
      double wayPoints_idx_2;
      coeffVec[0] = wayPoints[tGreaterThanTfIdx_data + 3 * b_i];
      coeffVec[1] = varargin_2[tGreaterThanTfIdx_data + 3 * b_i];
      coeffVec[2] = varargin_4[tGreaterThanTfIdx_data + 3 * b_i] / 2.0;
      coeffVec[3] = 0.0;
      coeffVec[4] = 0.0;
      coeffVec[5] = 0.0;
      dv[0] = 1.0;
      dv[3] = finalTime;
      dv[6] = d3;
      dv[1] = 0.0;
      dv[4] = 1.0;
      dv[7] = 2.0 * finalTime;
      dv[2] = 0.0;
      dv[5] = 0.0;
      dv[8] = 2.0;
      wayPoints_idx_0 = coeffVec[0];
      xtmp = coeffVec[1];
      wayPoints_idx_2 = coeffVec[2];
      for (i1 = 0; i1 < 3; i1++) {
        y[i1] = (dv[i1] * wayPoints_idx_0 + dv[i1 + 3] * xtmp) +
                dv[i1 + 6] * wayPoints_idx_2;
      }
      dv[0] = 10.0 / d;
      dv[3] = -4.0 / d3;
      dv[6] = 1.0 / (2.0 * finalTime);
      dv[1] = -15.0 / d1;
      dv[4] = 7.0 / d;
      dv[7] = -1.0 / d3;
      dv[2] = 6.0 / d2;
      dv[5] = -3.0 / d1;
      dv[8] = 1.0 / (2.0 * d);
      wayPoints_idx_0 =
          wayPoints[tGreaterThanTfIdx_data +
                    3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                          1.0) -
                         1)] -
          y[0];
      xtmp = varargin_2[tGreaterThanTfIdx_data +
                        3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                              1.0) -
                             1)] -
             y[1];
      wayPoints_idx_2 =
          varargin_4[tGreaterThanTfIdx_data +
                     3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                           1.0) -
                          1)] -
          y[2];
      for (loop_ub_tmp = 0; loop_ub_tmp < 3; loop_ub_tmp++) {
        coeffVec[loop_ub_tmp + 3] =
            (dv[loop_ub_tmp] * wayPoints_idx_0 + dv[loop_ub_tmp + 3] * xtmp) +
            dv[loop_ub_tmp + 6] * wayPoints_idx_2;
      }
      xtmp = coeffVec[0];
      coeffVec[0] = coeffVec[5];
      coeffVec[5] = xtmp;
      xtmp = coeffVec[1];
      coeffVec[1] = coeffVec[4];
      coeffVec[4] = xtmp;
      xtmp = coeffVec[2];
      coeffVec[2] = coeffVec[3];
      coeffVec[3] = xtmp;
      loop_ub_tmp =
          static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) * 3.0 +
                           (static_cast<double>(tGreaterThanTfIdx_data) + 1.0));
      for (i1 = 0; i1 < 6; i1++) {
        coefMat[(loop_ub_tmp + coefMat.size(0) * i1) - 1] = coeffVec[i1];
      }
    }
  }
  std::memset(&coeffMat[0], 0, 18U * sizeof(double));
  for (i = 0; i < 3; i++) {
    d = 0.0;
    for (i1 = 0; i1 < 6; i1++) {
      d += coefMat[i + coefMat.size(0) * i1] * static_cast<double>(iv[i1]);
    }
    coeffMat[i + 15] = d;
  }
  dCoeffs.set_size(coefMat.size(0) + 3, 6);
  tGreaterThanTfIdx_data = (coefMat.size(0) + 3) * 6;
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    dCoeffs[i] = 0.0;
  }
  for (i = 0; i < 6; i++) {
    dCoeffs[dCoeffs.size(0) * i] = coeffMat[3 * i];
    dCoeffs[dCoeffs.size(0) * i + 1] = coeffMat[3 * i + 1];
    dCoeffs[dCoeffs.size(0) * i + 2] = coeffMat[3 * i + 2];
  }
  if (static_cast<unsigned int>(coefMat.size(0)) + 3U < 4U) {
    i = 0;
    i1 = 0;
  } else {
    i = 3;
    i1 = static_cast<int>(static_cast<unsigned int>(coefMat.size(0)) + 3U);
  }
  tGreaterThanTfIdx_data = i1 - i;
  for (i1 = 0; i1 < 6; i1++) {
    for (loop_ub_tmp = 0; loop_ub_tmp < tGreaterThanTfIdx_data; loop_ub_tmp++) {
      dCoeffs[(i + loop_ub_tmp) + dCoeffs.size(0) * i1] =
          coefMat[loop_ub_tmp + coefMat.size(0) * i1];
    }
  }
  b_timePoints.set_size(1, timePoints.size(1) + 1);
  b_timePoints[0] = timePoints[0] - 1.0;
  tGreaterThanTfIdx_data = timePoints.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    b_timePoints[i + 1] = timePoints[i];
  }
  wayPoints_idx_0 = b_timePoints[b_timePoints.size(1) - 1];
  xtmp = wayPoints_idx_0 - b_timePoints[b_timePoints.size(1) - 2];
  for (int b_i{0}; b_i < 6; b_i++) {
    coeffVec[b_i] = rt_powd_snf(xtmp, 6.0 - (static_cast<double>(b_i) + 1.0));
  }
  if (dCoeffs.size(0) - 2 > dCoeffs.size(0)) {
    i = 0;
    i1 = 0;
  } else {
    i = dCoeffs.size(0) - 3;
    i1 = dCoeffs.size(0);
  }
  tGreaterThanTfIdx_data = i1 - i;
  valueAtEnd.set_size(tGreaterThanTfIdx_data);
  for (int b_i{0}; b_i < tGreaterThanTfIdx_data; b_i++) {
    xtmp = 0.0;
    for (loop_ub_tmp = 0; loop_ub_tmp < 6; loop_ub_tmp++) {
      i1 = loop_ub_tmp * tGreaterThanTfIdx_data + b_i;
      xtmp += dCoeffs[(i + i1 % tGreaterThanTfIdx_data) +
                      dCoeffs.size(0) * (i1 / tGreaterThanTfIdx_data)] *
              coeffVec[loop_ub_tmp];
    }
    valueAtEnd[b_i] = xtmp;
  }
  std::memset(&coeffMat[0], 0, 18U * sizeof(double));
  coeffMat[15] = valueAtEnd[0];
  coeffMat[16] = valueAtEnd[1];
  coeffMat[17] = valueAtEnd[2];
  coefMat.set_size(dCoeffs.size(0) + 3, 6);
  tGreaterThanTfIdx_data = (dCoeffs.size(0) + 3) * 6;
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    coefMat[i] = 0.0;
  }
  tGreaterThanTfIdx_data = dCoeffs.size(0);
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < tGreaterThanTfIdx_data; i1++) {
      coefMat[i1 + coefMat.size(0) * i] = dCoeffs[i1 + dCoeffs.size(0) * i];
    }
  }
  for (i = 0; i < 6; i++) {
    coefMat[dCoeffs.size(0) + coefMat.size(0) * i] = coeffMat[3 * i];
    coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 1] = coeffMat[3 * i + 1];
    coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 2] = coeffMat[3 * i + 2];
  }
  derivativeBreaks.set_size(1, b_timePoints.size(1) + 1);
  tGreaterThanTfIdx_data = b_timePoints.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    derivativeBreaks[i] = b_timePoints[i];
  }
  derivativeBreaks[b_timePoints.size(1)] = wayPoints_idx_0 + 1.0;
  pp_coefs.set_size(3, derivativeBreaks.size(1) - 1, 6);
  loop_ub_tmp = 3 * (derivativeBreaks.size(1) - 1) * 6;
  for (i = 0; i < loop_ub_tmp; i++) {
    pp_coefs[i] = coefMat[i];
  }
  b_timePoints.set_size(1, derivativeBreaks.size(1));
  tGreaterThanTfIdx_data = derivativeBreaks.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  xtmp = 0.01;
  wayPoints_idx_0 = derivativeBreaks[derivativeBreaks.size(1) - 2];
  b_t.set_size(1, t.size(1));
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    q[3 * i] = r[3 * i];
    q[3 * i + 1] = r[3 * i + 1];
    q[3 * i + 2] = r[3 * i + 2];
    b_t[i] = (t[i] > wayPoints_idx_0);
  }
  eml_find(b_t, (int *)&tGreaterThanTfIdx_data, tGreaterThanTfIdx_size);
  if (tGreaterThanTfIdx_size[1] != 0) {
    xtmp = std::fmin((t[tGreaterThanTfIdx_data - 1] - wayPoints_idx_0) / 2.0,
                     0.01);
  }
  derivativeBreaks[derivativeBreaks.size(1) - 2] = wayPoints_idx_0 + xtmp;
  dCoeffs.set_size(coefMat.size(0), 6);
  tGreaterThanTfIdx_data = coefMat.size(0) * 6;
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    dCoeffs[i] = 0.0;
  }
  for (int b_i{0}; b_i < 5; b_i++) {
    tGreaterThanTfIdx_data = dCoeffs.size(0);
    for (i = 0; i < tGreaterThanTfIdx_data; i++) {
      dCoeffs[i + dCoeffs.size(0) * (b_i + 1)] =
          ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
          coefMat[i + coefMat.size(0) * b_i];
    }
  }
  pp_coefs.set_size(3, derivativeBreaks.size(1) - 1, 6);
  for (i = 0; i < loop_ub_tmp; i++) {
    pp_coefs[i] = dCoeffs[i];
  }
  b_timePoints.set_size(1, derivativeBreaks.size(1));
  tGreaterThanTfIdx_data = derivativeBreaks.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qd[3 * i] = r[3 * i];
    qd[3 * i + 1] = r[3 * i + 1];
    qd[3 * i + 2] = r[3 * i + 2];
  }
  coefMat.set_size(dCoeffs.size(0), 6);
  tGreaterThanTfIdx_data = dCoeffs.size(0) * 6;
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    coefMat[i] = 0.0;
  }
  for (int b_i{0}; b_i < 5; b_i++) {
    tGreaterThanTfIdx_data = coefMat.size(0);
    for (i = 0; i < tGreaterThanTfIdx_data; i++) {
      coefMat[i + coefMat.size(0) * (b_i + 1)] =
          ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
          dCoeffs[i + dCoeffs.size(0) * b_i];
    }
  }
  pp_coefs.set_size(3, derivativeBreaks.size(1) - 1, 6);
  for (i = 0; i < loop_ub_tmp; i++) {
    pp_coefs[i] = coefMat[i];
  }
  b_timePoints.set_size(1, derivativeBreaks.size(1));
  tGreaterThanTfIdx_data = derivativeBreaks.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qdd[3 * i] = r[3 * i];
    qdd[3 * i + 1] = r[3 * i + 1];
    qdd[3 * i + 2] = r[3 * i + 2];
  }
}

//
// Arguments    : const double timePoints[2]
//                const ::coder::array<double, 2U> &t
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
// Return Type  : void
//
void quinticpolytraj(const double timePoints[2],
                     const ::coder::array<double, 2U> &t,
                     ::coder::array<double, 2U> &q,
                     ::coder::array<double, 2U> &qd,
                     ::coder::array<double, 2U> &qdd)
{
  static const signed char iv[6]{0, 0, 0, 0, 0, 1};
  ::coder::array<double, 2U> r;
  ::coder::array<boolean_T, 2U> b_t;
  c_struct_T b_expl_temp;
  c_struct_T expl_temp;
  double dCoeffs[18];
  double coefsWithFlatStart[12];
  double dv[9];
  double coeffVec[6];
  double newSegmentCoeffs[6];
  double y[3];
  double dt;
  double finalTime_tmp;
  double unnamed_idx_1;
  double unnamed_idx_2;
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
  finalTime_tmp = timePoints[1] - timePoints[0];
  coeffVec[3] = 0.0;
  coeffVec[4] = 0.0;
  coeffVec[5] = 0.0;
  dv[0] = 1.0;
  dv[3] = finalTime_tmp;
  unnamed_idx_2 = finalTime_tmp * finalTime_tmp;
  dv[6] = unnamed_idx_2;
  dv[1] = 0.0;
  dv[4] = 1.0;
  dv[7] = 2.0 * finalTime_tmp;
  dv[2] = 0.0;
  dv[5] = 0.0;
  dv[8] = 2.0;
  for (i = 0; i < 3; i++) {
    y[i] = dv[i + 3] * 0.0 + dv[i + 6] * 0.0;
  }
  dt = rt_powd_snf(finalTime_tmp, 3.0);
  dv[0] = 10.0 / dt;
  dv[3] = -4.0 / unnamed_idx_2;
  dv[6] = 1.0 / (2.0 * finalTime_tmp);
  unnamed_idx_1 = rt_powd_snf(finalTime_tmp, 4.0);
  dv[1] = -15.0 / unnamed_idx_1;
  dv[4] = 7.0 / dt;
  dv[7] = -1.0 / unnamed_idx_2;
  dv[2] = 6.0 / rt_powd_snf(finalTime_tmp, 5.0);
  dv[5] = -3.0 / unnamed_idx_1;
  dv[8] = 1.0 / (2.0 * dt);
  dt = 1.0 - y[0];
  unnamed_idx_1 = 0.0 - y[1];
  unnamed_idx_2 = 0.0 - y[2];
  for (int b_i{0}; b_i < 3; b_i++) {
    coeffVec[b_i + 3] = (dv[b_i] * dt + dv[b_i + 3] * unnamed_idx_1) +
                        dv[b_i + 6] * unnamed_idx_2;
  }
  coeffVec[0] = coeffVec[5];
  coeffVec[5] = 0.0;
  coeffVec[1] = coeffVec[4];
  coeffVec[4] = 0.0;
  coeffVec[2] = coeffVec[3];
  coeffVec[3] = 0.0;
  dt = 0.0;
  for (i = 0; i < 6; i++) {
    newSegmentCoeffs[i] = 0.0;
    dt += coeffVec[i] * static_cast<double>(iv[i]);
  }
  newSegmentCoeffs[5] = dt;
  dt = 0.0;
  for (int b_i{0}; b_i < 6; b_i++) {
    tGreaterThanTfIdx_data = b_i << 1;
    coefsWithFlatStart[tGreaterThanTfIdx_data] = newSegmentCoeffs[b_i];
    unnamed_idx_2 = coeffVec[b_i];
    coefsWithFlatStart[tGreaterThanTfIdx_data + 1] = unnamed_idx_2;
    newSegmentCoeffs[b_i] = 0.0;
    dt += unnamed_idx_2 *
          rt_powd_snf(finalTime_tmp, 6.0 - (static_cast<double>(b_i) + 1.0));
  }
  newSegmentCoeffs[5] = dt;
  std::memset(&expl_temp.coefs[0], 0, 18U * sizeof(double));
  for (i = 0; i < 6; i++) {
    tGreaterThanTfIdx_data = i << 1;
    expl_temp.coefs[3 * i] = coefsWithFlatStart[tGreaterThanTfIdx_data];
    expl_temp.coefs[3 * i + 1] = coefsWithFlatStart[tGreaterThanTfIdx_data + 1];
    expl_temp.coefs[3 * i + 2] = newSegmentCoeffs[i];
  }
  b_expl_temp.breaks[0] = timePoints[0] - 1.0;
  b_expl_temp.breaks[1] = timePoints[0];
  b_expl_temp.breaks[2] = timePoints[1];
  b_expl_temp.breaks[3] = timePoints[1] + 1.0;
  std::copy(&expl_temp.coefs[0], &expl_temp.coefs[18], &b_expl_temp.coefs[0]);
  ppval(b_expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  dt = 0.01;
  b_t.set_size(1, t.size(1));
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    q[i] = r[i];
    b_t[i] = (t[i] > b_expl_temp.breaks[2]);
  }
  eml_find(b_t, (int *)&tGreaterThanTfIdx_data, tGreaterThanTfIdx_size);
  if (tGreaterThanTfIdx_size[1] != 0) {
    dt = std::fmin((t[tGreaterThanTfIdx_data - 1] - timePoints[1]) / 2.0, 0.01);
  }
  b_expl_temp.breaks[2] = timePoints[1] + dt;
  std::memset(&dCoeffs[0], 0, 18U * sizeof(double));
  for (int b_i{0}; b_i < 5; b_i++) {
    tGreaterThanTfIdx_data = 3 * (b_i + 1);
    dCoeffs[tGreaterThanTfIdx_data] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i];
    dCoeffs[tGreaterThanTfIdx_data + 1] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i + 1];
    dCoeffs[tGreaterThanTfIdx_data + 2] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
        expl_temp.coefs[3 * b_i + 2];
  }
  std::copy(&dCoeffs[0], &dCoeffs[18], &b_expl_temp.coefs[0]);
  ppval(b_expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qd[i] = r[i];
  }
  std::memset(&expl_temp.coefs[0], 0, 18U * sizeof(double));
  for (int b_i{0}; b_i < 5; b_i++) {
    i = 3 * (b_i + 1);
    expl_temp.coefs[i] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i];
    expl_temp.coefs[i + 1] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i + 1];
    expl_temp.coefs[i + 2] =
        ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) * dCoeffs[3 * b_i + 2];
  }
  expl_temp.breaks[0] = timePoints[0] - 1.0;
  expl_temp.breaks[1] = timePoints[0];
  expl_temp.breaks[2] = b_expl_temp.breaks[2];
  expl_temp.breaks[3] = timePoints[1] + 1.0;
  ppval(expl_temp, t, r);
  tGreaterThanTfIdx_data = t.size(1);
  for (i = 0; i < tGreaterThanTfIdx_data; i++) {
    qdd[i] = r[i];
  }
}

} // namespace coder
} // namespace TrajectoryGeneration

//
// File trailer for quinticpolytraj.cpp
//
// [EOF]
//
