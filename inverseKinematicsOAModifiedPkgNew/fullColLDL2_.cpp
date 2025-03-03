//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fullColLDL2_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "fullColLDL2_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo u_emlrtBCI{
    -1,             // iFirst
    -1,             // iLast
    1,              // lineNo
    1,              // colNo
    "",             // aName
    "fullColLDL2_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
    "+DynamicRegCholManager/fullColLDL2_.p", // pName
    0                                        // checkKind
};

// Function Definitions
//
// Arguments    : f_struct_T &obj
//                int NColsRemain
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace DynamicRegCholManager {
void fullColLDL2_(f_struct_T &obj, int NColsRemain)
{
  array<double, 2U> *A;
  double y;
  int LDimSizeP1_tmp;
  int jA;
  int lastDiag;
  LDimSizeP1_tmp = obj.ldm + 1;
  if (NColsRemain > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k < NColsRemain; k++) {
    double alpha1;
    int LD_diagOffset;
    int b;
    int offset1;
    int subMatrixDim;
    LD_diagOffset = LDimSizeP1_tmp * k + 1;
    jA = obj.FMat.size(0) * obj.FMat.size(1);
    if ((LD_diagOffset < 1) || (LD_diagOffset > jA)) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    y = obj.FMat[LD_diagOffset - 1];
    if (std::abs(y) <= obj.regTol_) {
      if (LD_diagOffset > jA) {
        rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
      }
      obj.FMat[LD_diagOffset - 1] = y;
    }
    if (LD_diagOffset > jA) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    subMatrixDim = (NColsRemain - k) - 1;
    offset1 = LD_diagOffset + 1;
    y = obj.workspace_;
    if (subMatrixDim > 2147483646) {
      check_forloop_overflow_error();
    }
    for (lastDiag = 0; lastDiag < subMatrixDim; lastDiag++) {
      y = obj.FMat[LD_diagOffset + lastDiag];
    }
    obj.workspace_ = y;
    alpha1 = -1.0 / obj.FMat[LD_diagOffset - 1];
    A = &obj.FMat;
    lastDiag = obj.ldm;
    if (!(alpha1 == 0.0)) {
      jA = LD_diagOffset + LDimSizeP1_tmp;
      for (int j{0}; j < subMatrixDim; j++) {
        if (y != 0.0) {
          double temp;
          temp = y * alpha1;
          b = (subMatrixDim + jA) - 1;
          if ((jA <= b) && (b > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (int ijA{jA}; ijA <= b; ijA++) {
            (*A)[ijA - 1] = (*A)[ijA - 1] + y * temp;
          }
        }
        jA += lastDiag;
      }
    }
    jA = obj.FMat.size(0) * obj.FMat.size(1);
    if (LD_diagOffset > jA) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    y = 1.0 / obj.FMat[LD_diagOffset - 1];
    b = LD_diagOffset + subMatrixDim;
    if ((LD_diagOffset + 1 <= b) && (b > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (lastDiag = offset1; lastDiag <= b; lastDiag++) {
      obj.FMat[lastDiag - 1] = y * obj.FMat[lastDiag - 1];
    }
  }
  lastDiag = LDimSizeP1_tmp * (NColsRemain - 1) + 1;
  jA = obj.FMat.size(0) * obj.FMat.size(1);
  if ((lastDiag < 1) || (lastDiag > jA)) {
    rtDynamicBoundsError(lastDiag, 1, jA, u_emlrtBCI);
  }
  y = obj.FMat[lastDiag - 1];
  if (std::abs(y) <= obj.regTol_) {
    if (lastDiag > jA) {
      rtDynamicBoundsError(lastDiag, 1, jA, u_emlrtBCI);
    }
    obj.FMat[lastDiag - 1] = y;
  }
}

//
// Arguments    : f_struct_T &obj
//                int LD_offset
//                int NColsRemain
// Return Type  : void
//
void fullColLDL2_(f_struct_T &obj, int LD_offset, int NColsRemain)
{
  array<double, 2U> *A;
  double y;
  int LDimSizeP1_tmp;
  int jA;
  int lastDiag;
  LDimSizeP1_tmp = obj.ldm + 1;
  if (NColsRemain > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k < NColsRemain; k++) {
    double alpha1;
    int LD_diagOffset;
    int b;
    int offset1;
    int subMatrixDim;
    LD_diagOffset = LD_offset + LDimSizeP1_tmp * k;
    jA = obj.FMat.size(0) * obj.FMat.size(1);
    if ((LD_diagOffset < 1) || (LD_diagOffset > jA)) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    y = obj.FMat[LD_diagOffset - 1];
    if (std::abs(y) <= obj.regTol_) {
      if (LD_diagOffset > jA) {
        rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
      }
      obj.FMat[LD_diagOffset - 1] = y;
    }
    if (LD_diagOffset > jA) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    subMatrixDim = (NColsRemain - k) - 1;
    offset1 = LD_diagOffset + 1;
    y = obj.workspace_;
    if (subMatrixDim > 2147483646) {
      check_forloop_overflow_error();
    }
    for (lastDiag = 0; lastDiag < subMatrixDim; lastDiag++) {
      y = obj.FMat[LD_diagOffset + lastDiag];
    }
    obj.workspace_ = y;
    alpha1 = -1.0 / obj.FMat[LD_diagOffset - 1];
    A = &obj.FMat;
    lastDiag = obj.ldm;
    if (!(alpha1 == 0.0)) {
      jA = LD_diagOffset + LDimSizeP1_tmp;
      for (int j{0}; j < subMatrixDim; j++) {
        if (y != 0.0) {
          double temp;
          temp = y * alpha1;
          b = (subMatrixDim + jA) - 1;
          if ((jA <= b) && (b > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (int ijA{jA}; ijA <= b; ijA++) {
            (*A)[ijA - 1] = (*A)[ijA - 1] + y * temp;
          }
        }
        jA += lastDiag;
      }
    }
    jA = obj.FMat.size(0) * obj.FMat.size(1);
    if (LD_diagOffset > jA) {
      rtDynamicBoundsError(LD_diagOffset, 1, jA, u_emlrtBCI);
    }
    y = 1.0 / obj.FMat[LD_diagOffset - 1];
    b = LD_diagOffset + subMatrixDim;
    if ((LD_diagOffset + 1 <= b) && (b > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (lastDiag = offset1; lastDiag <= b; lastDiag++) {
      obj.FMat[lastDiag - 1] = y * obj.FMat[lastDiag - 1];
    }
  }
  lastDiag = LD_offset + LDimSizeP1_tmp * (NColsRemain - 1);
  jA = obj.FMat.size(0) * obj.FMat.size(1);
  if ((lastDiag < 1) || (lastDiag > jA)) {
    rtDynamicBoundsError(lastDiag, 1, jA, u_emlrtBCI);
  }
  y = obj.FMat[lastDiag - 1];
  if (std::abs(y) <= obj.regTol_) {
    if (lastDiag > jA) {
      rtDynamicBoundsError(lastDiag, 1, jA, u_emlrtBCI);
    }
    obj.FMat[lastDiag - 1] = y;
  }
}

} // namespace DynamicRegCholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for fullColLDL2_.cpp
//
// [EOF]
//
