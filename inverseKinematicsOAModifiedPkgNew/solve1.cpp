//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: solve1.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "solve1.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const f_struct_T &obj
//                array<double, 1U> &rhs
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace DynamicRegCholManager {
void solve(const f_struct_T &obj, array<double, 1U> &rhs)
{
  int b;
  int ix;
  int jA;
  int jjA;
  jA = obj.ndims - 1;
  if ((rhs.size(0) != 0) && (obj.ndims != 0)) {
    if (obj.ndims > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int j{0}; j <= jA; j++) {
      jjA = j + j * obj.ldm;
      b = jA - j;
      if (b > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int i{0}; i < b; i++) {
        ix = (j + i) + 1;
        rhs[ix] = rhs[ix] - rhs[j] * obj.FMat[(jjA + i) + 1];
      }
    }
  }
  b = obj.ndims;
  if (obj.ndims > 2147483646) {
    check_forloop_overflow_error();
  }
  for (jjA = 0; jjA < b; jjA++) {
    ix = rhs.size(0);
    if ((jjA + 1 < 1) || (jjA + 1 > ix)) {
      rtDynamicBoundsError(jjA + 1, 1, ix, t_emlrtBCI);
    }
    ix = obj.FMat.size(0) * obj.FMat.size(1);
    jA = (jjA + obj.ldm * jjA) + 1;
    if ((jA < 1) || (jA > ix)) {
      rtDynamicBoundsError(jA, 1, ix, t_emlrtBCI);
    }
    ix = rhs.size(0);
    if (jjA + 1 > ix) {
      rtDynamicBoundsError(jjA + 1, 1, ix, t_emlrtBCI);
    }
    rhs[jjA] = rhs[jjA] / obj.FMat[jA - 1];
  }
  if ((rhs.size(0) != 0) && (obj.ndims != 0)) {
    for (int j{b}; j >= 1; j--) {
      double temp;
      jA = (j - 1) * obj.ldm;
      temp = rhs[j - 1];
      ix = j + 1;
      for (int i{b}; i >= ix; i--) {
        temp -= obj.FMat[(jA + i) - 1] * rhs[i - 1];
      }
      rhs[j - 1] = temp;
    }
  }
}

} // namespace DynamicRegCholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for solve1.cpp
//
// [EOF]
//
