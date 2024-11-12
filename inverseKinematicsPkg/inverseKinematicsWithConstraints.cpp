//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsWithConstraints.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "inverseKinematicsWithConstraints.h"
#include "Hessian_final_link.h"
#include "Jacobi_final_link.h"
#include "anonymous_function.h"
#include "configurableObjective.h"
#include "fmincon.h"
#include "inverseKinematics_internal_types1.h"
#include "inverseKinematics_types.h"
#include "rt_nonfinite.h"
#include "unsafeSxfun.h"
#include "xgetrf.h"
#include "xnrm2.h"
#include "xzsvdc.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Declarations
static void
inverseKinematics_init(inverseKinematicsWithConstraints *aInstancePtr);

// Function Definitions
//
// Arguments    : void
// Return Type  : inverseKinematicsStackData *
//
inverseKinematicsStackData *inverseKinematicsWithConstraints::getStackData()
{
  return &SD_;
}

//
// function [optimal_joint_velocity,Exit_Flag]  =
// inverseKinematics(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,config)
//
// Arguments    : inverseKinematicsWithConstraints *aInstancePtr
// Return Type  : void
//
static void
inverseKinematics_init(inverseKinematicsWithConstraints *aInstancePtr)
{
  inverseKinematicsStackData *localSD;
  localSD = aInstancePtr->getStackData();
  localSD->pd->is_first_step_not_empty = false;
  localSD->pd->starting_joint_vel_not_empty = false;
}

//
// function [optimal_joint_velocity,Exit_Flag]  =
// inverseKinematics(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,config)
//
// Arguments    : const coder::array<double, 1U> &joint_values
//                const coder::array<double, 2U> &jacobi
//                const double xd_eff_vel_data[]
//                const int xd_eff_vel_size[1]
//                const coder::array<double, 1U> &jointminvalues
//                const coder::array<double, 1U> &jointmaxvalues
//                const coder::array<double, 1U> &joint_min_vel
//                const coder::array<double, 1U> &joint_max_vel
//                const struct0_T *config
//                coder::array<double, 1U> &optimal_joint_velocity
//                double *Exit_Flag
// Return Type  : void
//
void inverseKinematicsWithConstraints::inverseKinematics(
    const coder::array<double, 1U> &joint_values,
    const coder::array<double, 2U> &jacobi, const double xd_eff_vel_data[],
    const int xd_eff_vel_size[1], const coder::array<double, 1U> &,
    const coder::array<double, 1U> &,
    const coder::array<double, 1U> &joint_min_vel,
    const coder::array<double, 1U> &joint_max_vel, const struct0_T *config,
    coder::array<double, 1U> &optimal_joint_velocity, double *Exit_Flag)
{
  static const signed char iv[36]{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
                                  0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                                  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1};
  coder::anonymous_function b_this;
  coder::array<double, 2U> Aeq;
  int coffset;
  int loop_ub;
  // 'inverseKinematics:5' n_joints = length(joint_values);
  //  Assuming 'joint_values' is a column vector
  // total_variables = n_joints + n_slack;  % Total number of variables in the
  // optimization problem
  //  Ensure sizes are compatible for code generation
  //  coder.varsize('starting_joint_vel', [n_joints, 1], [true, false]);
  //  coder.varsize('optimal_joint_velocity', [n_joints, 1], [true, false]);
  //  Initialize 'starting_joint_vel' if it's the first execution or if it's
  //  undefined (starting joint velocity)
  // 'inverseKinematics:16' if isempty(is_first_step) ||
  // isempty(starting_joint_vel)
  if ((!pd_.is_first_step_not_empty) || (!pd_.starting_joint_vel_not_empty)) {
    // 'inverseKinematics:17' is_first_step = true;
    pd_.is_first_step = true;
    pd_.is_first_step_not_empty = true;
    // 'inverseKinematics:18' starting_joint_vel = zeros(n_joints,1);
    pd_.starting_joint_vel.set_size(joint_values.size(0));
    loop_ub = joint_values.size(0);
    for (coffset = 0; coffset < loop_ub; coffset++) {
      pd_.starting_joint_vel[coffset] = 0.0;
    }
    //  Default initialization
  }
  //  Then, check if it's the first execution step to perform any first-time
  //  setup
  // 'inverseKinematics:22' if is_first_step
  if (pd_.is_first_step) {
    // 'inverseKinematics:23' is_first_step = false;
    pd_.is_first_step = false;
    //  Ensure this block won't execute again
    //  No need to set 'starting_joint_vel' here again if it's already set above
  }
  // 'inverseKinematics:27' Weightmatrix = diag([1 1 1 1 1 1]);
  //  Default initializations
  // 'inverseKinematics:30' Aeq = Weightmatrix * jacobi;
  loop_ub = jacobi.size(1);
  Aeq.set_size(6, jacobi.size(1));
  for (int j{0}; j < loop_ub; j++) {
    int boffset;
    coffset = j * 6;
    boffset = j * jacobi.size(0);
    for (int i{0}; i < 6; i++) {
      double s;
      s = 0.0;
      for (int k{0}; k < 6; k++) {
        s += static_cast<double>(iv[k * 6 + i]) * jacobi[boffset + k];
      }
      Aeq[coffset + i] = s;
    }
  }
  // 'inverseKinematics:31' beq = xd_eff_vel;
  // 'inverseKinematics:32' A = [];
  // 'inverseKinematics:33' b = [];
  // 'inverseKinematics:34' lb = joint_min_vel;
  // 'inverseKinematics:35' ub = joint_max_vel;
  //  Initial guess should include the slack variables
  // 'inverseKinematics:39' q_vel_initial_guess = starting_joint_vel;
  //  Initial guess for the decision variable (joint velocities and slack)
  // 'Display','off', ...
  // 'Display', 'iter', ... % Displays each iteration progress
  //  Set up optimization options
  // 'inverseKinematics:47' options = optimoptions('fmincon', 'Algorithm',
  // 'sqp', ... 'inverseKinematics:48'                          'Display',
  // 'iter',                   ... 'inverseKinematics:49' 'OptimalityTolerance',
  // 1e-6, ... 'inverseKinematics:50' 'ConstraintTolerance', 1e-6, ...
  // 'inverseKinematics:51'                        'StepTolerance', 1e-6, ...
  // 'inverseKinematics:52'                        'MaxIterations', 1000);
  //  Run the optimization
  // 'inverseKinematics:55' [q_vel_opt, ~, Exit_Flag] =
  // fmincon(@(q_vel)objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel,
  // joint_values, config), q_vel_initial_guess, A, b, Aeq, beq, lb, ub, [],
  // options);
  b_this.workspace.jacobi.set_size(jacobi.size(0), jacobi.size(1));
  loop_ub = jacobi.size(0) * jacobi.size(1);
  for (coffset = 0; coffset < loop_ub; coffset++) {
    b_this.workspace.jacobi[coffset] = jacobi[coffset];
  }
  b_this.workspace.xd_eff_vel.size[0] = xd_eff_vel_size[0];
  loop_ub = xd_eff_vel_size[0];
  if (loop_ub - 1 >= 0) {
    std::copy(&xd_eff_vel_data[0], &xd_eff_vel_data[loop_ub],
              &b_this.workspace.xd_eff_vel.data[0]);
  }
  b_this.workspace.starting_joint_vel.set_size(pd_.starting_joint_vel.size(0));
  loop_ub = pd_.starting_joint_vel.size(0);
  for (coffset = 0; coffset < loop_ub; coffset++) {
    b_this.workspace.starting_joint_vel[coffset] =
        pd_.starting_joint_vel[coffset];
  }
  b_this.workspace.joint_values.set_size(joint_values.size(0));
  loop_ub = joint_values.size(0);
  for (coffset = 0; coffset < loop_ub; coffset++) {
    b_this.workspace.joint_values[coffset] = joint_values[coffset];
  }
  b_this.workspace.config = *config;
  optimal_joint_velocity.set_size(pd_.starting_joint_vel.size(0));
  loop_ub = pd_.starting_joint_vel.size(0);
  for (coffset = 0; coffset < loop_ub; coffset++) {
    optimal_joint_velocity[coffset] = pd_.starting_joint_vel[coffset];
  }
  coder::fmincon(b_this, optimal_joint_velocity, Aeq, xd_eff_vel_data,
                 xd_eff_vel_size[0], joint_min_vel, joint_max_vel, Exit_Flag);
  //  Separate the optimal joint velocities and slack variables if slack was
  //  applied
  // 'inverseKinematics:58' optimal_joint_velocity = q_vel_opt;
  // 'inverseKinematics:60' starting_joint_vel = optimal_joint_velocity;
  pd_.starting_joint_vel.set_size(optimal_joint_velocity.size(0));
  loop_ub = optimal_joint_velocity.size(0);
  for (coffset = 0; coffset < loop_ub; coffset++) {
    pd_.starting_joint_vel[coffset] = optimal_joint_velocity[coffset];
  }
  pd_.starting_joint_vel_not_empty = (pd_.starting_joint_vel.size(0) != 0);
}

//
// Arguments    : void
// Return Type  : void
//
inverseKinematicsWithConstraints::inverseKinematicsWithConstraints()
{
  SD_.pd = &pd_;
  inverseKinematics_init(this);
}

//
// Arguments    : void
// Return Type  : void
//
inverseKinematicsWithConstraints::~inverseKinematicsWithConstraints() = default;

//
// @(q_vel)
//
// Arguments    : const coder::array<double, 2U> &jacobi
//                const double xd_eff_vel_data[]
//                int xd_eff_vel_size
//                const coder::array<double, 1U> &starting_joint_vel
//                const coder::array<double, 1U> &joint_values
//                boolean_T config_useObjective1
//                double config_weight1
//                boolean_T config_useObjective2
//                double config_weight2
//                boolean_T config_useObjective3
//                double config_weight3
//                boolean_T config_useObjective4
//                double config_weight4
//                boolean_T config_useObjective5
//                double config_weight5
//                boolean_T config_useObjective6
//                double config_weight6
//                const coder::array<double, 1U> &q_vel
// Return Type  : double
//
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
    const coder::array<double, 1U> &q_vel)
{
  coder::array<double, 2U> b_jacobi;
  coder::array<double, 2U> b_joint_values;
  coder::array<double, 1U> b_q_vel;
  coder::array<double, 1U> x;
  double JJt[36];
  double inv_JJt[36];
  double s_data[6];
  double absx;
  double bsum;
  double varargout_1;
  int aoffset;
  int firstBlockLength;
  int hi;
  int i;
  int k;
  int lastBlockLength;
  int s_size;
  boolean_T isodd;
  // 'inverseKinematics:55' @(q_vel)objFun(q_vel, jacobi, xd_eff_vel,
  // starting_joint_vel, joint_values, config) 'objFun:2' n_joints =
  // length(joint_values); 'objFun:3' objVal =
  // configurableObjective(q_vel(1:n_joints), jacobi, xd_eff_vel,
  // starting_joint_vel, joint_values, config);
  lastBlockLength = joint_values.size(0);
  // 'configurableObjective:2' objVal = 0;
  varargout_1 = 0.0;
  // 'configurableObjective:4' if config.useObjective1
  if (config_useObjective1) {
    // 'configurableObjective:5' objVal = objVal + config.weight1 * (norm(q_vel,
    // Inf)^2) / 2;
    if (joint_values.size(0) == 0) {
      bsum = 0.0;
    } else {
      bsum = 0.0;
      for (k = 0; k < lastBlockLength; k++) {
        absx = std::abs(q_vel[k]);
        if (std::isnan(absx) || (absx > bsum)) {
          bsum = absx;
        }
      }
    }
    varargout_1 = config_weight1 * (bsum * bsum) / 2.0;
  }
  // 'configurableObjective:8' if config.useObjective2
  if (config_useObjective2) {
    // 'configurableObjective:9' objVal = objVal + config.weight2 * (norm(q_vel,
    // 2)^2) / 2;
    if (joint_values.size(0) == 0) {
      bsum = 0.0;
    } else {
      b_q_vel.set_size(joint_values.size(0));
      for (i = 0; i < lastBlockLength; i++) {
        b_q_vel[i] = q_vel[i];
      }
      bsum = coder::internal::blas::xnrm2(joint_values.size(0), b_q_vel);
    }
    varargout_1 += config_weight2 * (bsum * bsum) / 2.0;
  }
  // 'configurableObjective:12' if config.useObjective3
  if (config_useObjective3) {
    // 'configurableObjective:13' objVal = objVal + config.weight3 *
    // norm((jacobi * q_vel - xd_eff_vel), 2)^2 / 2;
    firstBlockLength = jacobi.size(0) - 1;
    hi = jacobi.size(1);
    s_size = jacobi.size(0);
    if (firstBlockLength >= 0) {
      std::memset(&s_data[0], 0,
                  static_cast<unsigned int>(firstBlockLength + 1) *
                      sizeof(double));
    }
    for (k = 0; k < hi; k++) {
      aoffset = k * jacobi.size(0);
      for (int b_i{0}; b_i <= firstBlockLength; b_i++) {
        s_data[b_i] += jacobi[aoffset + b_i] * q_vel[k];
      }
    }
    if (s_size == xd_eff_vel_size) {
      x.set_size(s_size);
      for (i = 0; i < s_size; i++) {
        x[i] = s_data[i] - xd_eff_vel_data[i];
      }
    } else {
      binary_expand_op_1(x, s_data, s_size, xd_eff_vel_data, xd_eff_vel_size);
    }
    if (x.size(0) == 0) {
      bsum = 0.0;
    } else {
      bsum = coder::internal::blas::xnrm2(x.size(0), x);
    }
    varargout_1 += config_weight3 * (bsum * bsum) / 2.0;
  }
  // 'configurableObjective:16' if config.useObjective4
  if (config_useObjective4) {
    // 'configurableObjective:17' objVal = objVal + config.weight4 * sum((q_vel
    // - starting_joint_vel).^2);
    if (joint_values.size(0) == starting_joint_vel.size(0)) {
      x.set_size(joint_values.size(0));
      for (i = 0; i < lastBlockLength; i++) {
        absx = q_vel[i] - starting_joint_vel[i];
        x[i] = absx * absx;
      }
    } else {
      binary_expand_op(x, q_vel, joint_values.size(0) - 1, starting_joint_vel);
    }
    if (x.size(0) == 0) {
      absx = 0.0;
    } else {
      if (x.size(0) <= 1024) {
        firstBlockLength = x.size(0);
        lastBlockLength = 0;
        aoffset = 1;
      } else {
        firstBlockLength = 1024;
        aoffset = static_cast<int>(static_cast<unsigned int>(x.size(0)) >> 10);
        lastBlockLength = x.size(0) - (aoffset << 10);
        if (lastBlockLength > 0) {
          aoffset++;
        } else {
          lastBlockLength = 1024;
        }
      }
      absx = x[0];
      for (k = 2; k <= firstBlockLength; k++) {
        absx += x[k - 1];
      }
      for (s_size = 2; s_size <= aoffset; s_size++) {
        firstBlockLength = (s_size - 1) << 10;
        bsum = x[firstBlockLength];
        if (s_size == aoffset) {
          hi = lastBlockLength;
        } else {
          hi = 1024;
        }
        for (k = 2; k <= hi; k++) {
          bsum += x[(firstBlockLength + k) - 1];
        }
        absx += bsum;
      }
    }
    varargout_1 += config_weight4 * absx;
  }
  // 'configurableObjective:20' if config.useObjective5
  if (config_useObjective5) {
    // 'configurableObjective:21' objVal  = objVal + config.weight5 * 1/(1 +
    // smin(jacobi)); 'smin:2' s = svd(J);
    firstBlockLength = jacobi.size(0) * jacobi.size(1);
    isodd = true;
    for (k = 0; k < firstBlockLength; k++) {
      if ((!isodd) || (std::isinf(jacobi[k]) || std::isnan(jacobi[k]))) {
        isodd = false;
      }
    }
    if (isodd) {
      if ((jacobi.size(0) == 0) || (jacobi.size(1) == 0)) {
        s_size = 0;
      } else {
        b_jacobi.set_size(jacobi.size(0), jacobi.size(1));
        lastBlockLength = jacobi.size(0) * jacobi.size(1) - 1;
        for (i = 0; i <= lastBlockLength; i++) {
          b_jacobi[i] = jacobi[i];
        }
        s_size = coder::internal::reflapack::xzsvdc(b_jacobi, s_data);
      }
    } else {
      lastBlockLength =
          static_cast<int>(std::fmin(static_cast<double>(jacobi.size(0)),
                                     static_cast<double>(jacobi.size(1))));
      s_size = static_cast<int>(std::fmin(static_cast<double>(jacobi.size(0)),
                                          static_cast<double>(jacobi.size(1))));
      for (i = 0; i < lastBlockLength; i++) {
        s_data[i] = rtNaN;
      }
    }
    // 'smin:3' s_min = min(s);
    if (s_size <= 2) {
      if (s_size == 1) {
        absx = s_data[0];
      } else {
        absx = s_data[s_size - 1];
        if ((!(s_data[0] > absx)) &&
            ((!std::isnan(s_data[0])) || std::isnan(absx))) {
          absx = s_data[0];
        }
      }
    } else {
      if (!std::isnan(s_data[0])) {
        firstBlockLength = 1;
      } else {
        boolean_T exitg1;
        firstBlockLength = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= s_size)) {
          if (!std::isnan(s_data[k - 1])) {
            firstBlockLength = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }
      if (firstBlockLength == 0) {
        absx = s_data[0];
      } else {
        absx = s_data[firstBlockLength - 1];
        i = firstBlockLength + 1;
        for (k = i; k <= s_size; k++) {
          bsum = s_data[k - 1];
          if (absx > bsum) {
            absx = bsum;
          }
        }
      }
    }
    varargout_1 += config_weight5 / (absx + 1.0);
  }
  // 'configurableObjective:24' if config.useObjective6
  if (config_useObjective6) {
    double H[294];
    double J[42];
    double Jm_transposed;
    double m;
    int ipiv[6];
    signed char p[6];
    // 'configurableObjective:25' objVal  = objVal + config.weight6 *
    // (-computeManipulabilityJacobian(joint_values') * q_vel);
    //  Compute the Jacobian and Hessian first
    // [~, J, H] = computeKinematics(joint_values, mdhparams); because of syms
    // was too slow J = Jacobi_final_link1(joint_values); %for code generation
    // H = Hessian_final_link(joint_values); %for code generation
    // 'computeManipulabilityJacobian:7' J = Jacobi_final_link(joint_values);
    b_joint_values.set_size(1, joint_values.size(0));
    lastBlockLength = joint_values.size(0);
    for (i = 0; i < lastBlockLength; i++) {
      b_joint_values[i] = joint_values[i];
    }
    Jacobi_final_link(b_joint_values, J);
    // 'computeManipulabilityJacobian:8' H = Hessian_final_link(joint_values);
    b_joint_values.set_size(1, joint_values.size(0));
    lastBlockLength = joint_values.size(0);
    for (i = 0; i < lastBlockLength; i++) {
      b_joint_values[i] = joint_values[i];
    }
    Hessian_final_link(b_joint_values, H);
    //  Compute the manipulability measure 'm'
    // 'computeManipulabilityJacobian:10' JJt = J * J';
    for (i = 0; i < 6; i++) {
      for (hi = 0; hi < 6; hi++) {
        bsum = 0.0;
        for (firstBlockLength = 0; firstBlockLength < 7; firstBlockLength++) {
          bsum += J[i + 6 * firstBlockLength] * J[hi + 6 * firstBlockLength];
        }
        JJt[i + 6 * hi] = bsum;
      }
    }
    // 'computeManipulabilityJacobian:11' m = sqrt(det(JJt));
    std::copy(&JJt[0], &JJt[36], &inv_JJt[0]);
    coder::internal::lapack::xgetrf(inv_JJt, ipiv);
    absx = inv_JJt[0];
    isodd = false;
    for (k = 0; k < 5; k++) {
      absx *= inv_JJt[(k + 6 * (k + 1)) + 1];
      if (ipiv[k] > k + 1) {
        isodd = !isodd;
      }
    }
    if (isodd) {
      absx = -absx;
    }
    m = std::sqrt(absx);
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
    for (i = 0; i < 6; i++) {
      p[i] = static_cast<signed char>(i + 1);
    }
    for (k = 0; k < 5; k++) {
      i = ipiv[k];
      if (i > k + 1) {
        firstBlockLength = p[i - 1];
        p[i - 1] = p[k];
        p[k] = static_cast<signed char>(firstBlockLength);
      }
    }
    for (k = 0; k < 6; k++) {
      aoffset = 6 * (p[k] - 1);
      inv_JJt[k + aoffset] = 1.0;
      for (lastBlockLength = k + 1; lastBlockLength < 7; lastBlockLength++) {
        i = (lastBlockLength + aoffset) - 1;
        if (inv_JJt[i] != 0.0) {
          hi = lastBlockLength + 1;
          for (int b_i{hi}; b_i < 7; b_i++) {
            firstBlockLength = (b_i + aoffset) - 1;
            inv_JJt[firstBlockLength] -=
                inv_JJt[i] * JJt[(b_i + 6 * (lastBlockLength - 1)) - 1];
          }
        }
      }
    }
    for (lastBlockLength = 0; lastBlockLength < 6; lastBlockLength++) {
      firstBlockLength = 6 * lastBlockLength;
      for (k = 5; k >= 0; k--) {
        hi = 6 * k;
        i = k + firstBlockLength;
        bsum = inv_JJt[i];
        if (bsum != 0.0) {
          inv_JJt[i] = bsum / JJt[k + hi];
          for (int b_i{0}; b_i < k; b_i++) {
            aoffset = b_i + firstBlockLength;
            inv_JJt[aoffset] -= inv_JJt[i] * JJt[b_i + hi];
          }
        }
      }
    }
    //
    //  Pre-compute the pseudo-inverse of JJt
    // pseudo_inv_JJt = pinv(JJt);
    // 'computeManipulabilityJacobian:26' for i = 1:n
    Jm_transposed = 0.0;
    for (int b_i{0}; b_i < 7; b_i++) {
      // 'computeManipulabilityJacobian:27' Hi = H(:,:,i);
      // 'computeManipulabilityJacobian:28' term1 = vec(J * Hi')' *
      // vec(inv_JJt); 'computeManipulabilityJacobian:18' @(X) reshape(X, [], 1)
      // 'computeManipulabilityJacobian:29' Jm_transposed(i) = m * term1;
      for (i = 0; i < 6; i++) {
        for (hi = 0; hi < 6; hi++) {
          bsum = 0.0;
          for (firstBlockLength = 0; firstBlockLength < 7; firstBlockLength++) {
            bsum += J[i + 6 * firstBlockLength] *
                    H[(hi + 6 * firstBlockLength) + 42 * b_i];
          }
          JJt[i + 6 * hi] = bsum;
        }
      }
      absx = 0.0;
      for (i = 0; i < 36; i++) {
        absx += JJt[i] * inv_JJt[i];
      }
      //  Equation (11)
      Jm_transposed += -(m * absx) * q_vel[b_i];
    }
    varargout_1 += config_weight6 * Jm_transposed;
  }
  //  You can add more terms similarly with their checks and weights
  return varargout_1;
}

//
// File trailer for inverseKinematicsWithConstraints.cpp
//
// [EOF]
//
