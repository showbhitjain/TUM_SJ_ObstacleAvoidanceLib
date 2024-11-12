//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsWithConstraints.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef INVERSEKINEMATICSWITHCONSTRAINTS_H
#define INVERSEKINEMATICSWITHCONSTRAINTS_H

// Include Files
#include "inverseKinematics_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class inverseKinematicsWithConstraints {
public:
  inverseKinematicsWithConstraints();
  ~inverseKinematicsWithConstraints();
  void inverseKinematics(const coder::array<double, 1U> &joint_values,
                         const coder::array<double, 2U> &jacobi,
                         const double xd_eff_vel_data[],
                         const int xd_eff_vel_size[1],
                         const coder::array<double, 1U> &jointminvalues,
                         const coder::array<double, 1U> &jointmaxvalues,
                         const coder::array<double, 1U> &joint_min_vel,
                         const coder::array<double, 1U> &joint_max_vel,
                         const struct0_T *config,
                         coder::array<double, 1U> &optimal_joint_velocity,
                         double *Exit_Flag);
  inverseKinematicsStackData *getStackData();

private:
  inverseKinematicsPersistentData pd_;
  inverseKinematicsStackData SD_;
};

// Function Declarations
double inverseKinematics_anonFcn1(
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel_data[],
    int xd_eff_vel_size, const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    boolean_T config_useObjective1, double config_weight1,
    boolean_T config_useObjective2, double config_weight2,
    boolean_T config_useObjective3, double config_weight3,
    boolean_T config_useObjective4, double config_weight4,
    boolean_T config_useObjective5, double config_weight5,
    boolean_T config_useObjective6, double config_weight6,
    const coder::array<double, 1U> &q_vel);

#endif
//
// File trailer for inverseKinematicsWithConstraints.h
//
// [EOF]
//
