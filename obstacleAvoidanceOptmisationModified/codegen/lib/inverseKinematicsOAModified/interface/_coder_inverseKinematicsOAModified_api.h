//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_inverseKinematicsOAModified_api.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef _CODER_INVERSEKINEMATICSOAMODIFIED_API_H
#define _CODER_INVERSEKINEMATICSOAMODIFIED_API_H

// Include Files
#include "coder_array_mex.h"
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Type Definitions
struct struct0_T {
  boolean_T useObjectiveNormInfinity;
  real_T weightNormInfinity;
  boolean_T useObjectiveNormL2;
  real_T weightNormL2;
  boolean_T useObjectiveTrajectoryFollowing;
  real_T weightTrajectoryFollowing;
  boolean_T useObjectiveJointAcceleration;
  real_T weightJointAcceleration;
  boolean_T useObjectiveManipulability;
  real_T weightManipulability;
  boolean_T applyEqualityConstraints;
  boolean_T applyInequalityConstraints;
  boolean_T applySlack;
  real_T Slacklowerbound[6];
  real_T Slackupperbound[6];
  real_T SlackPenaltyWeight[6];
  real_T SlackObjectiveWeight;
  boolean_T obstacleAvoidanceScheme;
  real_T k;
  real_T gamma;
  boolean_T dynamicSlack;
  boolean_T applyVelocityDamper;
  real_T jointLimitActivationDistance;
  real_T jointLimitStopDistance;
  real_T jointLimitGain;
};

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void inverseKinematicsOAModified(
    coder::array<real_T, 1U> *joint_values, coder::array<real_T, 2U> *jacobi,
    real_T xd_eff_vel[6], coder::array<real_T, 1U> *jointminvalues,
    coder::array<real_T, 1U> *jointmaxvalues,
    coder::array<real_T, 1U> *joint_min_vel,
    coder::array<real_T, 1U> *joint_max_vel, coder::array<real_T, 2U> *J_g,
    coder::array<real_T, 1U> *b_g,
    coder::array<real_T, 2U> *jointVelocityWeightMatrix, struct0_T *configInput,
    coder::array<real_T, 1U> *optimal_joint_velocity, real_T *Exit_Flag);

void inverseKinematicsOAModified_api(const mxArray *const prhs[11],
                                     int32_T nlhs, const mxArray *plhs[2]);

void inverseKinematicsOAModified_atexit();

void inverseKinematicsOAModified_initialize();

void inverseKinematicsOAModified_terminate();

void inverseKinematicsOAModified_xil_shutdown();

void inverseKinematicsOAModified_xil_terminate();

#endif
//
// File trailer for _coder_inverseKinematicsOAModified_api.h
//
// [EOF]
//
