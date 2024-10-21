//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: Trajectory.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

// Include Files
#include "Trajectory.h"
#include "cartesian_trajectory_data.h"
#include "cartesian_trajectory_rtwutil.h"
#include "changeEndSegBreaks.h"
#include "ppval.h"
#include "quinticpolytraj.h"
#include "rt_nonfinite.h"
#include "trapveltraj.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
Trajectory::Trajectory()
{
  omp_init_nest_lock(&cartesian_trajectory_nestLockGlobal);
}

//
// Arguments    : void
// Return Type  : void
//
Trajectory::~Trajectory()
{
  omp_destroy_nest_lock(&cartesian_trajectory_nestLockGlobal);
}

//
// Validate input arguments
//
// Arguments    : const coder::array<double, 2U> &waypoints
//                const coder::array<double, 2U> &waypointTimes
//                double ts
//                const char trajType_data[]
//                const int trajType_size[2]
//                coder::array<double, 2U> &waypointVels
//                coder::array<double, 2U> &waypointAccels
//                coder::array<double, 2U> &xd
//                coder::array<double, 2U> &xd_vel
//                coder::array<double, 2U> &xdd
// Return Type  : void
//
void Trajectory::cartesian_trajectory(
    const coder::array<double, 2U> &waypoints,
    const coder::array<double, 2U> &waypointTimes, double ts,
    const char trajType_data[], const int trajType_size[2],
    coder::array<double, 2U> &waypointVels,
    coder::array<double, 2U> &waypointAccels, coder::array<double, 2U> &xd,
    coder::array<double, 2U> &xd_vel, coder::array<double, 2U> &xdd)
{
  static const char cv2[7]{'q', 'u', 'i', 'n', 't', 'i', 'c'};
  static const char cv1[5]{'c', 'u', 'b', 'i', 'c'};
  static const char cv[4]{'t', 'r', 'a', 'p'};
  coder::array<double, 3U> pp_coefs;
  coder::array<double, 2U> b;
  coder::array<double, 2U> b_tmp;
  coder::array<double, 2U> coefMat;
  coder::array<double, 2U> dCoeffs;
  coder::array<double, 2U> trajTimes;
  coder::array<double, 2U> waypointAccelTimes;
  coder::array<double, 1U> valueAtEnd;
  double coeffMat[12];
  double apnd;
  double d;
  double tmp1;
  double work_data;
  int dimSize;
  int exitg1;
  int i;
  int k;
  int ncols;
  boolean_T guard1;
  boolean_T result;
  // trajectory interpolation between waypoints
  //  INPUTS:
  //  waypoints: size = (3xn), row order x;y;z
  //  waypointTimes : timepoints at waypoints
  //  ts : sample time
  //  trajType : type of interpolation trajectory 'trap' , 'cubic' or 'quintic'
  //  possible values waypointVels: optional input (only for 'cubic' or
  //  'quintic') with default value to be chosen as same in the code
  //  waypointAccels: optional input (only for 'quintic' ) with default value to
  //  be chosen as same in the code
  // OUTPUTS: [xd,xd_vel,xdd]
  // xd: desired cartesian order x;y;z (3xn) positions between waypoints
  //  xd_vel: desired cartesian velocity (3xn)
  //  xdd : desired cartesian accelration (3xn)
  if (waypointVels.size(1) == 0) {
    waypointVels.set_size(3, waypoints.size(1));
    ncols = 3 * waypoints.size(1);
    for (i = 0; i < ncols; i++) {
      waypointVels[i] = 0.0;
    }
    //  Default velocities
  }
  if (waypointAccels.size(1) == 0) {
    waypointAccels.set_size(3, waypointVels.size(1));
    ncols = 3 * waypointVels.size(1);
    for (i = 0; i < ncols; i++) {
      waypointAccels[i] = 0.0;
    }
    //  Default accelerations
  }
  //  Calculate acceleration times if applicable
  dimSize = waypointTimes.size(1);
  if (waypointTimes.size(1) == 0) {
    b_tmp.set_size(1, 0);
  } else {
    ncols = waypointTimes.size(1) - 1;
    if (ncols > 1) {
      ncols = 1;
    }
    if (ncols < 1) {
      b_tmp.set_size(1, 0);
    } else {
      b_tmp.set_size(1, waypointTimes.size(1) - 1);
      if (waypointTimes.size(1) - 1 != 0) {
        work_data = waypointTimes[0];
        for (ncols = 2; ncols <= dimSize; ncols++) {
          tmp1 = waypointTimes[ncols - 1];
          d = tmp1;
          tmp1 -= work_data;
          work_data = d;
          b_tmp[ncols - 2] = tmp1;
        }
      }
    }
  }
  waypointAccelTimes.set_size(1, b_tmp.size(1));
  ncols = b_tmp.size(1);
  for (i = 0; i < ncols; i++) {
    waypointAccelTimes[i] = 0.25 * b_tmp[i];
  }
  //  Define trajectory sample times
  guard1 = false;
  if (std::isnan(ts)) {
    guard1 = true;
  } else {
    d = waypointTimes[waypointTimes.size(1) - 1];
    if (std::isnan(d)) {
      guard1 = true;
    } else if ((ts == 0.0) || ((d > 0.0) && (ts < 0.0)) ||
               ((d < 0.0) && (ts > 0.0))) {
      trajTimes.set_size(1, 0);
    } else if (std::isinf(d) && std::isinf(ts)) {
      trajTimes.set_size(1, 1);
      trajTimes[0] = rtNaN;
    } else if (std::isinf(ts)) {
      trajTimes.set_size(1, 1);
      trajTimes[0] = 0.0;
    } else if (std::floor(ts) == ts) {
      ncols = static_cast<int>(d / ts);
      trajTimes.set_size(1, ncols + 1);
      for (i = 0; i <= ncols; i++) {
        trajTimes[i] = ts * static_cast<double>(i);
      }
    } else {
      work_data = std::floor(d / ts + 0.5);
      apnd = work_data * ts;
      if (ts > 0.0) {
        tmp1 = apnd - d;
      } else {
        tmp1 = d - apnd;
      }
      if (std::abs(tmp1) < 4.4408920985006262E-16 * std::abs(d)) {
        work_data++;
        apnd = d;
      } else if (tmp1 > 0.0) {
        apnd = (work_data - 1.0) * ts;
      } else {
        work_data++;
      }
      if (work_data >= 0.0) {
        ncols = static_cast<int>(work_data);
      } else {
        ncols = 0;
      }
      trajTimes.set_size(1, ncols);
      if (ncols > 0) {
        trajTimes[0] = 0.0;
        if (ncols > 1) {
          trajTimes[ncols - 1] = apnd;
          dimSize = (ncols - 1) / 2;
          for (k = 0; k <= dimSize - 2; k++) {
            work_data = (static_cast<double>(k) + 1.0) * ts;
            trajTimes[k + 1] = work_data;
            trajTimes[(ncols - k) - 2] = apnd - work_data;
          }
          if (dimSize << 1 == ncols - 1) {
            trajTimes[dimSize] = apnd / 2.0;
          } else {
            work_data = static_cast<double>(dimSize) * ts;
            trajTimes[dimSize] = work_data;
            trajTimes[dimSize + 1] = apnd - work_data;
          }
        }
      }
    }
  }
  if (guard1) {
    trajTimes.set_size(1, 1);
    trajTimes[0] = rtNaN;
  }
  //  Switch between trajectory types and generate the desired trajectory
  result = false;
  if (trajType_size[1] == 4) {
    ncols = 0;
    do {
      exitg1 = 0;
      if (ncols < 4) {
        if (cv[ncols] != trajType_data[ncols]) {
          exitg1 = 1;
        } else {
          ncols++;
        }
      } else {
        result = true;
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  if (result) {
    ncols = 0;
  } else {
    result = false;
    if (trajType_size[1] == 5) {
      ncols = 0;
      do {
        exitg1 = 0;
        if (ncols < 5) {
          if (cv1[ncols] != trajType_data[ncols]) {
            exitg1 = 1;
          } else {
            ncols++;
          }
        } else {
          result = true;
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
    if (result) {
      ncols = 1;
    } else {
      result = false;
      if (trajType_size[1] == 7) {
        ncols = 0;
        do {
          exitg1 = 0;
          if (ncols < 7) {
            if (cv2[ncols] != trajType_data[ncols]) {
              exitg1 = 1;
            } else {
              ncols++;
            }
          } else {
            result = true;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
      if (result) {
        ncols = 2;
      } else {
        ncols = -1;
      }
    }
  }
  switch (ncols) {
  case 0:
    b.set_size(3, waypointAccelTimes.size(1));
    ncols = waypointAccelTimes.size(1);
    coefMat.set_size(3, b_tmp.size(1));
    for (k = 0; k < ncols; k++) {
      dimSize = k * 3;
      b[dimSize] = waypointAccelTimes[k];
      coefMat[dimSize] = b_tmp[k];
      b[dimSize + 1] = waypointAccelTimes[k];
      coefMat[dimSize + 1] = b_tmp[k];
      b[dimSize + 2] = waypointAccelTimes[k];
      coefMat[dimSize + 2] = b_tmp[k];
    }
    coder::trapveltraj(waypoints, static_cast<double>(trajTimes.size(1)), b,
                       coefMat, xd, xd_vel, xdd);
    break;
  case 1: {
    double coeffVec_idx_0;
    double coeffVec_idx_1;
    double coeffVec_idx_2;
    xd.set_size(3, trajTimes.size(1));
    dimSize = 3 * trajTimes.size(1);
    xd_vel.set_size(3, trajTimes.size(1));
    xdd.set_size(3, trajTimes.size(1));
    for (i = 0; i < dimSize; i++) {
      xd[i] = 0.0;
      xd_vel[i] = 0.0;
      xdd[i] = 0.0;
    }
    i = (waypoints.size(1) - 1) * 3;
    coefMat.set_size(i, 4);
    ncols = i << 2;
    for (i = 0; i < ncols; i++) {
      coefMat[i] = 0.0;
    }
    i = waypoints.size(1);
    for (int b_i{0}; b_i <= i - 2; b_i++) {
      work_data = waypointTimes[b_i + 1] - waypointTimes[b_i];
      d = rt_powd_snf(work_data, 3.0);
      tmp1 = work_data * work_data;
      for (ncols = 0; ncols < 3; ncols++) {
        coeffVec_idx_0 = waypoints[ncols + 3 * b_i];
        coeffVec_idx_1 = waypointVels[ncols + 3 * b_i];
        apnd =
            waypoints[ncols + 3 * (static_cast<int>(
                                       (static_cast<double>(b_i) + 1.0) + 1.0) -
                                   1)] -
            (coeffVec_idx_0 + coeffVec_idx_1 * work_data);
        coeffVec_idx_2 =
            waypointVels[ncols +
                         3 * (static_cast<int>(
                                  (static_cast<double>(b_i) + 1.0) + 1.0) -
                              1)] -
            (0.0 * coeffVec_idx_0 + coeffVec_idx_1);
        dimSize =
            static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) * 3.0 +
                             (static_cast<double>(ncols) + 1.0));
        coefMat[dimSize - 1] = -2.0 / d * apnd + 1.0 / tmp1 * coeffVec_idx_2;
        coefMat[(dimSize + coefMat.size(0)) - 1] =
            3.0 / tmp1 * apnd + -1.0 / work_data * coeffVec_idx_2;
        coefMat[(dimSize + coefMat.size(0) * 2) - 1] = coeffVec_idx_1;
        coefMat[(dimSize + coefMat.size(0) * 3) - 1] = coeffVec_idx_0;
      }
    }
    std::memset(&coeffMat[0], 0, 12U * sizeof(double));
    for (i = 0; i < 3; i++) {
      coeffMat[i + 9] =
          ((coefMat[i] * 0.0 + coefMat[i + coefMat.size(0)] * 0.0) +
           coefMat[i + coefMat.size(0) * 2] * 0.0) +
          coefMat[i + coefMat.size(0) * 3];
    }
    dCoeffs.set_size(coefMat.size(0) + 3, 4);
    ncols = (coefMat.size(0) + 3) << 2;
    for (i = 0; i < ncols; i++) {
      dCoeffs[i] = 0.0;
    }
    for (i = 0; i < 4; i++) {
      dCoeffs[dCoeffs.size(0) * i] = coeffMat[3 * i];
      dCoeffs[dCoeffs.size(0) * i + 1] = coeffMat[3 * i + 1];
      dCoeffs[dCoeffs.size(0) * i + 2] = coeffMat[3 * i + 2];
    }
    if (static_cast<unsigned int>(coefMat.size(0)) + 3U < 4U) {
      i = 0;
      k = 0;
    } else {
      i = 3;
      k = static_cast<int>(static_cast<unsigned int>(coefMat.size(0)) + 3U);
    }
    ncols = k - i;
    for (k = 0; k < 4; k++) {
      for (dimSize = 0; dimSize < ncols; dimSize++) {
        dCoeffs[(i + dimSize) + dCoeffs.size(0) * k] =
            coefMat[dimSize + coefMat.size(0) * k];
      }
    }
    b_tmp.set_size(1, waypointTimes.size(1) + 1);
    b_tmp[0] = waypointTimes[0] - 1.0;
    ncols = waypointTimes.size(1);
    for (i = 0; i < ncols; i++) {
      b_tmp[i + 1] = waypointTimes[i];
    }
    apnd = b_tmp[b_tmp.size(1) - 1];
    work_data = apnd - b_tmp[b_tmp.size(1) - 2];
    coeffVec_idx_0 = rt_powd_snf(work_data, 3.0);
    coeffVec_idx_1 = rt_powd_snf(work_data, 2.0);
    coeffVec_idx_2 = rt_powd_snf(work_data, 1.0);
    work_data = rt_powd_snf(work_data, 0.0);
    if (dCoeffs.size(0) - 2 > dCoeffs.size(0)) {
      i = 0;
      k = 0;
    } else {
      i = dCoeffs.size(0) - 3;
      k = dCoeffs.size(0);
    }
    dimSize = k - i;
    valueAtEnd.set_size(dimSize);
    for (int b_i{0}; b_i < dimSize; b_i++) {
      k = dimSize + b_i;
      tmp1 = dCoeffs[(i + b_i % dimSize) + dCoeffs.size(0) * (b_i / dimSize)] *
                 coeffVec_idx_0 +
             dCoeffs[(i + k % dimSize) + dCoeffs.size(0) * (k / dimSize)] *
                 coeffVec_idx_1;
      k = (dimSize << 1) + b_i;
      tmp1 += dCoeffs[(i + k % dimSize) + dCoeffs.size(0) * (k / dimSize)] *
              coeffVec_idx_2;
      k = 3 * dimSize + b_i;
      tmp1 += dCoeffs[(i + k % dimSize) + dCoeffs.size(0) * (k / dimSize)] *
              work_data;
      valueAtEnd[b_i] = tmp1;
    }
    std::memset(&coeffMat[0], 0, 12U * sizeof(double));
    coeffMat[9] = valueAtEnd[0];
    coeffMat[10] = valueAtEnd[1];
    coeffMat[11] = valueAtEnd[2];
    coefMat.set_size(dCoeffs.size(0) + 3, 4);
    ncols = (dCoeffs.size(0) + 3) << 2;
    for (i = 0; i < ncols; i++) {
      coefMat[i] = 0.0;
    }
    ncols = dCoeffs.size(0);
    for (i = 0; i < 4; i++) {
      for (k = 0; k < ncols; k++) {
        coefMat[k + coefMat.size(0) * i] = dCoeffs[k + dCoeffs.size(0) * i];
      }
    }
    for (i = 0; i < 4; i++) {
      coefMat[dCoeffs.size(0) + coefMat.size(0) * i] = coeffMat[3 * i];
      coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 1] =
          coeffMat[3 * i + 1];
      coefMat[(dCoeffs.size(0) + coefMat.size(0) * i) + 2] =
          coeffMat[3 * i + 2];
    }
    waypointAccelTimes.set_size(1, b_tmp.size(1) + 1);
    ncols = b_tmp.size(1);
    for (i = 0; i < ncols; i++) {
      waypointAccelTimes[i] = b_tmp[i];
    }
    waypointAccelTimes[b_tmp.size(1)] = apnd + 1.0;
    pp_coefs.set_size(3, waypointAccelTimes.size(1) - 1, 4);
    ncols = (3 * (waypointAccelTimes.size(1) - 1)) << 2;
    for (i = 0; i < ncols; i++) {
      pp_coefs[i] = coefMat[i];
    }
    b_tmp.set_size(1, waypointAccelTimes.size(1));
    ncols = waypointAccelTimes.size(1);
    for (i = 0; i < ncols; i++) {
      b_tmp[i] = waypointAccelTimes[i];
    }
    coder::ppval(b_tmp, pp_coefs, trajTimes, b);
    ncols = trajTimes.size(1);
    for (i = 0; i < ncols; i++) {
      xd[3 * i] = b[3 * i];
      xd[3 * i + 1] = b[3 * i + 1];
      xd[3 * i + 2] = b[3 * i + 2];
    }
    coder::robotics::core::internal::changeEndSegBreaks(waypointAccelTimes,
                                                        trajTimes);
    dCoeffs.set_size(coefMat.size(0), 4);
    ncols = coefMat.size(0) << 2;
    for (i = 0; i < ncols; i++) {
      dCoeffs[i] = 0.0;
    }
    for (int b_i{0}; b_i < 3; b_i++) {
      ncols = dCoeffs.size(0);
      for (i = 0; i < ncols; i++) {
        dCoeffs[i + dCoeffs.size(0) * (b_i + 1)] =
            ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
            coefMat[i + coefMat.size(0) * b_i];
      }
    }
    pp_coefs.set_size(3, waypointAccelTimes.size(1) - 1, 4);
    dimSize = (3 * (waypointAccelTimes.size(1) - 1)) << 2;
    for (i = 0; i < dimSize; i++) {
      pp_coefs[i] = dCoeffs[i];
    }
    b_tmp.set_size(1, waypointAccelTimes.size(1));
    ncols = waypointAccelTimes.size(1);
    for (i = 0; i < ncols; i++) {
      b_tmp[i] = waypointAccelTimes[i];
    }
    coder::ppval(b_tmp, pp_coefs, trajTimes, b);
    ncols = trajTimes.size(1);
    for (i = 0; i < ncols; i++) {
      xd_vel[3 * i] = b[3 * i];
      xd_vel[3 * i + 1] = b[3 * i + 1];
      xd_vel[3 * i + 2] = b[3 * i + 2];
    }
    coefMat.set_size(dCoeffs.size(0), 4);
    ncols = dCoeffs.size(0) << 2;
    for (i = 0; i < ncols; i++) {
      coefMat[i] = 0.0;
    }
    for (int b_i{0}; b_i < 3; b_i++) {
      ncols = coefMat.size(0);
      for (i = 0; i < ncols; i++) {
        coefMat[i + coefMat.size(0) * (b_i + 1)] =
            ((4.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
            dCoeffs[i + dCoeffs.size(0) * b_i];
      }
    }
    pp_coefs.set_size(3, waypointAccelTimes.size(1) - 1, 4);
    for (i = 0; i < dimSize; i++) {
      pp_coefs[i] = coefMat[i];
    }
    b_tmp.set_size(1, waypointAccelTimes.size(1));
    ncols = waypointAccelTimes.size(1);
    for (i = 0; i < ncols; i++) {
      b_tmp[i] = waypointAccelTimes[i];
    }
    coder::ppval(b_tmp, pp_coefs, trajTimes, b);
    ncols = trajTimes.size(1);
    for (i = 0; i < ncols; i++) {
      xdd[3 * i] = b[3 * i];
      xdd[3 * i + 1] = b[3 * i + 1];
      xdd[3 * i + 2] = b[3 * i + 2];
    }
  } break;
  case 2:
    b_tmp.set_size(1, waypointTimes.size(1));
    ncols = waypointTimes.size(0) * waypointTimes.size(1) - 1;
    for (i = 0; i <= ncols; i++) {
      b_tmp[i] = waypointTimes[i];
    }
    coder::quinticpolytraj(waypoints, b_tmp, trajTimes, waypointVels,
                           waypointAccels, xd, xd_vel, xdd);
    break;
  }
}

//
// File trailer for Trajectory.cpp
//
// [EOF]
//
