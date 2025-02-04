//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ixfun.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "ixfun.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void b_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

// Function Definitions
//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void b_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Arrays have incompatible sizes for this operation.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const array<double, 1U> &a
//                const array<double, 1U> &b
//                array<double, 1U> &c
// Return Type  : void
//
namespace coder {
namespace internal {
void expand_max(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c)
{
  static rtRunTimeErrorInfo c_emlrtRTEI{
      225,         // lineNo
      "expand_max" // fName
  };
  int csz_idx_0;
  int u0;
  if (b.size(0) == 1) {
    csz_idx_0 = a.size(0);
  } else if (a.size(0) == 1) {
    csz_idx_0 = b.size(0);
  } else {
    u0 = a.size(0);
    csz_idx_0 = b.size(0);
    if (u0 <= csz_idx_0) {
      csz_idx_0 = u0;
    }
    if (a.size(0) != b.size(0)) {
      b_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
    }
  }
  c.set_size(csz_idx_0);
  if (csz_idx_0 != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a.size(0) != 1);
    b1 = (b.size(0) != 1);
    u0 = csz_idx_0 - 1;
    for (csz_idx_0 = 0; csz_idx_0 <= u0; csz_idx_0++) {
      c[csz_idx_0] = std::fmax(a[b_b * csz_idx_0], b[b1 * csz_idx_0]);
    }
  }
}

//
// Arguments    : const array<double, 1U> &a
//                const array<double, 1U> &b
//                array<double, 1U> &c
// Return Type  : void
//
void expand_min(const array<double, 1U> &a, const array<double, 1U> &b,
                array<double, 1U> &c)
{
  static rtRunTimeErrorInfo c_emlrtRTEI{
      225,         // lineNo
      "expand_min" // fName
  };
  int csz_idx_0;
  int u0;
  if (b.size(0) == 1) {
    csz_idx_0 = a.size(0);
  } else if (a.size(0) == 1) {
    csz_idx_0 = b.size(0);
  } else {
    u0 = a.size(0);
    csz_idx_0 = b.size(0);
    if (u0 <= csz_idx_0) {
      csz_idx_0 = u0;
    }
    if (a.size(0) != b.size(0)) {
      b_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
    }
  }
  c.set_size(csz_idx_0);
  if (csz_idx_0 != 0) {
    boolean_T b1;
    boolean_T b_b;
    b_b = (a.size(0) != 1);
    b1 = (b.size(0) != 1);
    u0 = csz_idx_0 - 1;
    for (csz_idx_0 = 0; csz_idx_0 <= u0; csz_idx_0++) {
      c[csz_idx_0] = std::fmin(a[b_b * csz_idx_0], b[b1 * csz_idx_0]);
    }
  }
}

} // namespace internal
} // namespace coder

//
// File trailer for ixfun.cpp
//
// [EOF]
//
