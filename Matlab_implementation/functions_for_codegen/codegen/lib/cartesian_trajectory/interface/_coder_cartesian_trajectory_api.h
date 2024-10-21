//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_cartesian_trajectory_api.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

#ifndef _CODER_CARTESIAN_TRAJECTORY_API_H
#define _CODER_CARTESIAN_TRAJECTORY_API_H

// Include Files
#include "coder_array_mex.h"
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void cartesian_trajectory(coder::array<real_T, 2U> *waypoints,
                          coder::array<real_T, 2U> *waypointTimes, real_T ts,
                          char_T trajType_data[], int32_T trajType_size[2],
                          coder::array<real_T, 2U> *waypointVels,
                          coder::array<real_T, 2U> *waypointAccels,
                          coder::array<real_T, 2U> *xd,
                          coder::array<real_T, 2U> *xd_vel,
                          coder::array<real_T, 2U> *xdd);

void cartesian_trajectory_api(const mxArray *const prhs[6], int32_T nlhs,
                              const mxArray *plhs[3]);

void cartesian_trajectory_atexit();

void cartesian_trajectory_initialize();

void cartesian_trajectory_terminate();

void cartesian_trajectory_xil_shutdown();

void cartesian_trajectory_xil_terminate();

#endif
//
// File trailer for _coder_cartesian_trajectory_api.h
//
// [EOF]
//
