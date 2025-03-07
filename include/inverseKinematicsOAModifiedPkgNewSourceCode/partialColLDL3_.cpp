//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: partialColLDL3_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "partialColLDL3_.h"
#include "inverseKinematicsOAModified_internal_types.h"
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
void partialColLDL3_(f_struct_T &obj, int LD_offset, int NColsRemain)
{
  double d;
  int LD_diagOffset;
  int LDimSizeP1;
  int i;
  int i1;
  int i2;
  int iac;
  int idx;
  int ix;
  int subRows;
  LDimSizeP1 = obj.ldm + 1;
  i = NColsRemain - 1;
  for (int k{0}; k < 48; k++) {
    double y;
    subRows = (NColsRemain - k) - 1;
    LD_diagOffset = (LD_offset + LDimSizeP1 * k) - 1;
    for (idx = 0; idx <= subRows; idx++) {
      obj.workspace_ = obj.FMat[LD_diagOffset + idx];
    }
    for (idx = 0; idx <= i; idx++) {
      obj.workspace2_ = obj.workspace_;
    }
    idx = obj.ldm;
    y = obj.workspace2_;
    if ((NColsRemain != 0) && (k != 0)) {
      ix = LD_offset + k;
      i1 = obj.ldm * (k - 1) + 1;
      for (iac = 1; idx < 0 ? iac >= i1 : iac <= i1; iac += idx) {
        i2 = (iac + NColsRemain) - 1;
        for (int ia{iac}; ia <= i2; ia++) {
          y += obj.workspace_ * -obj.FMat[ix - 1];
        }
        ix += obj.ldm;
      }
    }
    obj.workspace2_ = y;
    for (idx = 0; idx <= i; idx++) {
      obj.workspace_ = y;
    }
    for (idx = 0; idx <= subRows; idx++) {
      obj.FMat[LD_diagOffset + idx] = obj.workspace_;
    }
    if (std::abs(obj.FMat[LD_diagOffset]) <= obj.regTol_) {
      obj.FMat[LD_diagOffset] = obj.FMat[LD_diagOffset];
    }
    for (idx = 0; idx < subRows; idx++) {
      i1 = (LD_diagOffset + idx) + 1;
      obj.FMat[i1] = obj.FMat[i1] / obj.FMat[LD_diagOffset];
    }
  }
  for (int j{48}; j <= i; j += 48) {
    int i3;
    int iy0;
    int m;
    int subBlockSize;
    int u1_tmp;
    u1_tmp = NColsRemain - j;
    if (u1_tmp >= 48) {
      subBlockSize = 48;
    } else {
      subBlockSize = u1_tmp;
    }
    i1 = j + subBlockSize;
    i2 = i1 - 1;
    for (int k{j}; k <= i2; k++) {
      m = i1 - k;
      iy0 = (LD_offset + LDimSizeP1 * k) - 1;
      i3 = obj.ldm;
      for (idx = 0; idx < 48; idx++) {
        d = obj.FMat[((LD_offset + k) + idx * i3) - 1];
      }
      obj.workspace2_ = d;
      ix = k + 1;
      idx = obj.ldm;
      if (m != 0) {
        i3 = (k + obj.ldm * 47) + 1;
        for (iac = ix; idx < 0 ? iac >= i3 : iac <= i3; iac += idx) {
          subRows = (iac + m) - 1;
          for (int ia{iac}; ia <= subRows; ia++) {
            LD_diagOffset = (iy0 + ia) - iac;
            obj.FMat[LD_diagOffset] =
                obj.FMat[LD_diagOffset] + obj.workspace_ * -obj.workspace2_;
          }
        }
      }
    }
    if (i1 < NColsRemain) {
      m = u1_tmp - subBlockSize;
      iac = ((LD_offset + subBlockSize) + LDimSizeP1 * j) - 1;
      i1 = subBlockSize - 1;
      for (idx = 0; idx < 48; idx++) {
        ix = (LD_offset + j) + idx * obj.ldm;
        for (subRows = 0; subRows <= i1; subRows++) {
          obj.workspace2_ = obj.FMat[(ix + subRows) - 1];
        }
      }
      LD_diagOffset = obj.ldm;
      if ((m != 0) && (subBlockSize != 0)) {
        ix = iac + obj.ldm * (subBlockSize - 1);
        subRows = 0;
        for (idx = iac; LD_diagOffset < 0 ? idx >= ix : idx <= ix;
             idx += LD_diagOffset) {
          subRows++;
          i1 = subRows + LD_diagOffset * 47;
          for (u1_tmp = subRows;
               LD_diagOffset < 0 ? u1_tmp >= i1 : u1_tmp <= i1;
               u1_tmp += LD_diagOffset) {
            i2 = idx + 1;
            i3 = idx + m;
            for (iy0 = i2; iy0 <= i3; iy0++) {
              obj.FMat[iy0 - 1] =
                  obj.FMat[iy0 - 1] + -obj.workspace2_ * obj.workspace_;
            }
          }
        }
      }
    }
  }
}

} // namespace DynamicRegCholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for partialColLDL3_.cpp
//
// [EOF]
//
