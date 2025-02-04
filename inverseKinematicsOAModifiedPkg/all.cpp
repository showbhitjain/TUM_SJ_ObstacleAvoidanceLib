//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: all.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "all.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<boolean_T, 1U> &x
// Return Type  : boolean_T
//
namespace coder {
boolean_T all(const array<boolean_T, 1U> &x)
{
  int ix;
  boolean_T exitg1;
  boolean_T y;
  y = true;
  if (x.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  ix = 1;
  exitg1 = false;
  while ((!exitg1) && (ix <= x.size(0))) {
    if (!x[ix - 1]) {
      y = false;
      exitg1 = true;
    } else {
      ix++;
    }
  }
  return y;
}

} // namespace coder

//
// File trailer for all.cpp
//
// [EOF]
//
