//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOA_types.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef INVERSEKINEMATICSOA_TYPES_H
#define INVERSEKINEMATICSOA_TYPES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"

// Type Definitions
struct struct0_T {
  boolean_T useObjectiveNormInfinity;
  double weightNormInfinity;
  boolean_T useObjectiveNormL2;
  double weightNormL2;
  boolean_T useObjectiveTrajectoryFollowing;
  double weightTrajectoryFollowing;
  boolean_T useObjectiveJointAcceleration;
  double weightJointAcceleration;
  boolean_T useObjectiveManipulability;
  double weightManipulability;
  boolean_T applyEqualityConstraints;
  boolean_T applyInequalityConstraints;
  boolean_T applySlack;
  double Slacklowerbound[6];
  double Slackupperbound[6];
  double Slack_penalty_weightmatrix[36];
  double Slack_objective_weight;
  boolean_T obstacle_avoidance_scheme;
  double k;
  double gamma;
  boolean_T applyVelocityDamper;
  double jointLimitActivationDistance;
  double jointLimitStopDistance;
  double jointLimitGain;
};
typedef struct0_T  structIkWithOA;

struct inverseKinematicsOAPersistentData {
  coder::array<double, 1U> starting_joint_vel;
  boolean_T starting_joint_vel_not_empty;
  boolean_T is_first_step;
  boolean_T is_first_step_not_empty;
};

struct inverseKinematicsOAStackData {
  inverseKinematicsOAPersistentData *pd;
};

#endif
//
// File trailer for inverseKinematicsOA_types.h
//
// [EOF]
//
