//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factor.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "factor.h"
#include "fullColLDL2_.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "partialColLDL3_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : f_struct_T &obj
//                const array<double, 2U> &A
//                int ndims
//                int ldA
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace DynamicRegCholManager {
void factor(f_struct_T &obj, const array<double, 2U> &A, int ndims, int ldA)
{
  int A_maxDiag_idx;
  int LDimSizeP1;
  int idx;
  int k;
  int order;
  LDimSizeP1 = obj.ldm + 1;
  obj.ndims = ndims;
  if ((A.size(0) != 0) && (A.size(1) != 0)) {
    for (idx = 0; idx < ndims; idx++) {
      A_maxDiag_idx = ldA * idx;
      order = obj.ldm * idx;
      for (k = 0; k < ndims; k++) {
        obj.FMat[order + k] = A[A_maxDiag_idx + k];
      }
    }
  }
  if (ndims < 1) {
    A_maxDiag_idx = -1;
  } else {
    A_maxDiag_idx = 0;
    if (ndims > 1) {
      double smax;
      smax = std::abs(obj.FMat[0]);
      for (k = 2; k <= ndims; k++) {
        double s;
        s = std::abs(obj.FMat[(k - 1) * LDimSizeP1]);
        if (s > smax) {
          A_maxDiag_idx = k - 1;
          smax = s;
        }
      }
    }
  }
  obj.regTol_ =
      std::fmax(std::abs(obj.FMat[A_maxDiag_idx + obj.ldm * A_maxDiag_idx]) *
                    2.2204460492503131E-16,
                0.0);
  if ((obj.FMat.size(0) * obj.FMat.size(1) > 16384) && (ndims > 128)) {
    boolean_T exitg1;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < ndims)) {
      A_maxDiag_idx = LDimSizeP1 * k + 1;
      order = ndims - k;
      if (k + 48 <= ndims) {
        partialColLDL3_(obj, A_maxDiag_idx, order);
        k += 48;
      } else {
        fullColLDL2_(obj, A_maxDiag_idx, order);
        exitg1 = true;
      }
    }
  } else {
    fullColLDL2_(obj, 1, ndims);
  }
  if (obj.ConvexCheck) {
    idx = 0;
    int exitg2;
    do {
      exitg2 = 0;
      if (idx <= ndims - 1) {
        if (obj.FMat[idx + obj.ldm * idx] <= 0.0) {
          obj.info = -idx - 1;
          exitg2 = 1;
        } else {
          idx++;
        }
      } else {
        obj.ConvexCheck = false;
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  }
}

} // namespace DynamicRegCholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factor.cpp
//
// [EOF]
//
