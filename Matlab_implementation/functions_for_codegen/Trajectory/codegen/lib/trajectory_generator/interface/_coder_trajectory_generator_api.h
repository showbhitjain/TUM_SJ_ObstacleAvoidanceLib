//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_trajectory_generator_api.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

#ifndef _CODER_TRAJECTORY_GENERATOR_API_H
#define _CODER_TRAJECTORY_GENERATOR_API_H

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
                          coder::array<char_T, 2U> *trajType,
                          coder::array<real_T, 2U> *waypointVels,
                          coder::array<real_T, 2U> *waypointAccels,
                          coder::array<real_T, 2U> *xd,
                          coder::array<real_T, 2U> *xd_vel,
                          coder::array<real_T, 2U> *xdd);

void cartesian_trajectory_api(const mxArray *const prhs[6], int32_T nlhs,
                              const mxArray *plhs[3]);

void orientation_trajectory(coder::array<real_T, 2U> *orientations,
                            coder::array<real_T, 2U> *waypointTimes, real_T ts,
                            coder::array<char_T, 2U> *trajType,
                            coder::array<real_T, 2U> *desired_quaternions,
                            coder::array<real_T, 2U> *desired_angular_velocity,
                            coder::array<real_T, 2U> *desired_angular_accel);

void orientation_trajectory_api(const mxArray *const prhs[4], int32_T nlhs,
                                const mxArray *plhs[3]);

void trajectory_generator_atexit();

void trajectory_generator_initialize();

void trajectory_generator_terminate();

void trajectory_generator_xil_shutdown();

void trajectory_generator_xil_terminate();

#endif
//
// File trailer for _coder_trajectory_generator_api.h
//
// [EOF]
//
