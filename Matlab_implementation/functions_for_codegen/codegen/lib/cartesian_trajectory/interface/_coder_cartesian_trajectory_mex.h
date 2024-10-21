//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_cartesian_trajectory_mex.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

#ifndef _CODER_CARTESIAN_TRAJECTORY_MEX_H
#define _CODER_CARTESIAN_TRAJECTORY_MEX_H

// Include Files
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

// Function Declarations
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS();

void unsafe_cartesian_trajectory_mexFunction(int32_T nlhs, mxArray *plhs[3],
                                             int32_T nrhs,
                                             const mxArray *prhs[6]);

#endif
//
// File trailer for _coder_cartesian_trajectory_mex.h
//
// [EOF]
//
