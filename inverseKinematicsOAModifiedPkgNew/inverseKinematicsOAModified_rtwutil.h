//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_rtwutil.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef INVERSEKINEMATICSOAMODIFIED_RTWUTIL_H
#define INVERSEKINEMATICSOAMODIFIED_RTWUTIL_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct rtBoundsCheckInfo;

struct rtEqualityCheckInfo;

// Function Declarations
extern void d_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

extern void emlrtDimSizeImpxCheckR2021b(const int aDim1, const int aDim2,
                                        const rtEqualityCheckInfo &aInfo);

extern void g_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

extern void h_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

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
