//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematics_types.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef INVERSEKINEMATICS_TYPES_H
#define INVERSEKINEMATICS_TYPES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"

// type Definitions
struct struct0_T {
  boolean_T useObjective1;
  double weight1;
  boolean_T useObjective2;
  double weight2;
  boolean_T useObjective3;
  double weight3;
  boolean_T useObjective4;
  double weight4;
  boolean_T useObjective5;
  double weight5;
  boolean_T useObjective6;
  double weight6;
};

struct inverseKinematicsPersistentData {
  coder::array<double, 1U> starting_joint_vel;
  boolean_T starting_joint_vel_not_empty;
  boolean_T is_first_step;
  boolean_T is_first_step_not_empty;
};

struct inverseKinematicsStackData {
  inverseKinematicsPersistentData *pd;
};

#endif
//
// File trailer for inverseKinematics_types.h
//
// [EOF]
//
