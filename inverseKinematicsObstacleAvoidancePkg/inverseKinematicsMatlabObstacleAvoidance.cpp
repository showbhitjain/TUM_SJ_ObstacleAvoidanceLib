//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsMatlabObstacleAvoidance.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "inverseKinematicsMatlabObstacleAvoidance.h"
#include "Hessian_final_link.h"
#include "Jacobi_final_link.h"
#include "anonymous_function.h"
#include "configurableObjective.h"
#include "eye.h"
#include "fmincon.h"
#include "inverseKinematicsOA_internal_types1.h"
#include "inverseKinematicsOA_types.h"
#include "ixfun.h"
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "xgetrf.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Declarations
static void binary_expand_op(coder::array<double, 1U> &in1,
                             const struct0_T *in2,
                             const coder::array<double, 1U> &in3,
                             const coder::array<double, 1U> &in4);

static void inverseKinematicsOA_init(
    inverseKinematicsMatlabObstacleAvoidance *aInstancePtr);

static void minus(coder::array<double, 1U> &in1,
                  const coder::array<double, 1U> &in2,
                  const coder::array<double, 1U> &in3);

// Function Definitions
//
// Arguments    : void
// Return Type  : inverseKinematicsOAStackData *
//
inverseKinematicsOAStackData *
inverseKinematicsMatlabObstacleAvoidance::getStackData()
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
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2->gamma * (in3[i * stride_0_0] - in4[i * stride_1_0]);
  }
}

//
// function [optimal_joint_velocity,Exit_Flag]  =
// inverseKinematicsOA(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,J_g,b_g,jointVelocityWeightMatrix,config)
//
// Arguments    : inverseKinematicsMatlabObstacleAvoidance *aInstancePtr
// Return Type  : void
//
static void
inverseKinematicsOA_init(inverseKinematicsMatlabObstacleAvoidance *aInstancePtr)
{
  inverseKinematicsOAStackData *localSD;
  localSD = aInstancePtr->getStackData();
  localSD->pd->is_first_step_not_empty = false;
  localSD->pd->starting_joint_vel_not_empty = false;
}

//
// Arguments    : coder::array<double, 1U> &in1
//                const coder::array<double, 1U> &in2
//                const coder::array<double, 1U> &in3
// Return Type  : void
//
static void minus(coder::array<double, 1U> &in1,
                  const coder::array<double, 1U> &in2,
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
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2[i * stride_0_0] - in3[i * stride_1_0];
  }
}

//
// Arguments    : void
// Return Type  : void
//
inverseKinematicsMatlabObstacleAvoidance::
    inverseKinematicsMatlabObstacleAvoidance()
{
  SD_.pd = &pd_;
  inverseKinematicsOA_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
inverseKinematicsMatlabObstacleAvoidance::
    ~inverseKinematicsMatlabObstacleAvoidance() = default;

//
// function [optimal_joint_velocity,Exit_Flag]  =
// inverseKinematicsOA(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,J_g,b_g,jointVelocityWeightMatrix,config)
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
//                struct0_T *config
//                coder::array<double, 1U> &optimal_joint_velocity
//                double *Exit_Flag
// Return Type  : void
//
void inverseKinematicsMatlabObstacleAvoidance::inverseKinematicsOA(
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &jointminvalues,
    const coder::array<double, 1U> &jointmaxvalues,
    const coder::array<double, 1U> &joint_min_vel,
    const coder::array<double, 1U> &joint_max_vel,
    const coder::array<double, 2U> &J_g, const coder::array<double, 1U> &b_g,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    struct0_T *config, coder::array<double, 1U> &optimal_joint_velocity,
    double *Exit_Flag)
{
  coder::anonymous_function b_this;
  coder::array<double, 2U> A;
  coder::array<double, 2U> Aeq;
  coder::array<double, 2U> b;
  coder::array<double, 2U> b_result;
  coder::array<double, 2U> lb;
  coder::array<double, 2U> ub;
  coder::array<double, 1U> bp;
  coder::array<double, 1U> min_dist;
  coder::array<double, 1U> varargin_1;
  coder::array<double, 1U> varargin_2;
  coder::array<signed char, 2U> In;
  coder::array<signed char, 2U> c_result;
  double beq_data[6];
  double infuence_dist;
  double stop_dist;
  int beq_size[2];
  int i;
  int loop_ub_tmp;
  int nx;
  int result;
  int sizes_idx_0;
  boolean_T b_b;
  boolean_T empty_non_axis_sizes;
  // 'inverseKinematicsOA:4' n_joints = length(joint_values);
  //  Assuming 'joint_values' is a column vector
  // 'inverseKinematicsOA:5' n_slack = 6;
  //  Number of slack variables, one for each end-effector velocity component
  // total_variables = n_joints + n_slack;  % Total number of variables in the
  // optimization problem
  //  Ensure sizes are compatible for code generation
  // coder.varsize('starting_joint_vel', [n_joints, 1], [true, false]);
  // coder.varsize('optimal_joint_velocity', [n_joints, 1], [true, false]);
  //  Initialize 'starting_joint_vel' if it's the first execution or if it's
  //  undefined (starting joint velocity)
  // 'inverseKinematicsOA:16' if isempty(is_first_step) ||
  // isempty(starting_joint_vel)
  if ((!pd_.is_first_step_not_empty) || (!pd_.starting_joint_vel_not_empty)) {
    // 'inverseKinematicsOA:17' is_first_step = true;
    pd_.is_first_step = true;
    pd_.is_first_step_not_empty = true;
    // 'inverseKinematicsOA:18' starting_joint_vel = zeros(n_joints,1);
    pd_.starting_joint_vel.set_size(joint_values.size(0));
    nx = joint_values.size(0);
    for (i = 0; i < nx; i++) {
      pd_.starting_joint_vel[i] = 0.0;
    }
    //  Default initialization
  }
  //  Then, check if it's the first execution step to perform any first-time
  //  setup
  // 'inverseKinematicsOA:23' if is_first_step
  if (pd_.is_first_step) {
    // 'inverseKinematicsOA:24' is_first_step = false;
    pd_.is_first_step = false;
    //  Ensure this block won't execute again
    //  No need to set 'starting_joint_vel' here again if it's already set above
  }
  // 'inverseKinematicsOA:29' JacobiWeightMatrix = diag([1 1 1 1 1 1]);
  //  delta = weights(1);
  //  alpha = weights(2);
  //  beta = weights(3);
  //  omega = weights(4);
  // 'inverseKinematicsOA:34' bp = zeros(n_joints,1);
  bp.set_size(joint_values.size(0));
  nx = joint_values.size(0);
  for (i = 0; i < nx; i++) {
    bp[i] = 0.0;
  }
  // 'inverseKinematicsOA:35' In = zeros(n_joints);
  In.set_size(joint_values.size(0), joint_values.size(0));
  loop_ub_tmp = joint_values.size(0) * joint_values.size(0);
  for (i = 0; i < loop_ub_tmp; i++) {
    In[i] = 0;
  }
  // 'inverseKinematicsOA:37' if config.applyVelocityDamper
  if (config->applyVelocityDamper) {
    // 'inverseKinematicsOA:38' min_dist = min(abs(jointminvalues -
    // joint_values),abs(jointmaxvalues-joint_values));
    if (jointminvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointminvalues.size(0));
      nx = jointminvalues.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = jointminvalues[i] - joint_values[i];
      }
    } else {
      minus(min_dist, jointminvalues, joint_values);
    }
    nx = min_dist.size(0);
    varargin_1.set_size(min_dist.size(0));
    for (sizes_idx_0 = 0; sizes_idx_0 < nx; sizes_idx_0++) {
      varargin_1[sizes_idx_0] = std::abs(min_dist[sizes_idx_0]);
    }
    if (jointmaxvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointmaxvalues.size(0));
      nx = jointmaxvalues.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = jointmaxvalues[i] - joint_values[i];
      }
    } else {
      minus(min_dist, jointmaxvalues, joint_values);
    }
    nx = min_dist.size(0);
    varargin_2.set_size(min_dist.size(0));
    for (sizes_idx_0 = 0; sizes_idx_0 < nx; sizes_idx_0++) {
      varargin_2[sizes_idx_0] = std::abs(min_dist[sizes_idx_0]);
    }
    if (varargin_1.size(0) == varargin_2.size(0)) {
      min_dist.set_size(varargin_1.size(0));
      nx = varargin_1.size(0);
      for (i = 0; i < nx; i++) {
        infuence_dist = varargin_1[i];
        stop_dist = varargin_2[i];
        min_dist[i] = std::fmin(infuence_dist, stop_dist);
      }
    } else {
      coder::internal::expand_min(varargin_1, varargin_2, min_dist);
    }
    // 'inverseKinematicsOA:39' infuence_dist =
    // deg2rad(config.jointLimitActivationDistance);
    infuence_dist = 0.017453292519943295 * config->jointLimitActivationDistance;
    // 'inverseKinematicsOA:40' stop_dist =
    // deg2rad(config.jointLimitStopDistance);
    stop_dist = 0.017453292519943295 * config->jointLimitStopDistance;
    // 'inverseKinematicsOA:41' joint_limit_gain = config.jointLimitGain;
    // 'inverseKinematicsOA:45' for i= 1:n_joints
    i = joint_values.size(0);
    for (nx = 0; nx < i; nx++) {
      // 'inverseKinematicsOA:46' if min_dist(i) < infuence_dist
      if (min_dist[nx] < infuence_dist) {
        // 'inverseKinematicsOA:47' In(i,i) = 1;
        In[nx + In.size(0) * nx] = 1;
        // 'inverseKinematicsOA:48' bp(i) = joint_limit_gain * (min_dist(i) -
        // stop_dist)/(infuence_dist - stop_dist) ;
        bp[nx] = config->jointLimitGain * (min_dist[nx] - stop_dist) /
                 (infuence_dist - stop_dist);
      }
    }
  }
  //  min_slack = [0.2,0.2,0.2,deg2rad(1),deg2rad(1),deg2rad(60)];
  //  max_slack = [-0.2,-0.2,-0.2,deg2rad(1),deg2rad(1),deg2rad(60)];
  //  %slack distance minimization
  //  slack_dist = [0.1,0.1,0.1,];
  //  Default initializations
  // 'inverseKinematicsOA:60' Aeq = [];
  Aeq.set_size(0, 0);
  // 'inverseKinematicsOA:61' beq = [];
  beq_size[0] = 0;
  beq_size[1] = 0;
  // 'inverseKinematicsOA:62' A   = [];
  A.set_size(0, 0);
  // 'inverseKinematicsOA:63' b   = [];
  b.set_size(0, 0);
  // 'inverseKinematicsOA:64' lb = [];
  lb.set_size(0, 0);
  // 'inverseKinematicsOA:65' ub = [];
  ub.set_size(0, 0);
  //  Check if equality constrjoint_valuesaints should be applied
  // 'inverseKinematicsOA:68' if config.applyEqualityConstraints
  if (config->applyEqualityConstraints) {
    // 'inverseKinematicsOA:69' Aeq = JacobiWeightMatrix * jacobi;
    coder::internal::blas::mtimes(jacobi, Aeq);
    // 'inverseKinematicsOA:70' beq = xd_eff_vel;
    beq_size[0] = 6;
    beq_size[1] = 1;
    for (i = 0; i < 6; i++) {
      beq_data[i] = xd_eff_vel[i];
    }
  }
  //  Check if inequality constraints should be applied
  // 'inverseKinematicsOA:74' if config.applyInequalityConstraints
  if (config->applyInequalityConstraints) {
    // 'inverseKinematicsOA:76' if ~config.applyVelocityDamper
    if (!config->applyVelocityDamper) {
      // 'inverseKinematicsOA:77' A = J_g;
      A.set_size(J_g.size(0), J_g.size(1));
      nx = J_g.size(0) * J_g.size(1);
      for (i = 0; i < nx; i++) {
        A[i] = J_g[i];
      }
      // 'inverseKinematicsOA:78' b = b_g;
      b.set_size(b_g.size(0), 1);
      nx = b_g.size(0);
      for (i = 0; i < nx; i++) {
        b[i] = b_g[i];
      }
    }
    // 'inverseKinematicsOA:80' if config.applyVelocityDamper
    if (config->applyVelocityDamper) {
      // 'inverseKinematicsOA:81' A = [J_g;In];
      b_b = ((J_g.size(0) != 0) && (J_g.size(1) != 0));
      if (b_b) {
        result = J_g.size(1);
      } else if ((In.size(0) != 0) && (In.size(1) != 0)) {
        result = In.size(1);
      } else {
        result = J_g.size(1);
        if (In.size(1) > J_g.size(1)) {
          result = In.size(1);
        }
      }
      empty_non_axis_sizes = (result == 0);
      if (empty_non_axis_sizes || b_b) {
        nx = J_g.size(0);
      } else {
        nx = 0;
      }
      if (empty_non_axis_sizes || ((In.size(0) != 0) && (In.size(1) != 0))) {
        sizes_idx_0 = In.size(0);
      } else {
        sizes_idx_0 = 0;
      }
      A.set_size(nx + sizes_idx_0, result);
      for (i = 0; i < result; i++) {
        for (int i1{0}; i1 < nx; i1++) {
          A[i1 + A.size(0) * i] = J_g[i1 + nx * i];
        }
        for (int i1{0}; i1 < sizes_idx_0; i1++) {
          A[(i1 + nx) + A.size(0) * i] = In[i1 + sizes_idx_0 * i];
        }
      }
      // 'inverseKinematicsOA:82' b = [b_g;bp];
      min_dist.set_size(b_g.size(0) + bp.size(0));
      nx = b_g.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = b_g[i];
      }
      nx = bp.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i + b_g.size(0)] = bp[i];
      }
      b.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        b[i] = min_dist[i];
      }
    }
  }
  // 'inverseKinematicsOA:87' if isempty(J_g) && isempty(b_g)
  if (((J_g.size(0) == 0) || (J_g.size(1) == 0)) && (b_g.size(0) == 0)) {
    // 'inverseKinematicsOA:88' Aeq = JacobiWeightMatrix * jacobi;
    coder::internal::blas::mtimes(jacobi, Aeq);
    // 'inverseKinematicsOA:89' beq = xd_eff_vel;
    beq_size[0] = 6;
    beq_size[1] = 1;
    for (i = 0; i < 6; i++) {
      beq_data[i] = xd_eff_vel[i];
    }
    //  A = J_g;
    //  b = b_g;
    // 'inverseKinematicsOA:92' if config.applyVelocityDamper
    if (config->applyVelocityDamper) {
      // 'inverseKinematicsOA:93' A = In;
      A.set_size(In.size(0), In.size(1));
      for (i = 0; i < loop_ub_tmp; i++) {
        A[i] = In[i];
      }
      // 'inverseKinematicsOA:94' b = bp;
      b.set_size(bp.size(0), 1);
      nx = bp.size(0);
      for (i = 0; i < nx; i++) {
        b[i] = bp[i];
      }
    }
    // 'inverseKinematicsOA:96' config.applySlack = false ;
    config->applySlack = false;
    //  Define the objective function without slack variables
    // objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel,
    // starting_joint_vel,joint_values, config);
    //  Set bounds without slack variables
    // 'inverseKinematicsOA:101' if ~config.applyVelocityDamper
    if (!config->applyVelocityDamper) {
      // 'inverseKinematicsOA:102' lb = max(config.gamma .* (jointminvalues -
      // joint_values), joint_min_vel);
      if (jointminvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointminvalues.size(0));
        nx = jointminvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointminvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointminvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_min_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_min_vel[i];
          min_dist[i] = std::fmax(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_max(varargin_1, joint_min_vel, min_dist);
      }
      lb.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = min_dist[i];
      }
      //  Element-wise max
      // 'inverseKinematicsOA:103' ub = min(config.gamma .* (jointmaxvalues -
      // joint_values), joint_max_vel);
      if (jointmaxvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointmaxvalues.size(0));
        nx = jointmaxvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointmaxvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointmaxvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_max_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_max_vel[i];
          min_dist[i] = std::fmin(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_min(varargin_1, joint_max_vel, min_dist);
      }
      ub.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = min_dist[i];
      }
      //  Element-wise min
    }
    // 'inverseKinematicsOA:105' if config.applyVelocityDamper
    if (config->applyVelocityDamper) {
      // 'inverseKinematicsOA:106' lb = joint_min_vel;
      lb.set_size(joint_min_vel.size(0), 1);
      nx = joint_min_vel.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = joint_min_vel[i];
      }
      // 'inverseKinematicsOA:107' ub = joint_max_vel;
      ub.set_size(joint_max_vel.size(0), 1);
      nx = joint_max_vel.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = joint_max_vel[i];
      }
    }
    //  Initial guess without slack variables
    // q_vel_initial_guess = starting_joint_vel;
  }
  // 'inverseKinematicsOA:113' if config.applySlack
  if (config->applySlack) {
    //  Define the augmented objective function that includes slack penalty
    //  objective = @(q_vel) configurableObjective(q_vel(1:n_joints), jacobi,
    //  xd_eff_vel, starting_joint_vel, joint_values,config) ...
    //           + config.Slack_objective_weight* q_vel(n_joints+1:end)' *
    //           config.Slack_penalty_JacobiWeightMatrix * q_vel(n_joints+1:end)
    //           ;
    //  Augment the bounds for the slack variables
    // 'inverseKinematicsOA:119' if ~config.applyVelocityDamper
    b_b = !config->applyVelocityDamper;
    if (b_b) {
      // 'inverseKinematicsOA:120' lb = [max(config.gamma .* (jointminvalues -
      // joint_values), joint_min_vel); config.Slacklowerbound];
      if (jointminvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointminvalues.size(0));
        nx = jointminvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointminvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointminvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_min_vel.size(0)) {
        varargin_2.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_min_vel[i];
          varargin_2[i] = std::fmax(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_max(varargin_1, joint_min_vel, varargin_2);
      }
      min_dist.set_size(varargin_2.size(0) + 6);
      nx = varargin_2.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = varargin_2[i];
      }
      for (i = 0; i < 6; i++) {
        min_dist[i + varargin_2.size(0)] = config->Slacklowerbound[i];
      }
      lb.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = min_dist[i];
      }
      //  Slack lower bounds
      // 'inverseKinematicsOA:121' ub = [min(config.gamma .* (jointmaxvalues -
      // joint_values), joint_max_vel); config.Slackupperbound];
      if (jointmaxvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointmaxvalues.size(0));
        nx = jointmaxvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointmaxvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointmaxvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_max_vel.size(0)) {
        varargin_2.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_max_vel[i];
          varargin_2[i] = std::fmin(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_min(varargin_1, joint_max_vel, varargin_2);
      }
      min_dist.set_size(varargin_2.size(0) + 6);
      nx = varargin_2.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = varargin_2[i];
      }
      for (i = 0; i < 6; i++) {
        min_dist[i + varargin_2.size(0)] = config->Slackupperbound[i];
      }
      ub.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = min_dist[i];
      }
      //  Slack upper bounds
    }
    // 'inverseKinematicsOA:123' if config.applyVelocityDamper
    if (config->applyVelocityDamper) {
      // 'inverseKinematicsOA:124' lb = [joint_min_vel;config.Slacklowerbound];
      min_dist.set_size(joint_min_vel.size(0) + 6);
      nx = joint_min_vel.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = joint_min_vel[i];
      }
      for (i = 0; i < 6; i++) {
        min_dist[i + joint_min_vel.size(0)] = config->Slacklowerbound[i];
      }
      lb.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = min_dist[i];
      }
      // 'inverseKinematicsOA:125' ub = [joint_max_vel;config.Slackupperbound];
      min_dist.set_size(joint_max_vel.size(0) + 6);
      nx = joint_max_vel.size(0);
      for (i = 0; i < nx; i++) {
        min_dist[i] = joint_max_vel[i];
      }
      for (i = 0; i < 6; i++) {
        min_dist[i + joint_max_vel.size(0)] = config->Slackupperbound[i];
      }
      ub.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = min_dist[i];
      }
    }
    //  Augment the equality constraint matrices for slack variables
    // 'inverseKinematicsOA:128' if config.applyEqualityConstraints
    if (config->applyEqualityConstraints) {
      double dv[36];
      // 'inverseKinematicsOA:130' Aeq = [JacobiWeightMatrix * jacobi,
      // eye(n_slack)];
      coder::internal::blas::mtimes(jacobi, Aeq);
      coder::eye(dv);
      i = Aeq.size(1);
      Aeq.set_size(Aeq.size(0), Aeq.size(1) + 6);
      // 'inverseKinematicsOA:131' beq = xd_eff_vel;
      beq_size[0] = 6;
      beq_size[1] = 1;
      for (int i1{0}; i1 < 6; i1++) {
        for (nx = 0; nx < 6; nx++) {
          Aeq[nx + Aeq.size(0) * (i1 + i)] = dv[nx + 6 * i1];
        }
        beq_data[i1] = xd_eff_vel[i1];
      }
      //  Make sure this accounts for the slack if necessary
    }
    // 'inverseKinematicsOA:135' if config.applyInequalityConstraints
    if (config->applyInequalityConstraints) {
      signed char sizes_idx_1;
      // 'inverseKinematicsOA:136' num_rows = size(J_g,1);
      // 'inverseKinematicsOA:138' if config.applyVelocityDamper
      if (config->applyVelocityDamper) {
        boolean_T b1;
        // 'inverseKinematicsOA:139' A = [[J_g,
        // zeros(num_rows,n_slack)];[In,zeros(n_joints,n_slack)]];
        b1 = ((J_g.size(0) != 0) && (J_g.size(1) != 0));
        if (b1) {
          result = J_g.size(0);
        } else if (J_g.size(0) != 0) {
          result = J_g.size(0);
        } else {
          result = 0;
        }
        empty_non_axis_sizes = (result == 0);
        if (empty_non_axis_sizes || b1) {
          sizes_idx_0 = J_g.size(1);
        } else {
          sizes_idx_0 = 0;
        }
        if (empty_non_axis_sizes || (J_g.size(0) != 0)) {
          sizes_idx_1 = 6;
        } else {
          sizes_idx_1 = 0;
        }
        b_result.set_size(result, sizes_idx_0 + sizes_idx_1);
        for (i = 0; i < sizes_idx_0; i++) {
          for (int i1{0}; i1 < result; i1++) {
            b_result[i1 + b_result.size(0) * i] = J_g[i1 + result * i];
          }
        }
        nx = sizes_idx_1;
        for (i = 0; i < nx; i++) {
          for (int i1{0}; i1 < result; i1++) {
            b_result[i1 + b_result.size(0) * (i + sizes_idx_0)] = 0.0;
          }
        }
        b1 = ((In.size(0) != 0) && (In.size(1) != 0));
        if (b1) {
          result = In.size(0);
        } else if (joint_values.size(0) != 0) {
          result = joint_values.size(0);
        } else {
          result = In.size(0);
        }
        empty_non_axis_sizes = (result == 0);
        if (empty_non_axis_sizes || b1) {
          sizes_idx_0 = In.size(1);
        } else {
          sizes_idx_0 = 0;
        }
        if (empty_non_axis_sizes || (joint_values.size(0) != 0)) {
          nx = 6;
        } else {
          nx = 0;
        }
        c_result.set_size(result, sizes_idx_0 + nx);
        for (i = 0; i < sizes_idx_0; i++) {
          for (int i1{0}; i1 < result; i1++) {
            c_result[i1 + c_result.size(0) * i] = In[i1 + result * i];
          }
        }
        for (i = 0; i < nx; i++) {
          for (int i1{0}; i1 < result; i1++) {
            c_result[i1 + c_result.size(0) * (i + sizes_idx_0)] = 0;
          }
        }
        b1 = ((b_result.size(0) != 0) && (b_result.size(1) != 0));
        if (b1) {
          result = b_result.size(1);
        } else if ((c_result.size(0) != 0) && (c_result.size(1) != 0)) {
          result = c_result.size(1);
        } else {
          result = b_result.size(1);
          if (c_result.size(1) > b_result.size(1)) {
            result = c_result.size(1);
          }
        }
        empty_non_axis_sizes = (result == 0);
        if (empty_non_axis_sizes || b1) {
          nx = b_result.size(0);
        } else {
          nx = 0;
        }
        if (empty_non_axis_sizes ||
            ((c_result.size(0) != 0) && (c_result.size(1) != 0))) {
          sizes_idx_0 = c_result.size(0);
        } else {
          sizes_idx_0 = 0;
        }
        A.set_size(nx + sizes_idx_0, result);
        for (i = 0; i < result; i++) {
          for (int i1{0}; i1 < nx; i1++) {
            A[i1 + A.size(0) * i] = b_result[i1 + nx * i];
          }
          for (int i1{0}; i1 < sizes_idx_0; i1++) {
            A[(i1 + nx) + A.size(0) * i] = c_result[i1 + sizes_idx_0 * i];
          }
        }
        // 'inverseKinematicsOA:140' b = [b_g; bp];
        min_dist.set_size(b_g.size(0) + bp.size(0));
        nx = b_g.size(0);
        for (i = 0; i < nx; i++) {
          min_dist[i] = b_g[i];
        }
        nx = bp.size(0);
        for (i = 0; i < nx; i++) {
          min_dist[i + b_g.size(0)] = bp[i];
        }
        b.set_size(min_dist.size(0), 1);
        nx = min_dist.size(0);
        for (i = 0; i < nx; i++) {
          b[i] = min_dist[i];
        }
        //  Make sure this accounts for the slack if necessary
      }
      // 'inverseKinematicsOA:142' if ~config.applyVelocityDamper
      if (b_b) {
        // 'inverseKinematicsOA:143' A = [J_g, zeros(num_rows,n_slack)];
        b_b = ((J_g.size(0) != 0) && (J_g.size(1) != 0));
        if (b_b) {
          result = J_g.size(0);
        } else if (J_g.size(0) != 0) {
          result = J_g.size(0);
        } else {
          result = 0;
        }
        empty_non_axis_sizes = (result == 0);
        if (empty_non_axis_sizes || b_b) {
          sizes_idx_0 = J_g.size(1);
        } else {
          sizes_idx_0 = 0;
        }
        if (empty_non_axis_sizes || (J_g.size(0) != 0)) {
          sizes_idx_1 = 6;
        } else {
          sizes_idx_1 = 0;
        }
        A.set_size(result, sizes_idx_0 + sizes_idx_1);
        for (i = 0; i < sizes_idx_0; i++) {
          for (int i1{0}; i1 < result; i1++) {
            A[i1 + A.size(0) * i] = J_g[i1 + result * i];
          }
        }
        nx = sizes_idx_1;
        for (i = 0; i < nx; i++) {
          for (int i1{0}; i1 < result; i1++) {
            A[i1 + A.size(0) * (i + sizes_idx_0)] = 0.0;
          }
        }
        // 'inverseKinematicsOA:144' b = b_g;
        b.set_size(b_g.size(0), 1);
        nx = b_g.size(0);
        for (i = 0; i < nx; i++) {
          b[i] = b_g[i];
        }
      }
    }
    //  Initial guess should include the slack variables
    // 'inverseKinematicsOA:149' q_vel_initial_guess = [starting_joint_vel;
    // zeros(n_slack, 1)];
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0) + 6);
    nx = pd_.starting_joint_vel.size(0);
    for (i = 0; i < nx; i++) {
      optimal_joint_velocity[i] = pd_.starting_joint_vel[i];
    }
    for (i = 0; i < 6; i++) {
      optimal_joint_velocity[i + pd_.starting_joint_vel.size(0)] = 0.0;
    }
    //  Initial guess for the decision variable (joint velocities and slack)
  } else {
    // 'inverseKinematicsOA:152' else
    //  Define the objective function without slack variables
    // objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel,
    // starting_joint_vel,joint_values, config);
    //  % Set bounds without slack variables
    // 'inverseKinematicsOA:157' if ~config.applyVelocityDamper
    if (!config->applyVelocityDamper) {
      // 'inverseKinematicsOA:158' lb = max(config.gamma .* (jointminvalues -
      // joint_values), joint_min_vel);
      if (jointminvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointminvalues.size(0));
        nx = jointminvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointminvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointminvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_min_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_min_vel[i];
          min_dist[i] = std::fmax(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_max(varargin_1, joint_min_vel, min_dist);
      }
      lb.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = min_dist[i];
      }
      //  Element-wise max
      // 'inverseKinematicsOA:159' ub = min(config.gamma .* (jointmaxvalues -
      // joint_values), joint_max_vel);
      if (jointmaxvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointmaxvalues.size(0));
        nx = jointmaxvalues.size(0);
        for (i = 0; i < nx; i++) {
          varargin_1[i] = config->gamma * (jointmaxvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, config, jointmaxvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_max_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        nx = varargin_1.size(0);
        for (i = 0; i < nx; i++) {
          infuence_dist = varargin_1[i];
          stop_dist = joint_max_vel[i];
          min_dist[i] = std::fmin(infuence_dist, stop_dist);
        }
      } else {
        coder::internal::expand_min(varargin_1, joint_max_vel, min_dist);
      }
      ub.set_size(min_dist.size(0), 1);
      nx = min_dist.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = min_dist[i];
      }
      //  Element-wise min
    }
    // 'inverseKinematicsOA:161' if config.applyVelocityDamper
    if (config->applyVelocityDamper) {
      // 'inverseKinematicsOA:162' lb = joint_min_vel;
      lb.set_size(joint_min_vel.size(0), 1);
      nx = joint_min_vel.size(0);
      for (i = 0; i < nx; i++) {
        lb[i] = joint_min_vel[i];
      }
      // 'inverseKinematicsOA:163' ub = joint_max_vel;
      ub.set_size(joint_max_vel.size(0), 1);
      nx = joint_max_vel.size(0);
      for (i = 0; i < nx; i++) {
        ub[i] = joint_max_vel[i];
      }
    }
    //  Initial guess without slack variables
    // 'inverseKinematicsOA:166' q_vel_initial_guess = starting_joint_vel;
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0));
    nx = pd_.starting_joint_vel.size(0);
    for (i = 0; i < nx; i++) {
      optimal_joint_velocity[i] = pd_.starting_joint_vel[i];
    }
  }
  // 'Display','off', ...
  // 'Display', 'iter', ... % Displays each iteration progress
  //  Set up optimization options
  // 'inverseKinematicsOA:173' options = optimoptions('fmincon', 'Algorithm',
  // 'sqp', ... 'inverseKinematicsOA:174'     'Display','off', ...
  // 'inverseKinematicsOA:175'     'OptimalityTolerance', 1e-6, ...
  // 'inverseKinematicsOA:176'     'ConstraintTolerance', 1e-6, ...
  // 'inverseKinematicsOA:177'     'StepTolerance', 1e-6, ...
  // 'inverseKinematicsOA:178'     'MaxIterations', 1000);
  //  Run the optimization
  // 'inverseKinematicsOA:181' [q_vel_opt, ~, Exit_Flag] =
  // fmincon(@(q_vel)objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel,
  // joint_values,jointVelocityWeightMatrix, config), q_vel_initial_guess, A, b,
  // Aeq, beq, lb, ub, [], options);
  b_this.workspace.jacobi.set_size(6, jacobi.size(1));
  nx = 6 * jacobi.size(1);
  for (i = 0; i < nx; i++) {
    b_this.workspace.jacobi[i] = jacobi[i];
  }
  for (nx = 0; nx < 6; nx++) {
    b_this.workspace.xd_eff_vel[nx] = xd_eff_vel[nx];
  }
  b_this.workspace.starting_joint_vel.set_size(pd_.starting_joint_vel.size(0));
  nx = pd_.starting_joint_vel.size(0);
  for (i = 0; i < nx; i++) {
    b_this.workspace.starting_joint_vel[i] = pd_.starting_joint_vel[i];
  }
  b_this.workspace.joint_values.set_size(joint_values.size(0));
  nx = joint_values.size(0);
  for (i = 0; i < nx; i++) {
    b_this.workspace.joint_values[i] = joint_values[i];
  }
  b_this.workspace.jointVelocityWeightMatrix.set_size(
      jointVelocityWeightMatrix.size(0), jointVelocityWeightMatrix.size(1));
  nx = jointVelocityWeightMatrix.size(0) * jointVelocityWeightMatrix.size(1);
  for (i = 0; i < nx; i++) {
    b_this.workspace.jointVelocityWeightMatrix[i] =
        jointVelocityWeightMatrix[i];
  }
  b_this.workspace.config = *config;
  coder::fmincon(b_this, optimal_joint_velocity, A, b, Aeq, beq_data, beq_size,
                 lb, ub, Exit_Flag);
  //  Separate the optimal joint velocities and slack variables if slack was
  //  applied
  // 'inverseKinematicsOA:184' if config.applySlack
  if (config->applySlack) {
    // 'inverseKinematicsOA:185' optimal_joint_velocity = q_vel_opt(1:n_joints);
    optimal_joint_velocity.set_size(joint_values.size(0));
    // optimal_slack = q_vel_opt(n_joints+1:end);
  } else {
    // 'inverseKinematicsOA:188' else
    // 'inverseKinematicsOA:189' optimal_joint_velocity = q_vel_opt;
  }
  // 'inverseKinematicsOA:192' starting_joint_vel = optimal_joint_velocity;
  pd_.starting_joint_vel.set_size(optimal_joint_velocity.size(0));
  nx = optimal_joint_velocity.size(0);
  for (i = 0; i < nx; i++) {
    pd_.starting_joint_vel[i] = optimal_joint_velocity[i];
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
//                const coder::array<double, 1U> &q_vel
// Return Type  : double
//
double inverseKinematicsOA_anonFcn1(
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel[6],
    const coder::array<double, 1U> &starting_joint_vel,
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jointVelocityWeightMatrix,
    const struct0_T &config, const coder::array<double, 1U> &q_vel)
{
  coder::array<double, 2U> C;
  coder::array<double, 1U> b;
  double JJt[36];
  double inv_JJt[36];
  double a;
  double absx;
  double d;
  double scale;
  double t;
  double varargout_1;
  int aoffset;
  int b_i;
  int boffset;
  int inner;
  int loop_ub;
  // 'inverseKinematicsOA:181' @(q_vel)objFun(q_vel, jacobi, xd_eff_vel,
  // starting_joint_vel, joint_values,jointVelocityWeightMatrix, config)
  // 'objFun:2' n_joints = length(joint_values);
  // 'objFun:3' objVal = configurableObjective(q_vel(1:n_joints), jacobi,
  // xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix,
  // config);
  loop_ub = joint_values.size(0);
  // 'configurableObjective:2' objVal = 0;
  varargout_1 = 0.0;
  // 'configurableObjective:4' if config.useObjectiveNormInfinity
  if (config.useObjectiveNormInfinity) {
    // 'configurableObjective:5' objVal = objVal + config.weightNormInfinity *
    // (norm(q_vel, Inf)^2) / 2;
    if (joint_values.size(0) == 0) {
      a = 0.0;
    } else {
      a = 0.0;
      for (int k{0}; k < loop_ub; k++) {
        absx = std::abs(q_vel[k]);
        if (std::isnan(absx) || (absx > a)) {
          a = absx;
        }
      }
    }
    varargout_1 = config.weightNormInfinity * (a * a) / 2.0;
  }
  // 'configurableObjective:8' if config.useObjectiveNormL2
  if (config.useObjectiveNormL2) {
    // 'configurableObjective:9' objVal = objVal + config.weightNormL2 * (q_vel'
    // * WeightMatrix * q_vel) / 2;
    inner = joint_values.size(0) - 1;
    aoffset = jointVelocityWeightMatrix.size(1);
    C.set_size(1, jointVelocityWeightMatrix.size(1));
    for (int j{0}; j < aoffset; j++) {
      boffset = j * jointVelocityWeightMatrix.size(0);
      C[j] = 0.0;
      for (int k{0}; k <= inner; k++) {
        C[j] = C[j] + q_vel[k] * jointVelocityWeightMatrix[boffset + k];
      }
    }
    absx = 0.0;
    aoffset = C.size(1);
    for (b_i = 0; b_i < aoffset; b_i++) {
      absx += C[b_i] * q_vel[b_i];
    }
    varargout_1 += config.weightNormL2 * absx / 2.0;
  }
  // 'configurableObjective:12' if config.useObjectiveTrajectoryFollowing
  if (config.useObjectiveTrajectoryFollowing) {
    double y[6];
    // 'configurableObjective:13' objVal = objVal +
    // config.weightTrajectoryFollowing * norm((jacobi * q_vel - xd_eff_vel),
    // 2)^2 / 2;
    inner = jacobi.size(1);
    for (int i{0}; i < 6; i++) {
      y[i] = 0.0;
    }
    for (int k{0}; k < inner; k++) {
      aoffset = k * 6;
      for (int i{0}; i < 6; i++) {
        y[i] += jacobi[aoffset + i] * q_vel[k];
      }
    }
    a = 0.0;
    scale = 3.3121686421112381E-170;
    for (int k{0}; k < 6; k++) {
      d = y[k] - xd_eff_vel[k];
      y[k] = d;
      absx = std::abs(d);
      if (absx > scale) {
        t = scale / absx;
        a = a * t * t + 1.0;
        scale = absx;
      } else {
        t = absx / scale;
        a += t * t;
      }
    }
    a = scale * std::sqrt(a);
    varargout_1 += config.weightTrajectoryFollowing * (a * a) / 2.0;
  }
  // 'configurableObjective:16' if config.useObjectiveJointAcceleration
  if (config.useObjectiveJointAcceleration) {
    // 'configurableObjective:17' objVal =  objVal +
    // config.weightJointAcceleration * ((q_vel - starting_joint_vel)' *
    // WeightMatrix * (q_vel - starting_joint_vel)) /2 ;
    if (joint_values.size(0) == starting_joint_vel.size(0)) {
      b.set_size(joint_values.size(0));
      for (b_i = 0; b_i < loop_ub; b_i++) {
        b[b_i] = q_vel[b_i] - starting_joint_vel[b_i];
      }
    } else {
      binary_expand_op_6(b, q_vel, joint_values.size(0) - 1,
                         starting_joint_vel);
    }
    inner = b.size(0);
    aoffset = jointVelocityWeightMatrix.size(1);
    C.set_size(1, jointVelocityWeightMatrix.size(1));
    for (int j{0}; j < aoffset; j++) {
      boffset = j * jointVelocityWeightMatrix.size(0);
      C[j] = 0.0;
      for (int k{0}; k < inner; k++) {
        C[j] = C[j] + b[k] * jointVelocityWeightMatrix[boffset + k];
      }
    }
    absx = 0.0;
    loop_ub = C.size(1);
    for (b_i = 0; b_i < loop_ub; b_i++) {
      absx += C[b_i] * b[b_i];
    }
    varargout_1 += config.weightJointAcceleration * absx / 2.0;
  }
  // 'configurableObjective:20' if config.useObjectiveManipulability
  if (config.useObjectiveManipulability) {
    double H[294];
    double J[42];
    int ipiv[6];
    signed char p[6];
    boolean_T isodd;
    // 'configurableObjective:21' objVal  = objVal + config.weightManipulability
    // * (-computeManipulabilityJacobian(joint_values') * q_vel);
    //  Compute the Jacobian and Hessian first
    // [~, J, H] = computeKinematics(joint_values, mdhparams); because of syms
    // was too slow J = Jacobi_final_link1(joint_values); %for code generation
    // H = Hessian_final_link(joint_values); %for code generation
    // 'computeManipulabilityJacobian:7' J = Jacobi_final_link(joint_values);
    C.set_size(1, joint_values.size(0));
    loop_ub = joint_values.size(0);
    for (b_i = 0; b_i < loop_ub; b_i++) {
      C[b_i] = joint_values[b_i];
    }
    Jacobi_final_link(C, J);
    // 'computeManipulabilityJacobian:8' H = Hessian_final_link(joint_values);
    C.set_size(1, joint_values.size(0));
    loop_ub = joint_values.size(0);
    for (b_i = 0; b_i < loop_ub; b_i++) {
      C[b_i] = joint_values[b_i];
    }
    Hessian_final_link(C, H);
    //  Compute the manipulability measure 'm'
    // 'computeManipulabilityJacobian:10' JJt = J * J';
    for (b_i = 0; b_i < 6; b_i++) {
      for (boffset = 0; boffset < 6; boffset++) {
        d = 0.0;
        for (aoffset = 0; aoffset < 7; aoffset++) {
          d += J[b_i + 6 * aoffset] * J[boffset + 6 * aoffset];
        }
        JJt[b_i + 6 * boffset] = d;
      }
    }
    // 'computeManipulabilityJacobian:11' m = sqrt(det(JJt));
    std::copy(&JJt[0], &JJt[36], &inv_JJt[0]);
    coder::internal::lapack::xgetrf(inv_JJt, ipiv);
    absx = inv_JJt[0];
    isodd = false;
    for (int k{0}; k < 5; k++) {
      absx *= inv_JJt[(k + 6 * (k + 1)) + 1];
      if (ipiv[k] > k + 1) {
        isodd = !isodd;
      }
    }
    if (isodd) {
      absx = -absx;
    }
    t = std::sqrt(absx);
    //  Compute the manipulability Jacobian 'Jm'
    // 'computeManipulabilityJacobian:14' n = size(J, 2);
    //  Number of joints
    // 'computeManipulabilityJacobian:15' Jm_transposed = zeros(1, n);
    //  Initialize manipulability Jacobian transposed
    //  vec operation implemented as a function for convenience
    // 'computeManipulabilityJacobian:18' vec = @(X) reshape(X, [], 1);
    //  Pre-compute the inverse of JJt
    // 'computeManipulabilityJacobian:21' inv_JJt = inv(JJt);
    std::memset(&inv_JJt[0], 0, 36U * sizeof(double));
    coder::internal::lapack::xgetrf(JJt, ipiv);
    for (b_i = 0; b_i < 6; b_i++) {
      p[b_i] = static_cast<signed char>(b_i + 1);
    }
    for (int k{0}; k < 5; k++) {
      b_i = ipiv[k];
      if (b_i > k + 1) {
        aoffset = p[b_i - 1];
        p[b_i - 1] = p[k];
        p[k] = static_cast<signed char>(aoffset);
      }
    }
    for (int k{0}; k < 6; k++) {
      inner = 6 * (p[k] - 1);
      inv_JJt[k + inner] = 1.0;
      for (int j{k + 1}; j < 7; j++) {
        b_i = (j + inner) - 1;
        if (inv_JJt[b_i] != 0.0) {
          boffset = j + 1;
          for (int i{boffset}; i < 7; i++) {
            aoffset = (i + inner) - 1;
            inv_JJt[aoffset] -= inv_JJt[b_i] * JJt[(i + 6 * (j - 1)) - 1];
          }
        }
      }
    }
    for (int j{0}; j < 6; j++) {
      aoffset = 6 * j;
      for (int k{5}; k >= 0; k--) {
        boffset = 6 * k;
        b_i = k + aoffset;
        d = inv_JJt[b_i];
        if (d != 0.0) {
          inv_JJt[b_i] = d / JJt[k + boffset];
          for (int i{0}; i < k; i++) {
            inner = i + aoffset;
            inv_JJt[inner] -= inv_JJt[b_i] * JJt[i + boffset];
          }
        }
      }
    }
    //
    //  Pre-compute the pseudo-inverse of JJt
    // pseudo_inv_JJt = pinv(JJt);
    // 'computeManipulabilityJacobian:26' for i = 1:n
    scale = 0.0;
    for (int i{0}; i < 7; i++) {
      // 'computeManipulabilityJacobian:27' Hi = H(:,:,i);
      // 'computeManipulabilityJacobian:28' term1 = vec(J * Hi')' *
      // vec(inv_JJt); 'computeManipulabilityJacobian:18' @(X) reshape(X, [], 1)
      // 'computeManipulabilityJacobian:29' Jm_transposed(i) = m * term1;
      for (b_i = 0; b_i < 6; b_i++) {
        for (boffset = 0; boffset < 6; boffset++) {
          d = 0.0;
          for (aoffset = 0; aoffset < 7; aoffset++) {
            d += J[b_i + 6 * aoffset] * H[(boffset + 6 * aoffset) + 42 * i];
          }
          JJt[b_i + 6 * boffset] = d;
        }
      }
      absx = 0.0;
      for (b_i = 0; b_i < 36; b_i++) {
        absx += JJt[b_i] * inv_JJt[b_i];
      }
      //  Equation (11)
      scale += -(t * absx) * q_vel[i];
    }
    varargout_1 += config.weightManipulability * scale;
  }
  //  You can add more terms similarly with their checks and weights
  //  Add the cost for the slack variables
  // 'objFun:6' if config.applySlack
  if (config.applySlack) {
    // 'objFun:7' slack = q_vel(n_joints+1:end);
    if (static_cast<unsigned int>(joint_values.size(0)) + 1U >
        static_cast<unsigned int>(q_vel.size(0))) {
      b_i = 0;
    } else {
      b_i = joint_values.size(0);
    }
    // 'objFun:8' objVal = objVal + config.Slack_objective_weight * slack' *
    // config.Slack_penalty_weightmatrix * slack;
    absx = 0.0;
    for (boffset = 0; boffset < 6; boffset++) {
      d = 0.0;
      for (aoffset = 0; aoffset < 6; aoffset++) {
        d += config.Slack_objective_weight * q_vel[b_i + aoffset] *
             config.Slack_penalty_weightmatrix[aoffset + 6 * boffset];
      }
      absx += d * q_vel[b_i + boffset];
    }
    varargout_1 += absx;
  }
  return varargout_1;
}

//
// File trailer for inverseKinematicsMatlabObstacleAvoidance.cpp
//
// [EOF]
//
