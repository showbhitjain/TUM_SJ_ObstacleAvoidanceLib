//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: OptimizationBasedIKWithOAModified.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "configurableObjective.h"
#include "fmincon.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_types.h"
#include "ixfun.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
// Function Declarations
static void binary_expand_op(coder::array<double, 1U> &in1,
                             const struct0_T *in2,
                             const coder::array<double, 1U> &in3,
                             const coder::array<double, 1U> &in4);

static void inverseKinematicsOAModified_init(
    OptimizationBasedIKWithOAModified *aInstancePtr);

// Function Definitions
//
// Arguments    : void
// Return Type  : inverseKinematicsOAModifiedStackData *
//
inverseKinematicsOAModifiedStackData *
OptimizationBasedIKWithOAModified::getStackData()
{
  return &SD_;
}

//
// Arguments    : coder::array<double, 1U> &in1
//                const struct0_T *in2
//                const coder::array<double, 1U> &in3
//                const coder::array<double, 1U> &in4
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 1U> &in1,
                             const struct0_T *in2,
                             const coder::array<double, 1U> &in3,
                             const coder::array<double, 1U> &in4)
{
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  if (in4.size(0) == 1) {
    loop_ub = in3.size(0);
  } else {
    loop_ub = in4.size(0);
  }
  in1.set_size(loop_ub);
  stride_0_0 = (in3.size(0) != 1);
  stride_1_0 = (in4.size(0) != 1);
  if (static_cast<int>(loop_ub < 200)) {
    for (int i{0}; i < loop_ub; i++) {
      in1[i] = in2->gamma * (in3[i * stride_0_0] - in4[i * stride_1_0]);
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int i = 0; i < loop_ub; i++) {
      in1[i] = in2->gamma * (in3[i * stride_0_0] - in4[i * stride_1_0]);
    }
  }
}

//
// function [optimal_joint_velocity, Exit_Flag] = inverseKinematicsOAModified(
// ...
//     joint_values,       ... % [n_joints x 1]
//     jacobi,             ... % [6 x n_joints]
//     xd_eff_vel,         ... % [6 x 1]
//     jointminvalues,     ... % [n_joints x 1]
//     jointmaxvalues,     ... % [n_joints x 1]
//     joint_min_vel,      ... % [n_joints x 1]
//     joint_max_vel,      ... % [n_joints x 1]
//     J_g,                ... % Inequality constraint matrix
//     b_g,                ... % Inequality constraint vector
//     jointVelocityWeightMatrix, ...
//     configInput)
//
// % [n_joints x 1]
//  % [6 x n_joints]
//  % [6 x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % Inequality constraint matrix
//  % Inequality constraint vector
//
// Arguments    : OptimizationBasedIKWithOAModified *aInstancePtr
// Return Type  : void
//
static void inverseKinematicsOAModified_init(
    OptimizationBasedIKWithOAModified *aInstancePtr)
{
  inverseKinematicsOAModifiedStackData *localSD;
  localSD = aInstancePtr->getStackData();
  localSD->pd->starting_joint_vel_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
OptimizationBasedIKWithOAModified::OptimizationBasedIKWithOAModified()
{
  SD_.pd = &pd_;
  omp_init_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
  inverseKinematicsOAModified_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
OptimizationBasedIKWithOAModified::~OptimizationBasedIKWithOAModified()
{
  omp_destroy_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
}

//
// function [optimal_joint_velocity, Exit_Flag] = inverseKinematicsOAModified(
// ...
//     joint_values,       ... % [n_joints x 1]
//     jacobi,             ... % [6 x n_joints]
//     xd_eff_vel,         ... % [6 x 1]
//     jointminvalues,     ... % [n_joints x 1]
//     jointmaxvalues,     ... % [n_joints x 1]
//     joint_min_vel,      ... % [n_joints x 1]
//     joint_max_vel,      ... % [n_joints x 1]
//     J_g,                ... % Inequality constraint matrix
//     b_g,                ... % Inequality constraint vector
//     jointVelocityWeightMatrix, ...
//     configInput)
//
// % [n_joints x 1]
//  % [6 x n_joints]
//  % [6 x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % [n_joints x 1]
//  % Inequality constraint matrix
//  % Inequality constraint vector
//
// Arguments    : const coder::array<double, 1U> &joint_values
//                const coder::array<double, 2U> &jacobi
//                const double xd_eff_vel[6]
//                const coder::array<double, 1U> &jointminvalues
//                const coder::array<double, 1U> &jointmaxvalues
//                const coder::array<double, 1U> &joint_min_vel
//                const coder::array<double, 1U> &joint_max_vel
//                const coder::array<double, 2U> &J_g
//                const coder::array<double, 1U> &b_g
//                const coder::array<double, 2U> &jointVelocityWeightMatrix
//                const struct0_T *configInput
//                coder::array<double, 1U> &optimal_joint_velocity
//                double *Exit_Flag
// Return Type  : void
//
void OptimizationBasedIKWithOAModified::inverseKinematicsOAModified(
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &jointminvalues,
    const coder::array<double, 1U> &jointmaxvalues,
    const coder::array<double, 1U> &joint_min_vel,
    const coder::array<double, 1U> &joint_max_vel,
    const coder::array<double, 2U> &J_g, const coder::array<double, 1U> &b_g,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    const struct0_T *configInput,
    coder::array<double, 1U> &optimal_joint_velocity,
    double constraintTolerance,int maxIterations,double optimalityTolerance,double stepTolerance, double *Exit_Flag)
{

  OAGlobals::setConstraintTolerance(constraintTolerance);
  OAGlobals::setMaxIterations(maxIterations);
  OAGlobals::setOptimalityTolerance(optimalityTolerance);
  OAGlobals::setStepTolerance(stepTolerance);

  static const signed char iv[36]{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                                  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
  coder::anonymous_function b_this;
  coder::array<double, 2U> A;
  coder::array<double, 2U> Aeq;
  coder::array<double, 2U> b;
  coder::array<double, 1U> bp;
  coder::array<double, 1U> min_dist;
  coder::array<double, 1U> ub_joints;
  coder::array<double, 1U> varargin_1;
  coder::array<signed char, 2U> In;
  double beq_data[6];
  double b_varargin_1;
  double b_varargin_2;
  double infuence_dist;
  double s;
  double stop_dist;
  double varargin_2;
  int beq_size[2];
  int i;
  int i1;
  int input_sizes_idx_1;
  int loop_ub;
  int loop_ub_tmp;
  int nx;
  boolean_T slackIsUsed;
  // 'inverseKinematicsOAModified:16'
  // coder.varsize("optimal_joint_velocity",[Inf 1],[1 0]);
  // 'inverseKinematicsOAModified:18' coder.varsize("Exit_Flag",[1 1],[0 0]);
  // ----------------------------------------------------------------------
  //  1. PERSISTENT VARIABLES
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:25' n_joints = length(joint_values);
  //  Number of joints
  // 'inverseKinematicsOAModified:26' n_slack  = 6;
  //  One slack variable per end-effector velocity component (if used)
  //  Initialize the persistent variables only on the first call
  // 'inverseKinematicsOAModified:29' if isempty(starting_joint_vel)
  if (!pd_.starting_joint_vel_not_empty) {
    // 'inverseKinematicsOAModified:31' starting_joint_vel = zeros(n_joints,1);
    pd_.starting_joint_vel.set_size(joint_values.size(0));
    loop_ub = joint_values.size(0);
    if (static_cast<int>(joint_values.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        pd_.starting_joint_vel[k] = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        pd_.starting_joint_vel[k] = 0.0;
      }
    }
    //  Default
  }
  //  Unpack config for convenience
  // 'inverseKinematicsOAModified:38' config = configInput;
  // ----------------------------------------------------------------------
  //  2. DETERMINE IF SLACK IS ACTUALLY USED
  // ----------------------------------------------------------------------
  //  The new dynamic logic:
  //    - If config.dynamicSlack is true AND (J_g or b_g are empty),
  //      we forcibly turn off slack.
  //    - Otherwise, we do whatever config.applySlack says.
  //
  // 'inverseKinematicsOAModified:48' slackIsUsed = config.applySlack;
  slackIsUsed = configInput->applySlack;
  // 'inverseKinematicsOAModified:49' if config.dynamicSlack &&
  // config.obstacleAvoidanceScheme
  if (configInput->dynamicSlack && configInput->obstacleAvoidanceScheme &&
      ((J_g.size(0) == 0) || (J_g.size(1) == 0) || (b_g.size(0) == 0))) {
    // 'inverseKinematicsOAModified:50' if isempty(J_g) || isempty(b_g)
    // 'inverseKinematicsOAModified:51' slackIsUsed = false;
    slackIsUsed = false;
  }
  // ----------------------------------------------------------------------
  //  3. BUILD VELOCITY DAMPER MATRICES IF NEEDED
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:58' In            = zeros(n_joints);
  In.set_size(joint_values.size(0), joint_values.size(0));
  loop_ub_tmp = joint_values.size(0) * joint_values.size(0);
  i = (loop_ub_tmp < 200);
  if (i) {
    for (int k{0}; k < loop_ub_tmp; k++) {
      In[k] = 0;
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < loop_ub_tmp; k++) {
      In[k] = 0;
    }
  }
  //  Will hold diagonal matrix for damped joints
  // 'inverseKinematicsOAModified:59' bp            = zeros(n_joints,1);
  bp.set_size(joint_values.size(0));
  loop_ub = joint_values.size(0);
  i1 = (joint_values.size(0) < 200);
  if (i1) {
    for (int k{0}; k < loop_ub; k++) {
      bp[k] = 0.0;
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < loop_ub; k++) {
      bp[k] = 0.0;
    }
  }
  //  Will hold vector for velocity damper if needed
  // 'inverseKinematicsOAModified:60' infuence_dist =
  // deg2rad(config.jointLimitActivationDistance);
  infuence_dist =
      0.017453292519943295 * configInput->jointLimitActivationDistance;
  // 'inverseKinematicsOAModified:61' stop_dist     =
  // deg2rad(config.jointLimitStopDistance);
  stop_dist = 0.017453292519943295 * configInput->jointLimitStopDistance;
  // 'inverseKinematicsOAModified:62' joint_gain    = config.jointLimitGain;
  // 'inverseKinematicsOAModified:64' if config.applyVelocityDamper
  if (configInput->applyVelocityDamper) {
    //  Compute distance to each joint limit, apply a linear velocity damper
    // 'inverseKinematicsOAModified:67' min_dist = min(abs(jointminvalues -
    // joint_values), ... 'inverseKinematicsOAModified:68' abs(jointmaxvalues -
    // joint_values));
    if (jointminvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointminvalues.size(0));
      loop_ub = jointminvalues.size(0);
      if (static_cast<int>(jointminvalues.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          min_dist[k] = jointminvalues[k] - joint_values[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          min_dist[k] = jointminvalues[k] - joint_values[k];
        }
      }
    } else {
      minus(min_dist, jointminvalues, joint_values);
    }
    nx = min_dist.size(0);
    varargin_1.set_size(min_dist.size(0));
    input_sizes_idx_1 = (min_dist.size(0) < 200);
    if (input_sizes_idx_1) {
      for (int k{0}; k < nx; k++) {
        varargin_1[k] = std::abs(min_dist[k]);
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < nx; k++) {
        varargin_1[k] = std::abs(min_dist[k]);
      }
    }
    if (jointmaxvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointmaxvalues.size(0));
      loop_ub = jointmaxvalues.size(0);
      if (static_cast<int>(jointmaxvalues.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          min_dist[k] = jointmaxvalues[k] - joint_values[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          min_dist[k] = jointmaxvalues[k] - joint_values[k];
        }
      }
    } else {
      minus(min_dist, jointmaxvalues, joint_values);
    }
    nx = min_dist.size(0);
    ub_joints.set_size(min_dist.size(0));
    if (static_cast<int>(min_dist.size(0) < 200)) {
      for (int k{0}; k < nx; k++) {
        ub_joints[k] = std::abs(min_dist[k]);
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < nx; k++) {
        ub_joints[k] = std::abs(min_dist[k]);
      }
    }
    if (varargin_1.size(0) == ub_joints.size(0)) {
      min_dist.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      if (input_sizes_idx_1) {
        for (int k{0}; k < loop_ub; k++) {
          s = varargin_1[k];
          varargin_2 = ub_joints[k];
          min_dist[k] = std::fmin(s, varargin_2);
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_varargin_1, b_varargin_2)

        for (int k = 0; k < loop_ub; k++) {
          b_varargin_1 = varargin_1[k];
          b_varargin_2 = ub_joints[k];
          min_dist[k] = std::fmin(b_varargin_1, b_varargin_2);
        }
      }
    } else {
      coder::internal::expand_min(varargin_1, ub_joints, min_dist);
    }
    // 'inverseKinematicsOAModified:69' for i = 1:n_joints
    input_sizes_idx_1 = joint_values.size(0);
    for (int b_i{0}; b_i < input_sizes_idx_1; b_i++) {
      // 'inverseKinematicsOAModified:70' if min_dist(i) < infuence_dist
      if (min_dist[b_i] < infuence_dist) {
        // 'inverseKinematicsOAModified:71' In(i,i)  = 1;
        In[b_i + In.size(0) * b_i] = 1;
        // 'inverseKinematicsOAModified:72' bp(i)    = joint_gain * (min_dist(i)
        // - stop_dist)/(infuence_dist - stop_dist);
        bp[b_i] = configInput->jointLimitGain * (min_dist[b_i] - stop_dist) /
                  (infuence_dist - stop_dist);
      }
    }
  }
  // ----------------------------------------------------------------------
  //  4. SET UP CONSTRAINTS AND BOUNDS
  // ----------------------------------------------------------------------
  //  Default: no constraints
  // 'inverseKinematicsOAModified:81' Aeq = [];
  Aeq.set_size(0, 0);
  // 'inverseKinematicsOAModified:82' beq = [];
  beq_size[0] = 0;
  beq_size[1] = 0;
  // 'inverseKinematicsOAModified:83' A   = [];
  A.set_size(0, 0);
  // 'inverseKinematicsOAModified:84' b   = [];
  b.set_size(0, 0);
  //  lb  = [];
  //  ub  = [];
  //  Weight matrix for Jacobian in equality constraints
  // 'inverseKinematicsOAModified:89' JacobiWeightMatrix = diag([1,1,1,1,1,1]);
  //  ---- 4.1 Equality constraints ---------------------------------------
  // 'inverseKinematicsOAModified:92' if config.applyEqualityConstraints
  if (configInput->applyEqualityConstraints) {
    // 'inverseKinematicsOAModified:93' Aeq = JacobiWeightMatrix * jacobi;
    nx = jacobi.size(1);
    Aeq.set_size(6, jacobi.size(1));
    for (loop_ub = 0; loop_ub < nx; loop_ub++) {
      input_sizes_idx_1 = loop_ub * 6;
      for (int b_i{0}; b_i < 6; b_i++) {
        s = ((((static_cast<double>(iv[b_i]) * jacobi[input_sizes_idx_1] +
                static_cast<double>(iv[b_i + 6]) *
                    jacobi[input_sizes_idx_1 + 1]) +
               static_cast<double>(iv[b_i + 12]) *
                   jacobi[input_sizes_idx_1 + 2]) +
              static_cast<double>(iv[b_i + 18]) *
                  jacobi[input_sizes_idx_1 + 3]) +
             static_cast<double>(iv[b_i + 24]) *
                 jacobi[input_sizes_idx_1 + 4]) +
            static_cast<double>(iv[b_i + 30]) * jacobi[input_sizes_idx_1 + 5];
        Aeq[input_sizes_idx_1 + b_i] = s;
      }
    }
    // 'inverseKinematicsOAModified:94' beq = xd_eff_vel;
    beq_size[0] = 6;
    beq_size[1] = 1;
    beq_data[0] = xd_eff_vel[0];
    beq_data[1] = xd_eff_vel[1];
    beq_data[2] = xd_eff_vel[2];
    beq_data[3] = xd_eff_vel[3];
    beq_data[4] = xd_eff_vel[4];
    beq_data[5] = xd_eff_vel[5];
  }
  //  ---- 4.2 Inequality constraints -------------------------------------
  // 'inverseKinematicsOAModified:98' if config.applyInequalityConstraints
  if (configInput->applyInequalityConstraints) {
    // 'inverseKinematicsOAModified:99' if ~isempty(J_g) && ~isempty(b_g)
    if ((J_g.size(0) != 0) && (J_g.size(1) != 0) && (b_g.size(0) != 0)) {
      // 'inverseKinematicsOAModified:100' if config.applyVelocityDamper
      if (configInput->applyVelocityDamper) {
        //  Combine both standard inequalities and velocity damper
        // 'inverseKinematicsOAModified:102' A = [J_g; In];
        loop_ub_tmp = J_g.size(0);
        if ((In.size(0) != 0) && (In.size(1) != 0)) {
          nx = In.size(0);
        } else {
          nx = 0;
        }
        A.set_size(J_g.size(0) + nx, J_g.size(1));
        loop_ub = J_g.size(1);
        for (i = 0; i < loop_ub; i++) {
          for (input_sizes_idx_1 = 0; input_sizes_idx_1 < loop_ub_tmp;
               input_sizes_idx_1++) {
            A[input_sizes_idx_1 + A.size(0) * i] =
                J_g[input_sizes_idx_1 + loop_ub_tmp * i];
          }
          for (input_sizes_idx_1 = 0; input_sizes_idx_1 < nx;
               input_sizes_idx_1++) {
            A[(input_sizes_idx_1 + loop_ub_tmp) + A.size(0) * i] =
                In[input_sizes_idx_1 + nx * i];
          }
        }
        // 'inverseKinematicsOAModified:103' b = [b_g; bp];
        min_dist.set_size(b_g.size(0) + bp.size(0));
        loop_ub = b_g.size(0);
        if (static_cast<int>(b_g.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            min_dist[k] = b_g[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            min_dist[k] = b_g[k];
          }
        }
        loop_ub = bp.size(0);
        if (i1) {
          for (int k{0}; k < loop_ub; k++) {
            min_dist[k + b_g.size(0)] = bp[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            min_dist[k + b_g.size(0)] = bp[k];
          }
        }
        b.set_size(min_dist.size(0), 1);
        loop_ub = min_dist.size(0);
        if (static_cast<int>(min_dist.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            b[k] = min_dist[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            b[k] = min_dist[k];
          }
        }
      } else {
        // 'inverseKinematicsOAModified:104' else
        //  No velocity damper
        // 'inverseKinematicsOAModified:106' A = J_g;
        A.set_size(J_g.size(0), J_g.size(1));
        loop_ub = J_g.size(0) * J_g.size(1);
        if (static_cast<int>(loop_ub < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            A[k] = J_g[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            A[k] = J_g[k];
          }
        }
        // 'inverseKinematicsOAModified:107' b = b_g;
        b.set_size(b_g.size(0), 1);
        loop_ub = b_g.size(0);
        if (static_cast<int>(b_g.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            b[k] = b_g[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            b[k] = b_g[k];
          }
        }
      }

      // 'inverseKinematicsOAModified:109' else
      //  If J_g, b_g are empty, we only apply velocity damper if set
      // 'inverseKinematicsOAModified:111' if config.applyVelocityDamper
    } else if (configInput->applyVelocityDamper) {
      // 'inverseKinematicsOAModified:112' A = In;
      A.set_size(In.size(0), In.size(1));
      if (i) {
        for (int k{0}; k < loop_ub_tmp; k++) {
          A[k] = In[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub_tmp; k++) {
          A[k] = In[k];
        }
      }
      // 'inverseKinematicsOAModified:113' b = bp;
      b.set_size(bp.size(0), 1);
      loop_ub = bp.size(0);
      if (i1) {
        for (int k{0}; k < loop_ub; k++) {
          b[k] = bp[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          b[k] = bp[k];
        }
      }
    }

    // 'inverseKinematicsOAModified:116' else
    //  If we do not apply inequality constraints at all but velocity
    //  damper is on, we can still do that by adding A=In, b=bp.
    // 'inverseKinematicsOAModified:119' if config.applyVelocityDamper
  } else if (configInput->applyVelocityDamper) {
    // 'inverseKinematicsOAModified:120' A = In;
    A.set_size(In.size(0), In.size(1));
    if (i) {
      for (int k{0}; k < loop_ub_tmp; k++) {
        A[k] = In[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub_tmp; k++) {
        A[k] = In[k];
      }
    }
    // 'inverseKinematicsOAModified:121' b = bp;
    b.set_size(bp.size(0), 1);
    loop_ub = bp.size(0);
    if (i1) {
      for (int k{0}; k < loop_ub; k++) {
        b[k] = bp[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        b[k] = bp[k];
      }
    }

    //  if config.applyVelocityDamper && (isempty(J_g) || isempty(b_g))
    //      A = In;
    //      b = bp;
    //  end
  }
  //  ---- 4.3 Determine final use of Slack and build lb, ub --------------
  // 'inverseKinematicsOAModified:131' if ~slackIsUsed
  if (!slackIsUsed) {
    //  BOUNDS without slack
    // 'inverseKinematicsOAModified:133' if config.applyVelocityDamper
    if (configInput->applyVelocityDamper) {
      //  We do not scale velocities by gamma if velocity damper is active
      // 'inverseKinematicsOAModified:135' lb = joint_min_vel;
      bp.set_size(joint_min_vel.size(0));
      loop_ub = joint_min_vel.size(0);
      if (static_cast<int>(joint_min_vel.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          bp[k] = joint_min_vel[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          bp[k] = joint_min_vel[k];
        }
      }
      // 'inverseKinematicsOAModified:136' ub = joint_max_vel;
      min_dist.set_size(joint_max_vel.size(0));
      loop_ub = joint_max_vel.size(0);
      if (static_cast<int>(joint_max_vel.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          min_dist[k] = joint_max_vel[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          min_dist[k] = joint_max_vel[k];
        }
      }
    } else {
      // 'inverseKinematicsOAModified:137' else
      //  Normal bounds: scale the move by config.gamma
      // 'inverseKinematicsOAModified:139' lb = max(config.gamma .*
      // (jointminvalues - joint_values), joint_min_vel);
      if (jointminvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointminvalues.size(0));
        loop_ub = jointminvalues.size(0);
        if (static_cast<int>(jointminvalues.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            varargin_1[k] =
                configInput->gamma * (jointminvalues[k] - joint_values[k]);
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            varargin_1[k] =
                configInput->gamma * (jointminvalues[k] - joint_values[k]);
          }
        }
      } else {
        binary_expand_op(varargin_1, configInput, jointminvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_min_vel.size(0)) {
        bp.set_size(varargin_1.size(0));
        loop_ub = varargin_1.size(0);
        if (static_cast<int>(varargin_1.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            s = varargin_1[k];
            varargin_2 = joint_min_vel[k];
            bp[k] = std::fmax(s, varargin_2);
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_varargin_1, b_varargin_2)

          for (int k = 0; k < loop_ub; k++) {
            b_varargin_1 = varargin_1[k];
            b_varargin_2 = joint_min_vel[k];
            bp[k] = std::fmax(b_varargin_1, b_varargin_2);
          }
        }
      } else {
        coder::internal::expand_max(varargin_1, joint_min_vel, bp);
      }
      // 'inverseKinematicsOAModified:140' ub = min(config.gamma .*
      // (jointmaxvalues - joint_values), joint_max_vel);
      if (jointmaxvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointmaxvalues.size(0));
        loop_ub = jointmaxvalues.size(0);
        if (static_cast<int>(jointmaxvalues.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            varargin_1[k] =
                configInput->gamma * (jointmaxvalues[k] - joint_values[k]);
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k < loop_ub; k++) {
            varargin_1[k] =
                configInput->gamma * (jointmaxvalues[k] - joint_values[k]);
          }
        }
      } else {
        binary_expand_op(varargin_1, configInput, jointmaxvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_max_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        loop_ub = varargin_1.size(0);
        if (static_cast<int>(varargin_1.size(0) < 200)) {
          for (int k{0}; k < loop_ub; k++) {
            s = varargin_1[k];
            varargin_2 = joint_max_vel[k];
            min_dist[k] = std::fmin(s, varargin_2);
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_varargin_1, b_varargin_2)

          for (int k = 0; k < loop_ub; k++) {
            b_varargin_1 = varargin_1[k];
            b_varargin_2 = joint_max_vel[k];
            min_dist[k] = std::fmin(b_varargin_1, b_varargin_2);
          }
        }
      } else {
        coder::internal::expand_min(varargin_1, joint_max_vel, min_dist);
      }
    }

    // 'inverseKinematicsOAModified:142' else
    //  BOUNDS with slack
    //     first n_joints are for joint velocities
    //     next   n_slack are for slack
    // 'inverseKinematicsOAModified:146' if config.applyVelocityDamper
  } else if (configInput->applyVelocityDamper) {
    // 'inverseKinematicsOAModified:147' lb_joints = joint_min_vel;
    // 'inverseKinematicsOAModified:148' ub_joints = joint_max_vel;
    // 'inverseKinematicsOAModified:149' lb_slack = config.Slacklowerbound;
    // 'inverseKinematicsOAModified:150' ub_slack = config.Slackupperbound;
    // 'inverseKinematicsOAModified:151' lb       = [lb_joints; lb_slack];
    bp.set_size(joint_min_vel.size(0) + 6);
    loop_ub = joint_min_vel.size(0);
    if (static_cast<int>(joint_min_vel.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        bp[k] = joint_min_vel[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        bp[k] = joint_min_vel[k];
      }
    }
    bp[joint_min_vel.size(0)] = configInput->Slacklowerbound[0];
    bp[joint_min_vel.size(0) + 1] = configInput->Slacklowerbound[1];
    bp[joint_min_vel.size(0) + 2] = configInput->Slacklowerbound[2];
    bp[joint_min_vel.size(0) + 3] = configInput->Slacklowerbound[3];
    bp[joint_min_vel.size(0) + 4] = configInput->Slacklowerbound[4];
    bp[joint_min_vel.size(0) + 5] = configInput->Slacklowerbound[5];
    // 'inverseKinematicsOAModified:152' ub       = [ub_joints; ub_slack];
    min_dist.set_size(joint_max_vel.size(0) + 6);
    loop_ub = joint_max_vel.size(0);
    if (static_cast<int>(joint_max_vel.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        min_dist[k] = joint_max_vel[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        min_dist[k] = joint_max_vel[k];
      }
    }
    min_dist[joint_max_vel.size(0)] = configInput->Slackupperbound[0];
    min_dist[joint_max_vel.size(0) + 1] = configInput->Slackupperbound[1];
    min_dist[joint_max_vel.size(0) + 2] = configInput->Slackupperbound[2];
    min_dist[joint_max_vel.size(0) + 3] = configInput->Slackupperbound[3];
    min_dist[joint_max_vel.size(0) + 4] = configInput->Slackupperbound[4];
    min_dist[joint_max_vel.size(0) + 5] = configInput->Slackupperbound[5];
  } else {
    // 'inverseKinematicsOAModified:153' else
    // 'inverseKinematicsOAModified:154' lb_joints = max(config.gamma .*
    // (jointminvalues - joint_values), joint_min_vel);
    if (jointminvalues.size(0) == joint_values.size(0)) {
      varargin_1.set_size(jointminvalues.size(0));
      loop_ub = jointminvalues.size(0);
      if (static_cast<int>(jointminvalues.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          varargin_1[k] =
              configInput->gamma * (jointminvalues[k] - joint_values[k]);
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          varargin_1[k] =
              configInput->gamma * (jointminvalues[k] - joint_values[k]);
        }
      }
    } else {
      binary_expand_op(varargin_1, configInput, jointminvalues, joint_values);
    }
    if (varargin_1.size(0) == joint_min_vel.size(0)) {
      min_dist.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      if (static_cast<int>(varargin_1.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          s = varargin_1[k];
          varargin_2 = joint_min_vel[k];
          min_dist[k] = std::fmax(s, varargin_2);
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_varargin_1, b_varargin_2)

        for (int k = 0; k < loop_ub; k++) {
          b_varargin_1 = varargin_1[k];
          b_varargin_2 = joint_min_vel[k];
          min_dist[k] = std::fmax(b_varargin_1, b_varargin_2);
        }
      }
    } else {
      coder::internal::expand_max(varargin_1, joint_min_vel, min_dist);
    }
    // 'inverseKinematicsOAModified:155' ub_joints = min(config.gamma .*
    // (jointmaxvalues - joint_values), joint_max_vel);
    if (jointmaxvalues.size(0) == joint_values.size(0)) {
      varargin_1.set_size(jointmaxvalues.size(0));
      loop_ub = jointmaxvalues.size(0);
      if (static_cast<int>(jointmaxvalues.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          varargin_1[k] =
              configInput->gamma * (jointmaxvalues[k] - joint_values[k]);
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k < loop_ub; k++) {
          varargin_1[k] =
              configInput->gamma * (jointmaxvalues[k] - joint_values[k]);
        }
      }
    } else {
      binary_expand_op(varargin_1, configInput, jointmaxvalues, joint_values);
    }
    if (varargin_1.size(0) == joint_max_vel.size(0)) {
      ub_joints.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      if (static_cast<int>(varargin_1.size(0) < 200)) {
        for (int k{0}; k < loop_ub; k++) {
          s = varargin_1[k];
          varargin_2 = joint_max_vel[k];
          ub_joints[k] = std::fmin(s, varargin_2);
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_varargin_1, b_varargin_2)

        for (int k = 0; k < loop_ub; k++) {
          b_varargin_1 = varargin_1[k];
          b_varargin_2 = joint_max_vel[k];
          ub_joints[k] = std::fmin(b_varargin_1, b_varargin_2);
        }
      }
    } else {
      coder::internal::expand_min(varargin_1, joint_max_vel, ub_joints);
    }
    // 'inverseKinematicsOAModified:156' lb_slack = config.Slacklowerbound;
    // 'inverseKinematicsOAModified:157' ub_slack = config.Slackupperbound;
    // 'inverseKinematicsOAModified:158' lb       = [lb_joints; lb_slack];
    bp.set_size(min_dist.size(0) + 6);
    loop_ub = min_dist.size(0);
    if (static_cast<int>(min_dist.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        bp[k] = min_dist[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        bp[k] = min_dist[k];
      }
    }
    bp[min_dist.size(0)] = configInput->Slacklowerbound[0];
    bp[min_dist.size(0) + 1] = configInput->Slacklowerbound[1];
    bp[min_dist.size(0) + 2] = configInput->Slacklowerbound[2];
    bp[min_dist.size(0) + 3] = configInput->Slacklowerbound[3];
    bp[min_dist.size(0) + 4] = configInput->Slacklowerbound[4];
    bp[min_dist.size(0) + 5] = configInput->Slacklowerbound[5];
    // 'inverseKinematicsOAModified:159' ub       = [ub_joints; ub_slack];
    min_dist.set_size(ub_joints.size(0) + 6);
    loop_ub = ub_joints.size(0);
    if (static_cast<int>(ub_joints.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        min_dist[k] = ub_joints[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        min_dist[k] = ub_joints[k];
      }
    }
    min_dist[ub_joints.size(0)] = configInput->Slackupperbound[0];
    min_dist[ub_joints.size(0) + 1] = configInput->Slackupperbound[1];
    min_dist[ub_joints.size(0) + 2] = configInput->Slackupperbound[2];
    min_dist[ub_joints.size(0) + 3] = configInput->Slackupperbound[3];
    min_dist[ub_joints.size(0) + 4] = configInput->Slackupperbound[4];
    min_dist[ub_joints.size(0) + 5] = configInput->Slackupperbound[5];
  }
  //  ---- 4.4 Expand constraints for Slack if needed ----------------------
  // 'inverseKinematicsOAModified:166' if slackIsUsed
  if (slackIsUsed) {
    //  Expand the equality constraints
    // 'inverseKinematicsOAModified:168' if config.applyEqualityConstraints
    if (configInput->applyEqualityConstraints) {
      signed char b_I[36];
      // 'inverseKinematicsOAModified:169' Aeq = [JacobiWeightMatrix * jacobi,
      // eye(n_slack)];
      for (i = 0; i < 36; i++) {
        b_I[i] = 0;
      }
      b_I[0] = 1;
      b_I[7] = 1;
      b_I[14] = 1;
      b_I[21] = 1;
      b_I[28] = 1;
      b_I[35] = 1;
      nx = jacobi.size(1);
      Aeq.set_size(6, jacobi.size(1));
      for (loop_ub = 0; loop_ub < nx; loop_ub++) {
        input_sizes_idx_1 = loop_ub * 6;
        for (int b_i{0}; b_i < 6; b_i++) {
          s = ((((static_cast<double>(iv[b_i]) * jacobi[input_sizes_idx_1] +
                  static_cast<double>(iv[b_i + 6]) *
                      jacobi[input_sizes_idx_1 + 1]) +
                 static_cast<double>(iv[b_i + 12]) *
                     jacobi[input_sizes_idx_1 + 2]) +
                static_cast<double>(iv[b_i + 18]) *
                    jacobi[input_sizes_idx_1 + 3]) +
               static_cast<double>(iv[b_i + 24]) *
                   jacobi[input_sizes_idx_1 + 4]) +
              static_cast<double>(iv[b_i + 30]) * jacobi[input_sizes_idx_1 + 5];
          Aeq[input_sizes_idx_1 + b_i] = s;
        }
      }
      i = Aeq.size(1);
      Aeq.set_size(Aeq.size(0), Aeq.size(1) + 6);
      // 'inverseKinematicsOAModified:170' beq = xd_eff_vel;
      beq_size[0] = 6;
      beq_size[1] = 1;
      for (i1 = 0; i1 < 6; i1++) {
        nx = i1 + i;
        Aeq[Aeq.size(0) * nx] = b_I[6 * i1];
        Aeq[Aeq.size(0) * nx + 1] = b_I[6 * i1 + 1];
        Aeq[Aeq.size(0) * nx + 2] = b_I[6 * i1 + 2];
        Aeq[Aeq.size(0) * nx + 3] = b_I[6 * i1 + 3];
        Aeq[Aeq.size(0) * nx + 4] = b_I[6 * i1 + 4];
        Aeq[Aeq.size(0) * nx + 5] = b_I[6 * i1 + 5];
        beq_data[i1] = xd_eff_vel[i1];
      }
    }
    //  Expand the inequality constraints
    // 'inverseKinematicsOAModified:174' if ~isempty(A)
    if ((A.size(0) != 0) && (A.size(1) != 0)) {
      // 'inverseKinematicsOAModified:175' A = [A, zeros(size(A,1), n_slack)];
      nx = A.size(0);
      loop_ub_tmp = A.size(0);
      input_sizes_idx_1 = A.size(1);
      loop_ub = A.size(1);
      for (i = 0; i < loop_ub; i++) {
        for (i1 = 0; i1 < nx; i1++) {
          A[i1 + A.size(0) * i] = A[i1 + loop_ub_tmp * i];
        }
      }
      A.set_size(A.size(0), A.size(1) + 6);
      for (i = 0; i < 6; i++) {
        for (i1 = 0; i1 < nx; i1++) {
          A[i1 + A.size(0) * (i + input_sizes_idx_1)] = 0.0;
        }
      }
    }
    // 'inverseKinematicsOAModified:183' q_vel_initial_guess =
    // [starting_joint_vel;zeros(n_slack,1)];
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0) + 6);
    loop_ub = pd_.starting_joint_vel.size(0);
    if (static_cast<int>(pd_.starting_joint_vel.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        optimal_joint_velocity[k] = pd_.starting_joint_vel[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        optimal_joint_velocity[k] = pd_.starting_joint_vel[k];
      }
    }
    optimal_joint_velocity[pd_.starting_joint_vel.size(0)] = 0.0;
    optimal_joint_velocity[pd_.starting_joint_vel.size(0) + 1] = 0.0;
    optimal_joint_velocity[pd_.starting_joint_vel.size(0) + 2] = 0.0;
    optimal_joint_velocity[pd_.starting_joint_vel.size(0) + 3] = 0.0;
    optimal_joint_velocity[pd_.starting_joint_vel.size(0) + 4] = 0.0;
    optimal_joint_velocity[pd_.starting_joint_vel.size(0) + 5] = 0.0;
  } else {
    // 'inverseKinematicsOAModified:184' else
    // 'inverseKinematicsOAModified:185' q_vel_initial_guess =
    // starting_joint_vel;
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0));
    loop_ub = pd_.starting_joint_vel.size(0);
    if (static_cast<int>(pd_.starting_joint_vel.size(0) < 200)) {
      for (int k{0}; k < loop_ub; k++) {
        optimal_joint_velocity[k] = pd_.starting_joint_vel[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < loop_ub; k++) {
        optimal_joint_velocity[k] = pd_.starting_joint_vel[k];
      }
    }
  }
  // ----------------------------------------------------------------------
  //  5. INITIAL GUESS
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:182' if slackIsUsed
  // ----------------------------------------------------------------------
  //  6. OPTIMIZATION OPTIONS
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:191' options = optimoptions('fmincon', ...
  // 'inverseKinematicsOAModified:192'         'Algorithm',           'sqp', ...
  // 'inverseKinematicsOAModified:193'         'Display',            'off', ...
  // % or 'iter' to see solver progress 'inverseKinematicsOAModified:194'
  // 'OptimalityTolerance', 1.254e-3, ... 'inverseKinematicsOAModified:195'
  // 'ConstraintTolerance', 1.8967e-5, ... 'inverseKinematicsOAModified:196'
  // 'StepTolerance',       1.245e-4, ... 'inverseKinematicsOAModified:197'
  // 'MaxIterations',       550,... 'inverseKinematicsOAModified:198'
  // 'ObjectiveLimit',-1e6);
  //   % or 'iter' to see solver progress
  //   % Code generation supports these options:
  //
  //  Algorithm — Must be 'sqp' or 'sqp-legacy'
  //
  //  ConstraintTolerance: of interest
  //
  //  FiniteDifferenceStepSize not of interest
  //
  //  FiniteDifferenceType: not of interest
  //
  //  MaxFunctionEvaluations: not of interest
  //
  //  MaxIterations: of interest
  //
  //  ObjectiveLimit: of interest only for unbounded problems (i.e)
  //
  //  OptimalityTolerance: of interest
  //
  //  ScaleProblem: not of interest
  //
  //  SpecifyConstraintGradient: not of interest
  //
  //  SpecifyObjectiveGradient: not of interest
  //
  //  StepTolerance: of interest
  //
  //  TypicalX: not of interest
  // ----------------------------------------------------------------------
  //  7. CALL THE OPTIMIZER (fmincon)
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:231' [q_vel_opt, ~, Exit_Flag] =
  // fmincon(@(q_vel) objFun( ... 'inverseKinematicsOAModified:232' q_vel, ...
  // 'inverseKinematicsOAModified:233'             jacobi, ...
  // 'inverseKinematicsOAModified:234'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:235'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:236'             joint_values, ...
  // 'inverseKinematicsOAModified:237'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:238'             config,
  // .../usr/local/src/AndreiUtils 'inverseKinematicsOAModified:239'
  // slackIsUsed), ... 'inverseKinematicsOAModified:240' q_vel_initial_guess,
  // ... 'inverseKinematicsOAModified:241'         A, b, Aeq, beq, lb, ub, [],
  // options);
  b_this.workspace.slackIsUsed = slackIsUsed;
  b_this.workspace.jacobi.set_size(6, jacobi.size(1));
  loop_ub = 6 * jacobi.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.jacobi[i] = jacobi[i];
  }
  b_this.workspace.xd_eff_vel[0] = xd_eff_vel[0];
  b_this.workspace.xd_eff_vel[1] = xd_eff_vel[1];
  b_this.workspace.xd_eff_vel[2] = xd_eff_vel[2];
  b_this.workspace.xd_eff_vel[3] = xd_eff_vel[3];
  b_this.workspace.xd_eff_vel[4] = xd_eff_vel[4];
  b_this.workspace.xd_eff_vel[5] = xd_eff_vel[5];
  b_this.workspace.starting_joint_vel.set_size(pd_.starting_joint_vel.size(0));
  loop_ub = pd_.starting_joint_vel.size(0);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.starting_joint_vel[i] = pd_.starting_joint_vel[i];
  }
  b_this.workspace.joint_values.set_size(joint_values.size(0));
  loop_ub = joint_values.size(0);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.joint_values[i] = joint_values[i];
  }
  b_this.workspace.jointVelocityWeightMatrix.set_size(
      jointVelocityWeightMatrix.size(0), jointVelocityWeightMatrix.size(1));
  loop_ub =
      jointVelocityWeightMatrix.size(0) * jointVelocityWeightMatrix.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.jointVelocityWeightMatrix[i] =
        jointVelocityWeightMatrix[i];
  }
  b_this.workspace.config = *configInput;
  coder::fmincon(b_this, optimal_joint_velocity, A, b, Aeq, beq_data, beq_size,
                 bp, min_dist, Exit_Flag);
  // ----------------------------------------------------------------------
  //  8. EXTRACT THE RESULT
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:246' if slackIsUsed
  if (slackIsUsed) {
    // 'inverseKinematicsOAModified:247' optimal_joint_velocity =
    // q_vel_opt(1:n_joints);
    optimal_joint_velocity.set_size(joint_values.size(0));
  } else {
    // 'inverseKinematicsOAModified:249' else
    // 'inverseKinematicsOAModified:250' optimal_joint_velocity = q_vel_opt;
  }
  // ----------------------------------------------------------------------
  //  9. UPDATE PERSISTENT STATE
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:256' starting_joint_vel =
  // optimal_joint_velocity;
  pd_.starting_joint_vel.set_size(optimal_joint_velocity.size(0));
  loop_ub = optimal_joint_velocity.size(0);
  if (static_cast<int>(optimal_joint_velocity.size(0) < 200)) {
    for (int k{0}; k < loop_ub; k++) {
      pd_.starting_joint_vel[k] = optimal_joint_velocity[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < loop_ub; k++) {
      pd_.starting_joint_vel[k] = optimal_joint_velocity[k];
    }
  }
  pd_.starting_joint_vel_not_empty = (pd_.starting_joint_vel.size(0) != 0);
}

//
// @(q_vel)
//
// Arguments    : const coder::array<double, 2U> &jacobi
//                const double xd_eff_vel[6]
//                const coder::array<double, 1U> &starting_joint_vel
//                const coder::array<double, 1U> &joint_values
//                const coder::array<double, 2U> &jointVelocityWeightMatrix
//                const struct0_T &config
//                boolean_T slackIsUsed
//                const coder::array<double, 1U> &q_vel
// Return Type  : double
//
double inverseKinematicsOAModified_anonFcn1(
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    const struct0_T &config, boolean_T slackIsUsed,
    const coder::array<double, 1U> &q_vel)
{
  coder::array<double, 1U> b_q_vel;
  double b[36];
  double varargout_1;
  // 'inverseKinematicsOAModified:231' @(q_vel) objFun( ...
  // 'inverseKinematicsOAModified:232'             q_vel, ...
  // 'inverseKinematicsOAModified:233'             jacobi, ...
  // 'inverseKinematicsOAModified:234'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:235'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:236'             joint_values, ...
  // 'inverseKinematicsOAModified:237'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:238'             config,
  // .../usr/local/src/AndreiUtils 'inverseKinematicsOAModified:239'
  // slackIsUsed) 'objFun:2' n_joints = length(joint_values);
  //  Add the cost for the slack variables
  // 'objFun:4' if slackIsUsed
  if (slackIsUsed) {
    double b_config;
    int b_loop_ub;
    int i;
    int i1;
    int loop_ub;
    // 'objFun:6' objVal = configurableObjective(q_vel(1:n_joints), jacobi,
    // xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix,
    // config) + config.SlackObjectiveWeight * q_vel(n_joints+1:end)' *
    // diag(config.SlackPenaltyWeight) * q_vel(n_joints+1:end);
    loop_ub = joint_values.size(0);
    if (static_cast<unsigned int>(joint_values.size(0)) + 1U >
        static_cast<unsigned int>(q_vel.size(0))) {
      i = 0;
      i1 = 0;
    } else {
      i = joint_values.size(0);
      i1 = joint_values.size(0);
    }
    std::memset(&b[0], 0, 36U * sizeof(double));
    b[0] = config.SlackPenaltyWeight[0];
    b[7] = config.SlackPenaltyWeight[1];
    b[14] = config.SlackPenaltyWeight[2];
    b[21] = config.SlackPenaltyWeight[3];
    b[28] = config.SlackPenaltyWeight[4];
    b[35] = config.SlackPenaltyWeight[5];
    b_q_vel.set_size(joint_values.size(0));
    b_loop_ub = joint_values.size(0);
    if (static_cast<int>(joint_values.size(0) < 200)) {
      for (int i2{0}; i2 < loop_ub; i2++) {
        b_q_vel[i2] = q_vel[i2];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int i2 = 0; i2 < b_loop_ub; i2++) {
        b_q_vel[i2] = q_vel[i2];
      }
    }
    b_config = 0.0;
    for (loop_ub = 0; loop_ub < 6; loop_ub++) {
      double d;
      d = ((((config.SlackObjectiveWeight * q_vel[i] * b[6 * loop_ub] +
              config.SlackObjectiveWeight * q_vel[i + 1] * b[6 * loop_ub + 1]) +
             config.SlackObjectiveWeight * q_vel[i + 2] * b[6 * loop_ub + 2]) +
            config.SlackObjectiveWeight * q_vel[i + 3] * b[6 * loop_ub + 3]) +
           config.SlackObjectiveWeight * q_vel[i + 4] * b[6 * loop_ub + 4]) +
          config.SlackObjectiveWeight * q_vel[i + 5] * b[6 * loop_ub + 5];
      b_config += d * q_vel[i1 + loop_ub];
    }
    varargout_1 =
        configurableObjective(
            b_q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,
            jointVelocityWeightMatrix, config.useObjectiveNormInfinity,
            config.weightNormInfinity, config.useObjectiveNormL2,
            config.weightNormL2, config.useObjectiveTrajectoryFollowing,
            config.weightTrajectoryFollowing,
            config.useObjectiveJointAcceleration,
            config.weightJointAcceleration, config.useObjectiveManipulability,
            config.weightManipulability) +
        b_config;
  } else {
    int b_loop_ub;
    int loop_ub;
    // 'objFun:7' else
    // 'objFun:8' objVal = configurableObjective(q_vel(1:n_joints), jacobi,
    // xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix,
    // config);
    loop_ub = joint_values.size(0);
    b_q_vel.set_size(joint_values.size(0));
    b_loop_ub = joint_values.size(0);
    if (static_cast<int>(joint_values.size(0) < 200)) {
      for (int i2{0}; i2 < loop_ub; i2++) {
        b_q_vel[i2] = q_vel[i2];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int i2 = 0; i2 < b_loop_ub; i2++) {
        b_q_vel[i2] = q_vel[i2];
      }
    }
    varargout_1 = configurableObjective(
        b_q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,
        jointVelocityWeightMatrix, config.useObjectiveNormInfinity,
        config.weightNormInfinity, config.useObjectiveNormL2,
        config.weightNormL2, config.useObjectiveTrajectoryFollowing,
        config.weightTrajectoryFollowing, config.useObjectiveJointAcceleration,
        config.weightJointAcceleration, config.useObjectiveManipulability,
        config.weightManipulability);
  }
  // /usr/local/src/AndreiUtils
  return varargout_1;
}

//
// Arguments    : coder::array<double, 1U> &in1
//                const coder::array<double, 1U> &in2
//                const coder::array<double, 1U> &in3
// Return Type  : void
//
void minus(coder::array<double, 1U> &in1, const coder::array<double, 1U> &in2,
           const coder::array<double, 1U> &in3)
{
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  if (in3.size(0) == 1) {
    loop_ub = in2.size(0);
  } else {
    loop_ub = in3.size(0);
  }
  in1.set_size(loop_ub);
  stride_0_0 = (in2.size(0) != 1);
  stride_1_0 = (in3.size(0) != 1);
  if (static_cast<int>(loop_ub < 200)) {
    for (int i{0}; i < loop_ub; i++) {
      in1[i] = in2[i * stride_0_0] - in3[i * stride_1_0];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int i = 0; i < loop_ub; i++) {
      in1[i] = in2[i * stride_0_0] - in3[i * stride_1_0];
    }
  }
}

//
// File trailer for OptimizationBasedIKWithOAModified.cpp
//
// [EOF]
//
