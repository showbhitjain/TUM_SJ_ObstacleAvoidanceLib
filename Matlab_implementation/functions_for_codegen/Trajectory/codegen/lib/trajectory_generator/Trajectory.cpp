//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: Trajectory.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "Trajectory.h"
#include "colon.h"
#include "cubicpolytraj.h"
#include "find.h"
#include "ppval.h"
#include "quaternion.h"
#include "quinticpolytraj.h"
#include "rottraj.h"
#include "rt_nonfinite.h"
#include "trajectory_generator_data.h"
#include "trajectory_generator_rtwutil.h"
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
namespace TrajectoryGeneration {
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
// function [xd, xd_vel, xdd] = cartesian_trajectory(waypoints, waypointTimes,
// ts, trajType, waypointVels, waypointAccels)
//
// trajectory interpolation between waypoints
//  INPUTS:
//  waypoints: size = (3xn), row order x;y;z
//  waypointTimes : timepoints at waypoints
//  ts : sample time
//  trajType : type of interpolation trajectory 'trap' , 'cubic' or 'quintic'
//  possible values waypointVels: optional input (only for 'cubic' or 'quintic')
//  with default value to be chosen as same in the code waypointAccels: optional
//  input (only for 'quintic' ) with default value to be chosen as same in the
//  code
//
// Arguments    : const ::coder::array<double, 2U> &waypoints
//                const ::coder::array<double, 2U> &waypointTimes
//                double ts
//                const ::coder::array<char, 2U> &trajType
//                ::coder::array<double, 2U> &waypointVels
//                ::coder::array<double, 2U> &waypointAccels
//                ::coder::array<double, 2U> &xd
//                ::coder::array<double, 2U> &xd_vel
//                ::coder::array<double, 2U> &xdd
// Return Type  : void
//
void Trajectory::cartesian_trajectory(
    const ::coder::array<double, 2U> &waypoints,
    const ::coder::array<double, 2U> &waypointTimes, double ts,
    const ::coder::array<char, 2U> &trajType,
    ::coder::array<double, 2U> &waypointVels,
    ::coder::array<double, 2U> &waypointAccels, ::coder::array<double, 2U> &xd,
    ::coder::array<double, 2U> &xd_vel, ::coder::array<double, 2U> &xdd)
{
  static const char cv2[7]{'q', 'u', 'i', 'n', 't', 'i', 'c'};
  static const char cv1[5]{'c', 'u', 'b', 'i', 'c'};
  static const char cv[4]{'t', 'r', 'a', 'p'};
  ::coder::array<double, 3U> pp_coefs;
  ::coder::array<double, 2U> b;
  ::coder::array<double, 2U> b_tmp;
  ::coder::array<double, 2U> coefMat;
  ::coder::array<double, 2U> dCoeffs;
  ::coder::array<double, 2U> trajTimes;
  ::coder::array<double, 2U> waypointAccelTimes;
  ::coder::array<double, 1U> valueAtEnd;
  ::coder::array<boolean_T, 2U> b_trajTimes;
  double coeffMat[12];
  double d;
  double tmp1;
  double work_data;
  int dimSize;
  int exitg1;
  int i;
  int ncols;
  boolean_T guard1;
  boolean_T result;
  // OUTPUTS: [xd,xd_vel,xdd]
  // xd: desired cartesian order x;y;z (3xn) positions between waypoints
  //  xd_vel: desired cartesian velocity (3xn)
  //  xdd : desired cartesian accelration (3xn)
  //  Validate input arguments
  // 'cartesian_trajectory:18' if nargin < 5 || isempty(waypointVels)
  if (waypointVels.size(1) == 0) {
    // 'cartesian_trajectory:19' waypointVels = 0.1 * zeros(3, size(waypoints,
    // 2));
    waypointVels.set_size(3, waypoints.size(1));
    ncols = 3 * waypoints.size(1);
    for (i = 0; i < ncols; i++) {
      waypointVels[i] = 0.0;
    }
    //  Default velocities
  }
  // 'cartesian_trajectory:21' if nargin < 6 || isempty(waypointAccels)
  if (waypointAccels.size(1) == 0) {
    // 'cartesian_trajectory:22' waypointAccels = zeros(size(waypointVels));
    waypointAccels.set_size(3, waypointVels.size(1));
    ncols = 3 * waypointVels.size(1);
    for (i = 0; i < ncols; i++) {
      waypointAccels[i] = 0.0;
    }
    //  Default accelerations
  }
  //  Calculate acceleration times if applicable
  // 'cartesian_trajectory:26' waypointAccelTimes = 0.25 * diff(waypointTimes);
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
  // 'cartesian_trajectory:29' trajTimes = 0:ts:waypointTimes(end);
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
      coder::eml_float_colon(ts, d, trajTimes);
    }
  }
  if (guard1) {
    trajTimes.set_size(1, 1);
    trajTimes[0] = rtNaN;
  }
  // 'cartesian_trajectory:30' coder.varsize("xd",[3 Inf],[0 1]);
  // 'cartesian_trajectory:31' coder.varsize("xd_vel",[3 Inf],[0 1]);
  // 'cartesian_trajectory:32' coder.varsize("xdd",[3 Inf],[0 1]);
  //  Switch between trajectory types and generate the desired trajectory
  // 'cartesian_trajectory:36' switch trajType
  result = false;
  if (trajType.size(1) == 4) {
    ncols = 0;
    do {
      exitg1 = 0;
      if (ncols < 4) {
        if (cv[ncols] != trajType[ncols]) {
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
    if (trajType.size(1) == 5) {
      ncols = 0;
      do {
        exitg1 = 0;
        if (ncols < 5) {
          if (cv1[ncols] != trajType[ncols]) {
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
      if (trajType.size(1) == 7) {
        ncols = 0;
        do {
          exitg1 = 0;
          if (ncols < 7) {
            if (cv2[ncols] != trajType[ncols]) {
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
    // 'cartesian_trajectory:37' case 'trap'
    // 'cartesian_trajectory:38' [xd, xd_vel, xdd] = trapveltraj(waypoints,
    // numel(trajTimes), ... 'cartesian_trajectory:39' 'AccelTime',
    // repmat(waypointAccelTimes, [3, 1]), ... 'cartesian_trajectory:40'
    // 'EndTime', repmat(diff(waypointTimes), [3, 1]));
    b.set_size(3, waypointAccelTimes.size(1));
    ncols = waypointAccelTimes.size(1);
    coefMat.set_size(3, b_tmp.size(1));
    for (int jcol{0}; jcol < ncols; jcol++) {
      dimSize = jcol * 3;
      b[dimSize] = waypointAccelTimes[jcol];
      coefMat[dimSize] = b_tmp[jcol];
      b[dimSize + 1] = waypointAccelTimes[jcol];
      coefMat[dimSize + 1] = b_tmp[jcol];
      b[dimSize + 2] = waypointAccelTimes[jcol];
      coefMat[dimSize + 2] = b_tmp[jcol];
    }
    coder::trapveltraj(waypoints, static_cast<double>(trajTimes.size(1)), b,
                       coefMat, xd, xd_vel, xdd);
    break;
  case 1: {
    double coeffVec_idx_0;
    double coeffVec_idx_1;
    double coeffVec_idx_2;
    double waypoints_idx_0;
    int tGreaterThanTfIdx_size[2];
    int jcol;
    // 'cartesian_trajectory:41' case 'cubic'
    // 'cartesian_trajectory:42' [xd, xd_vel, xdd] = cubicpolytraj(waypoints,
    // waypointTimes, trajTimes, ... 'cartesian_trajectory:43'
    // 'VelocityBoundaryCondition', waypointVels);
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
        waypoints_idx_0 =
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
        coefMat[dimSize - 1] =
            -2.0 / d * waypoints_idx_0 + 1.0 / tmp1 * coeffVec_idx_2;
        coefMat[(dimSize + coefMat.size(0)) - 1] =
            3.0 / tmp1 * waypoints_idx_0 + -1.0 / work_data * coeffVec_idx_2;
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
      jcol = 0;
    } else {
      i = 3;
      jcol = static_cast<int>(static_cast<unsigned int>(coefMat.size(0)) + 3U);
    }
    ncols = jcol - i;
    for (jcol = 0; jcol < 4; jcol++) {
      for (dimSize = 0; dimSize < ncols; dimSize++) {
        dCoeffs[(i + dimSize) + dCoeffs.size(0) * jcol] =
            coefMat[dimSize + coefMat.size(0) * jcol];
      }
    }
    b_tmp.set_size(1, waypointTimes.size(1) + 1);
    b_tmp[0] = waypointTimes[0] - 1.0;
    ncols = waypointTimes.size(1);
    for (i = 0; i < ncols; i++) {
      b_tmp[i + 1] = waypointTimes[i];
    }
    waypoints_idx_0 = b_tmp[b_tmp.size(1) - 1];
    work_data = waypoints_idx_0 - b_tmp[b_tmp.size(1) - 2];
    coeffVec_idx_0 = rt_powd_snf(work_data, 3.0);
    coeffVec_idx_1 = rt_powd_snf(work_data, 2.0);
    coeffVec_idx_2 = rt_powd_snf(work_data, 1.0);
    work_data = rt_powd_snf(work_data, 0.0);
    if (dCoeffs.size(0) - 2 > dCoeffs.size(0)) {
      i = 0;
      jcol = 0;
    } else {
      i = dCoeffs.size(0) - 3;
      jcol = dCoeffs.size(0);
    }
    dimSize = jcol - i;
    valueAtEnd.set_size(dimSize);
    for (int b_i{0}; b_i < dimSize; b_i++) {
      jcol = dimSize + b_i;
      tmp1 =
          dCoeffs[(i + b_i % dimSize) + dCoeffs.size(0) * (b_i / dimSize)] *
              coeffVec_idx_0 +
          dCoeffs[(i + jcol % dimSize) + dCoeffs.size(0) * (jcol / dimSize)] *
              coeffVec_idx_1;
      jcol = (dimSize << 1) + b_i;
      tmp1 +=
          dCoeffs[(i + jcol % dimSize) + dCoeffs.size(0) * (jcol / dimSize)] *
          coeffVec_idx_2;
      jcol = 3 * dimSize + b_i;
      tmp1 +=
          dCoeffs[(i + jcol % dimSize) + dCoeffs.size(0) * (jcol / dimSize)] *
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
      for (jcol = 0; jcol < ncols; jcol++) {
        coefMat[jcol + coefMat.size(0) * i] =
            dCoeffs[jcol + dCoeffs.size(0) * i];
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
    waypointAccelTimes[b_tmp.size(1)] = waypoints_idx_0 + 1.0;
    pp_coefs.set_size(3, waypointAccelTimes.size(1) - 1, 4);
    dimSize = (3 * (waypointAccelTimes.size(1) - 1)) << 2;
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
    work_data = 0.01;
    tmp1 = waypointAccelTimes[waypointAccelTimes.size(1) - 2];
    b_trajTimes.set_size(1, trajTimes.size(1));
    for (i = 0; i < ncols; i++) {
      xd[3 * i] = b[3 * i];
      xd[3 * i + 1] = b[3 * i + 1];
      xd[3 * i + 2] = b[3 * i + 2];
      b_trajTimes[i] = (trajTimes[i] > tmp1);
    }
    coder::eml_find(b_trajTimes, (int *)&ncols, tGreaterThanTfIdx_size);
    if (tGreaterThanTfIdx_size[1] != 0) {
      work_data = std::fmin((trajTimes[ncols - 1] - tmp1) / 2.0, 0.01);
    }
    waypointAccelTimes[waypointAccelTimes.size(1) - 2] = tmp1 + work_data;
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
    // 'cartesian_trajectory:44' case 'quintic'
    // 'cartesian_trajectory:45' [xd, xd_vel, xdd] = quinticpolytraj(waypoints,
    // waypointTimes, trajTimes, ... 'cartesian_trajectory:46'
    // 'VelocityBoundaryCondition', waypointVels, ... 'cartesian_trajectory:47'
    // 'AccelerationBoundaryCondition', waypointAccels);
    b_tmp.set_size(1, waypointTimes.size(1));
    ncols = waypointTimes.size(0) * waypointTimes.size(1) - 1;
    for (i = 0; i <= ncols; i++) {
      b_tmp[i] = waypointTimes[i];
    }
    coder::quinticpolytraj(waypoints, b_tmp, trajTimes, waypointVels,
                           waypointAccels, xd, xd_vel, xdd);
    break;
  default:
    // 'cartesian_trajectory:48' otherwise
    // 'cartesian_trajectory:49' error('Invalid trajectory type! Use ''trap'',
    // ''cubic'', ''quintic'', or ''bspline''');
    break;
  }
}

//
// function [desired_quaternions,desired_angular_velocity,desired_angular_accel]
// = orientation_trajectory(orientations,waypointTimes,ts,trajType)
//
// UNTITLED Summary of this function goes here
//    Detailed explanation goes here
//
// Arguments    : const ::coder::array<double, 2U> &orientations
//                const ::coder::array<double, 2U> &waypointTimes
//                double ts
//                const ::coder::array<char, 2U> &trajType
//                ::coder::array<double, 2U> &desired_quaternions
//                ::coder::array<double, 2U> &desired_angular_velocity
//                ::coder::array<double, 2U> &desired_angular_accel
// Return Type  : void
//
void Trajectory::orientation_trajectory(
    const ::coder::array<double, 2U> &orientations,
    const ::coder::array<double, 2U> &waypointTimes, double ts,
    const ::coder::array<char, 2U> &trajType,
    ::coder::array<double, 2U> &desired_quaternions,
    ::coder::array<double, 2U> &desired_angular_velocity,
    ::coder::array<double, 2U> &desired_angular_accel)
{
  static const char cv2[7]{'q', 'u', 'i', 'n', 't', 'i', 'c'};
  static const char cv1[5]{'c', 'u', 'b', 'i', 'c'};
  static const char cv[4]{'t', 'r', 'a', 'p'};
  coder::b_quaternion R;
  coder::quaternion R0;
  coder::quaternion Rf;
  ::coder::array<double, 2U> alpha;
  ::coder::array<double, 2U> b_R;
  ::coder::array<double, 2U> b_s;
  ::coder::array<double, 2U> omega;
  ::coder::array<double, 2U> s;
  ::coder::array<double, 2U> sd;
  ::coder::array<double, 2U> sdd;
  ::coder::array<double, 2U> trajTimes_rot;
  double d;
  int i;
  int loop_ub;
  int loop_ub_tmp;
  int nm1d2;
  boolean_T b;
  //  Define trajectory sample times
  // 'orientation_trajectory:9' trajTimes = 0:ts:waypointTimes(end);
  b = std::isnan(ts);
  if (b) {
    nm1d2 = 1;
  } else {
    d = waypointTimes[waypointTimes.size(1) - 1];
    if (std::isnan(d)) {
      nm1d2 = 1;
    } else if ((ts == 0.0) || ((d > 0.0) && (ts < 0.0)) ||
               ((d < 0.0) && (ts > 0.0))) {
      nm1d2 = 0;
    } else if (std::isinf(d) && std::isinf(ts)) {
      nm1d2 = 1;
    } else if (std::isinf(ts)) {
      nm1d2 = 1;
    } else if (std::floor(ts) == ts) {
      nm1d2 = static_cast<int>(d / ts) + 1;
    } else {
      coder::eml_float_colon(ts, waypointTimes[waypointTimes.size(1) - 1],
                             trajTimes_rot);
      nm1d2 = trajTimes_rot.size(1);
    }
  }
  // 'orientation_trajectory:11' desired_quaternions =
  // zeros(4,numel(trajTimes));
  desired_quaternions.set_size(4, nm1d2);
  loop_ub = nm1d2 << 2;
  for (i = 0; i < loop_ub; i++) {
    desired_quaternions[i] = 0.0;
  }
  // 'orientation_trajectory:12' desired_angular_velocity =
  // zeros(3,numel(trajTimes));
  desired_angular_velocity.set_size(3, nm1d2);
  loop_ub_tmp = 3 * nm1d2;
  // 'orientation_trajectory:13' desired_angular_accel =
  // zeros(3,numel(trajTimes));
  desired_angular_accel.set_size(3, nm1d2);
  for (i = 0; i < loop_ub_tmp; i++) {
    desired_angular_velocity[i] = 0.0;
    desired_angular_accel[i] = 0.0;
  }
  // 'orientation_trajectory:14' numWaypoints = size(orientations,2);
  //  Loop through trajectory generation
  // 'orientation_trajectory:18' for w = 1:numWaypoints-1
  i = orientations.size(1);
  for (int w{0}; w <= i - 2; w++) {
    double timeInterval[2];
    double c_data_idx_0;
    double c_data_idx_1;
    double cdiff;
    double ndbl;
    double s_data_idx_0;
    int exitg1;
    int i1;
    boolean_T result;
    // 'orientation_trajectory:20' if size(orientations,1) == 3
    if (orientations.size(0) == 3) {
      double c_data_idx_2;
      double s_data_idx_1;
      // 'orientation_trajectory:21' R0 =
      // quaternion(eul2quat(orientations(:,w)'));
      d = orientations[orientations.size(0) * w] / 2.0;
      c_data_idx_0 = std::cos(d);
      d = std::sin(d);
      s_data_idx_0 = d;
      d = orientations[orientations.size(0) * w + 1] / 2.0;
      c_data_idx_1 = std::cos(d);
      d = std::sin(d);
      s_data_idx_1 = d;
      d = orientations[orientations.size(0) * w + 2] / 2.0;
      c_data_idx_2 = std::cos(d);
      d = std::sin(d);
      ndbl = c_data_idx_0 * c_data_idx_1;
      cdiff = s_data_idx_0 * s_data_idx_1;
      R0.a = ndbl * c_data_idx_2 + cdiff * d;
      R0.b = ndbl * d - cdiff * c_data_idx_2;
      ndbl = s_data_idx_0 * c_data_idx_1;
      cdiff = c_data_idx_0 * s_data_idx_1;
      R0.c = cdiff * c_data_idx_2 + ndbl * d;
      R0.d = ndbl * c_data_idx_2 - cdiff * d;
      // 'orientation_trajectory:22' Rf =
      // quaternion(eul2quat(orientations(:,w+1)'));
      d = orientations[orientations.size(0) * (w + 1)] / 2.0;
      c_data_idx_0 = std::cos(d);
      d = std::sin(d);
      s_data_idx_0 = d;
      d = orientations[orientations.size(0) * (w + 1) + 1] / 2.0;
      c_data_idx_1 = std::cos(d);
      d = std::sin(d);
      s_data_idx_1 = d;
      d = orientations[orientations.size(0) * (w + 1) + 2] / 2.0;
      c_data_idx_2 = std::cos(d);
      d = std::sin(d);
      ndbl = c_data_idx_0 * c_data_idx_1;
      cdiff = s_data_idx_0 * s_data_idx_1;
      Rf.a = ndbl * c_data_idx_2 + cdiff * d;
      Rf.b = ndbl * d - cdiff * c_data_idx_2;
      ndbl = s_data_idx_0 * c_data_idx_1;
      cdiff = c_data_idx_0 * s_data_idx_1;
      Rf.c = cdiff * c_data_idx_2 + ndbl * d;
      Rf.d = ndbl * c_data_idx_2 - cdiff * d;
    } else if (orientations.size(0) == 4) {
      // 'orientation_trajectory:23' elseif size(orientations,1) == 4
      // 'orientation_trajectory:24' R0 = quaternion(orientations(:,w)')  ;
      R0.a = orientations[orientations.size(0) * w];
      R0.b = orientations[orientations.size(0) * w + 1];
      R0.c = orientations[orientations.size(0) * w + 2];
      R0.d = orientations[orientations.size(0) * w + 3];
      // 'orientation_trajectory:25' Rf = quaternion(orientations(:,w+1)');
      Rf.a = orientations[orientations.size(0) * (w + 1)];
      Rf.b = orientations[orientations.size(0) * (w + 1) + 1];
      Rf.c = orientations[orientations.size(0) * (w + 1) + 2];
      Rf.d = orientations[orientations.size(0) * (w + 1) + 3];
    } else {
      // 'orientation_trajectory:26' else
      // 'orientation_trajectory:27' error('Invalid Orientation')
    }
    // 'orientation_trajectory:29' timeInterval = waypointTimes(w:w+1);
    timeInterval[0] = waypointTimes[w];
    timeInterval[1] = waypointTimes[w + 1];
    //  Recalculate trajectory times for the segment
    // 'orientation_trajectory:32' trajTimes_rot =
    // timeInterval(1):ts:timeInterval(2);
    c_data_idx_0 = waypointTimes[w];
    c_data_idx_1 =
        waypointTimes[static_cast<int>(static_cast<unsigned int>(w) + 1U)];
    if (std::isnan(c_data_idx_0) || b || std::isnan(c_data_idx_1)) {
      trajTimes_rot.set_size(1, 1);
      trajTimes_rot[0] = rtNaN;
    } else if ((ts == 0.0) ||
               ((waypointTimes[w] < c_data_idx_1) && (ts < 0.0)) ||
               ((c_data_idx_1 < waypointTimes[w]) && (ts > 0.0))) {
      trajTimes_rot.set_size(1, 0);
    } else if ((std::isinf(c_data_idx_0) || std::isinf(c_data_idx_1)) &&
               (std::isinf(ts) || (waypointTimes[w] == c_data_idx_1))) {
      trajTimes_rot.set_size(1, 1);
      trajTimes_rot[0] = rtNaN;
    } else if (std::isinf(ts)) {
      trajTimes_rot.set_size(1, 1);
      trajTimes_rot[0] = waypointTimes[w];
    } else if ((std::floor(c_data_idx_0) == waypointTimes[w]) &&
               (std::floor(ts) == ts)) {
      d = waypointTimes[w];
      loop_ub = static_cast<int>((c_data_idx_1 - d) / ts);
      trajTimes_rot.set_size(1, loop_ub + 1);
      for (nm1d2 = 0; nm1d2 <= loop_ub; nm1d2++) {
        trajTimes_rot[nm1d2] = d + ts * static_cast<double>(nm1d2);
      }
    } else {
      c_data_idx_0 = waypointTimes[w];
      ndbl = std::floor((c_data_idx_1 - c_data_idx_0) / ts + 0.5);
      s_data_idx_0 = c_data_idx_0 + ndbl * ts;
      if (ts > 0.0) {
        cdiff = s_data_idx_0 - c_data_idx_1;
      } else {
        cdiff = c_data_idx_1 - s_data_idx_0;
      }
      if (std::abs(cdiff) <
          4.4408920985006262E-16 *
              std::fmax(std::abs(c_data_idx_0), std::abs(c_data_idx_1))) {
        ndbl++;
        s_data_idx_0 = c_data_idx_1;
      } else if (cdiff > 0.0) {
        s_data_idx_0 = c_data_idx_0 + (ndbl - 1.0) * ts;
      } else {
        ndbl++;
      }
      if (ndbl >= 0.0) {
        loop_ub_tmp = static_cast<int>(ndbl);
      } else {
        loop_ub_tmp = 0;
      }
      trajTimes_rot.set_size(1, loop_ub_tmp);
      if (loop_ub_tmp > 0) {
        trajTimes_rot[0] = c_data_idx_0;
        if (loop_ub_tmp > 1) {
          trajTimes_rot[loop_ub_tmp - 1] = s_data_idx_0;
          nm1d2 = (loop_ub_tmp - 1) / 2;
          for (loop_ub = 0; loop_ub <= nm1d2 - 2; loop_ub++) {
            cdiff = (static_cast<double>(loop_ub) + 1.0) * ts;
            trajTimes_rot[loop_ub + 1] = c_data_idx_0 + cdiff;
            trajTimes_rot[(loop_ub_tmp - loop_ub) - 2] = s_data_idx_0 - cdiff;
          }
          if (nm1d2 << 1 == loop_ub_tmp - 1) {
            trajTimes_rot[nm1d2] = (c_data_idx_0 + s_data_idx_0) / 2.0;
          } else {
            cdiff = static_cast<double>(nm1d2) * ts;
            trajTimes_rot[nm1d2] = c_data_idx_0 + cdiff;
            trajTimes_rot[nm1d2 + 1] = s_data_idx_0 - cdiff;
          }
        }
      }
    }
    // 'orientation_trajectory:33' num_samples = round((timeInterval(2) -
    // timeInterval(1))/ts) + 1;
    //  Use appropriate trajectory generation method
    // 'orientation_trajectory:36' switch trajType
    result = false;
    if (trajType.size(1) == 4) {
      loop_ub_tmp = 0;
      do {
        exitg1 = 0;
        if (loop_ub_tmp < 4) {
          if (cv[loop_ub_tmp] != trajType[loop_ub_tmp]) {
            exitg1 = 1;
          } else {
            loop_ub_tmp++;
          }
        } else {
          result = true;
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
    if (result) {
      loop_ub_tmp = 0;
    } else {
      result = false;
      if (trajType.size(1) == 5) {
        loop_ub_tmp = 0;
        do {
          exitg1 = 0;
          if (loop_ub_tmp < 5) {
            if (cv1[loop_ub_tmp] != trajType[loop_ub_tmp]) {
              exitg1 = 1;
            } else {
              loop_ub_tmp++;
            }
          } else {
            result = true;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
      if (result) {
        loop_ub_tmp = 1;
      } else {
        result = false;
        if (trajType.size(1) == 7) {
          loop_ub_tmp = 0;
          do {
            exitg1 = 0;
            if (loop_ub_tmp < 7) {
              if (cv2[loop_ub_tmp] != trajType[loop_ub_tmp]) {
                exitg1 = 1;
              } else {
                loop_ub_tmp++;
              }
            } else {
              result = true;
              exitg1 = 1;
            }
          } while (exitg1 == 0);
        }
        if (result) {
          loop_ub_tmp = 2;
        } else {
          loop_ub_tmp = -1;
        }
      }
    }
    switch (loop_ub_tmp) {
    case 0:
      // 'orientation_trajectory:37' case 'trap'
      // 'orientation_trajectory:38' [s,sd,sdd] = trapveltraj([0 1],num_samples,
      // ... 'orientation_trajectory:39' 'AccelTime',diff(timeInterval)/3, ...
      // 'orientation_trajectory:40' 'EndTime',diff(timeInterval)*2/3);
      d = c_data_idx_1 - waypointTimes[w];
      coder::trapveltraj(std::round(d / ts) + 1.0, d / 3.0, d * 2.0 / 3.0, s,
                         sd, sdd);
      break;
    case 1:
      // 'orientation_trajectory:41' case 'cubic'
      // 'orientation_trajectory:42' [s,sd,sdd] = cubicpolytraj([0
      // 1],timeInterval,trajTimes_rot);
      coder::cubicpolytraj(timeInterval, trajTimes_rot, s, sd, sdd);
      break;
    case 2:
      // 'orientation_trajectory:43' case 'quintic'
      // 'orientation_trajectory:44' [s,sd,sdd] = quinticpolytraj([0
      // 1],timeInterval,trajTimes_rot);
      coder::quinticpolytraj(timeInterval, trajTimes_rot, s, sd, sdd);
      break;
    default:
      // 'orientation_trajectory:45' otherwise
      // 'orientation_trajectory:46' error('Invalid trajectory type! Use
      // ''trap'', ''cubic'', or ''quintic''');
      break;
    }
    //  Generate the rotation trajectory
    // 'orientation_trajectory:50' [R, omega, alpha] = rottraj(R0, Rf,
    // timeInterval, trajTimes_rot, 'TimeScaling', [s; sd; sdd]);
    loop_ub = s.size(1);
    b_s.set_size(3, s.size(1));
    for (nm1d2 = 0; nm1d2 < loop_ub; nm1d2++) {
      b_s[3 * nm1d2] = s[nm1d2];
    }
    loop_ub = sd.size(1);
    for (nm1d2 = 0; nm1d2 < loop_ub; nm1d2++) {
      b_s[3 * nm1d2 + 1] = sd[nm1d2];
    }
    loop_ub = sdd.size(1);
    for (nm1d2 = 0; nm1d2 < loop_ub; nm1d2++) {
      b_s[3 * nm1d2 + 2] = sdd[nm1d2];
    }
    coder::rottraj(R0, Rf, trajTimes_rot, b_s, R, omega, alpha);
    //  Calculate start and end indices for the full trajectory
    // 'orientation_trajectory:53' start_idx = round(timeInterval(1)/ts) + 1;
    ndbl = std::round(waypointTimes[w] / ts);
    // 'orientation_trajectory:54' end_idx = round(timeInterval(2)/ts) + 1;
    cdiff = std::round(c_data_idx_1 / ts);
    //  Insert data
    // 'orientation_trajectory:57' desired_quaternions(:, start_idx:end_idx) =
    // compact(R)';
    if (ndbl + 1.0 > cdiff + 1.0) {
      nm1d2 = 0;
      i1 = 0;
    } else {
      nm1d2 = static_cast<int>(ndbl + 1.0) - 1;
      i1 = static_cast<int>(cdiff + 1.0);
    }
    loop_ub = R.a.size(0);
    b_R.set_size(4, R.a.size(0));
    for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
      b_R[4 * loop_ub_tmp] = R.a[loop_ub_tmp];
    }
    loop_ub = R.b.size(0);
    for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
      b_R[4 * loop_ub_tmp + 1] = R.b[loop_ub_tmp];
    }
    loop_ub = R.c.size(0);
    for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
      b_R[4 * loop_ub_tmp + 2] = R.c[loop_ub_tmp];
    }
    loop_ub = R.d.size(0);
    for (loop_ub_tmp = 0; loop_ub_tmp < loop_ub; loop_ub_tmp++) {
      b_R[4 * loop_ub_tmp + 3] = R.d[loop_ub_tmp];
    }
    loop_ub = i1 - nm1d2;
    for (i1 = 0; i1 < loop_ub; i1++) {
      loop_ub_tmp = nm1d2 + i1;
      desired_quaternions[4 * loop_ub_tmp] = b_R[4 * i1];
      desired_quaternions[4 * loop_ub_tmp + 1] = b_R[4 * i1 + 1];
      desired_quaternions[4 * loop_ub_tmp + 2] = b_R[4 * i1 + 2];
      desired_quaternions[4 * loop_ub_tmp + 3] = b_R[4 * i1 + 3];
    }
    // 'orientation_trajectory:58' desired_angular_velocity(:,
    // start_idx:end_idx) = omega;
    if (ndbl + 1.0 > cdiff + 1.0) {
      nm1d2 = 0;
      i1 = 0;
    } else {
      nm1d2 = static_cast<int>(ndbl + 1.0) - 1;
      i1 = static_cast<int>(cdiff + 1.0);
    }
    loop_ub = i1 - nm1d2;
    for (i1 = 0; i1 < loop_ub; i1++) {
      loop_ub_tmp = nm1d2 + i1;
      desired_angular_velocity[3 * loop_ub_tmp] = omega[3 * i1];
      desired_angular_velocity[3 * loop_ub_tmp + 1] = omega[3 * i1 + 1];
      desired_angular_velocity[3 * loop_ub_tmp + 2] = omega[3 * i1 + 2];
    }
    // 'orientation_trajectory:59' desired_angular_accel(:, start_idx:end_idx) =
    // alpha;
    if (ndbl + 1.0 > cdiff + 1.0) {
      nm1d2 = 0;
      i1 = 0;
    } else {
      nm1d2 = static_cast<int>(ndbl + 1.0) - 1;
      i1 = static_cast<int>(cdiff + 1.0);
    }
    loop_ub = i1 - nm1d2;
    for (i1 = 0; i1 < loop_ub; i1++) {
      loop_ub_tmp = nm1d2 + i1;
      desired_angular_accel[3 * loop_ub_tmp] = alpha[3 * i1];
      desired_angular_accel[3 * loop_ub_tmp + 1] = alpha[3 * i1 + 1];
      desired_angular_accel[3 * loop_ub_tmp + 2] = alpha[3 * i1 + 2];
    }
  }
}

} // namespace TrajectoryGeneration

//
// File trailer for Trajectory.cpp
//
// [EOF]
//
