//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factor1.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "factor1.h"
#include "eml_int_forloop_overflow_check.h"
#include "fullColLDL2_.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
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
  int LD_diagOffset;
  int LDimSizeP1;
  int idx;
  int k;
  int order;
  LDimSizeP1 = obj.ldm + 1;
  obj.ndims = ndims;
  if ((A.size(0) != 0) && (A.size(1) != 0)) {
    if (ndims > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < ndims; idx++) {
      LD_diagOffset = ldA * idx;
      order = obj.ldm * idx;
      for (k = 0; k < ndims; k++) {
        obj.FMat[order + k] = A[LD_diagOffset + k];
      }
    }
  }
  LD_diagOffset = obj.ldm + 1;
  if (ndims < 1) {
    idx = 0;
  } else {
    idx = 1;
    if (ndims > 1) {
      double smax;
      smax = std::abs(obj.FMat[0]);
      if (ndims > 2147483646) {
        check_forloop_overflow_error();
      }
      for (k = 2; k <= ndims; k++) {
        double s;
        s = std::abs(obj.FMat[(k - 1) * LD_diagOffset]);
        if (s > smax) {
          idx = k;
          smax = s;
        }
      }
    }
  }
  LD_diagOffset = obj.FMat.size(0) * obj.FMat.size(1);
  order = idx + obj.ldm * (idx - 1);
  if ((order < 1) || (order > LD_diagOffset)) {
    rtDynamicBoundsError(order, 1, LD_diagOffset, s_emlrtBCI);
  }
  obj.regTol_ = std::fmax(std::abs(obj.FMat[(idx + obj.ldm * (idx - 1)) - 1]) *
                              2.2204460492503131E-16,
                          0.0);
  if ((obj.FMat.size(0) * obj.FMat.size(1) > 16384) && (ndims > 128)) {
    boolean_T exitg1;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k < ndims)) {
      LD_diagOffset = LDimSizeP1 * k + 1;
      order = ndims - k;
      if (k + 48 <= ndims) {
        partialColLDL3_(obj, LD_diagOffset, order);
        k += 48;
      } else {
        fullColLDL2_(obj, LD_diagOffset, order);
        exitg1 = true;
      }
    }
  } else {
    fullColLDL2_(obj, ndims);
  }
  if (obj.ConvexCheck) {
    idx = 0;
    int exitg2;
    do {
      exitg2 = 0;
      if (idx <= ndims - 1) {
        LD_diagOffset = obj.FMat.size(0) * obj.FMat.size(1);
        order = (idx + obj.ldm * idx) + 1;
        if ((order < 1) || (order > LD_diagOffset)) {
          rtDynamicBoundsError(order, 1, LD_diagOffset, s_emlrtBCI);
        }
        if (obj.FMat[order - 1] <= 0.0) {
          obj.info = -(idx + 1);
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
// File trailer for factor1.cpp
//
// [EOF]
//
