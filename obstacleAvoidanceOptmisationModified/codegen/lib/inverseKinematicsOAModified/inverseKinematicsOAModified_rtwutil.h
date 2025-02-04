//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_rtwutil.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef INVERSEKINEMATICSOAMODIFIED_RTWUTIL_H
#define INVERSEKINEMATICSOAMODIFIED_RTWUTIL_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct rtBoundsCheckInfo;

// Function Declarations
extern void d_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

extern void m_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

extern void rtDynamicBoundsError(int aIndexValue, int aLoBound, int aHiBound,
                                 const rtBoundsCheckInfo &aInfo);

extern void rtErrorWithMessageID(const char *r, const char *aFcnName,
                                 int aLineNum);

extern boolean_T rtIsNullOrEmptyString(const char *aString);

#endif
//
// File trailer for inverseKinematicsOAModified_rtwutil.h
//
// [EOF]
//
