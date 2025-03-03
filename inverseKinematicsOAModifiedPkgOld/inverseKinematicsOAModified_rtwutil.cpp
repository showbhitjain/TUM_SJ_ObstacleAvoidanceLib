//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_rtwutil.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Definitions
//
// Arguments    : const char *r
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
void d_rtErrorWithMessageID(const char *r, const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "Expected ") << r) << " to be nonempty.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
void m_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Vector expression with zero stride is not supported.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : int aIndexValue
//                int aLoBound
//                int aHiBound
//                const rtBoundsCheckInfo &aInfo
// Return Type  : void
//
void rtDynamicBoundsError(int aIndexValue, int aLoBound, int aHiBound,
                          const rtBoundsCheckInfo &aInfo)
{
  std::stringstream b_outStream;
  std::stringstream outStream;
  if (aLoBound == 0) {
    aIndexValue++;
    aLoBound = 1;
    aHiBound++;
  }
  if (rtIsNullOrEmptyString(aInfo.aName)) {
    ((((((b_outStream << "Index exceeds array dimensions. Index value ")
         << aIndexValue)
        << " exceeds valid range [")
       << aLoBound)
      << "-")
     << aHiBound)
        << "].";
    b_outStream << "\n";
    ((((b_outStream << "Error in ") << aInfo.fName) << " (line ")
     << aInfo.lineNo)
        << ")";
    throw std::runtime_error(b_outStream.str());
  } else {
    ((((((((outStream << "Index exceeds array dimensions. Index value ")
           << aIndexValue)
          << " exceeds valid range [")
         << aLoBound)
        << "-")
       << aHiBound)
      << "] for array \'")
     << aInfo.aName)
        << "\'.";
    outStream << "\n";
    ((((outStream << "Error in ") << aInfo.fName) << " (line ") << aInfo.lineNo)
        << ")";
    throw std::runtime_error(outStream.str());
  }
}

//
// Arguments    : const char *r
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
void rtErrorWithMessageID(const char *r, const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "\'") << r) << "\' must contain only finite values.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aString
// Return Type  : boolean_T
//
boolean_T rtIsNullOrEmptyString(const char *aString)
{
  return (aString == nullptr) || (*aString == '\x00');
}

//
// File trailer for inverseKinematicsOAModified_rtwutil.cpp
//
// [EOF]
//
