//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eml_int_forloop_overflow_check.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void c_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

// Function Definitions
//
// Arguments    : const char *r
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void c_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "The loop variable of class ") << r)
      << " might overflow on the last iteration of the for loop. This could "
         "lead to an infinite loop.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : void
// Return Type  : void
//
namespace coder {
void check_forloop_overflow_error()
{
  static rtRunTimeErrorInfo c_emlrtRTEI{
      87,                            // lineNo
      "check_forloop_overflow_error" // fName
  };
  c_rtErrorWithMessageID("int32", c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
}

} // namespace coder

//
// File trailer for eml_int_forloop_overflow_check.cpp
//
// [EOF]
//
