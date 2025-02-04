//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_types.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef INVERSEKINEMATICSOAMODIFIED_TYPES_H
#define INVERSEKINEMATICSOAMODIFIED_TYPES_H

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
  double SlackPenaltyWeight[6];
  double SlackObjectiveWeight;
  boolean_T obstacleAvoidanceScheme;
  double k;
  double gamma;
  boolean_T dynamicSlack;
  boolean_T applyVelocityDamper;
  double jointLimitActivationDistance;
  double jointLimitStopDistance;
  double jointLimitGain;
};

struct rtBoundsCheckInfo {
  int iFirst;
  int iLast;
  int lineNo;
  int colNo;
  const char *aName;
  const char *fName;
  const char *pName;
  int checkKind;
};

struct rtRunTimeErrorInfo {
  int lineNo;
  const char *fName;
};

struct inverseKinematicsOAModifiedPersistentData {
  coder::array<double, 1U> starting_joint_vel;
  boolean_T starting_joint_vel_not_empty;
  boolean_T is_first_step;
  boolean_T is_first_step_not_empty;
};

struct inverseKinematicsOAModifiedStackData {
  inverseKinematicsOAModifiedPersistentData *pd;
};

#endif
//
// File trailer for inverseKinematicsOAModified_types.h
//
// [EOF]
//
