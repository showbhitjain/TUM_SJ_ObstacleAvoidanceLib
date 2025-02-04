//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: OptimizationBasedIKWithOAModified.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "OptimizationBasedIKWithOAModified.h"
#include "Hessian_final_link.h"
#include "Jacobi_final_link.h"
#include "anonymous_function.h"
#include "configurableObjective.h"
#include "eml_int_forloop_overflow_check.h"
#include "fmincon.h"
#include "indexShapeCheck.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "ixfun.h"
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "xgetrf.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Type Definitions
struct rtEqualityCheckInfo {
  int nDims;
  int lineNo;
  int colNo;
  const char *fName;
  const char *pName;
};

// Function Declarations
static void b_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

static void binary_expand_op(coder::array<double, 1U> &in1,
                             const struct0_T *in2,
                             const coder::array<double, 1U> &in3,
                             const coder::array<double, 1U> &in4);

static void emlrtDimSizeImpxCheckR2021b(const int aDim1, const int aDim2,
                                        const rtEqualityCheckInfo &aInfo);

static void g_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void h_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void inverseKinematicsOAModified_init(
    OptimizationBasedIKWithOAModified *aInstancePtr);

static void minus(coder::array<double, 1U> &in1,
                  const coder::array<double, 1U> &in2,
                  const coder::array<double, 1U> &in3);

static void rtErrorWithMessageID(const char *aFcnName, int aLineNum);

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
// Arguments    : const char *r
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void b_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "Domain error. To compute complex results from real x, use \'")
   << r)
      << "(complex(x))\'.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
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
// Arguments    : const int aDim1
//                const int aDim2
//                const rtEqualityCheckInfo &aInfo
// Return Type  : void
//
static void emlrtDimSizeImpxCheckR2021b(const int aDim1, const int aDim2,
                                        const rtEqualityCheckInfo &aInfo)
{
  std::stringstream outStream;
  ((((((outStream << "Size mismatch error on dimension ") << aInfo.nDims)
      << ": expected ")
     << aDim1)
    << " or a singleton, but actual size is ")
   << aDim2)
      << ".";
  outStream << "\n";
  ((((outStream << "Error in ") << aInfo.fName) << " (line ") << aInfo.lineNo)
      << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void g_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream
      << "Incorrect dimensions for matrix multiplication. Check that the "
         "number of columns in the first matrix matches the number of rows "
         "in the second matrix. To operate on each element of the matrix "
         "individually, use TIMES (.*) for elementwise multiplication.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void h_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream
      << "Inner dimensions must agree. Generated code for a general matrix "
         "multiplication at this call site. If this should have been a sc"
         "alar times a variable-size matrix, the scalar input must be "
         "fixed-size.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// function [optimal_joint_velocity, Exit_Flag] = inverseKinematicsOAModified(
// ...
//     joint_values,       ... % [n_joints x 1]
//     jacobi,             ... % [6 x n_joints]
//     xd_eff_vel,         ... % [6 x 1]
//     jointminvalues,     ...
//     jointmaxvalues,     ...
//     joint_min_vel,      ...
//     joint_max_vel,      ...
//     J_g,                ... % Inequality constraint matrix
//     b_g,                ... % Inequality constraint vector
//     jointVelocityWeightMatrix, ...
//     configInput)
//
// % [n_joints x 1]
//  % [6 x n_joints]
//  % [6 x 1]
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
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Dimensions of arrays being concatenated are not consistent.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
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
//     jointminvalues,     ...
//     jointmaxvalues,     ...
//     joint_min_vel,      ...
//     joint_max_vel,      ...
//     J_g,                ... % Inequality constraint matrix
//     b_g,                ... % Inequality constraint vector
//     jointVelocityWeightMatrix, ...
//     configInput)
//
// % [n_joints x 1]
//  % [6 x n_joints]
//  % [6 x 1]
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
  static rtBoundsCheckInfo ab_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      68,                            // lineNo
      51,                            // colNo
      "min_dist",                    // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo bb_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      68,                            // lineNo
      20,                            // colNo
      "bp",                          // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo cb_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      204,                           // lineNo
      44,                            // colNo
      "q_vel_opt",                   // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo db_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      204,                           // lineNo
      46,                            // colNo
      "q_vel_opt",                   // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      66,                            // lineNo
      25,                            // colNo
      "min_dist",                    // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      67,                            // lineNo
      20,                            // colNo
      "In",                          // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                            // iFirst
      -1,                            // iLast
      67,                            // lineNo
      22,                            // colNo
      "In",                          // aName
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m", // pName
      0                                                             // checkKind
  };
  static rtEqualityCheckInfo b_emlrtECI{
      1,                             // nDims
      64,                            // lineNo
      28,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtEqualityCheckInfo c_emlrtECI{
      1,                             // nDims
      130,                           // lineNo
      39,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtEqualityCheckInfo d_emlrtECI{
      1,                             // nDims
      131,                           // lineNo
      39,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtEqualityCheckInfo e_emlrtECI{
      1,                             // nDims
      141,                           // lineNo
      46,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtEqualityCheckInfo emlrtECI{
      1,                             // nDims
      63,                            // lineNo
      28,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtEqualityCheckInfo f_emlrtECI{
      1,                             // nDims
      142,                           // lineNo
      46,                            // colNo
      "inverseKinematicsOAModified", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/inverseKinematicsOAModified.m" // pName
  };
  static rtRunTimeErrorInfo c_emlrtRTEI{
      288,                  // lineNo
      "check_non_axis_size" // fName
  };
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
  int input_sizes[2];
  int i;
  int k;
  int loop_ub;
  int loop_ub_tmp;
  int nx;
  boolean_T slackIsUsed;
  //  % Inequality constraint matrix
  //  % Inequality constraint vector
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
    pd_.starting_joint_vel_not_empty = (pd_.starting_joint_vel.size(0) != 0);
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
  // 'inverseKinematicsOAModified:45' if config.dynamicSlack
  if (configInput->dynamicSlack &&
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
    if ((jointminvalues.size(0) != joint_values.size(0)) &&
        ((jointminvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
      emlrtDimSizeImpxCheckR2021b(jointminvalues.size(0), joint_values.size(0),
                                  emlrtECI);
    }
    if ((jointmaxvalues.size(0) != joint_values.size(0)) &&
        ((jointmaxvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
      emlrtDimSizeImpxCheckR2021b(jointmaxvalues.size(0), joint_values.size(0),
                                  b_emlrtECI);
    }
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
    if (min_dist.size(0) > 2147483646) {
      coder::check_forloop_overflow_error();
    }
    for (k = 0; k < nx; k++) {
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
    if (min_dist.size(0) > 2147483646) {
      coder::check_forloop_overflow_error();
    }
    for (k = 0; k < nx; k++) {
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
    for (nx = 0; nx < i; nx++) {
      // 'inverseKinematicsOAModified:66' if min_dist(i) < infuence_dist
      if (nx + 1 > min_dist.size(0)) {
        rtDynamicBoundsError(nx + 1, 1, min_dist.size(0), w_emlrtBCI);
      }
      b_varargin_1 = min_dist[nx];
      if (b_varargin_1 < infuence_dist) {
        // 'inverseKinematicsOAModified:67' In(i,i)  = 1;
        if (nx + 1 > In.size(0)) {
          rtDynamicBoundsError(nx + 1, 1, In.size(0), x_emlrtBCI);
        }
        if (nx + 1 > In.size(1)) {
          rtDynamicBoundsError(nx + 1, 1, In.size(1), y_emlrtBCI);
        }
        In[nx + In.size(0) * nx] = 1;
        // 'inverseKinematicsOAModified:68' bp(i)    = joint_gain * (min_dist(i)
        // - stop_dist)/(infuence_dist - stop_dist);
        if (nx + 1 > min_dist.size(0)) {
          rtDynamicBoundsError(nx + 1, 1, min_dist.size(0), ab_emlrtBCI);
        }
        if (nx + 1 > bp.size(0)) {
          rtDynamicBoundsError(nx + 1, 1, bp.size(0), bb_emlrtBCI);
        }
        bp[nx] = configInput->jointLimitGain * (b_varargin_1 - stop_dist) /
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
  input_sizes[0] = 0;
  input_sizes[1] = 0;
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
    coder::internal::blas::mtimes(jacobi, Aeq);
    // 'inverseKinematicsOAModified:90' beq = xd_eff_vel;
    input_sizes[0] = 6;
    input_sizes[1] = 1;
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
        if ((In.size(1) != J_g.size(1)) &&
            ((In.size(0) != 0) && (In.size(1) != 0))) {
          rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
        }
        nx = J_g.size(0);
        if ((In.size(0) != 0) && (In.size(1) != 0)) {
          loop_ub_tmp = In.size(0);
        } else {
          loop_ub_tmp = 0;
        }
        A.set_size(J_g.size(0) + loop_ub_tmp, J_g.size(1));
        loop_ub = J_g.size(1);
        for (i = 0; i < loop_ub; i++) {
          for (int i1{0}; i1 < nx; i1++) {
            A[i1 + A.size(0) * i] = J_g[i1 + nx * i];
          }
          for (int i1{0}; i1 < loop_ub_tmp; i1++) {
            A[(i1 + nx) + A.size(0) * i] = In[i1 + loop_ub_tmp * i];
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
    // 'inverseKinematicsOAModified:115' if config.applyVelocityDamper &&
    // (isempty(J_g) || isempty(b_g))
  } else if (configInput->applyVelocityDamper &&
             ((J_g.size(0) == 0) || (J_g.size(1) == 0) || (b_g.size(0) == 0))) {
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
  }
  //  ---- 4.3 Determine final use of Slack and build lb, ub --------------
  // 'inverseKinematicsOAModified:122' if ~slackIsUsed
  if (!slackIsUsed) {
    //  BOUNDS without slack
    // 'inverseKinematicsOAModified:124' if config.applyVelocityDamper
    if (configInput->applyVelocityDamper) {
      //  We do not scale velocities by gamma if velocity damper is active
      // 'inverseKinematicsOAModified:126' lb = joint_min_vel;
      lb.set_size(joint_min_vel.size(0), 1);
      loop_ub = joint_min_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        lb[i] = joint_min_vel[i];
      }
      // 'inverseKinematicsOAModified:127' ub = joint_max_vel;
      ub.set_size(joint_max_vel.size(0), 1);
      loop_ub = joint_max_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        ub[i] = joint_max_vel[i];
      }
    } else {
      // 'inverseKinematicsOAModified:128' else
      //  Normal bounds: scale the move by config.gamma
      // 'inverseKinematicsOAModified:130' lb = max(config.gamma .*
      // (jointminvalues - joint_values), joint_min_vel);
      if ((jointminvalues.size(0) != joint_values.size(0)) &&
          ((jointminvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
        emlrtDimSizeImpxCheckR2021b(jointminvalues.size(0),
                                    joint_values.size(0), c_emlrtECI);
      }
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
      // 'inverseKinematicsOAModified:131' ub = min(config.gamma .*
      // (jointmaxvalues - joint_values), joint_max_vel);
      if ((jointmaxvalues.size(0) != joint_values.size(0)) &&
          ((jointmaxvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
        emlrtDimSizeImpxCheckR2021b(jointmaxvalues.size(0),
                                    joint_values.size(0), d_emlrtECI);
      }
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
  } else {
    // 'inverseKinematicsOAModified:133' else
    //  BOUNDS with slack
    //     first n_joints are for joint velocities
    //     next   n_slack are for slack
    // 'inverseKinematicsOAModified:137' if config.applyVelocityDamper
    if (configInput->applyVelocityDamper) {
      // 'inverseKinematicsOAModified:138' lb_joints = joint_min_vel;
      min_dist.set_size(joint_min_vel.size(0));
      loop_ub = joint_min_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        min_dist[i] = joint_min_vel[i];
      }
      // 'inverseKinematicsOAModified:139' ub_joints = joint_max_vel;
      bp.set_size(joint_max_vel.size(0));
      loop_ub = joint_max_vel.size(0);
      for (i = 0; i < loop_ub; i++) {
        bp[i] = joint_max_vel[i];
      }
    } else {
      // 'inverseKinematicsOAModified:140' else
      // 'inverseKinematicsOAModified:141' lb_joints = max(config.gamma .*
      // (jointminvalues - joint_values), joint_min_vel);
      if ((jointminvalues.size(0) != joint_values.size(0)) &&
          ((jointminvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
        emlrtDimSizeImpxCheckR2021b(jointminvalues.size(0),
                                    joint_values.size(0), e_emlrtECI);
      }
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
      // 'inverseKinematicsOAModified:142' ub_joints = min(config.gamma .*
      // (jointmaxvalues - joint_values), joint_max_vel);
      if ((jointmaxvalues.size(0) != joint_values.size(0)) &&
          ((jointmaxvalues.size(0) != 1) && (joint_values.size(0) != 1))) {
        emlrtDimSizeImpxCheckR2021b(jointmaxvalues.size(0),
                                    joint_values.size(0), f_emlrtECI);
      }
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
    }
    // 'inverseKinematicsOAModified:145' lb_slack = config.Slacklowerbound;
    // 'inverseKinematicsOAModified:146' ub_slack = config.Slackupperbound;
    // 'inverseKinematicsOAModified:147' lb       = [lb_joints; lb_slack];
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
    // 'inverseKinematicsOAModified:148' ub       = [ub_joints; ub_slack];
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
  // 'inverseKinematicsOAModified:152' if slackIsUsed
  if (slackIsUsed) {
    //  Expand the equality constraints
    // 'inverseKinematicsOAModified:154' if config.applyEqualityConstraints
    if (configInput->applyEqualityConstraints) {
      signed char b_I[36];
      // 'inverseKinematicsOAModified:155' Aeq = [JacobiWeightMatrix * jacobi,
      // eye(n_slack)];
      coder::internal::blas::mtimes(jacobi, Aeq);
      for (i = 0; i < 36; i++) {
        b_I[i] = 0;
      }
      for (k = 0; k < 6; k++) {
        b_I[k + 6 * k] = 1;
      }
      i = Aeq.size(1);
      Aeq.set_size(Aeq.size(0), Aeq.size(1) + 6);
      // 'inverseKinematicsOAModified:156' beq = xd_eff_vel;
      input_sizes[0] = 6;
      input_sizes[1] = 1;
      for (int i1{0}; i1 < 6; i1++) {
        for (nx = 0; nx < 6; nx++) {
          Aeq[nx + Aeq.size(0) * (i1 + i)] = b_I[nx + 6 * i1];
        }
        beq_data[i1] = xd_eff_vel[i1];
      }
    }
    //  Expand the inequality constraints
    // 'inverseKinematicsOAModified:160' if ~isempty(A)
    if ((A.size(0) != 0) && (A.size(1) != 0)) {
      // 'inverseKinematicsOAModified:161' A = [A, zeros(size(A,1), n_slack)];
      nx = A.size(0);
      loop_ub_tmp = A.size(0);
      k = A.size(1);
      loop_ub = A.size(1);
      for (i = 0; i < loop_ub; i++) {
        for (int i1{0}; i1 < nx; i1++) {
          A[i1 + A.size(0) * i] = A[i1 + loop_ub_tmp * i];
        }
      }
      A.set_size(A.size(0), A.size(1) + 6);
      for (i = 0; i < 6; i++) {
        for (int i1{0}; i1 < nx; i1++) {
          A[i1 + A.size(0) * (i + k)] = 0.0;
        }
      }
    }
    // 'inverseKinematicsOAModified:169' q_vel_initial_guess =
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
    // 'inverseKinematicsOAModified:170' else
    // 'inverseKinematicsOAModified:171' q_vel_initial_guess =
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
  // 'inverseKinematicsOAModified:168' if slackIsUsed
  // ----------------------------------------------------------------------
  //  6. OPTIMIZATION OPTIONS
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:177' options = optimoptions('fmincon', ...
  // 'inverseKinematicsOAModified:178'         'Algorithm',           'sqp', ...
  // 'inverseKinematicsOAModified:179'         'Display',            'off', ...
  // % or 'iter' to see solver progress 'inverseKinematicsOAModified:180'
  // 'OptimalityTolerance', 1e-6, ... 'inverseKinematicsOAModified:181'
  // 'ConstraintTolerance', 1e-6, ... 'inverseKinematicsOAModified:182'
  // 'StepTolerance',       1e-6, ... 'inverseKinematicsOAModified:183'
  // 'MaxIterations',       1000);
  //   % or 'iter' to see solver progress
  // ----------------------------------------------------------------------
  //  7. CALL THE OPTIMIZER (fmincon)
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:188' [q_vel_opt, ~, Exit_Flag] =
  // fmincon(@(q_vel) objFun( ... 'inverseKinematicsOAModified:189' q_vel, ...
  // 'inverseKinematicsOAModified:190'             jacobi, ...
  // 'inverseKinematicsOAModified:191'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:192'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:193'             joint_values, ...
  // 'inverseKinematicsOAModified:194'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:195'             config, ...
  // 'inverseKinematicsOAModified:196'             slackIsUsed), ...
  // 'inverseKinematicsOAModified:197'         q_vel_initial_guess, ...
  // 'inverseKinematicsOAModified:198'         A, b, Aeq, beq, lb, ub, [],
  // options);
  b_this.workspace.slackIsUsed = slackIsUsed;
  b_this.workspace.jacobi.set_size(6, jacobi.size(1));
  loop_ub = 6 * jacobi.size(1);
  for (i = 0; i < loop_ub; i++) {
    b_this.workspace.jacobi[i] = jacobi[i];
  }
  for (nx = 0; nx < 6; nx++) {
    b_this.workspace.xd_eff_vel[nx] = xd_eff_vel[nx];
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
  coder::fmincon(b_this, optimal_joint_velocity, A, b, Aeq, beq_data,
                 input_sizes, lb, ub, Exit_Flag);
  // ----------------------------------------------------------------------
  //  8. EXTRACT THE RESULT
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:203' if slackIsUsed
  if (slackIsUsed) {
    // 'inverseKinematicsOAModified:204' optimal_joint_velocity =
    // q_vel_opt(1:n_joints);
    if (joint_values.size(0) < 1) {
      i = 0;
    } else {
      if (optimal_joint_velocity.size(0) < 1) {
        rtDynamicBoundsError(1, 1, optimal_joint_velocity.size(0), cb_emlrtBCI);
      }
      if (joint_values.size(0) > optimal_joint_velocity.size(0)) {
        rtDynamicBoundsError(joint_values.size(0), 1,
                             optimal_joint_velocity.size(0), db_emlrtBCI);
      }
      i = joint_values.size(0);
    }
    input_sizes[0] = 1;
    input_sizes[1] = i;
    coder::internal::indexShapeCheck(optimal_joint_velocity.size(0),
                                     input_sizes);
    optimal_joint_velocity.set_size(i);
  } else {
    // 'inverseKinematicsOAModified:206' else
    // 'inverseKinematicsOAModified:207' optimal_joint_velocity = q_vel_opt;
  }
  // ----------------------------------------------------------------------
  //  9. UPDATE PERSISTENT STATE
  // ----------------------------------------------------------------------
  // 'inverseKinematicsOAModified:213' starting_joint_vel =
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
  static rtBoundsCheckInfo ab_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      3,        // lineNo
      44,       // colNo
      "q_vel",  // aName
      "objFun", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/objFun.m", // pName
      0                                        // checkKind
  };
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      7,        // lineNo
      23,       // colNo
      "q_vel",  // aName
      "objFun", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/objFun.m", // pName
      0                                        // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      7,        // lineNo
      34,       // colNo
      "q_vel",  // aName
      "objFun", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/objFun.m", // pName
      0                                        // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      3,        // lineNo
      42,       // colNo
      "q_vel",  // aName
      "objFun", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/objFun.m", // pName
      0                                        // checkKind
  };
  static rtEqualityCheckInfo b_emlrtECI{
      1,                       // nDims
      17,                      // lineNo
      110,                     // colNo
      "configurableObjective", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/configurableObjective.m" // pName
  };
  static rtEqualityCheckInfo emlrtECI{
      1,                       // nDims
      17,                      // lineNo
      63,                      // colNo
      "configurableObjective", // fName
      "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
      "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
      "AvoidanceOptmisationModified/configurableObjective.m" // pName
  };
  static rtRunTimeErrorInfo c_emlrtRTEI{
      133,                  // lineNo
      "dynamic_size_checks" // fName
  };
  static rtRunTimeErrorInfo d_emlrtRTEI{
      13,    // lineNo
      "sqrt" // fName
  };
  static rtRunTimeErrorInfo e_emlrtRTEI{
      138,                  // lineNo
      "dynamic_size_checks" // fName
  };
  coder::array<double, 2U> y;
  coder::array<double, 1U> b;
  coder::array<double, 1U> b_q_vel;
  double JJt[36];
  double x[36];
  double a;
  double absx;
  double d;
  double t;
  double varargout_1;
  int iv[2];
  int aoffset;
  int i;
  int loop_ub;
  int pipk;
  boolean_T isodd;
  // 'inverseKinematicsOAModified:188' @(q_vel) objFun( ...
  // 'inverseKinematicsOAModified:189'             q_vel, ...
  // 'inverseKinematicsOAModified:190'             jacobi, ...
  // 'inverseKinematicsOAModified:191'             xd_eff_vel, ...
  // 'inverseKinematicsOAModified:192'             starting_joint_vel, ...
  // 'inverseKinematicsOAModified:193'             joint_values, ...
  // 'inverseKinematicsOAModified:194'             jointVelocityWeightMatrix,
  // ... 'inverseKinematicsOAModified:195'             config, ...
  // 'inverseKinematicsOAModified:196'             slackIsUsed)
  // 'objFun:2' n_joints = length(joint_values);
  // 'objFun:3' objVal = configurableObjective(q_vel(1:n_joints), jacobi,
  // xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix,
  // config);
  if (joint_values.size(0) < 1) {
    loop_ub = 0;
  } else {
    if (q_vel.size(0) < 1) {
      rtDynamicBoundsError(1, 1, q_vel.size(0), y_emlrtBCI);
    }
    if (joint_values.size(0) > q_vel.size(0)) {
      rtDynamicBoundsError(joint_values.size(0), 1, q_vel.size(0), ab_emlrtBCI);
    }
    loop_ub = joint_values.size(0);
  }
  iv[0] = 1;
  iv[1] = loop_ub;
  coder::internal::indexShapeCheck(q_vel.size(0), iv);
  // 'configurableObjective:2' objVal = 0;
  varargout_1 = 0.0;
  // 'configurableObjective:4' if config.useObjectiveNormInfinity
  if (config.useObjectiveNormInfinity) {
    // 'configurableObjective:5' objVal = objVal + config.weightNormInfinity *
    // (norm(q_vel, Inf)^2) / 2;
    if (loop_ub == 0) {
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
    if (loop_ub != jointVelocityWeightMatrix.size(0)) {
      if ((loop_ub == 1) || ((jointVelocityWeightMatrix.size(0) == 1) &&
                             (jointVelocityWeightMatrix.size(1) == 1))) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    b_q_vel.set_size(loop_ub);
    for (i = 0; i < loop_ub; i++) {
      b_q_vel[i] = q_vel[i];
    }
    coder::internal::blas::mtimes(b_q_vel, jointVelocityWeightMatrix, y);
    if (y.size(1) != loop_ub) {
      if ((y.size(1) == 1) || (loop_ub == 1)) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    pipk = y.size(1);
    for (i = 0; i < pipk; i++) {
      absx += y[i] * q_vel[i];
    }
    varargout_1 += config.weightNormL2 * absx / 2.0;
  }
  // 'configurableObjective:12' if config.useObjectiveTrajectoryFollowing
  if (config.useObjectiveTrajectoryFollowing) {
    double b_y[6];
    double scale;
    // 'configurableObjective:13' objVal = objVal +
    // config.weightTrajectoryFollowing * norm((jacobi * q_vel - xd_eff_vel),
    // 2)^2 / 2;
    if (jacobi.size(1) != loop_ub) {
      if (loop_ub == 1) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    pipk = jacobi.size(1);
    for (int b_i{0}; b_i < 6; b_i++) {
      b_y[b_i] = 0.0;
    }
    if (jacobi.size(1) > 2147483646) {
      coder::check_forloop_overflow_error();
    }
    for (int k{0}; k < pipk; k++) {
      aoffset = k * 6;
      for (int b_i{0}; b_i < 6; b_i++) {
        b_y[b_i] += jacobi[aoffset + b_i] * q_vel[k];
      }
    }
    a = 0.0;
    scale = 3.3121686421112381E-170;
    for (int k{0}; k < 6; k++) {
      d = b_y[k] - xd_eff_vel[k];
      b_y[k] = d;
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
    isodd = ((loop_ub != starting_joint_vel.size(0)) &&
             ((loop_ub != 1) && (starting_joint_vel.size(0) != 1)));
    if (isodd) {
      emlrtDimSizeImpxCheckR2021b(loop_ub, starting_joint_vel.size(0),
                                  emlrtECI);
    }
    if (loop_ub == starting_joint_vel.size(0)) {
      b.set_size(loop_ub);
      for (i = 0; i < loop_ub; i++) {
        b[i] = q_vel[i] - starting_joint_vel[i];
      }
    } else {
      binary_expand_op_4(b, q_vel, loop_ub, starting_joint_vel);
    }
    if (isodd) {
      emlrtDimSizeImpxCheckR2021b(loop_ub, starting_joint_vel.size(0),
                                  b_emlrtECI);
    }
    if (b.size(0) != jointVelocityWeightMatrix.size(0)) {
      if ((b.size(0) == 1) || ((jointVelocityWeightMatrix.size(0) == 1) &&
                               (jointVelocityWeightMatrix.size(1) == 1))) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    coder::internal::blas::mtimes(b, jointVelocityWeightMatrix, y);
    if (b.size(0) != y.size(1)) {
      if ((y.size(1) == 1) || (b.size(0) == 1)) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    pipk = y.size(1);
    for (i = 0; i < pipk; i++) {
      absx += y[i] * b[i];
    }
    varargout_1 += config.weightJointAcceleration * absx / 2.0;
  }
  // 'configurableObjective:20' if config.useObjectiveManipulability
  if (config.useObjectiveManipulability) {
    double H[294];
    double J[42];
    double inv_JJt[36];
    double Jm_transposed[7];
    int ipiv[6];
    int inv_JJt_tmp;
    signed char p[6];
    // 'configurableObjective:21' objVal  = objVal + config.weightManipulability
    // * (-computeManipulabilityJacobian(joint_values') * q_vel);
    //  Compute the Jacobian and Hessian first
    // [~, J, H] = computeKinematics(joint_values, mdhparams); because of syms
    // was too slow J = Jacobi_final_link1(joint_values); %for code generation
    // H = Hessian_final_link(joint_values); %for code generation
    // 'computeManipulabilityJacobian:7' J = Jacobi_final_link(joint_values);
    y.set_size(1, joint_values.size(0));
    pipk = joint_values.size(0);
    for (i = 0; i < pipk; i++) {
      y[i] = joint_values[i];
    }
    Jacobi_final_link(y, J);
    // 'computeManipulabilityJacobian:8' H = Hessian_final_link(joint_values);
    y.set_size(1, joint_values.size(0));
    pipk = joint_values.size(0);
    for (i = 0; i < pipk; i++) {
      y[i] = joint_values[i];
    }
    Hessian_final_link(y, H);
    //  Compute the manipulability measure 'm'
    // 'computeManipulabilityJacobian:10' JJt = J * J';
    for (i = 0; i < 6; i++) {
      for (aoffset = 0; aoffset < 6; aoffset++) {
        d = 0.0;
        for (pipk = 0; pipk < 7; pipk++) {
          d += J[i + 6 * pipk] * J[aoffset + 6 * pipk];
        }
        JJt[i + 6 * aoffset] = d;
      }
    }
    // 'computeManipulabilityJacobian:11' m = sqrt(det(JJt));
    std::copy(&JJt[0], &JJt[36], &x[0]);
    coder::internal::lapack::xgetrf(x, ipiv);
    absx = x[0];
    isodd = false;
    for (int k{0}; k < 5; k++) {
      absx *= x[(k + 6 * (k + 1)) + 1];
      if (ipiv[k] > k + 1) {
        isodd = !isodd;
      }
    }
    if (isodd) {
      absx = -absx;
    }
    if (absx < 0.0) {
      b_rtErrorWithMessageID("sqrt", d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
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
    for (i = 0; i < 36; i++) {
      inv_JJt[i] = 0.0;
      x[i] = JJt[i];
    }
    coder::internal::lapack::xgetrf(x, ipiv);
    for (i = 0; i < 6; i++) {
      p[i] = static_cast<signed char>(i + 1);
    }
    for (int k{0}; k < 5; k++) {
      i = ipiv[k];
      if (i > k + 1) {
        pipk = p[i - 1];
        p[i - 1] = p[k];
        p[k] = static_cast<signed char>(pipk);
      }
    }
    for (int k{0}; k < 6; k++) {
      inv_JJt_tmp = 6 * (p[k] - 1);
      inv_JJt[k + inv_JJt_tmp] = 1.0;
      for (int j{k + 1}; j < 7; j++) {
        i = (j + inv_JJt_tmp) - 1;
        if (inv_JJt[i] != 0.0) {
          aoffset = j + 1;
          for (int b_i{aoffset}; b_i < 7; b_i++) {
            pipk = (b_i + inv_JJt_tmp) - 1;
            inv_JJt[pipk] -= inv_JJt[i] * x[(b_i + 6 * (j - 1)) - 1];
          }
        }
      }
    }
    for (int j{0}; j < 6; j++) {
      pipk = 6 * j;
      for (int k{5}; k >= 0; k--) {
        aoffset = 6 * k;
        i = k + pipk;
        d = inv_JJt[i];
        if (d != 0.0) {
          inv_JJt[i] = d / x[k + aoffset];
          for (int b_i{0}; b_i < k; b_i++) {
            inv_JJt_tmp = b_i + pipk;
            inv_JJt[inv_JJt_tmp] -= inv_JJt[i] * x[b_i + aoffset];
          }
        }
      }
    }
    //
    //  Pre-compute the pseudo-inverse of JJt
    // pseudo_inv_JJt = pinv(JJt);
    // 'computeManipulabilityJacobian:26' for i = 1:n
    for (int b_i{0}; b_i < 7; b_i++) {
      // 'computeManipulabilityJacobian:27' Hi = H(:,:,i);
      // 'computeManipulabilityJacobian:28' term1 = vec(J * Hi')' *
      // vec(inv_JJt); 'computeManipulabilityJacobian:18' @(X) reshape(X, [], 1)
      // 'computeManipulabilityJacobian:29' Jm_transposed(i) = m * term1;
      for (i = 0; i < 6; i++) {
        for (aoffset = 0; aoffset < 6; aoffset++) {
          d = 0.0;
          for (pipk = 0; pipk < 7; pipk++) {
            d += J[i + 6 * pipk] * H[(aoffset + 6 * pipk) + 42 * b_i];
          }
          JJt[i + 6 * aoffset] = d;
        }
      }
      absx = 0.0;
      for (i = 0; i < 36; i++) {
        absx += JJt[i] * inv_JJt[i];
      }
      Jm_transposed[b_i] = t * absx;
      //  Equation (11)
    }
    if (loop_ub != 7) {
      if (loop_ub == 1) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    for (i = 0; i < 7; i++) {
      absx += -Jm_transposed[i] * q_vel[i];
    }
    varargout_1 += config.weightManipulability * absx;
  }
  //  You can add more terms similarly with their checks and weights
  //  Add the cost for the slack variables
  // 'objFun:6' if slackIsUsed
  if (slackIsUsed) {
    // 'objFun:7' slack = q_vel(n_joints+1:end);
    if (static_cast<unsigned int>(joint_values.size(0)) + 1U >
        static_cast<unsigned int>(q_vel.size(0))) {
      i = 0;
      aoffset = 0;
    } else {
      if ((static_cast<int>(static_cast<unsigned int>(joint_values.size(0)) +
                            1U) < 1) ||
          (static_cast<int>(static_cast<unsigned int>(joint_values.size(0)) +
                            1U) > q_vel.size(0))) {
        rtDynamicBoundsError(
            static_cast<int>(static_cast<unsigned int>(joint_values.size(0)) +
                             1U),
            1, q_vel.size(0), w_emlrtBCI);
      }
      i = joint_values.size(0);
      if (q_vel.size(0) < 1) {
        rtDynamicBoundsError(q_vel.size(0), 1, q_vel.size(0), x_emlrtBCI);
      }
      aoffset = q_vel.size(0);
    }
    iv[0] = 1;
    loop_ub = aoffset - i;
    iv[1] = loop_ub;
    coder::internal::indexShapeCheck(q_vel.size(0), iv);
    // 'objFun:8' objVal = objVal + config.SlackObjectiveWeight * slack' *
    // diag(config.SlackPenaltyWeight) * slack;
    y.set_size(1, loop_ub);
    for (aoffset = 0; aoffset < loop_ub; aoffset++) {
      y[aoffset] = config.SlackObjectiveWeight * q_vel[i + aoffset];
    }
    std::memset(&JJt[0], 0, 36U * sizeof(double));
    for (int j{0}; j < 6; j++) {
      JJt[j + 6 * j] = config.SlackPenaltyWeight[j];
    }
    if (y.size(1) != 6) {
      if (y.size(1) == 1) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    if (loop_ub != 6) {
      if (loop_ub == 1) {
        h_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    for (aoffset = 0; aoffset < 6; aoffset++) {
      d = 0.0;
      for (pipk = 0; pipk < 6; pipk++) {
        d += y[pipk] * JJt[pipk + 6 * aoffset];
      }
      absx += d * q_vel[i + aoffset];
    }
    varargout_1 += absx;
  }
  return varargout_1;
}

//
// File trailer for OptimizationBasedIKWithOAModified.cpp
//
// [EOF]
//
