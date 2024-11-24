//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: integrateRungeKutta.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

// Include Files
#include "integrateRungeKutta.h"
#include "div.h"
#include "ntrp45.h"
#include "rt_nonfinite.h"
#include "utilsMatlab_data.h"
#include "utilsMatlab_initialize.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static double rt_powd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

//
// INTEGRATERUNGEKUTTA Summary of this function goes here
//    Detailed explanation goes here
//
// Arguments    : const coder::array<double, 1U> &desired_joint_velocity_current
//                const double tspan[2]
//                const coder::array<double, 1U> &start_joint_Velocity
//                coder::array<double, 1U> &desired_joint_vector
// Return Type  : void
//
void integrateRungeKutta(
    const coder::array<double, 1U> &desired_joint_velocity_current,
    const double tspan[2], const coder::array<double, 1U> &start_joint_Velocity,
    coder::array<double, 1U> &desired_joint_vector)
{
  static const double x[21]{0.2,
                            0.075,
                            0.225,
                            0.97777777777777775,
                            -3.7333333333333334,
                            3.5555555555555554,
                            2.9525986892242035,
                            -11.595793324188385,
                            9.8228928516994358,
                            -0.29080932784636487,
                            2.8462752525252526,
                            -10.757575757575758,
                            8.9064227177434727,
                            0.27840909090909088,
                            -0.2735313036020583,
                            0.091145833333333329,
                            0.0,
                            0.44923629829290207,
                            0.65104166666666663,
                            -0.322376179245283,
                            0.13095238095238096};
  static const double B[7]{0.0012326388888888888,
                           0.0,
                           -0.0042527702905061394,
                           0.036979166666666667,
                           -0.05086379716981132,
                           0.0419047619047619,
                           -0.025};
  coder::array<double, 2U> f;
  coder::array<double, 2U> tout;
  coder::array<double, 2U> varargin_1;
  coder::array<double, 2U> varargout_2;
  coder::array<double, 2U> yout;
  coder::array<double, 2U> youtnew;
  coder::array<double, 1U> C;
  coder::array<double, 1U> y;
  coder::array<double, 1U> ynew;
  double absh;
  double absx;
  double d2;
  double hmax;
  double rh;
  double t;
  double tdir;
  double tfinal;
  int Bcolidx;
  int b_i;
  int chunk;
  int exponent;
  int i;
  int loop_ub;
  int n;
  int neq;
  int nout;
  boolean_T Done;
  boolean_T MinStepExit;
  if (!isInitialized_utilsMatlab) {
    utilsMatlab_initialize();
  }
  tfinal = tspan[1];
  neq = start_joint_Velocity.size(0);
  //  Since velocity is constant over small intervals
  if (static_cast<unsigned int>(start_joint_Velocity.size(0)) == 0U) {
    i = MAX_int32_T;
  } else {
    i = static_cast<int>(
        8192U / static_cast<unsigned int>(start_joint_Velocity.size(0)));
  }
  if (i + 4 >= 200) {
    chunk = 200;
  } else {
    chunk = i + 4;
  }
  tout.set_size(1, chunk);
  for (i = 0; i < chunk; i++) {
    tout[i] = 0.0;
  }
  yout.set_size(start_joint_Velocity.size(0), chunk);
  loop_ub = start_joint_Velocity.size(0) * chunk;
  for (i = 0; i < loop_ub; i++) {
    yout[i] = 0.0;
  }
  nout = 0;
  loop_ub = start_joint_Velocity.size(0);
  tdir = tspan[1] - tspan[0];
  rh = std::abs(tdir);
  d2 = std::abs(tspan[0]);
  hmax =
      std::fmin(rh, std::fmax(0.1 * rh, 3.5527136788005009E-15 *
                                            std::fmax(d2, std::abs(tspan[1]))));
  absh = std::fmin(hmax, rh);
  y.set_size(start_joint_Velocity.size(0));
  for (Bcolidx = 0; Bcolidx < loop_ub; Bcolidx++) {
    yout[Bcolidx] = start_joint_Velocity[Bcolidx];
    y[Bcolidx] = std::abs(start_joint_Velocity[Bcolidx]);
  }
  if (desired_joint_velocity_current.size(0) == y.size(0)) {
    y.set_size(desired_joint_velocity_current.size(0));
    loop_ub = desired_joint_velocity_current.size(0);
    for (i = 0; i < loop_ub; i++) {
      rh = y[i];
      y[i] = desired_joint_velocity_current[i] / std::fmax(rh, 0.001);
    }
  } else {
    binary_expand_op_1(y, desired_joint_velocity_current);
  }
  if (y.size(0) == 0) {
    rh = 0.0;
  } else {
    rh = 0.0;
    i = y.size(0);
    for (Bcolidx = 0; Bcolidx < i; Bcolidx++) {
      absx = std::abs(y[Bcolidx]);
      if (std::isnan(absx) || (absx > rh)) {
        rh = absx;
      }
    }
  }
  rh /= 0.20095091452076641;
  if (absh * rh > 1.0) {
    absh = 1.0 / rh;
  }
  if (std::isinf(d2) || std::isnan(d2)) {
    rh = rtNaN;
  } else if (d2 < 4.4501477170144028E-308) {
    rh = 4.94065645841247E-324;
  } else {
    std::frexp(d2, &n);
    rh = std::ldexp(1.0, n - 53);
  }
  absh = std::fmax(absh, 16.0 * rh);
  t = tspan[0];
  y.set_size(start_joint_Velocity.size(0));
  loop_ub = start_joint_Velocity.size(0);
  for (i = 0; i < loop_ub; i++) {
    y[i] = start_joint_Velocity[i];
  }
  f.set_size(start_joint_Velocity.size(0), 7);
  n = start_joint_Velocity.size(0) * 7;
  for (i = 0; i < n; i++) {
    f[i] = 0.0;
  }
  loop_ub = start_joint_Velocity.size(0);
  for (i = 0; i < loop_ub; i++) {
    f[i] = desired_joint_velocity_current[i];
  }
  if (!std::isnan(tdir)) {
    if (tdir < 0.0) {
      tdir = -1.0;
    } else {
      tdir = (tdir > 0.0);
    }
  }
  MinStepExit = false;
  Done = false;
  int exitg1;
  do {
    double hmin;
    double tnew;
    int outidx;
    boolean_T NoFailedAttempts;
    exitg1 = 0;
    absx = std::abs(t);
    if (std::isinf(absx) || std::isnan(absx)) {
      rh = rtNaN;
    } else if (absx < 4.4501477170144028E-308) {
      rh = 4.94065645841247E-324;
    } else {
      std::frexp(absx, &exponent);
      rh = std::ldexp(1.0, exponent - 53);
    }
    hmin = 16.0 * rh;
    absh = std::fmin(hmax, std::fmax(hmin, absh));
    absx = tdir * absh;
    d2 = tfinal - t;
    rh = std::abs(d2);
    if (1.1 * absh >= rh) {
      absx = d2;
      absh = rh;
      Done = true;
    }
    NoFailedAttempts = true;
    int exitg2;
    do {
      double mxerr;
      exitg2 = 0;
      Bcolidx = 5;
      for (int j{0}; j < 5; j++) {
        Bcolidx += j;
        loop_ub = f.size(0);
        for (i = 0; i < loop_ub; i++) {
          f[i + f.size(0) * (j + 1)] = desired_joint_velocity_current[i];
        }
      }
      tnew = t + absx;
      ynew.set_size(y.size(0));
      loop_ub = y.size(0);
      for (i = 0; i < loop_ub; i++) {
        ynew[i] = y[i];
      }
      if ((neq != 0) && (!(absx == 0.0))) {
        i = neq * 5 + 1;
        for (loop_ub = 1; neq < 0 ? loop_ub >= i : loop_ub <= i;
             loop_ub += neq) {
          rh = absx * x[Bcolidx];
          b_i = (loop_ub + neq) - 1;
          for (outidx = loop_ub; outidx <= b_i; outidx++) {
            n = outidx - loop_ub;
            ynew[n] = ynew[n] + f[outidx - 1] * rh;
          }
          Bcolidx++;
        }
      }
      loop_ub = f.size(0);
      for (i = 0; i < loop_ub; i++) {
        f[i + f.size(0) * 6] = desired_joint_velocity_current[i];
      }
      if (Done) {
        tnew = tfinal;
      }
      n = f.size(0);
      C.set_size(f.size(0));
      for (i = 0; i < n; i++) {
        rh = 0.0;
        for (Bcolidx = 0; Bcolidx < 7; Bcolidx++) {
          rh += f[Bcolidx * f.size(0) + i] * B[Bcolidx];
        }
        C[i] = rh;
      }
      n = C.size(0);
      mxerr = 0.0;
      for (Bcolidx = 0; Bcolidx < n; Bcolidx++) {
        rh = std::abs(C[Bcolidx]);
        absx = std::abs(y[Bcolidx]);
        d2 = std::abs(ynew[Bcolidx]);
        if ((absx > d2) || std::isnan(d2)) {
          if (absx > 0.001) {
            rh /= absx;
          } else {
            rh /= 0.001;
          }
        } else if (d2 > 0.001) {
          rh /= d2;
        } else {
          rh /= 0.001;
        }
        if ((rh > mxerr) || std::isnan(rh)) {
          mxerr = rh;
        }
      }
      absx = absh * mxerr;
      if (!(absx <= 0.001)) {
        if (absh <= hmin) {
          MinStepExit = true;
          exitg2 = 1;
        } else {
          if (NoFailedAttempts) {
            NoFailedAttempts = false;
            absh = std::fmax(
                hmin,
                absh * std::fmax(0.1, 0.8 * rt_powd_snf(0.001 / absx, 0.2)));
          } else {
            absh = std::fmax(hmin, 0.5 * absh);
          }
          absx = tdir * absh;
          Done = false;
        }
      } else {
        exitg2 = 1;
      }
    } while (exitg2 == 0);
    if (MinStepExit) {
      exitg1 = 1;
    } else {
      double toutnew[4];
      double tref[3];
      signed char input_sizes_idx_1;
      signed char sizes_idx_1;
      boolean_T empty_non_axis_sizes;
      outidx = nout + 1;
      rh = tnew - t;
      d2 = t + rh * 0.25;
      tref[0] = d2;
      toutnew[0] = d2;
      d2 = t + rh * 0.5;
      tref[1] = d2;
      toutnew[1] = d2;
      d2 = t + rh * 0.75;
      tref[2] = d2;
      toutnew[2] = d2;
      toutnew[3] = tnew;
      coder::ntrp45(tref, t, y, rh, f, varargin_1);
      if (varargin_1.size(0) != 0) {
        n = varargin_1.size(0);
      } else if (ynew.size(0) != 0) {
        n = ynew.size(0);
      } else {
        n = 0;
      }
      empty_non_axis_sizes = (n == 0);
      if (empty_non_axis_sizes || (varargin_1.size(0) != 0)) {
        input_sizes_idx_1 = 3;
      } else {
        input_sizes_idx_1 = 0;
      }
      if (empty_non_axis_sizes || (ynew.size(0) != 0)) {
        sizes_idx_1 = 1;
      } else {
        sizes_idx_1 = 0;
      }
      youtnew.set_size(n, input_sizes_idx_1 + sizes_idx_1);
      loop_ub = input_sizes_idx_1;
      for (i = 0; i < loop_ub; i++) {
        for (b_i = 0; b_i < n; b_i++) {
          youtnew[b_i + youtnew.size(0) * i] = varargin_1[b_i + n * i];
        }
      }
      loop_ub = sizes_idx_1;
      for (i = 0; i < loop_ub; i++) {
        for (b_i = 0; b_i < n; b_i++) {
          youtnew[b_i + youtnew.size(0) * input_sizes_idx_1] = ynew[b_i];
        }
      }
      nout += 4;
      if (nout + 1 > tout.size(1)) {
        n = tout.size(1);
        tout.set_size(1, tout.size(1));
        i = tout.size(1) + chunk;
        tout.set_size(tout.size(0), i);
        b_i = static_cast<unsigned char>(chunk);
        Bcolidx = yout.size(0);
        yout.set_size(yout.size(0), i);
        for (int j{0}; j < b_i; j++) {
          loop_ub = n + j;
          tout[loop_ub] = 0.0;
          for (i = 0; i < Bcolidx; i++) {
            yout[i + yout.size(0) * loop_ub] = 0.0;
          }
        }
      }
      for (Bcolidx = 0; Bcolidx < 4; Bcolidx++) {
        loop_ub = Bcolidx + outidx;
        tout[loop_ub] = toutnew[Bcolidx];
        for (int j{0}; j < neq; j++) {
          yout[j + yout.size(0) * loop_ub] =
              youtnew[j + youtnew.size(0) * Bcolidx];
        }
      }
      if (Done) {
        exitg1 = 1;
      } else {
        if (NoFailedAttempts) {
          rh = 1.25 * rt_powd_snf(absx / 0.001, 0.2);
          if (rh > 0.2) {
            absh /= rh;
          } else {
            absh *= 5.0;
          }
        }
        t = tnew;
        y.set_size(ynew.size(0));
        loop_ub = ynew.size(0);
        for (i = 0; i < loop_ub; i++) {
          y[i] = ynew[i];
        }
        ynew.set_size(f.size(0));
        loop_ub = f.size(0);
        for (i = 0; i < loop_ub; i++) {
          ynew[i] = f[i + f.size(0) * 6];
        }
        loop_ub = ynew.size(0);
        for (i = 0; i < loop_ub; i++) {
          f[i] = ynew[i];
        }
      }
    }
  } while (exitg1 == 0);
  if (nout + 1 < 1) {
    loop_ub = -1;
  } else {
    loop_ub = nout;
  }
  varargout_2.set_size(loop_ub + 1, yout.size(0));
  n = yout.size(0);
  for (i = 0; i < n; i++) {
    for (b_i = 0; b_i <= loop_ub; b_i++) {
      varargout_2[b_i + varargout_2.size(0) * i] = yout[i + yout.size(0) * b_i];
    }
  }
  //  The new joint positions are the last row of Y
  desired_joint_vector.set_size(varargout_2.size(1));
  loop_ub = varargout_2.size(1);
  for (i = 0; i < loop_ub; i++) {
    desired_joint_vector[i] =
        varargout_2[(varargout_2.size(0) + varargout_2.size(0) * i) - 1];
  }
}

//
// File trailer for integrateRungeKutta.cpp
//
// [EOF]
//
