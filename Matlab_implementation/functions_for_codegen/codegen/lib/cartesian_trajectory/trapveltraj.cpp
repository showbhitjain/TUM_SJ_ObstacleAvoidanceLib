//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: trapveltraj.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

// Include Files
#include "trapveltraj.h"
#include "cartesian_trajectory_internal_types.h"
#include "cartesian_trajectory_rtwutil.h"
#include "linspace.h"
#include "ppval.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Type Definitions
struct cell_wrap_4 {
  coder::array<double, 2U> f1;
};

struct cell_wrap_5 {
  coder::array<double, 2U> f1;
};

// Function Declarations
namespace coder {
static boolean_T checkPolyForMultipleBreaks(const array<double, 2U> &breakMat);

static double computeProfileParams(double i, double j,
                                   const array<double, 2U> &wayPoints,
                                   const array<double, 2U> &TFi,
                                   const array<double, 2U> &TAc, double &aParam,
                                   double &tAParam, double &tFParam);

static void processPolynomialResults(const array<double, 2U> &breakMat,
                                     const array<double, 2U> &coeffMat,
                                     boolean_T hasMultipleBreaks, double p,
                                     cell_wrap_4 breaksCell[3],
                                     cell_wrap_5 coeffCell[3]);

static void reformatInput(const array<double, 2U> &input, double p,
                          array<double, 2U> &matrixInput);

} // namespace coder

// Function Definitions
//
// Arguments    : const array<double, 2U> &breakMat
// Return Type  : boolean_T
//
namespace coder {
static boolean_T checkPolyForMultipleBreaks(const array<double, 2U> &breakMat)
{
  array<double, 2U> x;
  array<double, 2U> y;
  array<boolean_T, 2U> b_x;
  int loop_ub;
  boolean_T hasMultipleBreaks;
  hasMultipleBreaks = false;
  loop_ub = breakMat.size(1);
  for (int i{0}; i < 2; i++) {
    int nx;
    boolean_T b_y;
    boolean_T exitg1;
    x.set_size(1, breakMat.size(1));
    for (int k{0}; k < loop_ub; k++) {
      x[k] = breakMat[i + 3 * k] - breakMat[(i + 3 * k) + 1];
    }
    nx = x.size(1);
    y.set_size(1, x.size(1));
    for (int k{0}; k < nx; k++) {
      y[k] = std::abs(x[k]);
    }
    b_x.set_size(1, y.size(1));
    nx = y.size(1);
    for (int k{0}; k < nx; k++) {
      b_x[k] = (y[k] > 2.2204460492503131E-16);
    }
    b_y = false;
    nx = 1;
    exitg1 = false;
    while ((!exitg1) && (nx <= b_x.size(1))) {
      if (b_x[nx - 1]) {
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
  return hasMultipleBreaks;
}

//
// Arguments    : double i
//                double j
//                const array<double, 2U> &wayPoints
//                const array<double, 2U> &TFi
//                const array<double, 2U> &TAc
//                double &aParam
//                double &tAParam
//                double &tFParam
// Return Type  : double
//
static double computeProfileParams(double i, double j,
                                   const array<double, 2U> &wayPoints,
                                   const array<double, 2U> &TFi,
                                   const array<double, 2U> &TAc, double &aParam,
                                   double &tAParam, double &tFParam)
{
  array<double, 2U> TAcSwitch;
  array<double, 2U> TFiSwitch;
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
// Arguments    : const array<double, 2U> &breakMat
//                const array<double, 2U> &coeffMat
//                boolean_T hasMultipleBreaks
//                double p
//                cell_wrap_4 breaksCell[3]
//                cell_wrap_5 coeffCell[3]
// Return Type  : void
//
static void processPolynomialResults(const array<double, 2U> &breakMat,
                                     const array<double, 2U> &coeffMat,
                                     boolean_T hasMultipleBreaks, double p,
                                     cell_wrap_4 breaksCell[3],
                                     cell_wrap_5 coeffCell[3])
{
  array<int, 1U> r;
  array<int, 1U> r1;
  array<int, 1U> r2;
  array<boolean_T, 1U> coeffIndex;
  if (hasMultipleBreaks) {
    double varargin_1_tmp;
    int end_tmp;
    int i;
    int ibcol;
    int loop_ub;
    int ntilerows_tmp;
    varargin_1_tmp = 3.0 * (p - 1.0);
    i = 3 * static_cast<int>(varargin_1_tmp);
    coeffIndex.set_size(i);
    ntilerows_tmp = static_cast<int>(varargin_1_tmp);
    for (loop_ub = 0; loop_ub < ntilerows_tmp; loop_ub++) {
      ibcol = loop_ub * 3;
      coeffIndex[ibcol] = true;
      coeffIndex[ibcol + 1] = false;
      coeffIndex[ibcol + 2] = false;
    }
    end_tmp = coeffIndex.size(0) - 1;
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        ibcol++;
      }
    }
    r.set_size(ibcol);
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        r[ibcol] = loop_ub;
        ibcol++;
      }
    }
    coeffCell[0].f1.set_size(r.size(0), 3);
    loop_ub = r.size(0);
    for (ibcol = 0; ibcol < 3; ibcol++) {
      for (int i1{0}; i1 < loop_ub; i1++) {
        coeffCell[0].f1[i1 + coeffCell[0].f1.size(0) * ibcol] =
            coeffMat[r[i1] + coeffMat.size(0) * ibcol];
      }
    }
    breaksCell[0].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (ibcol = 0; ibcol < loop_ub; ibcol++) {
      breaksCell[0].f1[ibcol] = breakMat[3 * ibcol];
    }
    coeffIndex.set_size(i);
    for (loop_ub = 0; loop_ub < ntilerows_tmp; loop_ub++) {
      ibcol = loop_ub * 3;
      coeffIndex[ibcol] = false;
      coeffIndex[ibcol + 1] = true;
      coeffIndex[ibcol + 2] = false;
    }
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        ibcol++;
      }
    }
    r1.set_size(ibcol);
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        r1[ibcol] = loop_ub;
        ibcol++;
      }
    }
    coeffCell[1].f1.set_size(r1.size(0), 3);
    loop_ub = r1.size(0);
    for (ibcol = 0; ibcol < 3; ibcol++) {
      for (int i1{0}; i1 < loop_ub; i1++) {
        coeffCell[1].f1[i1 + coeffCell[1].f1.size(0) * ibcol] =
            coeffMat[r1[i1] + coeffMat.size(0) * ibcol];
      }
    }
    breaksCell[1].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (ibcol = 0; ibcol < loop_ub; ibcol++) {
      breaksCell[1].f1[ibcol] = breakMat[3 * ibcol + 1];
    }
    coeffIndex.set_size(i);
    for (loop_ub = 0; loop_ub < ntilerows_tmp; loop_ub++) {
      ibcol = loop_ub * 3;
      coeffIndex[ibcol] = false;
      coeffIndex[ibcol + 1] = false;
      coeffIndex[ibcol + 2] = true;
    }
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        ibcol++;
      }
    }
    r2.set_size(ibcol);
    ibcol = 0;
    for (loop_ub = 0; loop_ub <= end_tmp; loop_ub++) {
      if (coeffIndex[loop_ub]) {
        r2[ibcol] = loop_ub;
        ibcol++;
      }
    }
    coeffCell[2].f1.set_size(r2.size(0), 3);
    loop_ub = r2.size(0);
    for (i = 0; i < 3; i++) {
      for (ibcol = 0; ibcol < loop_ub; ibcol++) {
        coeffCell[2].f1[ibcol + coeffCell[2].f1.size(0) * i] =
            coeffMat[r2[ibcol] + coeffMat.size(0) * i];
      }
    }
    breaksCell[2].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (i = 0; i < loop_ub; i++) {
      breaksCell[2].f1[i] = breakMat[3 * i + 2];
    }
  } else {
    int ibcol;
    int loop_ub;
    coeffCell[0].f1.set_size(coeffMat.size(0), 3);
    ibcol = coeffMat.size(0) * 3;
    for (int i{0}; i < ibcol; i++) {
      coeffCell[0].f1[i] = coeffMat[i];
    }
    breaksCell[0].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (int i{0}; i < loop_ub; i++) {
      breaksCell[0].f1[i] = breakMat[3 * i];
    }
    coeffCell[1].f1.set_size(coeffMat.size(0), 3);
    for (int i{0}; i < ibcol; i++) {
      coeffCell[1].f1[i] = coeffMat[i];
    }
    breaksCell[1].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (int i{0}; i < loop_ub; i++) {
      breaksCell[1].f1[i] = breakMat[3 * i];
    }
    coeffCell[2].f1.set_size(coeffMat.size(0), 3);
    for (int i{0}; i < ibcol; i++) {
      coeffCell[2].f1[i] = coeffMat[i];
    }
    breaksCell[2].f1.set_size(1, breakMat.size(1));
    loop_ub = breakMat.size(1);
    for (int i{0}; i < loop_ub; i++) {
      breaksCell[2].f1[i] = breakMat[3 * i];
    }
  }
}

//
// Arguments    : const array<double, 2U> &input
//                double p
//                array<double, 2U> &matrixInput
// Return Type  : void
//
static void reformatInput(const array<double, 2U> &input, double p,
                          array<double, 2U> &matrixInput)
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
// Arguments    : const array<double, 2U> &wayPoints
//                double numSamples
//                const array<double, 2U> &varargin_2
//                const array<double, 2U> &varargin_4
//                array<double, 2U> &q
//                array<double, 2U> &qd
//                array<double, 2U> &qdd
// Return Type  : void
//
void trapveltraj(const array<double, 2U> &wayPoints, double numSamples,
                 const array<double, 2U> &varargin_2,
                 const array<double, 2U> &varargin_4, array<double, 2U> &q,
                 array<double, 2U> &qd, array<double, 2U> &qdd)
{
  array<double, 3U> parameterMat;
  array<double, 2U> breakMat;
  array<double, 2U> breaksWithFlatStart;
  array<double, 2U> coeffMat;
  array<double, 2U> ddCoeffs;
  array<double, 2U> lspbSegIndices;
  array<double, 2U> modBreaks;
  array<double, 2U> modCoeffs;
  array<double, 2U> r2;
  array<double, 2U> tAc;
  array<double, 2U> tFi;
  array<double, 1U> valueAtEnd;
  array<int, 2U> r;
  array<int, 1U> r1;
  array<boolean_T, 1U> coefIndex;
  bounded_array<struct_T, 3U, 1U> ppCell;
  cell_wrap_4 breaksCell[3];
  cell_wrap_5 coeffsCell[3];
  struct_T b_expl_temp;
  struct_T expl_temp;
  double newSegmentCoeffs_data[9];
  double evalPointVector[3];
  double valueAtStart_data[3];
  double x_data[3];
  double holdPoint;
  double parameterMat_tmp;
  double s;
  double segATime;
  double segFTime;
  int b_loop_ub;
  int c_i;
  int i;
  int i1;
  int idx;
  int loop_ub;
  int loop_ub_tmp;
  int numComputedPolynomials;
  int p;
  boolean_T hasMultipleBreaks;
  p = wayPoints.size(1);
  reformatInput(varargin_4, static_cast<double>(wayPoints.size(1)), tFi);
  reformatInput(varargin_2, static_cast<double>(wayPoints.size(1)), tAc);
  q.set_size(3, static_cast<int>(numSamples));
  loop_ub_tmp = 3 * static_cast<int>(numSamples);
  qd.set_size(3, static_cast<int>(numSamples));
  qdd.set_size(3, static_cast<int>(numSamples));
  for (i = 0; i < loop_ub_tmp; i++) {
    q[i] = 0.0;
    qd[i] = 0.0;
    qdd[i] = 0.0;
  }
  parameterMat.set_size(3, wayPoints.size(1) - 1, 6);
  loop_ub = 3 * (wayPoints.size(1) - 1) * 6;
  for (i = 0; i < loop_ub; i++) {
    parameterMat[i] = 0.0;
  }
  holdPoint = 3.0 * (static_cast<double>(wayPoints.size(1)) - 1.0);
  loop_ub = static_cast<int>(holdPoint * 3.0);
  coeffMat.set_size(loop_ub, 3);
  b_loop_ub = loop_ub * 3;
  for (i = 0; i < b_loop_ub; i++) {
    coeffMat[i] = 0.0;
  }
  breakMat.set_size(3, static_cast<int>(holdPoint + 1.0));
  b_loop_ub = 3 * static_cast<int>(holdPoint + 1.0);
  for (i = 0; i < b_loop_ub; i++) {
    breakMat[i] = 0.0;
  }
  for (int b_i{0}; b_i < 3; b_i++) {
    for (loop_ub_tmp = 0; loop_ub_tmp <= p - 2; loop_ub_tmp++) {
      double s0;
      holdPoint =
          computeProfileParams(static_cast<double>(b_i) + 1.0,
                               static_cast<double>(loop_ub_tmp) + 1.0,
                               wayPoints, tFi, tAc, s, segATime, segFTime);
      parameterMat[b_i + 3 * loop_ub_tmp] = wayPoints[b_i + 3 * loop_ub_tmp];
      parameterMat_tmp = wayPoints[b_i + 3 * (loop_ub_tmp + 1)];
      parameterMat[(b_i + 3 * loop_ub_tmp) + 3 * parameterMat.size(1)] =
          parameterMat_tmp;
      parameterMat[(b_i + 3 * loop_ub_tmp) + 3 * parameterMat.size(1) * 2] =
          holdPoint;
      parameterMat[(b_i + 3 * loop_ub_tmp) + 3 * parameterMat.size(1) * 3] = s;
      parameterMat[(b_i + 3 * loop_ub_tmp) + 3 * parameterMat.size(1) * 4] =
          segATime;
      parameterMat[(b_i + 3 * loop_ub_tmp) + 3 * parameterMat.size(1) * 5] =
          segFTime;
      s0 = wayPoints[b_i + 3 * loop_ub_tmp];
      std::memset(&newSegmentCoeffs_data[0], 0, 9U * sizeof(double));
      if (holdPoint == 0.0) {
        newSegmentCoeffs_data[6] = s0;
        newSegmentCoeffs_data[7] = s0;
        newSegmentCoeffs_data[8] = s0;
      } else {
        double coefs_tmp;
        newSegmentCoeffs_data[0] = s / 2.0;
        newSegmentCoeffs_data[3] = 0.0;
        newSegmentCoeffs_data[6] = s0;
        newSegmentCoeffs_data[1] = 0.0;
        newSegmentCoeffs_data[4] = holdPoint;
        coefs_tmp = s / 2.0 * (segATime * segATime);
        newSegmentCoeffs_data[7] = coefs_tmp + s0;
        newSegmentCoeffs_data[2] = -s / 2.0;
        newSegmentCoeffs_data[5] = holdPoint;
        newSegmentCoeffs_data[8] =
            (parameterMat_tmp + coefs_tmp) - holdPoint * segATime;
      }
      coefIndex.set_size(loop_ub);
      for (i = 0; i < loop_ub; i++) {
        coefIndex[i] = false;
      }
      holdPoint = 3.0 * ((static_cast<double>(loop_ub_tmp) + 1.0) - 1.0) * 3.0 +
                  (static_cast<double>(b_i) + 1.0);
      if (holdPoint + 6.0 < holdPoint) {
        lspbSegIndices.set_size(1, 0);
      } else {
        b_loop_ub = static_cast<int>(((holdPoint + 6.0) - holdPoint) / 3.0);
        lspbSegIndices.set_size(1, b_loop_ub + 1);
        for (i = 0; i <= b_loop_ub; i++) {
          lspbSegIndices[i] = holdPoint + 3.0 * static_cast<double>(i);
        }
      }
      r.set_size(1, lspbSegIndices.size(1));
      b_loop_ub = lspbSegIndices.size(1);
      for (i = 0; i < b_loop_ub; i++) {
        r[i] = static_cast<int>(lspbSegIndices[i]);
      }
      b_loop_ub = r.size(1);
      for (i = 0; i < b_loop_ub; i++) {
        coefIndex[r[i] - 1] = true;
      }
      numComputedPolynomials = coefIndex.size(0) - 1;
      idx = 0;
      for (c_i = 0; c_i <= numComputedPolynomials; c_i++) {
        if (coefIndex[c_i]) {
          idx++;
        }
      }
      r1.set_size(idx);
      idx = 0;
      for (c_i = 0; c_i <= numComputedPolynomials; c_i++) {
        if (coefIndex[c_i]) {
          r1[idx] = c_i;
          idx++;
        }
      }
      b_loop_ub = r1.size(0);
      idx = r1.size(0);
      for (i = 0; i < 3; i++) {
        for (i1 = 0; i1 < b_loop_ub; i1++) {
          coeffMat[r1[i1] + coeffMat.size(0) * i] =
              newSegmentCoeffs_data[i1 + idx * i];
        }
      }
      holdPoint = 3.0 * (static_cast<double>(loop_ub_tmp) + 1.0) - 2.0;
      s = breakMat[b_i + 3 * (static_cast<int>(holdPoint) - 1)];
      breakMat[b_i + 3 * (static_cast<int>(holdPoint + 1.0) - 1)] =
          segATime + s;
      breakMat[b_i + 3 * (static_cast<int>(holdPoint + 2.0) - 1)] =
          (segFTime - segATime) + s;
      breakMat[b_i + 3 * (static_cast<int>(holdPoint + 3.0) - 1)] =
          segFTime + s;
    }
  }
  hasMultipleBreaks = checkPolyForMultipleBreaks(breakMat);
  processPolynomialResults(breakMat, coeffMat, hasMultipleBreaks,
                           static_cast<double>(wayPoints.size(1)), breaksCell,
                           coeffsCell);
  tFi.set_size(3, parameterMat.size(1));
  loop_ub = parameterMat.size(1);
  for (i = 0; i < loop_ub; i++) {
    tFi[3 * i] = parameterMat[3 * i + 3 * parameterMat.size(1) * 5];
    tFi[3 * i + 1] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 1];
    tFi[3 * i + 2] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 2];
  }
  sum(tFi, evalPointVector);
  tFi.set_size(3, parameterMat.size(1));
  loop_ub = parameterMat.size(1);
  for (i = 0; i < loop_ub; i++) {
    tFi[3 * i] = parameterMat[3 * i + 3 * parameterMat.size(1) * 5];
    tFi[3 * i + 1] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 1];
    tFi[3 * i + 2] = parameterMat[(3 * i + 3 * parameterMat.size(1) * 5) + 2];
  }
  sum(tFi, valueAtStart_data);
  x_data[0] = valueAtStart_data[0];
  x_data[1] = valueAtStart_data[1];
  x_data[2] = valueAtStart_data[2];
  if (!std::isnan(valueAtStart_data[0])) {
    idx = 1;
  } else {
    boolean_T exitg1;
    idx = 0;
    numComputedPolynomials = 2;
    exitg1 = false;
    while ((!exitg1) && (numComputedPolynomials < 4)) {
      if (!std::isnan(x_data[numComputedPolynomials - 1])) {
        idx = numComputedPolynomials;
        exitg1 = true;
      } else {
        numComputedPolynomials++;
      }
    }
  }
  if (idx == 0) {
    s = evalPointVector[0];
  } else {
    s = evalPointVector[idx - 1];
    i = idx + 1;
    for (numComputedPolynomials = i; numComputedPolynomials < 4;
         numComputedPolynomials++) {
      holdPoint = evalPointVector[numComputedPolynomials - 1];
      if (s < holdPoint) {
        s = holdPoint;
      }
    }
  }
  linspace(s, numSamples, lspbSegIndices);
  if (hasMultipleBreaks) {
    numComputedPolynomials = 2;
    p = 1;
  } else {
    numComputedPolynomials = 0;
    p = 3;
  }
  ppCell.size[0] = numComputedPolynomials + 1;
  loop_ub_tmp = p * 3;
  if (numSamples < 1.0) {
    loop_ub = 0;
  } else {
    loop_ub = static_cast<int>(numSamples);
  }
  for (int jj{0}; jj <= numComputedPolynomials; jj++) {
    unsigned int u;
    signed char rowSelection_data[3];
    signed char tmp_data[3];
    if (hasMultipleBreaks) {
      c_i = 1;
      rowSelection_data[0] = static_cast<signed char>(jj + 1);
      idx = jj;
    } else {
      c_i = 3;
      rowSelection_data[0] = 1;
      rowSelection_data[1] = 2;
      rowSelection_data[2] = 3;
      idx = 0;
    }
    for (i = 0; i < 3; i++) {
      for (i1 = 0; i1 < p; i1++) {
        newSegmentCoeffs_data[i1 + p * i] =
            coeffsCell[idx].f1[i1 + coeffsCell[idx].f1.size(0) * i];
      }
    }
    for (int b_i{0}; b_i < p; b_i++) {
      valueAtStart_data[b_i] = (newSegmentCoeffs_data[b_i] * 0.0 +
                                newSegmentCoeffs_data[p + b_i] * 0.0) +
                               newSegmentCoeffs_data[(p << 1) + b_i];
    }
    std::memset(&newSegmentCoeffs_data[0], 0,
                static_cast<unsigned int>(loop_ub_tmp) * sizeof(double));
    for (i = 0; i < p; i++) {
      newSegmentCoeffs_data[i + p * 2] = valueAtStart_data[i];
    }
    u = static_cast<unsigned int>(coeffsCell[idx].f1.size(0)) +
        static_cast<unsigned int>(p);
    coeffMat.set_size(static_cast<int>(u), 3);
    b_loop_ub = static_cast<int>(u) * 3;
    for (i = 0; i < b_loop_ub; i++) {
      coeffMat[i] = 0.0;
    }
    for (i = 0; i < 3; i++) {
      for (i1 = 0; i1 < p; i1++) {
        coeffMat[i1 + coeffMat.size(0) * i] = newSegmentCoeffs_data[i1 + p * i];
      }
    }
    if (static_cast<unsigned int>(p + 1) > u) {
      i = 0;
      i1 = 0;
    } else {
      i = p;
      i1 = static_cast<int>(u);
    }
    b_loop_ub = i1 - i;
    for (i1 = 0; i1 < 3; i1++) {
      for (int b_i{0}; b_i < b_loop_ub; b_i++) {
        coeffMat[(i + b_i) + coeffMat.size(0) * i1] =
            coeffsCell[idx].f1[b_i + coeffsCell[idx].f1.size(0) * i1];
      }
    }
    breaksWithFlatStart.set_size(1, breaksCell[idx].f1.size(1) + 1);
    breaksWithFlatStart[0] = breaksCell[idx].f1[0] - 1.0;
    b_loop_ub = breaksCell[idx].f1.size(1);
    for (i = 0; i < b_loop_ub; i++) {
      breaksWithFlatStart[i + 1] = breaksCell[idx].f1[i];
    }
    parameterMat_tmp = breaksWithFlatStart[breaksWithFlatStart.size(1) - 1];
    holdPoint =
        parameterMat_tmp - breaksWithFlatStart[breaksWithFlatStart.size(1) - 2];
    evalPointVector[0] = rt_powd_snf(holdPoint, 2.0);
    evalPointVector[1] = rt_powd_snf(holdPoint, 1.0);
    evalPointVector[2] = rt_powd_snf(holdPoint, 0.0);
    i = coeffMat.size(0) - p;
    if (i + 1 > coeffMat.size(0)) {
      i = 0;
      i1 = 0;
    } else {
      i1 = coeffMat.size(0);
    }
    idx = i1 - i;
    valueAtEnd.set_size(idx);
    for (int b_i{0}; b_i < idx; b_i++) {
      i1 = idx + b_i;
      s = coeffMat[(i + b_i % idx) + coeffMat.size(0) * (b_i / idx)] *
              evalPointVector[0] +
          coeffMat[(i + i1 % idx) + coeffMat.size(0) * (i1 / idx)] *
              evalPointVector[1];
      i1 = (idx << 1) + b_i;
      s += coeffMat[(i + i1 % idx) + coeffMat.size(0) * (i1 / idx)] *
           evalPointVector[2];
      valueAtEnd[b_i] = s;
    }
    std::memset(&newSegmentCoeffs_data[0], 0,
                static_cast<unsigned int>(loop_ub_tmp) * sizeof(double));
    for (i = 0; i < p; i++) {
      newSegmentCoeffs_data[i + p * 2] = valueAtEnd[i];
    }
    u = static_cast<unsigned int>(coeffMat.size(0)) +
        static_cast<unsigned int>(p);
    modCoeffs.set_size(static_cast<int>(u), 3);
    b_loop_ub = static_cast<int>(u) * 3;
    for (i = 0; i < b_loop_ub; i++) {
      modCoeffs[i] = 0.0;
    }
    b_loop_ub = coeffMat.size(0);
    for (i = 0; i < 3; i++) {
      for (i1 = 0; i1 < b_loop_ub; i1++) {
        modCoeffs[i1 + modCoeffs.size(0) * i] =
            coeffMat[i1 + coeffMat.size(0) * i];
      }
    }
    if (static_cast<unsigned int>(coeffMat.size(0)) + 1U > u) {
      i = 0;
      i1 = 0;
    } else {
      i = coeffMat.size(0);
      i1 = static_cast<int>(u);
    }
    b_loop_ub = i1 - i;
    for (i1 = 0; i1 < 3; i1++) {
      for (int b_i{0}; b_i < b_loop_ub; b_i++) {
        modCoeffs[(i + b_i) + modCoeffs.size(0) * i1] =
            newSegmentCoeffs_data[b_i + p * i1];
      }
    }
    modBreaks.set_size(1, breaksWithFlatStart.size(1) + 1);
    b_loop_ub = breaksWithFlatStart.size(1);
    for (i = 0; i < b_loop_ub; i++) {
      modBreaks[i] = breaksWithFlatStart[i];
    }
    modBreaks[breaksWithFlatStart.size(1)] = parameterMat_tmp + 1.0;
    coeffMat.set_size(modCoeffs.size(0), 3);
    b_loop_ub = modCoeffs.size(0) * 3;
    for (i = 0; i < b_loop_ub; i++) {
      coeffMat[i] = 0.0;
    }
    for (int b_i{0}; b_i < 2; b_i++) {
      b_loop_ub = coeffMat.size(0);
      for (i = 0; i < b_loop_ub; i++) {
        coeffMat[i + coeffMat.size(0) * (b_i + 1)] =
            ((3.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
            modCoeffs[i + modCoeffs.size(0) * b_i];
      }
    }
    ddCoeffs.set_size(coeffMat.size(0), 3);
    b_loop_ub = coeffMat.size(0) * 3;
    for (i = 0; i < b_loop_ub; i++) {
      ddCoeffs[i] = 0.0;
    }
    for (int b_i{0}; b_i < 2; b_i++) {
      b_loop_ub = ddCoeffs.size(0);
      for (i = 0; i < b_loop_ub; i++) {
        ddCoeffs[i + ddCoeffs.size(0) * (b_i + 1)] =
            ((3.0 - (static_cast<double>(b_i) + 2.0)) + 1.0) *
            coeffMat[i + coeffMat.size(0) * b_i];
      }
    }
    ppCell.data[jj].coefs.set_size(p, modBreaks.size(1) - 1, 3);
    idx = p * (modBreaks.size(1) - 1) * 3;
    for (i = 0; i < idx; i++) {
      ppCell.data[jj].coefs[i] = modCoeffs[i];
    }
    ppCell.data[jj].breaks.set_size(1, modBreaks.size(1));
    b_loop_ub = modBreaks.size(1);
    for (i = 0; i < b_loop_ub; i++) {
      ppCell.data[jj].breaks[i] = modBreaks[i];
    }
    expl_temp.coefs.set_size(p, modBreaks.size(1) - 1, 3);
    for (i = 0; i < idx; i++) {
      expl_temp.coefs[i] = coeffMat[i];
    }
    expl_temp.breaks.set_size(1, modBreaks.size(1));
    b_loop_ub = modBreaks.size(1);
    for (i = 0; i < b_loop_ub; i++) {
      expl_temp.breaks[i] = modBreaks[i];
    }
    b_expl_temp.coefs.set_size(p, modBreaks.size(1) - 1, 3);
    for (i = 0; i < idx; i++) {
      b_expl_temp.coefs[i] = ddCoeffs[i];
    }
    b_expl_temp.breaks.set_size(1, modBreaks.size(1));
    b_loop_ub = modBreaks.size(1);
    for (i = 0; i < b_loop_ub; i++) {
      b_expl_temp.breaks[i] = modBreaks[i];
    }
    for (i = 0; i < c_i; i++) {
      tmp_data[i] = static_cast<signed char>(rowSelection_data[i] - 1);
    }
    ppval(ppCell.data[jj], lspbSegIndices, r2);
    for (i = 0; i < loop_ub; i++) {
      for (i1 = 0; i1 < c_i; i1++) {
        q[tmp_data[i1] + 3 * i] = r2[i1 + c_i * i];
      }
    }
    ppval(expl_temp, lspbSegIndices, r2);
    for (i = 0; i < loop_ub; i++) {
      for (i1 = 0; i1 < c_i; i1++) {
        qd[tmp_data[i1] + 3 * i] = r2[i1 + c_i * i];
      }
    }
    ppval(b_expl_temp, lspbSegIndices, r2);
    for (i = 0; i < loop_ub; i++) {
      for (i1 = 0; i1 < c_i; i1++) {
        qdd[tmp_data[i1] + 3 * i] = r2[i1 + c_i * i];
      }
    }
  }
}

} // namespace coder

//
// File trailer for trapveltraj.cpp
//
// [EOF]
//
