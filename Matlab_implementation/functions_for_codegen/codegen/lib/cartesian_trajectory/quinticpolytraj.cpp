//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quinticpolytraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

// Include Files
#include "quinticpolytraj.h"
#include "cartesian_trajectory_rtwutil.h"
#include "changeEndSegBreaks.h"
#include "ppval.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &wayPoints
//                array<double, 2U> &timePoints
//                const array<double, 2U> &t
//                const array<double, 2U> &varargin_2
//                const array<double, 2U> &varargin_4
//                array<double, 2U> &q
//                array<double, 2U> &qd
//                array<double, 2U> &qdd
// Return Type  : void
//
namespace coder {
void quinticpolytraj(const array<double, 2U> &wayPoints,
                     array<double, 2U> &timePoints, const array<double, 2U> &t,
                     const array<double, 2U> &varargin_2,
                     const array<double, 2U> &varargin_4, array<double, 2U> &q,
                     array<double, 2U> &qd, array<double, 2U> &qdd)
{
  static const signed char iv[6]{0, 0, 0, 0, 0, 1};
  array<double, 3U> pp_coefs;
  array<double, 2U> b_timePoints;
  array<double, 2U> coefMat;
  array<double, 2U> dCoeffs;
  array<double, 2U> derivativeBreaks;
  array<double, 2U> r;
  array<double, 1U> valueAtEnd;
  double coeffMat[18];
  double coeffVec[6];
  double d;
  double wayPoints_idx_0;
  double xtmp;
  int i;
  int i1;
  int loop_ub;
  int loop_ub_tmp;
  b_timePoints.set_size(1, timePoints.size(1));
  loop_ub = timePoints.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_timePoints[i] = timePoints[i];
  }
  timePoints.set_size(1, b_timePoints.size(1));
  loop_ub = b_timePoints.size(1);
  for (i = 0; i < loop_ub; i++) {
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
  loop_ub = i * 6;
  for (i = 0; i < loop_ub; i++) {
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
    for (loop_ub_tmp = 0; loop_ub_tmp < 3; loop_ub_tmp++) {
      double dv[9];
      double y[3];
      double wayPoints_idx_2;
      coeffVec[0] = wayPoints[loop_ub_tmp + 3 * b_i];
      coeffVec[1] = varargin_2[loop_ub_tmp + 3 * b_i];
      coeffVec[2] = varargin_4[loop_ub_tmp + 3 * b_i] / 2.0;
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
          wayPoints[loop_ub_tmp +
                    3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                          1.0) -
                         1)] -
          y[0];
      xtmp = varargin_2[loop_ub_tmp +
                        3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                              1.0) -
                             1)] -
             y[1];
      wayPoints_idx_2 =
          varargin_4[loop_ub_tmp +
                     3 * (static_cast<int>((static_cast<double>(b_i) + 1.0) +
                                           1.0) -
                          1)] -
          y[2];
      for (loop_ub = 0; loop_ub < 3; loop_ub++) {
        coeffVec[loop_ub + 3] =
            (dv[loop_ub] * wayPoints_idx_0 + dv[loop_ub + 3] * xtmp) +
            dv[loop_ub + 6] * wayPoints_idx_2;
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
      loop_ub =
          static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) * 3.0 +
                           (static_cast<double>(loop_ub_tmp) + 1.0));
      for (i1 = 0; i1 < 6; i1++) {
        coefMat[(loop_ub + coefMat.size(0) * i1) - 1] = coeffVec[i1];
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
  loop_ub = (coefMat.size(0) + 3) * 6;
  for (i = 0; i < loop_ub; i++) {
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
  loop_ub = i1 - i;
  for (i1 = 0; i1 < 6; i1++) {
    for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
      dCoeffs[(i + loop_ub_tmp) + dCoeffs.size(0) * i1] =
          coefMat[loop_ub_tmp + coefMat.size(0) * i1];
    }
  }
  b_timePoints.set_size(1, timePoints.size(1) + 1);
  b_timePoints[0] = timePoints[0] - 1.0;
  loop_ub = timePoints.size(1);
  for (i = 0; i < loop_ub; i++) {
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
  loop_ub_tmp = i1 - i;
  valueAtEnd.set_size(loop_ub_tmp);
  for (int b_i{0}; b_i < loop_ub_tmp; b_i++) {
    xtmp = 0.0;
    for (loop_ub = 0; loop_ub < 6; loop_ub++) {
      i1 = loop_ub * loop_ub_tmp + b_i;
      xtmp += dCoeffs[(i + i1 % loop_ub_tmp) +
                      dCoeffs.size(0) * (i1 / loop_ub_tmp)] *
              coeffVec[loop_ub];
    }
    valueAtEnd[b_i] = xtmp;
  }
  std::memset(&coeffMat[0], 0, 18U * sizeof(double));
  coeffMat[15] = valueAtEnd[0];
  coeffMat[16] = valueAtEnd[1];
  coeffMat[17] = valueAtEnd[2];
  coefMat.set_size(dCoeffs.size(0) + 3, 6);
  loop_ub = (dCoeffs.size(0) + 3) * 6;
  for (i = 0; i < loop_ub; i++) {
    coefMat[i] = 0.0;
  }
  loop_ub = dCoeffs.size(0);
  for (i = 0; i < 6; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      coefMat[i1 + coefMat.size(0) * i] = dCoeffs[i1 + dCoeffs.size(0) * i];
    }
  }
  for (i = 0; i < 6; i++) {
    coefMat[dCoeffs.size(0) + coefMat.size(0) * i] = coeffMat[3 * i];
    coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 1] = coeffMat[3 * i + 1];
    coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 2] = coeffMat[3 * i + 2];
  }
  derivativeBreaks.set_size(1, b_timePoints.size(1) + 1);
  loop_ub = b_timePoints.size(1);
  for (i = 0; i < loop_ub; i++) {
    derivativeBreaks[i] = b_timePoints[i];
  }
  derivativeBreaks[b_timePoints.size(1)] = wayPoints_idx_0 + 1.0;
  pp_coefs.set_size(3, derivativeBreaks.size(1) - 1, 6);
  loop_ub = 3 * (derivativeBreaks.size(1) - 1) * 6;
  for (i = 0; i < loop_ub; i++) {
    pp_coefs[i] = coefMat[i];
  }
  b_timePoints.set_size(1, derivativeBreaks.size(1));
  loop_ub = derivativeBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  loop_ub = t.size(1);
  for (i = 0; i < loop_ub; i++) {
    q[3 * i] = r[3 * i];
    q[3 * i + 1] = r[3 * i + 1];
    q[3 * i + 2] = r[3 * i + 2];
  }
  robotics::core::internal::changeEndSegBreaks(derivativeBreaks, t);
  dCoeffs.set_size(coefMat.size(0), 6);
  loop_ub = coefMat.size(0) * 6;
  for (i = 0; i < loop_ub; i++) {
    dCoeffs[i] = 0.0;
  }
  for (int b_i{0}; b_i < 5; b_i++) {
    loop_ub = dCoeffs.size(0);
    for (i = 0; i < loop_ub; i++) {
      dCoeffs[i + dCoeffs.size(0) * (b_i + 1)] =
          ((6.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
          coefMat[i + coefMat.size(0) * b_i];
    }
  }
  pp_coefs.set_size(3, derivativeBreaks.size(1) - 1, 6);
  loop_ub_tmp = 3 * (derivativeBreaks.size(1) - 1) * 6;
  for (i = 0; i < loop_ub_tmp; i++) {
    pp_coefs[i] = dCoeffs[i];
  }
  b_timePoints.set_size(1, derivativeBreaks.size(1));
  loop_ub = derivativeBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  loop_ub = t.size(1);
  for (i = 0; i < loop_ub; i++) {
    qd[3 * i] = r[3 * i];
    qd[3 * i + 1] = r[3 * i + 1];
    qd[3 * i + 2] = r[3 * i + 2];
  }
  coefMat.set_size(dCoeffs.size(0), 6);
  loop_ub = dCoeffs.size(0) * 6;
  for (i = 0; i < loop_ub; i++) {
    coefMat[i] = 0.0;
  }
  for (int b_i{0}; b_i < 5; b_i++) {
    loop_ub = coefMat.size(0);
    for (i = 0; i < loop_ub; i++) {
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
  loop_ub = derivativeBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_timePoints[i] = derivativeBreaks[i];
  }
  b_ppval(b_timePoints, pp_coefs, t, r);
  loop_ub = t.size(1);
  for (i = 0; i < loop_ub; i++) {
    qdd[3 * i] = r[3 * i];
    qdd[3 * i + 1] = r[3 * i + 1];
    qdd[3 * i + 2] = r[3 * i + 2];
  }
}

} // namespace coder

//
// File trailer for quinticpolytraj.cpp
//
// [EOF]
//
