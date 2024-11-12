//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: trapveltraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

// Include Files
#include "trapveltraj.h"
#include "linspace.h"
#include "ppval.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "trajectory_generator_internal_types.h"
#include "trajectory_generator_rtwutil.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Type Definitions
namespace TrajectoryGeneration {
struct cell_wrap_24 {
  double f1[9];
};

struct cell_wrap_4 {
  ::coder::array<double, 2U> f1;
};

struct cell_wrap_5 {
  ::coder::array<double, 2U> f1;
};

} // namespace TrajectoryGeneration

// Function Declarations
namespace TrajectoryGeneration {
namespace coder {
static double computeProfileParams(double i, double j,
                                   const ::coder::array<double, 2U> &wayPoints,
                                   const ::coder::array<double, 2U> &TFi,
                                   const ::coder::array<double, 2U> &TAc,
                                   double &aParam, double &tAParam,
                                   double &tFParam);

static void generateTrajectoriesFromCoefs(
    const ::coder::array<double, 2U> &breaks,
    const ::coder::array<double, 2U> &coeffs, double dim,
    const ::coder::array<double, 2U> &t, ::coder::array<double, 2U> &q,
    ::coder::array<double, 2U> &qd, ::coder::array<double, 2U> &qdd,
    ::coder::array<double, 2U> &pp_breaks,
    ::coder::array<double, 3U> &pp_coefs);

static void reformatInput(const ::coder::array<double, 2U> &input, double p,
                          ::coder::array<double, 2U> &matrixInput);

} // namespace coder
} // namespace TrajectoryGeneration

// Function Definitions
//
// Arguments    : double i
//                double j
//                const ::coder::array<double, 2U> &wayPoints
//                const ::coder::array<double, 2U> &TFi
//                const ::coder::array<double, 2U> &TAc
//                double &aParam
//                double &tAParam
//                double &tFParam
// Return Type  : double
//
namespace TrajectoryGeneration {
namespace coder {
static double computeProfileParams(double i, double j,
                                   const ::coder::array<double, 2U> &wayPoints,
                                   const ::coder::array<double, 2U> &TFi,
                                   const ::coder::array<double, 2U> &TAc,
                                   double &aParam, double &tAParam,
                                   double &tFParam)
{
  ::coder::array<double, 2U> TAcSwitch;
  ::coder::array<double, 2U> TFiSwitch;
  double b_sF;
  double s0;
  double sF;
  double vParam;
  int deltaSign;
  int loop_ub;
  boolean_T inputCombo_idx_2;
  boolean_T inputCombo_idx_3;
  s0 = wayPoints[(static_cast<int>(i) + 3 * (static_cast<int>(j) - 1)) - 1];
  sF = wayPoints[(static_cast<int>(i) + 3 * (static_cast<int>(j + 1.0) - 1)) -
                 1];
  deltaSign = 1;
  if (sF < s0) {
    b_sF = s0;
    s0 = sF;
    sF = b_sF;
    deltaSign = -1;
  }
  inputCombo_idx_2 = (TFi.size(1) != 0);
  inputCombo_idx_3 = (TAc.size(1) != 0);
  if (inputCombo_idx_2) {
    TFiSwitch.set_size(3, TFi.size(1));
    loop_ub = 3 * TFi.size(1);
    for (int b_i{0}; b_i < loop_ub; b_i++) {
      TFiSwitch[b_i] = TFi[b_i];
    }
  } else {
    TFiSwitch.set_size(static_cast<int>(i), static_cast<int>(j));
    loop_ub = static_cast<int>(i) * static_cast<int>(j);
    for (int b_i{0}; b_i < loop_ub; b_i++) {
      TFiSwitch[b_i] = 1.0;
    }
  }
  if (inputCombo_idx_3) {
    TAcSwitch.set_size(3, TAc.size(1));
    loop_ub = 3 * TAc.size(1);
    for (int b_i{0}; b_i < loop_ub; b_i++) {
      TAcSwitch[b_i] = TAc[b_i];
    }
  } else {
    TAcSwitch.set_size(static_cast<int>(i), static_cast<int>(j));
    loop_ub = static_cast<int>(i) * static_cast<int>(j);
    for (int b_i{0}; b_i < loop_ub; b_i++) {
      TAcSwitch[b_i] = 1.0;
    }
  }
  switch (inputCombo_idx_2 * 2 + inputCombo_idx_3) {
  case 3:
    tFParam = TFiSwitch[(static_cast<int>(i) +
                         TFiSwitch.size(0) * (static_cast<int>(j) - 1)) -
                        1];
    tAParam = TAcSwitch[(static_cast<int>(i) +
                         TAcSwitch.size(0) * (static_cast<int>(j) - 1)) -
                        1];
    vParam = (sF - s0) / (tFParam - tAParam);
    aParam = vParam / tAParam;
    break;
  case 2:
    tFParam = TFiSwitch[(static_cast<int>(i) +
                         TFiSwitch.size(0) * (static_cast<int>(j) - 1)) -
                        1];
    vParam = 1.5 * (sF - s0) / tFParam;
    tAParam = ((s0 - sF) + vParam * tFParam) / vParam;
    aParam = vParam / tAParam;
    break;
  case 1: {
    double aParam_tmp;
    tAParam = TAcSwitch[(static_cast<int>(i) +
                         TAcSwitch.size(0) * (static_cast<int>(j) - 1)) -
                        1];
    b_sF = sF - s0;
    aParam_tmp = tAParam * tAParam;
    aParam = b_sF / (2.0 * aParam_tmp);
    vParam = aParam * tAParam;
    tFParam = (b_sF + aParam * aParam_tmp) / vParam;
  } break;
  default:
    tFParam = 1.0;
    vParam = 1.5 * (sF - s0);
    tAParam = ((s0 - sF) + vParam) / vParam;
    aParam = vParam / tAParam;
    break;
  }
  if (s0 == sF) {
    aParam = 0.0;
    vParam = 0.0;
    if (std::isnan(tFParam) || (tFParam == 0.0)) {
      tFParam = 1.0;
    }
    tAParam = tFParam / 3.0;
  }
  vParam *= static_cast<double>(deltaSign);
  aParam *= static_cast<double>(deltaSign);
  return vParam;
}

//
// Arguments    : const ::coder::array<double, 2U> &breaks
//                const ::coder::array<double, 2U> &coeffs
//                double dim
//                const ::coder::array<double, 2U> &t
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
//                ::coder::array<double, 2U> &pp_breaks
//                ::coder::array<double, 3U> &pp_coefs
// Return Type  : void
//
static void generateTrajectoriesFromCoefs(
    const ::coder::array<double, 2U> &breaks,
    const ::coder::array<double, 2U> &coeffs, double dim,
    const ::coder::array<double, 2U> &t, ::coder::array<double, 2U> &q,
    ::coder::array<double, 2U> &qd, ::coder::array<double, 2U> &qdd,
    ::coder::array<double, 2U> &pp_breaks, ::coder::array<double, 3U> &pp_coefs)
{
  ::coder::array<double, 2U> breaksWithFlatStart;
  ::coder::array<double, 2U> dCoeffs;
  ::coder::array<double, 2U> ddCoeffs;
  ::coder::array<double, 2U> modBreaks;
  ::coder::array<double, 2U> modCoeffs;
  ::coder::array<double, 1U> valueAtEnd;
  d_struct_T b_expl_temp;
  d_struct_T c_expl_temp;
  d_struct_T expl_temp;
  double newSegmentCoeffs_data[9];
  double valueAtStart_data[3];
  double evalPointVector_idx_0;
  double evalPointVector_idx_1;
  double evalPointVector_idx_2;
  double holdPoint;
  double holdPoint_tmp;
  int b_loop_ub;
  int i;
  int i1;
  int loop_ub;
  int loop_ub_tmp;
  loop_ub_tmp = static_cast<int>(dim);
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < loop_ub_tmp; i1++) {
      newSegmentCoeffs_data[i1 + static_cast<int>(dim) * i] =
          coeffs[i1 + coeffs.size(0) * i];
    }
  }
  loop_ub = static_cast<int>(dim) * 3;
  for (int b_i{0}; b_i < loop_ub_tmp; b_i++) {
    valueAtStart_data[b_i] =
        (newSegmentCoeffs_data[b_i] * 0.0 +
         newSegmentCoeffs_data[static_cast<int>(dim) + b_i] * 0.0) +
        newSegmentCoeffs_data[(static_cast<int>(dim) << 1) + b_i];
  }
  if (loop_ub - 1 >= 0) {
    std::memset(&newSegmentCoeffs_data[0], 0,
                static_cast<unsigned int>(loop_ub) * sizeof(double));
  }
  for (i = 0; i < loop_ub_tmp; i++) {
    newSegmentCoeffs_data[i + static_cast<int>(dim) * 2] = valueAtStart_data[i];
  }
  holdPoint = static_cast<double>(coeffs.size(0)) + dim;
  dCoeffs.set_size(static_cast<int>(holdPoint), 3);
  b_loop_ub = static_cast<int>(holdPoint) * 3;
  for (i = 0; i < b_loop_ub; i++) {
    dCoeffs[i] = 0.0;
  }
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < loop_ub_tmp; i1++) {
      dCoeffs[i1 + dCoeffs.size(0) * i] =
          newSegmentCoeffs_data[i1 + static_cast<int>(dim) * i];
    }
  }
  if (dim + 1.0 > holdPoint) {
    i = 0;
    i1 = 0;
  } else {
    i = static_cast<int>(dim + 1.0) - 1;
    i1 = static_cast<int>(holdPoint);
  }
  b_loop_ub = i1 - i;
  for (i1 = 0; i1 < 3; i1++) {
    for (int b_i{0}; b_i < b_loop_ub; b_i++) {
      dCoeffs[(i + b_i) + dCoeffs.size(0) * i1] =
          coeffs[b_i + coeffs.size(0) * i1];
    }
  }
  breaksWithFlatStart.set_size(1, breaks.size(1) + 1);
  breaksWithFlatStart[0] = breaks[0] - 1.0;
  b_loop_ub = breaks.size(1);
  for (i = 0; i < b_loop_ub; i++) {
    breaksWithFlatStart[i + 1] = breaks[i];
  }
  holdPoint_tmp = breaksWithFlatStart[breaksWithFlatStart.size(1) - 1];
  holdPoint =
      holdPoint_tmp - breaksWithFlatStart[breaksWithFlatStart.size(1) - 2];
  evalPointVector_idx_0 = rt_powd_snf(holdPoint, 2.0);
  evalPointVector_idx_1 = rt_powd_snf(holdPoint, 1.0);
  evalPointVector_idx_2 = rt_powd_snf(holdPoint, 0.0);
  holdPoint = (static_cast<double>(dCoeffs.size(0)) - dim) + 1.0;
  if (holdPoint > dCoeffs.size(0)) {
    i = 0;
    i1 = 0;
  } else {
    i = static_cast<int>(holdPoint) - 1;
    i1 = dCoeffs.size(0);
  }
  b_loop_ub = i1 - i;
  valueAtEnd.set_size(b_loop_ub);
  for (int b_i{0}; b_i < b_loop_ub; b_i++) {
    i1 = b_loop_ub + b_i;
    holdPoint =
        dCoeffs[(i + b_i % b_loop_ub) + dCoeffs.size(0) * (b_i / b_loop_ub)] *
            evalPointVector_idx_0 +
        dCoeffs[(i + i1 % b_loop_ub) + dCoeffs.size(0) * (i1 / b_loop_ub)] *
            evalPointVector_idx_1;
    i1 = (b_loop_ub << 1) + b_i;
    holdPoint +=
        dCoeffs[(i + i1 % b_loop_ub) + dCoeffs.size(0) * (i1 / b_loop_ub)] *
        evalPointVector_idx_2;
    valueAtEnd[b_i] = holdPoint;
  }
  if (loop_ub - 1 >= 0) {
    std::memset(&newSegmentCoeffs_data[0], 0,
                static_cast<unsigned int>(loop_ub) * sizeof(double));
  }
  for (i = 0; i < loop_ub_tmp; i++) {
    newSegmentCoeffs_data[i + static_cast<int>(dim) * 2] = valueAtEnd[i];
  }
  holdPoint = static_cast<double>(dCoeffs.size(0)) + dim;
  modCoeffs.set_size(static_cast<int>(holdPoint), 3);
  loop_ub = static_cast<int>(holdPoint) * 3;
  for (i = 0; i < loop_ub; i++) {
    modCoeffs[i] = 0.0;
  }
  loop_ub = dCoeffs.size(0);
  for (i = 0; i < 3; i++) {
    for (i1 = 0; i1 < loop_ub; i1++) {
      modCoeffs[i1 + modCoeffs.size(0) * i] = dCoeffs[i1 + dCoeffs.size(0) * i];
    }
  }
  if (static_cast<double>(dCoeffs.size(0)) + 1.0 > holdPoint) {
    i = 0;
    i1 = 0;
  } else {
    i = dCoeffs.size(0);
    i1 = static_cast<int>(holdPoint);
  }
  loop_ub = i1 - i;
  for (i1 = 0; i1 < 3; i1++) {
    for (int b_i{0}; b_i < loop_ub; b_i++) {
      modCoeffs[(i + b_i) + modCoeffs.size(0) * i1] =
          newSegmentCoeffs_data[b_i + static_cast<int>(dim) * i1];
    }
  }
  modBreaks.set_size(1, breaksWithFlatStart.size(1) + 1);
  loop_ub = breaksWithFlatStart.size(1);
  for (i = 0; i < loop_ub; i++) {
    modBreaks[i] = breaksWithFlatStart[i];
  }
  modBreaks[breaksWithFlatStart.size(1)] = holdPoint_tmp + 1.0;
  dCoeffs.set_size(modCoeffs.size(0), 3);
  loop_ub = modCoeffs.size(0) * 3;
  for (i = 0; i < loop_ub; i++) {
    dCoeffs[i] = 0.0;
  }
  for (int b_i{0}; b_i < 2; b_i++) {
    loop_ub = dCoeffs.size(0);
    for (i = 0; i < loop_ub; i++) {
      dCoeffs[i + dCoeffs.size(0) * (b_i + 1)] =
          ((3.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
          modCoeffs[i + modCoeffs.size(0) * b_i];
    }
  }
  ddCoeffs.set_size(dCoeffs.size(0), 3);
  loop_ub = dCoeffs.size(0) * 3;
  for (i = 0; i < loop_ub; i++) {
    ddCoeffs[i] = 0.0;
  }
  for (int b_i{0}; b_i < 2; b_i++) {
    loop_ub = ddCoeffs.size(0);
    for (i = 0; i < loop_ub; i++) {
      ddCoeffs[i + ddCoeffs.size(0) * (b_i + 1)] =
          ((3.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
          dCoeffs[i + dCoeffs.size(0) * b_i];
    }
  }
  pp_coefs.set_size(loop_ub_tmp, modBreaks.size(1) - 1, 3);
  b_loop_ub = static_cast<int>(dim) * (modBreaks.size(1) - 1) * 3;
  for (i = 0; i < b_loop_ub; i++) {
    pp_coefs[i] = modCoeffs[i];
  }
  pp_breaks.set_size(1, modBreaks.size(1));
  loop_ub = modBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    pp_breaks[i] = modBreaks[i];
  }
  expl_temp.coefs.set_size(loop_ub_tmp, modBreaks.size(1) - 1, 3);
  for (i = 0; i < b_loop_ub; i++) {
    expl_temp.coefs[i] = dCoeffs[i];
  }
  expl_temp.breaks.set_size(1, modBreaks.size(1));
  loop_ub = modBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    expl_temp.breaks[i] = modBreaks[i];
  }
  b_expl_temp.coefs.set_size(loop_ub_tmp, modBreaks.size(1) - 1, 3);
  for (i = 0; i < b_loop_ub; i++) {
    b_expl_temp.coefs[i] = ddCoeffs[i];
  }
  b_expl_temp.breaks.set_size(1, modBreaks.size(1));
  loop_ub = modBreaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_expl_temp.breaks[i] = modBreaks[i];
  }
  c_expl_temp.coefs.set_size(pp_coefs.size(0), pp_coefs.size(1), 3);
  for (i = 0; i < b_loop_ub; i++) {
    c_expl_temp.coefs[i] = pp_coefs[i];
  }
  c_expl_temp.breaks.set_size(1, pp_breaks.size(1));
  loop_ub = pp_breaks.size(1);
  for (i = 0; i < loop_ub; i++) {
    c_expl_temp.breaks[i] = pp_breaks[i];
  }
  ppval(c_expl_temp, t, q);
  ppval(expl_temp, t, qd);
  ppval(b_expl_temp, t, qdd);
}

//
// Arguments    : const ::coder::array<double, 2U> &input
//                double p
//                ::coder::array<double, 2U> &matrixInput
// Return Type  : void
//
static void reformatInput(const ::coder::array<double, 2U> &input, double p,
                          ::coder::array<double, 2U> &matrixInput)
{
  int i;
  int ibtile;
  matrixInput.set_size(3, input.size(1));
  ibtile = 3 * input.size(1);
  for (i = 0; i < ibtile; i++) {
    matrixInput[i] = input[i];
  }
  if (input.size(1) == 1) {
    i = static_cast<int>(p - 1.0);
    matrixInput.set_size(3, i);
    for (int jtilecol{0}; jtilecol < i; jtilecol++) {
      ibtile = jtilecol * 3 - 1;
      matrixInput[ibtile + 1] = input[0];
      matrixInput[ibtile + 2] = input[1];
      matrixInput[ibtile + 3] = input[2];
    }
  }
}

//
// Arguments    : const ::coder::array<double, 2U> &wayPoints
//                double numSamples
//                const ::coder::array<double, 2U> &varargin_2
//                const ::coder::array<double, 2U> &varargin_4
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
// Return Type  : void
//
void trapveltraj(const ::coder::array<double, 2U> &wayPoints, double numSamples,
                 const ::coder::array<double, 2U> &varargin_2,
                 const ::coder::array<double, 2U> &varargin_4,
                 ::coder::array<double, 2U> &q, ::coder::array<double, 2U> &qd,
                 ::coder::array<double, 2U> &qdd)
{
  ::coder::array<double, 3U> parameterMat;
  ::coder::array<double, 2U> b_coeffsCell;
  ::coder::array<double, 2U> breakMat;
  ::coder::array<double, 2U> lspbSegIndices;
  ::coder::array<double, 2U> r5;
  ::coder::array<double, 2U> r6;
  ::coder::array<double, 2U> r7;
  ::coder::array<double, 2U> tAc;
  ::coder::array<double, 2U> tFi;
  ::coder::array<double, 2U> y;
  ::coder::array<int, 2U> r1;
  ::coder::array<int, 1U> r;
  ::coder::array<int, 1U> r2;
  ::coder::array<int, 1U> r3;
  ::coder::array<int, 1U> r4;
  ::coder::array<boolean_T, 2U> x;
  ::coder::array<boolean_T, 1U> coefIndex;
  ::coder::bounded_array<d_struct_T, 3U, 1U> ppCell;
  cell_wrap_4 breaksCell[3];
  cell_wrap_5 coeffsCell[3];
  double coefs[9];
  double b_varargin_1[3];
  double dv[3];
  double x_data[3];
  double segATime;
  double segAcc;
  double segFTime;
  double segVel;
  double varargin_1;
  int c_i;
  int i;
  int loop_ub;
  int numComputedPolynomials;
  int nx;
  int p;
  boolean_T exitg1;
  boolean_T hasMultipleBreaks;
  p = wayPoints.size(1);
  reformatInput(varargin_4, static_cast<double>(wayPoints.size(1)), tFi);
  reformatInput(varargin_2, static_cast<double>(wayPoints.size(1)), tAc);
  q.set_size(3, static_cast<int>(numSamples));
  nx = 3 * static_cast<int>(numSamples);
  qd.set_size(3, static_cast<int>(numSamples));
  qdd.set_size(3, static_cast<int>(numSamples));
  for (i = 0; i < nx; i++) {
    q[i] = 0.0;
    qd[i] = 0.0;
    qdd[i] = 0.0;
  }
  parameterMat.set_size(3, wayPoints.size(1) - 1, 6);
  loop_ub = 3 * (wayPoints.size(1) - 1) * 6;
  for (i = 0; i < loop_ub; i++) {
    parameterMat[i] = 0.0;
  }
  varargin_1 = 3.0 * (static_cast<double>(wayPoints.size(1)) - 1.0);
  loop_ub = static_cast<int>(varargin_1 * 3.0);
  coeffsCell[2].f1.set_size(loop_ub, 3);
  nx = loop_ub * 3;
  for (i = 0; i < nx; i++) {
    coeffsCell[2].f1[i] = 0.0;
  }
  breakMat.set_size(3, static_cast<int>(varargin_1 + 1.0));
  nx = 3 * static_cast<int>(varargin_1 + 1.0);
  for (i = 0; i < nx; i++) {
    breakMat[i] = 0.0;
  }
  for (int b_i{0}; b_i < 3; b_i++) {
    for (int j{0}; j <= p - 2; j++) {
      double parameterMat_tmp;
      double s0;
      segVel = computeProfileParams(static_cast<double>(b_i) + 1.0,
                                    static_cast<double>(j) + 1.0, wayPoints,
                                    tFi, tAc, segAcc, segATime, segFTime);
      parameterMat[b_i + 3 * j] = wayPoints[b_i + 3 * j];
      parameterMat_tmp = wayPoints[b_i + 3 * (j + 1)];
      parameterMat[(b_i + 3 * j) + 3 * parameterMat.size(1)] = parameterMat_tmp;
      parameterMat[(b_i + 3 * j) + 3 * parameterMat.size(1) * 2] = segVel;
      parameterMat[(b_i + 3 * j) + 3 * parameterMat.size(1) * 3] = segAcc;
      parameterMat[(b_i + 3 * j) + 3 * parameterMat.size(1) * 4] = segATime;
      parameterMat[(b_i + 3 * j) + 3 * parameterMat.size(1) * 5] = segFTime;
      s0 = wayPoints[b_i + 3 * j];
      std::memset(&coefs[0], 0, 9U * sizeof(double));
      if (segVel == 0.0) {
        coefs[6] = s0;
        coefs[7] = s0;
        coefs[8] = s0;
      } else {
        double coefs_tmp;
        coefs[0] = segAcc / 2.0;
        coefs[3] = 0.0;
        coefs[6] = s0;
        coefs[1] = 0.0;
        coefs[4] = segVel;
        coefs_tmp = segAcc / 2.0 * (segATime * segATime);
        coefs[7] = coefs_tmp + s0;
        coefs[2] = -segAcc / 2.0;
        coefs[5] = segVel;
        coefs[8] = (parameterMat_tmp + coefs_tmp) - segVel * segATime;
      }
      coefIndex.set_size(loop_ub);
      for (i = 0; i < loop_ub; i++) {
        coefIndex[i] = false;
      }
      segVel = 3.0 * ((static_cast<double>(j) + 1.0) - 1.0) * 3.0 +
               (static_cast<double>(b_i) + 1.0);
      if (segVel + 6.0 < segVel) {
        lspbSegIndices.set_size(1, 0);
      } else {
        nx = static_cast<int>(((segVel + 6.0) - segVel) / 3.0);
        lspbSegIndices.set_size(1, nx + 1);
        for (i = 0; i <= nx; i++) {
          lspbSegIndices[i] = segVel + 3.0 * static_cast<double>(i);
        }
      }
      r1.set_size(1, lspbSegIndices.size(1));
      nx = lspbSegIndices.size(1);
      for (i = 0; i < nx; i++) {
        r1[i] = static_cast<int>(lspbSegIndices[i]);
      }
      nx = r1.size(1);
      for (i = 0; i < nx; i++) {
        coefIndex[r1[i] - 1] = true;
      }
      nx = coefIndex.size(0) - 1;
      numComputedPolynomials = 0;
      for (c_i = 0; c_i <= nx; c_i++) {
        if (coefIndex[c_i]) {
          numComputedPolynomials++;
        }
      }
      r3.set_size(numComputedPolynomials);
      numComputedPolynomials = 0;
      for (c_i = 0; c_i <= nx; c_i++) {
        if (coefIndex[c_i]) {
          r3[numComputedPolynomials] = c_i;
          numComputedPolynomials++;
        }
      }
      nx = r3.size(0);
      numComputedPolynomials = r3.size(0);
      for (i = 0; i < 3; i++) {
        for (int i1{0}; i1 < nx; i1++) {
          coeffsCell[2].f1[r3[i1] + coeffsCell[2].f1.size(0) * i] =
              coefs[i1 + numComputedPolynomials * i];
        }
      }
      segAcc = 3.0 * (static_cast<double>(j) + 1.0) - 2.0;
      segVel = breakMat[b_i + 3 * (static_cast<int>(segAcc) - 1)];
      breakMat[b_i + 3 * (static_cast<int>(segAcc + 1.0) - 1)] =
          segATime + segVel;
      breakMat[b_i + 3 * (static_cast<int>(segAcc + 2.0) - 1)] =
          (segFTime - segATime) + segVel;
      breakMat[b_i + 3 * (static_cast<int>(segAcc + 3.0) - 1)] =
          segFTime + segVel;
    }
  }
  hasMultipleBreaks = false;
  loop_ub = breakMat.size(1);
  for (int b_i{0}; b_i < 2; b_i++) {
    boolean_T b_y;
    lspbSegIndices.set_size(1, breakMat.size(1));
    for (i = 0; i < loop_ub; i++) {
      lspbSegIndices[i] = breakMat[b_i + 3 * i] - breakMat[(b_i + 3 * i) + 1];
    }
    nx = lspbSegIndices.size(1);
    y.set_size(1, lspbSegIndices.size(1));
    for (numComputedPolynomials = 0; numComputedPolynomials < nx;
         numComputedPolynomials++) {
      y[numComputedPolynomials] =
          std::abs(lspbSegIndices[numComputedPolynomials]);
    }
    x.set_size(1, y.size(1));
    nx = y.size(1);
    for (i = 0; i < nx; i++) {
      x[i] = (y[i] > 2.2204460492503131E-16);
    }
    b_y = false;
    nx = 1;
    exitg1 = false;
    while ((!exitg1) && (nx <= x.size(1))) {
      if (x[nx - 1]) {
        b_y = true;
        exitg1 = true;
      } else {
        nx++;
      }
    }
    if (b_y || hasMultipleBreaks) {
      hasMultipleBreaks = true;
    } else {
      hasMultipleBreaks = false;
    }
  }
  if (hasMultipleBreaks) {
    i = 3 * static_cast<int>(varargin_1);
    coefIndex.set_size(i);
    p = static_cast<int>(varargin_1);
    for (numComputedPolynomials = 0; numComputedPolynomials < p;
         numComputedPolynomials++) {
      nx = numComputedPolynomials * 3;
      coefIndex[nx] = true;
      coefIndex[nx + 1] = false;
      coefIndex[nx + 2] = false;
    }
    c_i = coefIndex.size(0) - 1;
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        numComputedPolynomials++;
      }
    }
    r.set_size(numComputedPolynomials);
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        r[numComputedPolynomials] = b_i;
        numComputedPolynomials++;
      }
    }
    coeffsCell[0].f1.set_size(r.size(0), 3);
    loop_ub = r.size(0);
    for (int i1{0}; i1 < 3; i1++) {
      for (nx = 0; nx < loop_ub; nx++) {
        coeffsCell[0].f1[nx + coeffsCell[0].f1.size(0) * i1] =
            coeffsCell[2].f1[r[nx] + coeffsCell[2].f1.size(0) * i1];
      }
    }
    breaksCell[0].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (int i1{0}; i1 < loop_ub; i1++) {
      breaksCell[0].f1[i1] = breakMat[3 * i1];
    }
    coefIndex.set_size(i);
    for (numComputedPolynomials = 0; numComputedPolynomials < p;
         numComputedPolynomials++) {
      nx = numComputedPolynomials * 3;
      coefIndex[nx] = false;
      coefIndex[nx + 1] = true;
      coefIndex[nx + 2] = false;
    }
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        numComputedPolynomials++;
      }
    }
    r2.set_size(numComputedPolynomials);
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        r2[numComputedPolynomials] = b_i;
        numComputedPolynomials++;
      }
    }
    coeffsCell[1].f1.set_size(r2.size(0), 3);
    loop_ub = r2.size(0);
    for (int i1{0}; i1 < 3; i1++) {
      for (nx = 0; nx < loop_ub; nx++) {
        coeffsCell[1].f1[nx + coeffsCell[1].f1.size(0) * i1] =
            coeffsCell[2].f1[r2[nx] + coeffsCell[2].f1.size(0) * i1];
      }
    }
    breaksCell[1].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (int i1{0}; i1 < loop_ub; i1++) {
      breaksCell[1].f1[i1] = breakMat[3 * i1 + 1];
    }
    coefIndex.set_size(i);
    for (numComputedPolynomials = 0; numComputedPolynomials < p;
         numComputedPolynomials++) {
      nx = numComputedPolynomials * 3;
      coefIndex[nx] = false;
      coefIndex[nx + 1] = false;
      coefIndex[nx + 2] = true;
    }
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        numComputedPolynomials++;
      }
    }
    r4.set_size(numComputedPolynomials);
    numComputedPolynomials = 0;
    for (int b_i{0}; b_i <= c_i; b_i++) {
      if (coefIndex[b_i]) {
        r4[numComputedPolynomials] = b_i;
        numComputedPolynomials++;
      }
    }
    b_coeffsCell.set_size(r4.size(0), 3);
    loop_ub = r4.size(0);
    for (i = 0; i < 3; i++) {
      for (int i1{0}; i1 < loop_ub; i1++) {
        b_coeffsCell[i1 + b_coeffsCell.size(0) * i] =
            coeffsCell[2].f1[r4[i1] + coeffsCell[2].f1.size(0) * i];
      }
    }
    coeffsCell[2].f1.set_size(b_coeffsCell.size(0), 3);
    loop_ub = b_coeffsCell.size(0) * 3;
    for (i = 0; i < loop_ub; i++) {
      coeffsCell[2].f1[i] = b_coeffsCell[i];
    }
    breaksCell[2].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (i = 0; i < loop_ub; i++) {
      breaksCell[2].f1[i] = breakMat[3 * i + 2];
    }
  } else {
    coeffsCell[0].f1.set_size(coeffsCell[2].f1.size(0), 3);
    nx = coeffsCell[2].f1.size(0) * 3;
    for (i = 0; i < nx; i++) {
      coeffsCell[0].f1[i] = coeffsCell[2].f1[i];
    }
    breaksCell[0].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (i = 0; i < loop_ub; i++) {
      breaksCell[0].f1[i] = breakMat[3 * i];
    }
    coeffsCell[1].f1.set_size(coeffsCell[2].f1.size(0), 3);
    for (i = 0; i < nx; i++) {
      coeffsCell[1].f1[i] = coeffsCell[2].f1[i];
    }
    breaksCell[1].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (i = 0; i < loop_ub; i++) {
      breaksCell[1].f1[i] = breakMat[3 * i];
    }
    breaksCell[2].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (i = 0; i < loop_ub; i++) {
      breaksCell[2].f1[i] = breakMat[3 * i];
    }
  }
  tFi.set_size(3, parameterMat.size(1));
  loop_ub = parameterMat.size(1);
  for (i = 0; i < loop_ub; i++) {
    tFi[3 * i] = parameterMat[3 * i + 3 * parameterMat.size(1) * 5];
    tFi[3 * i + 1] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 1];
    tFi[3 * i + 2] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 2];
  }
  sum(tFi, b_varargin_1);
  tFi.set_size(3, parameterMat.size(1));
  loop_ub = parameterMat.size(1);
  for (i = 0; i < loop_ub; i++) {
    tFi[3 * i] = parameterMat[3 * i + 3 * parameterMat.size(1) * 5];
    tFi[3 * i + 1] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 1];
    tFi[3 * i + 2] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 2];
  }
  sum(tFi, dv);
  x_data[0] = dv[0];
  x_data[1] = dv[1];
  x_data[2] = dv[2];
  if (!std::isnan(dv[0])) {
    nx = 1;
  } else {
    nx = 0;
    numComputedPolynomials = 2;
    exitg1 = false;
    while ((!exitg1) && (numComputedPolynomials < 4)) {
      if (!std::isnan(x_data[numComputedPolynomials - 1])) {
        nx = numComputedPolynomials;
        exitg1 = true;
      } else {
        numComputedPolynomials++;
      }
    }
  }
  if (nx == 0) {
    segVel = b_varargin_1[0];
  } else {
    segVel = b_varargin_1[nx - 1];
    i = nx + 1;
    for (numComputedPolynomials = i; numComputedPolynomials < 4;
         numComputedPolynomials++) {
      segAcc = b_varargin_1[numComputedPolynomials - 1];
      if (segVel < segAcc) {
        segVel = segAcc;
      }
    }
  }
  linspace(segVel, numSamples, lspbSegIndices);
  if (hasMultipleBreaks) {
    numComputedPolynomials = 2;
    p = 1;
  } else {
    numComputedPolynomials = 0;
    p = 3;
  }
  ppCell.size[0] = numComputedPolynomials + 1;
  if (numSamples < 1.0) {
    loop_ub = 0;
  } else {
    loop_ub = static_cast<int>(numSamples);
  }
  for (int j{0}; j <= numComputedPolynomials; j++) {
    signed char rowSelection_data[3];
    signed char tmp_data[3];
    if (hasMultipleBreaks) {
      c_i = 1;
      rowSelection_data[0] = static_cast<signed char>(j + 1);
      nx = j;
    } else {
      c_i = 3;
      rowSelection_data[0] = 1;
      rowSelection_data[1] = 2;
      rowSelection_data[2] = 3;
      nx = 0;
    }
    generateTrajectoriesFromCoefs(breaksCell[nx].f1, coeffsCell[nx].f1,
                                  static_cast<double>(p), lspbSegIndices, r5,
                                  r6, r7, ppCell.data[j].breaks,
                                  ppCell.data[j].coefs);
    for (i = 0; i < c_i; i++) {
      tmp_data[i] = static_cast<signed char>(rowSelection_data[i] - 1);
    }
    for (i = 0; i < loop_ub; i++) {
      for (int i1{0}; i1 < c_i; i1++) {
        q[tmp_data[i1] + 3 * i] = r5[i1 + c_i * i];
      }
    }
    for (i = 0; i < loop_ub; i++) {
      for (int i1{0}; i1 < c_i; i1++) {
        qd[tmp_data[i1] + 3 * i] = r6[i1 + c_i * i];
      }
    }
    for (i = 0; i < loop_ub; i++) {
      for (int i1{0}; i1 < c_i; i1++) {
        qdd[tmp_data[i1] + 3 * i] = r7[i1 + c_i * i];
      }
    }
  }
}

//
// Arguments    : double numSamples
//                double varargin_2
//                double varargin_4
//                ::coder::array<double, 2U> &q
//                ::coder::array<double, 2U> &qd
//                ::coder::array<double, 2U> &qdd
// Return Type  : void
//
void trapveltraj(double numSamples, double varargin_2, double varargin_4,
                 ::coder::array<double, 2U> &q, ::coder::array<double, 2U> &qd,
                 ::coder::array<double, 2U> &qdd)
{
  ::coder::array<double, 2U> r;
  ::coder::array<double, 2U> t;
  cell_wrap_24 coeffsCell;
  struct_T b_expl_temp;
  struct_T c_expl_temp;
  struct_T expl_temp;
  double coefsWithFlatStart[12];
  double coefs[9];
  double breaksWithFlatStart[5];
  double newSegmentCoeffs[3];
  double holdPoint;
  double segAcc;
  double segVel;
  int loop_ub_tmp;
  signed char tmp_data[3];
  loop_ub_tmp = static_cast<int>(numSamples);
  q.set_size(1, loop_ub_tmp);
  qd.set_size(1, loop_ub_tmp);
  qdd.set_size(1, loop_ub_tmp);
  for (int i{0}; i < loop_ub_tmp; i++) {
    q[i] = 0.0;
    qd[i] = 0.0;
    qdd[i] = 0.0;
  }
  holdPoint = varargin_4 - varargin_2;
  segVel = 1.0 / holdPoint;
  segAcc = segVel / varargin_2;
  std::memset(&coeffsCell.f1[0], 0, 9U * sizeof(double));
  std::memset(&coefs[0], 0, 9U * sizeof(double));
  if (segVel == 0.0) {
    coefs[6] = 0.0;
    coefs[7] = 0.0;
    coefs[8] = 0.0;
  } else {
    double coefs_tmp;
    coefs[0] = segAcc / 2.0;
    coefs[3] = 0.0;
    coefs[6] = 0.0;
    coefs[1] = 0.0;
    coefs[4] = segVel;
    coefs_tmp = segAcc / 2.0 * (varargin_2 * varargin_2);
    coefs[7] = coefs_tmp;
    coefs[2] = -segAcc / 2.0;
    coefs[5] = segVel;
    coefs[8] = (coefs_tmp + 1.0) - segVel * varargin_2;
  }
  tmp_data[0] = 0;
  tmp_data[1] = 1;
  tmp_data[2] = 2;
  linspace(varargin_4, numSamples, t);
  for (int i{0}; i < 3; i++) {
    for (loop_ub_tmp = 0; loop_ub_tmp < 3; loop_ub_tmp++) {
      coeffsCell.f1[tmp_data[loop_ub_tmp] + 3 * i] = coefs[loop_ub_tmp + 3 * i];
    }
    newSegmentCoeffs[i] = 0.0;
  }
  newSegmentCoeffs[2] =
      (coeffsCell.f1[0] * 0.0 + coeffsCell.f1[3] * 0.0) + coeffsCell.f1[6];
  std::memset(&coefsWithFlatStart[0], 0, 12U * sizeof(double));
  for (int i{0}; i < 3; i++) {
    loop_ub_tmp = i << 2;
    coefsWithFlatStart[loop_ub_tmp] = newSegmentCoeffs[i];
    coefsWithFlatStart[loop_ub_tmp + 1] = coeffsCell.f1[3 * i];
    coefsWithFlatStart[loop_ub_tmp + 2] = coeffsCell.f1[3 * i + 1];
    coefsWithFlatStart[loop_ub_tmp + 3] = coeffsCell.f1[3 * i + 2];
  }
  breaksWithFlatStart[0] = -1.0;
  breaksWithFlatStart[1] = 0.0;
  breaksWithFlatStart[2] = varargin_2;
  breaksWithFlatStart[3] = holdPoint;
  breaksWithFlatStart[4] = varargin_4;
  holdPoint = varargin_4 - holdPoint;
  newSegmentCoeffs[0] = 0.0;
  newSegmentCoeffs[1] = 0.0;
  newSegmentCoeffs[2] = (coefsWithFlatStart[3] * rt_powd_snf(holdPoint, 2.0) +
                         coefsWithFlatStart[7] * rt_powd_snf(holdPoint, 1.0)) +
                        coefsWithFlatStart[11] * rt_powd_snf(holdPoint, 0.0);
  std::memset(&expl_temp.coefs[0], 0, 15U * sizeof(double));
  for (int i{0}; i < 3; i++) {
    loop_ub_tmp = i << 2;
    expl_temp.coefs[5 * i] = coefsWithFlatStart[loop_ub_tmp];
    expl_temp.coefs[5 * i + 1] = coefsWithFlatStart[loop_ub_tmp + 1];
    expl_temp.coefs[5 * i + 2] = coefsWithFlatStart[loop_ub_tmp + 2];
    expl_temp.coefs[5 * i + 3] = coefsWithFlatStart[loop_ub_tmp + 3];
    expl_temp.coefs[5 * i + 4] = newSegmentCoeffs[i];
  }
  for (int i{0}; i < 5; i++) {
    expl_temp.breaks[i] = breaksWithFlatStart[i];
  }
  expl_temp.breaks[5] = varargin_4 + 1.0;
  std::memset(&b_expl_temp.coefs[0], 0, 15U * sizeof(double));
  for (loop_ub_tmp = 0; loop_ub_tmp < 2; loop_ub_tmp++) {
    for (int i{0}; i < 5; i++) {
      b_expl_temp.coefs[i + 5 * (loop_ub_tmp + 1)] =
          ((3.0 - (static_cast<double>(loop_ub_tmp) + 2.0)) + 1.0) *
          expl_temp.coefs[i + 5 * loop_ub_tmp];
    }
  }
  std::memset(&c_expl_temp.coefs[0], 0, 15U * sizeof(double));
  for (loop_ub_tmp = 0; loop_ub_tmp < 2; loop_ub_tmp++) {
    for (int i{0}; i < 5; i++) {
      c_expl_temp.coefs[i + 5 * (loop_ub_tmp + 1)] =
          ((3.0 - (static_cast<double>(loop_ub_tmp) + 2.0)) + 1.0) *
          b_expl_temp.coefs[i + 5 * loop_ub_tmp];
    }
  }
  ppval(expl_temp, t, r);
  if (numSamples < 1.0) {
    loop_ub_tmp = 0;
  } else {
    loop_ub_tmp = static_cast<int>(numSamples);
  }
  for (int i{0}; i < loop_ub_tmp; i++) {
    q[i] = r[i];
  }
  for (int i{0}; i < 6; i++) {
    b_expl_temp.breaks[i] = expl_temp.breaks[i];
  }
  ppval(b_expl_temp, t, r);
  for (int i{0}; i < loop_ub_tmp; i++) {
    qd[i] = r[i];
  }
  for (int i{0}; i < 6; i++) {
    c_expl_temp.breaks[i] = expl_temp.breaks[i];
  }
  ppval(c_expl_temp, t, r);
  for (int i{0}; i < loop_ub_tmp; i++) {
    qdd[i] = r[i];
  }
}

} // namespace coder
} // namespace TrajectoryGeneration

//
// File trailer for trapveltraj.cpp
//
// [EOF]
//
