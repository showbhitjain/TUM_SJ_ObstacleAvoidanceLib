//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factor.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "factor.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "xpotrf.h"
#include "coder_array.h"
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
namespace CholManager {
void factor(f_struct_T &obj, const array<double, 2U> &A, int ndims, int ldA)
{
  obj.ndims = ndims;
  if ((A.size(0) != 0) && (A.size(1) != 0)) {
    if (ndims > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < ndims; idx++) {
      int ix0;
      int iy0;
      ix0 = ldA * idx;
      iy0 = obj.ldm * idx;
      for (int k{0}; k < ndims; k++) {
        obj.FMat[iy0 + k] = A[ix0 + k];
      }
    }
  }
  obj.info = internal::lapack::xpotrf(ndims, obj.FMat, obj.ldm);
}

} // namespace CholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factor.cpp
//
// [EOF]
//
