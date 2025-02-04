//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: OptimizationBasedIKWithOAModified.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef OPTIMIZATIONBASEDIKWITHOAMODIFIED_H
#define OPTIMIZATIONBASEDIKWITHOAMODIFIED_H

// Include Files
#include "inverseKinematicsOAModified_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
class OptimizationBasedIKWithOAModified {
public:
  OptimizationBasedIKWithOAModified();
  ~OptimizationBasedIKWithOAModified();
  void inverseKinematicsOAModified(
      const coder::array<double, 1U> &joint_values,
      const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
      const coder::array<double, 1U> &jointminvalues,
      const coder::array<double, 1U> &jointmaxvalues,
      const coder::array<double, 1U> &joint_min_vel,
      const coder::array<double, 1U> &joint_max_vel,
      const coder::array<double, 2U> &J_g, const coder::array<double, 1U> &b_g,
      const coder::array<double, 2U> &jointVelocityWeightMatrix,
      const struct0_T *configInput,
      coder::array<double, 1U> &optimal_joint_velocity, double *Exit_Flag);
  inverseKinematicsOAModifiedStackData *getStackData();

private:
  inverseKinematicsOAModifiedPersistentData pd_;
  inverseKinematicsOAModifiedStackData SD_;
};

// Function Declarations
double inverseKinematicsOAModified_anonFcn1(
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    const struct0_T &config, boolean_T slackIsUsed,
    const coder::array<double, 1U> &q_vel);

#endif
//
// File trailer for OptimizationBasedIKWithOAModified.h
//
// [EOF]
//
