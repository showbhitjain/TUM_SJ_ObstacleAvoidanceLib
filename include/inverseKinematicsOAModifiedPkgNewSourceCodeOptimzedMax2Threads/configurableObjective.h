//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: configurableObjective.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

#ifndef CONFIGURABLEOBJECTIVE_H
#define CONFIGURABLEOBJECTIVE_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
double configurableObjective(
    const coder::array<double, 1U> &current_joint_velocity,
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &WeightMatrix,
    boolean_T config_useObjectiveNormInfinity, double config_weightNormInfinity,
    boolean_T config_useObjectiveNormL2, double config_weightNormL2,
    boolean_T config_useObjectiveTrajectoryFollowing,
    double config_weightTrajectoryFollowing,
    boolean_T config_useObjectiveJointAcceleration,
    double config_weightJointAcceleration,
    boolean_T config_useObjectiveManipulability,
    double config_weightManipulability);

#endif
//
// File trailer for configurableObjective.h
//
// [EOF]
//
