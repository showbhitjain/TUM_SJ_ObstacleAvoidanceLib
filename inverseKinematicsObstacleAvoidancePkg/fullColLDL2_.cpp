//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fullColLDL2_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "fullColLDL2_.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : f_struct_T &obj
//                int LD_offset
//                int NColsRemain
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace DynamicRegCholManager {
void fullColLDL2_(f_struct_T &obj, int LD_offset, int NColsRemain)
{
  int LDimSizeP1;
  int jA;
  LDimSizeP1 = obj.ldm;
  for (int k{0}; k < NColsRemain; k++) {
    double alpha1;
    double y;
    int LD_diagOffset;
    int i;
    int offset1;
    int subMatrixDim;
    LD_diagOffset = (LD_offset + (LDimSizeP1 + 1) * k) - 1;
    if (std::abs(obj.FMat[LD_diagOffset]) <= obj.regTol_) {
      obj.FMat[LD_diagOffset] = obj.FMat[LD_diagOffset];
    }
    alpha1 = -1.0 / obj.FMat[LD_diagOffset];
    subMatrixDim = NColsRemain - k;
    offset1 = LD_diagOffset + 2;
    y = obj.workspace_;
    for (jA = 0; jA <= subMatrixDim - 2; jA++) {
      y = obj.FMat[(LD_diagOffset + jA) + 1];
    }
    obj.workspace_ = y;
    if (!(alpha1 == 0.0)) {
      jA = LD_diagOffset + LDimSizeP1;
      for (int j{0}; j <= subMatrixDim - 2; j++) {
        if (y != 0.0) {
          double temp;
          int i1;
          temp = y * alpha1;
          i = jA + 2;
          i1 = subMatrixDim + jA;
          for (int ijA{i}; ijA <= i1; ijA++) {
            obj.FMat[ijA - 1] = obj.FMat[ijA - 1] + y * temp;
          }
        }
        jA += obj.ldm;
      }
    }
    alpha1 = 1.0 / obj.FMat[LD_diagOffset];
    i = LD_diagOffset + subMatrixDim;
    for (jA = offset1; jA <= i; jA++) {
      obj.FMat[jA - 1] = alpha1 * obj.FMat[jA - 1];
    }
  }
  jA = (LD_offset + (obj.ldm + 1) * (NColsRemain - 1)) - 1;
  if (std::abs(obj.FMat[jA]) <= obj.regTol_) {
    obj.FMat[jA] = obj.FMat[jA];
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
