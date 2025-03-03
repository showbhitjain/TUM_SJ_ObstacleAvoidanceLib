//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: indexShapeCheck.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "indexShapeCheck.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void i_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

// Function Definitions
//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void i_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream
      << "Compile-time size assumption violated. At run-time, a vector indexes "
         "a scalar, but the compile-time assumption is that the vecto"
         "r is indexed by a vector of the same size.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : int matrixSize
//                const int indexSize[2]
// Return Type  : void
//
namespace coder {
namespace internal {
void indexShapeCheck(int matrixSize, const int indexSize[2])
{
  static rtRunTimeErrorInfo e_emlrtRTEI{
      122,          // lineNo
      "errOrWarnIf" // fName
  };
  if ((matrixSize == 1) && (indexSize[1] != 1)) {
    i_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
}

} // namespace internal
} // namespace coder

//
// File trailer for indexShapeCheck.cpp
//
// [EOF]
//
