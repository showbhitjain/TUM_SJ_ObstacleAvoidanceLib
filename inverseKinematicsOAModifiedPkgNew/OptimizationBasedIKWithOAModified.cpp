//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: OptimizationBasedIKWithOAModified.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "configurableObjective.h"
#include "fmincon.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_types.h"
#include "ixfun.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

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
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2->gamma * (in3[i * stride_0_0] - in4[i * stride_1_0]);
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
  localSD->pd->is_first_step_not_empty = false;
  localSD->pd->starting_joint_vel_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
OptimizationBasedIKWithOAModified::OptimizationBasedIKWithOAModified()
{
  SD_.pd = &pd_;
  inverseKinematicsOAModified_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
OptimizationBasedIKWithOAModified::~OptimizationBasedIKWithOAModified() =
    default;

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
    coder::array<double, 1U> &optimal_joint_velocity, double *Exit_Flag)
{
  static const signed char iv[36]{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                                  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
  coder::anonymous_function b_this;
  coder::array<double, 2U> A;
  coder::array<double, 2U> Aeq;
  coder::array<double, 2U> b;
  coder::array<double, 2U> lb;
  coder::array<double, 2U> ub;
  coder::array<double, 1U> bp;
  coder::array<double, 1U> min_dist;
  coder::array<double, 1U> varargin_1;
  coder::array<double, 1U> varargin_2;
  coder::array<signed char, 2U> In;
  double beq_data[6];
  double b_varargin_1;
  double b_varargin_2;
  double infuence_dist;
  double stop_dist;
  int beq_size[2];
  int i;
  int input_sizes_idx_1;
  int loop_ub;
  int loop_ub_tmp;
  int nx;
  boolean_T slackIsUsed;
  // ----------------------------------------------------------------------
  //  1. PERSISTENT VARIABLES
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:19' n_joints = length(joint_values);
  //  Number of joints
  // 'inverseKinematicsOAModified:20' n_slack  = 6;
  //  One slack variable per end-effector velocity component (if used)
  //  Initialize the persistent variables only on the first call
  // 'inverseKinematicsOAModified:23' if isempty(is_first_step) ||
  // isempty(starting_joint_vel)
  if ((!pd_.is_first_step_not_empty) || (!pd_.starting_joint_vel_not_empty)) {
    // 'inverseKinematicsOAModified:24' is_first_step = true;
    pd_.is_first_step = true;
    pd_.is_first_step_not_empty = true;
    // 'inverseKinematicsOAModified:25' starting_joint_vel = zeros(n_joints,1);
    pd_.starting_joint_vel.set_size(joint_values.size(0));
    loop_ub = joint_values.size(0);
    for (i = 0; i < loop_ub; i++) {
      pd_.starting_joint_vel[i] = 0.0;
    }
    //  Default
  }
  // 'inverseKinematicsOAModified:29' if is_first_step
  if (pd_.is_first_step) {
    // 'inverseKinematicsOAModified:30' is_first_step = false;
    pd_.is_first_step = false;
  }
  //  Unpack config for convenience
  // 'inverseKinematicsOAModified:34' config = configInput;
  // ----------------------------------------------------------------------
  //  2. DETERMINE IF SLACK IS ACTUALLY USED
  // ----------------------------------------------------------------------
  //  The new dynamic logic:
  //    - If config.dynamicSlack is true AND (J_g or b_g are empty),
  //      we forcibly turn off slack.
  //    - Otherwise, we do whatever config.applySlack says.
  //
  // 'inverseKinematicsOAModified:44' slackIsUsed = config.applySlack;
  slackIsUsed = configInput->applySlack;
  // 'inverseKinematicsOAModified:45' if config.dynamicSlack &&
  // config.obstacleAvoidanceScheme
  if (configInput->dynamicSlack && configInput->obstacleAvoidanceScheme &&
      ((J_g.size(0) == 0) || (J_g.size(1) == 0) || (b_g.size(0) == 0))) {
    // 'inverseKinematicsOAModified:46' if isempty(J_g) || isempty(b_g)
    // 'inverseKinematicsOAModified:47' slackIsUsed = false;
    slackIsUsed = false;
  }
  // ----------------------------------------------------------------------
  //  3. BUILD VELOCITY DAMPER MATRICES IF NEEDED
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:54' In            = zeros(n_joints);
  In.set_size(joint_values.size(0), joint_values.size(0));
  loop_ub_tmp = joint_values.size(0) * joint_values.size(0);
  for (i = 0; i < loop_ub_tmp; i++) {
    In[i] = 0;
  }
  //  Will hold diagonal matrix for damped joints
  // 'inverseKinematicsOAModified:55' bp            = zeros(n_joints,1);
  bp.set_size(joint_values.size(0));
  loop_ub = joint_values.size(0);
  for (i = 0; i < loop_ub; i++) {
    bp[i] = 0.0;
  }
  //  Will hold vector for velocity damper if needed
  // 'inverseKinematicsOAModified:56' infuence_dist =
  // deg2rad(config.jointLimitActivationDistance);
  infuence_dist =
      0.017453292519943295 * configInput->jointLimitActivationDistance;
  // 'inverseKinematicsOAModified:57' stop_dist     =
  // deg2rad(config.jointLimitStopDistance);
  stop_dist = 0.017453292519943295 * configInput->jointLimitStopDistance;
  // 'inverseKinematicsOAModified:58' joint_gain    = config.jointLimitGain;
  // 'inverseKinematicsOAModified:60' if config.applyVelocityDamper
  if (configInput->applyVelocityDamper) {
    //  Compute distance to each joint limit, apply a linear velocity damper
    // 'inverseKinematicsOAModified:63' min_dist = min(abs(jointminvalues -
    // joint_values), ... 'inverseKinematicsOAModified:64' abs(jointmaxvalues -
    // joint_values));
    if (jointminvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointminvalues.size(0));
      loop_ub = jointminvalues.size(0);
      for (i = 0; i < loop_ub; i++) {
        min_dist[i] = jointminvalues[i] - joint_values[i];
      }
    } else {
      minus(min_dist, jointminvalues, joint_values);
    }
    nx = min_dist.size(0);
    varargin_1.set_size(min_dist.size(0));
    for (int k{0}; k < nx; k++) {
      varargin_1[k] = std::abs(min_dist[k]);
    }
    if (jointmaxvalues.size(0) == joint_values.size(0)) {
      min_dist.set_size(jointmaxvalues.size(0));
      loop_ub = jointmaxvalues.size(0);
      for (i = 0; i < loop_ub; i++) {
        min_dist[i] = jointmaxvalues[i] - joint_values[i];
      }
    } else {
      minus(min_dist, jointmaxvalues, joint_values);
    }
    nx = min_dist.size(0);
    varargin_2.set_size(min_dist.size(0));
    for (int k{0}; k < nx; k++) {
      varargin_2[k] = std::abs(min_dist[k]);
    }
    if (varargin_1.size(0) == varargin_2.size(0)) {
      min_dist.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      for (i = 0; i < loop_ub; i++) {
        b_varargin_1 = varargin_1[i];
        b_varargin_2 = varargin_2[i];
        min_dist[i] = std::fmin(b_varargin_1, b_varargin_2);
      }
    } else {
      coder::internal::expand_min(varargin_1, varargin_2, min_dist);
    }
    // 'inverseKinematicsOAModified:65' for i = 1:n_joints
    i = joint_values.size(0);
    for (int b_i{0}; b_i < i; b_i++) {
      // 'inverseKinematicsOAModified:66' if min_dist(i) < infuence_dist
      if (min_dist[b_i] < infuence_dist) {
        // 'inverseKinematicsOAModified:67' In(i,i)  = 1;
        In[b_i + In.size(0) * b_i] = 1;
        // 'inverseKinematicsOAModified:68' bp(i)    = joint_gain * (min_dist(i)
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
  // 'inverseKinematicsOAModified:77' Aeq = [];
  Aeq.set_size(0, 0);
  // 'inverseKinematicsOAModified:78' beq = [];
  beq_size[0] = 0;
  beq_size[1] = 0;
  // 'inverseKinematicsOAModified:79' A   = [];
  A.set_size(0, 0);
  // 'inverseKinematicsOAModified:80' b   = [];
  b.set_size(0, 0);
  // 'inverseKinematicsOAModified:81' lb  = [];
  // 'inverseKinematicsOAModified:82' ub  = [];
  //  Weight matrix for Jacobian in equality constraints
  // 'inverseKinematicsOAModified:85' JacobiWeightMatrix = diag([1,1,1,1,1,1]);
  //  ---- 4.1 Equality constraints ---------------------------------------
  // 'inverseKinematicsOAModified:88' if config.applyEqualityConstraints
  if (configInput->applyEqualityConstraints) {
    // 'inverseKinematicsOAModified:89' Aeq = JacobiWeightMatrix * jacobi;
    nx = jacobi.size(1);
    Aeq.set_size(6, jacobi.size(1));
    for (loop_ub = 0; loop_ub < nx; loop_ub++) {
      input_sizes_idx_1 = loop_ub * 6;
      for (int b_i{0}; b_i < 6; b_i++) {
        infuence_dist = 0.0;
        for (int k{0}; k < 6; k++) {
          infuence_dist += static_cast<double>(iv[k * 6 + b_i]) *
                           jacobi[input_sizes_idx_1 + k];
        }
        Aeq[input_sizes_idx_1 + b_i] = infuence_dist;
      }
    }
    // 'inverseKinematicsOAModified:90' beq = xd_eff_vel;
    beq_size[0] = 6;
    beq_size[1] = 1;
    for (i = 0; i < 6; i++) {
      beq_data[i] = xd_eff_vel[i];
    }
  }
  //  ---- 4.2 Inequality constraints -------------------------------------
  // 'inverseKinematicsOAModified:94' if config.applyInequalityConstraints
  if (configInput->applyInequalityConstraints) {
    // 'inverseKinematicsOAModified:95' if ~isempty(J_g) && ~isempty(b_g)
    if ((J_g.size(0) != 0) && (J_g.size(1) != 0) && (b_g.size(0) != 0)) {
      // 'inverseKinematicsOAModified:96' if config.applyVelocityDamper
      if (configInput->applyVelocityDamper) {
        //  Combine both standard inequalities and velocity damper
        // 'inverseKinematicsOAModified:98' A = [J_g; In];
        loop_ub_tmp = J_g.size(0);
        if ((In.size(0) != 0) && (In.size(1) != 0)) {
          nx = In.size(0);
        } else {
          nx = 0;
        }
        A.set_size(J_g.size(0) + nx, J_g.size(1));
        loop_ub = J_g.size(1);
        for (i = 0; i < loop_ub; i++) {
          for (int k{0}; k < loop_ub_tmp; k++) {
            A[k + A.size(0) * i] = J_g[k + loop_ub_tmp * i];
          }
          for (int k{0}; k < nx; k++) {
            A[(k + loop_ub_tmp) + A.size(0) * i] = In[k + nx * i];
          }
        }
        // 'inverseKinematicsOAModified:99' b = [b_g; bp];
        min_dist.set_size(b_g.size(0) + bp.size(0));
        loop_ub = b_g.size(0);
        for (i = 0; i < loop_ub; i++) {
          min_dist[i] = b_g[i];
        }
        loop_ub = bp.size(0);
        for (i = 0; i < loop_ub; i++) {
          min_dist[i + b_g.size(0)] = bp[i];
        }
        b.set_size(min_dist.size(0), 1);
        loop_ub = min_dist.size(0);
        for (i = 0; i < loop_ub; i++) {
          b[i] = min_dist[i];
        }
      } else {
        // 'inverseKinematicsOAModified:100' else
        //  No velocity damper
        // 'inverseKinematicsOAModified:102' A = J_g;
        A.set_size(J_g.size(0), J_g.size(1));
        loop_ub = J_g.size(0) * J_g.size(1);
        for (i = 0; i < loop_ub; i++) {
          A[i] = J_g[i];
        }
        // 'inverseKinematicsOAModified:103' b = b_g;
        b.set_size(b_g.size(0), 1);
        loop_ub = b_g.size(0);
        for (i = 0; i < loop_ub; i++) {
          b[i] = b_g[i];
        }
      }

      // 'inverseKinematicsOAModified:105' else
      //  If J_g, b_g are empty, we only apply velocity damper if set
      // 'inverseKinematicsOAModified:107' if config.applyVelocityDamper
    } else if (configInput->applyVelocityDamper) {
      // 'inverseKinematicsOAModified:108' A = In;
      A.set_size(In.size(0), In.size(1));
      for (i = 0; i < loop_ub_tmp; i++) {
        A[i] = In[i];
      }
      // 'inverseKinematicsOAModified:109' b = bp;
      b.set_size(bp.size(0), 1);
      loop_ub = bp.size(0);
      for (i = 0; i < loop_ub; i++) {
        b[i] = bp[i];
      }
    }

    // 'inverseKinematicsOAModified:112' else
    //  If we do not apply inequality constraints at all but velocity
    //  damper is on, we can still do that by adding A=In, b=bp.
    // 'inverseKinematicsOAModified:115' if config.applyVelocityDamper
  } else if (configInput->applyVelocityDamper) {
    // 'inverseKinematicsOAModified:116' A = In;
    A.set_size(In.size(0), In.size(1));
    for (i = 0; i < loop_ub_tmp; i++) {
      A[i] = In[i];
    }
    // 'inverseKinematicsOAModified:117' b = bp;
    b.set_size(bp.size(0), 1);
    loop_ub = bp.size(0);
    for (i = 0; i < loop_ub; i++) {
      b[i] = bp[i];
    }

    //  if config.applyVelocityDamper && (isempty(J_g) || isempty(b_g))
    //      A = In;
    //      b = bp;
    //  end
  }
  //  ---- 4.3 Determine final use of Slack and build lb, ub --------------
  // 'inverseKinematicsOAModified:127' if ~slackIsUsed
  if (!slackIsUsed) {
    //  BOUNDS without slack
    // 'inverseKinematicsOAModified:129' if config.applyVelocityDamper
    if (configInput->applyVelocityDamper) {
      //  We do not scale velocities by gamma if velocity damper is active
      // 'inverseKinematicsOAModified:131' lb = joint_min_vel;
      lb.set_size(joint_min_vel.size(0), 1);
      loop_ub = joint_min_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        lb[i] = joint_min_vel[i];
      }
      // 'inverseKinematicsOAModified:132' ub = joint_max_vel;
      ub.set_size(joint_max_vel.size(0), 1);
      loop_ub = joint_max_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        ub[i] = joint_max_vel[i];
      }
    } else {
      // 'inverseKinematicsOAModified:133' else
      //  Normal bounds: scale the move by config.gamma
      // 'inverseKinematicsOAModified:135' lb = max(config.gamma .*
      // (jointminvalues - joint_values), joint_min_vel);
      if (jointminvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointminvalues.size(0));
        loop_ub = jointminvalues.size(0);
        for (i = 0; i < loop_ub; i++) {
          varargin_1[i] =
              configInput->gamma * (jointminvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, configInput, jointminvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_min_vel.size(0)) {
        varargin_2.set_size(varargin_1.size(0));
        loop_ub = varargin_1.size(0);
        for (i = 0; i < loop_ub; i++) {
          b_varargin_1 = varargin_1[i];
          b_varargin_2 = joint_min_vel[i];
          varargin_2[i] = std::fmax(b_varargin_1, b_varargin_2);
        }
      } else {
        coder::internal::expand_max(varargin_1, joint_min_vel, varargin_2);
      }
      lb.set_size(varargin_2.size(0), 1);
      loop_ub = varargin_2.size(0);
      for (i = 0; i < loop_ub; i++) {
        lb[i] = varargin_2[i];
      }
      // 'inverseKinematicsOAModified:136' ub = min(config.gamma .*
      // (jointmaxvalues - joint_values), joint_max_vel);
      if (jointmaxvalues.size(0) == joint_values.size(0)) {
        varargin_1.set_size(jointmaxvalues.size(0));
        loop_ub = jointmaxvalues.size(0);
        for (i = 0; i < loop_ub; i++) {
          varargin_1[i] =
              configInput->gamma * (jointmaxvalues[i] - joint_values[i]);
        }
      } else {
        binary_expand_op(varargin_1, configInput, jointmaxvalues, joint_values);
      }
      if (varargin_1.size(0) == joint_max_vel.size(0)) {
        min_dist.set_size(varargin_1.size(0));
        loop_ub = varargin_1.size(0);
        for (i = 0; i < loop_ub; i++) {
          b_varargin_1 = varargin_1[i];
          b_varargin_2 = joint_max_vel[i];
          min_dist[i] = std::fmin(b_varargin_1, b_varargin_2);
        }
      } else {
        coder::internal::expand_min(varargin_1, joint_max_vel, min_dist);
      }
      ub.set_size(min_dist.size(0), 1);
      loop_ub = min_dist.size(0);
      for (i = 0; i < loop_ub; i++) {
        ub[i] = min_dist[i];
      }
    }

    // 'inverseKinematicsOAModified:138' else
    //  BOUNDS with slack
    //     first n_joints are for joint velocities
    //     next   n_slack are for slack
    // 'inverseKinematicsOAModified:142' if config.applyVelocityDamper
  } else if (configInput->applyVelocityDamper) {
    // 'inverseKinematicsOAModified:143' lb_joints = joint_min_vel;
    // 'inverseKinematicsOAModified:144' ub_joints = joint_max_vel;
    // 'inverseKinematicsOAModified:145' lb_slack = config.Slacklowerbound;
    // 'inverseKinematicsOAModified:146' ub_slack = config.Slackupperbound;
    // 'inverseKinematicsOAModified:147' lb       = [lb_joints; lb_slack];
    varargin_2.set_size(joint_min_vel.size(0) + 6);
    loop_ub = joint_min_vel.size(0);
    for (i = 0; i < loop_ub; i++) {
      varargin_2[i] = joint_min_vel[i];
    }
    for (i = 0; i < 6; i++) {
      varargin_2[i + joint_min_vel.size(0)] = configInput->Slacklowerbound[i];
    }
    lb.set_size(varargin_2.size(0), 1);
    loop_ub = varargin_2.size(0);
    for (i = 0; i < loop_ub; i++) {
      lb[i] = varargin_2[i];
    }
    // 'inverseKinematicsOAModified:148' ub       = [ub_joints; ub_slack];
    min_dist.set_size(joint_max_vel.size(0) + 6);
    loop_ub = joint_max_vel.size(0);
    for (i = 0; i < loop_ub; i++) {
      min_dist[i] = joint_max_vel[i];
    }
    for (i = 0; i < 6; i++) {
      min_dist[i + joint_max_vel.size(0)] = configInput->Slackupperbound[i];
    }
    ub.set_size(min_dist.size(0), 1);
    loop_ub = min_dist.size(0);
    for (i = 0; i < loop_ub; i++) {
      ub[i] = min_dist[i];
    }
  } else {
    // 'inverseKinematicsOAModified:149' else
    // 'inverseKinematicsOAModified:150' lb_joints = max(config.gamma .*
    // (jointminvalues - joint_values), joint_min_vel);
    if (jointminvalues.size(0) == joint_values.size(0)) {
      varargin_1.set_size(jointminvalues.size(0));
      loop_ub = jointminvalues.size(0);
      for (i = 0; i < loop_ub; i++) {
        varargin_1[i] =
            configInput->gamma * (jointminvalues[i] - joint_values[i]);
      }
    } else {
      binary_expand_op(varargin_1, configInput, jointminvalues, joint_values);
    }
    if (varargin_1.size(0) == joint_min_vel.size(0)) {
      min_dist.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      for (i = 0; i < loop_ub; i++) {
        b_varargin_1 = varargin_1[i];
        b_varargin_2 = joint_min_vel[i];
        min_dist[i] = std::fmax(b_varargin_1, b_varargin_2);
      }
    } else {
      coder::internal::expand_max(varargin_1, joint_min_vel, min_dist);
    }
    // 'inverseKinematicsOAModified:151' ub_joints = min(config.gamma .*
    // (jointmaxvalues - joint_values), joint_max_vel);
    if (jointmaxvalues.size(0) == joint_values.size(0)) {
      varargin_1.set_size(jointmaxvalues.size(0));
      loop_ub = jointmaxvalues.size(0);
      for (i = 0; i < loop_ub; i++) {
        varargin_1[i] =
            configInput->gamma * (jointmaxvalues[i] - joint_values[i]);
      }
    } else {
      binary_expand_op(varargin_1, configInput, jointmaxvalues, joint_values);
    }
    if (varargin_1.size(0) == joint_max_vel.size(0)) {
      bp.set_size(varargin_1.size(0));
      loop_ub = varargin_1.size(0);
      for (i = 0; i < loop_ub; i++) {
        b_varargin_1 = varargin_1[i];
        b_varargin_2 = joint_max_vel[i];
        bp[i] = std::fmin(b_varargin_1, b_varargin_2);
      }
    } else {
      coder::internal::expand_min(varargin_1, joint_max_vel, bp);
    }
    // 'inverseKinematicsOAModified:152' lb_slack = config.Slacklowerbound;
    // 'inverseKinematicsOAModified:153' ub_slack = config.Slackupperbound;
    // 'inverseKinematicsOAModified:154' lb       = [lb_joints; lb_slack];
    varargin_2.set_size(min_dist.size(0) + 6);
    loop_ub = min_dist.size(0);
    for (i = 0; i < loop_ub; i++) {
      varargin_2[i] = min_dist[i];
    }
    for (i = 0; i < 6; i++) {
      varargin_2[i + min_dist.size(0)] = configInput->Slacklowerbound[i];
    }
    lb.set_size(varargin_2.size(0), 1);
    loop_ub = varargin_2.size(0);
    for (i = 0; i < loop_ub; i++) {
      lb[i] = varargin_2[i];
    }
    // 'inverseKinematicsOAModified:155' ub       = [ub_joints; ub_slack];
    min_dist.set_size(bp.size(0) + 6);
    loop_ub = bp.size(0);
    for (i = 0; i < loop_ub; i++) {
      min_dist[i] = bp[i];
    }
    for (i = 0; i < 6; i++) {
      min_dist[i + bp.size(0)] = configInput->Slackupperbound[i];
    }
    ub.set_size(min_dist.size(0), 1);
    loop_ub = min_dist.size(0);
    for (i = 0; i < loop_ub; i++) {
      ub[i] = min_dist[i];
    }
  }
  //  ---- 4.4 Expand constraints for Slack if needed ----------------------
  // 'inverseKinematicsOAModified:162' if slackIsUsed
  if (slackIsUsed) {
    //  Expand the equality constraints
    // 'inverseKinematicsOAModified:164' if config.applyEqualityConstraints
    if (configInput->applyEqualityConstraints) {
      signed char b_I[36];
      // 'inverseKinematicsOAModified:165' Aeq = [JacobiWeightMatrix * jacobi,
      // eye(n_slack)];
      for (i = 0; i < 36; i++) {
        b_I[i] = 0;
      }
      for (int k{0}; k < 6; k++) {
        b_I[k + 6 * k] = 1;
      }
      nx = jacobi.size(1);
      Aeq.set_size(6, jacobi.size(1));
      for (loop_ub = 0; loop_ub < nx; loop_ub++) {
        input_sizes_idx_1 = loop_ub * 6;
        for (int b_i{0}; b_i < 6; b_i++) {
          infuence_dist = 0.0;
          for (int k{0}; k < 6; k++) {
            infuence_dist += static_cast<double>(iv[k * 6 + b_i]) *
                             jacobi[input_sizes_idx_1 + k];
          }
          Aeq[input_sizes_idx_1 + b_i] = infuence_dist;
        }
      }
      i = Aeq.size(1);
      Aeq.set_size(Aeq.size(0), Aeq.size(1) + 6);
      // 'inverseKinematicsOAModified:166' beq = xd_eff_vel;
      beq_size[0] = 6;
      beq_size[1] = 1;
      for (int k{0}; k < 6; k++) {
        for (nx = 0; nx < 6; nx++) {
          Aeq[nx + Aeq.size(0) * (k + i)] = b_I[nx + 6 * k];
        }
        beq_data[k] = xd_eff_vel[k];
      }
    }
    //  Expand the inequality constraints
    // 'inverseKinematicsOAModified:170' if ~isempty(A)
    if ((A.size(0) != 0) && (A.size(1) != 0)) {
      // 'inverseKinematicsOAModified:171' A = [A, zeros(size(A,1), n_slack)];
      nx = A.size(0);
      loop_ub_tmp = A.size(0);
      input_sizes_idx_1 = A.size(1);
      loop_ub = A.size(1);
      for (i = 0; i < loop_ub; i++) {
        for (int k{0}; k < nx; k++) {
          A[k + A.size(0) * i] = A[k + loop_ub_tmp * i];
        }
      }
      A.set_size(A.size(0), A.size(1) + 6);
      for (i = 0; i < 6; i++) {
        for (int k{0}; k < nx; k++) {
          A[k + A.size(0) * (i + input_sizes_idx_1)] = 0.0;
        }
      }
    }
    // 'inverseKinematicsOAModified:179' q_vel_initial_guess =
    // [starting_joint_vel;zeros(n_slack,1)];
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0) + 6);
    loop_ub = pd_.starting_joint_vel.size(0);
    for (i = 0; i < loop_ub; i++) {
      optimal_joint_velocity[i] = pd_.starting_joint_vel[i];
    }
    for (i = 0; i < 6; i++) {
      optimal_joint_velocity[i + pd_.starting_joint_vel.size(0)] = 0.0;
    }
  } else {
    // 'inverseKinematicsOAModified:180' else
    // 'inverseKinematicsOAModified:181' q_vel_initial_guess =
    // starting_joint_vel;
    optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0));
    loop_ub = pd_.starting_joint_vel.size(0);
    for (i = 0; i < loop_ub; i++) {
      optimal_joint_velocity[i] = pd_.starting_joint_vel[i];
    }
  }
  // ----------------------------------------------------------------------
  //  5. INITIAL GUESS
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:178' if slackIsUsed
  // ----------------------------------------------------------------------
  //  6. OPTIMIZATION OPTIONS
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:187' options = optimoptions('fmincon', ...
  // 'inverseKinematicsOAModified:188'         'Algorithm',           'sqp', ...
  // 'inverseKinematicsOAModified:189'         'Display',            'off', ...
  // % or 'iter' to see solver progress 'inverseKinematicsOAModified:190'
  // 'OptimalityTolerance', 1e-6, ... 'inverseKinematicsOAModified:191'
  // 'ConstraintTolerance', 1e-6, ... 'inverseKinematicsOAModified:192'
  // 'StepTolerance',       1e-6, ... 'inverseKinematicsOAModified:193'
  // 'MaxIterations',       1000);
  //   % or 'iter' to see solver progress
  // ----------------------------------------------------------------------
  //  7. CALL THE OPTIMIZER (fmincon)
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:198' [q_vel_opt, ~, Exit_Flag] =
  // fmincon(@(q_vel) objFun( ... 'inverseKinematicsOAModified:199' q_vel, ...
  // 'inverseKinematicsOAModified:200'             jacobi, ...
  // 'inverseKinematicsOAModified:201'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:202'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:203'             joint_values, ...
  // 'inverseKinematicsOAModified:204'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:205'             config, ...
  // 'inverseKinematicsOAModified:206'             slackIsUsed), ...
  // 'inverseKinematicsOAModified:207'         q_vel_initial_guess, ...
  // 'inverseKinematicsOAModified:208'         A, b, Aeq, beq, lb, ub, [],
  // options);
  b_this.workspace.slackIsUsed = slackIsUsed;
  b_this.workspace.jacobi.set_size(6, jacobi.size(1));
  loop_ub = 6 * jacobi.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.jacobi[i] = jacobi[i];
  }
  for (int b_i{0}; b_i < 6; b_i++) {
    b_this.workspace.xd_eff_vel[b_i] = xd_eff_vel[b_i];
  }
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
                 lb, ub, Exit_Flag);
  // ----------------------------------------------------------------------
  //  8. EXTRACT THE RESULT
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:213' if slackIsUsed
  if (slackIsUsed) {
    // 'inverseKinematicsOAModified:214' optimal_joint_velocity =
    // q_vel_opt(1:n_joints);
    optimal_joint_velocity.set_size(joint_values.size(0));
  } else {
    // 'inverseKinematicsOAModified:216' else
    // 'inverseKinematicsOAModified:217' optimal_joint_velocity = q_vel_opt;
  }
  // ----------------------------------------------------------------------
  //  9. UPDATE PERSISTENT STATE
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:223' starting_joint_vel =
  // optimal_joint_velocity;
  pd_.starting_joint_vel.set_size(optimal_joint_velocity.size(0));
  loop_ub = optimal_joint_velocity.size(0);
  for (i = 0; i < loop_ub; i++) {
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
  // 'inverseKinematicsOAModified:198' @(q_vel) objFun( ...
  // 'inverseKinematicsOAModified:199'             q_vel, ...
  // 'inverseKinematicsOAModified:200'             jacobi, ...
  // 'inverseKinematicsOAModified:201'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:202'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:203'             joint_values, ...
  // 'inverseKinematicsOAModified:204'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:205'             config, ...
  // 'inverseKinematicsOAModified:206'             slackIsUsed)
  // 'objFun:2' n_joints = length(joint_values);
  //  Add the cost for the slack variables
  // 'objFun:4' if slackIsUsed
  if (slackIsUsed) {
    double b_config;
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
    for (int j{0}; j < 6; j++) {
      b[j + 6 * j] = config.SlackPenaltyWeight[j];
    }
    b_q_vel.set_size(joint_values.size(0));
    for (int j{0}; j < loop_ub; j++) {
      b_q_vel[j] = q_vel[j];
    }
    b_config = 0.0;
    for (int j{0}; j < 6; j++) {
      double d;
      d = 0.0;
      for (loop_ub = 0; loop_ub < 6; loop_ub++) {
        d += config.SlackObjectiveWeight * q_vel[i + loop_ub] *
             b[loop_ub + 6 * j];
      }
      b_config += d * q_vel[i1 + j];
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
    int loop_ub;
    // 'objFun:7' else
    // 'objFun:8' objVal = configurableObjective(q_vel(1:n_joints), jacobi,
    // xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix,
    // config);
    loop_ub = joint_values.size(0);
    b_q_vel.set_size(joint_values.size(0));
    for (int i{0}; i < loop_ub; i++) {
      b_q_vel[i] = q_vel[i];
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
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2[i * stride_0_0] - in3[i * stride_1_0];
  }
}

//
// File trailer for OptimizationBasedIKWithOAModified.cpp
//
// [EOF]
//
