//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: configurableObjective.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "configurableObjective.h"
#include "Hessian_final_link.h"
#include "Jacobi_final_link.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
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

// Function Declarations
static void b_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

// Function Definitions
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
// function objVal = configurableObjective(current_joint_velocity, jacobi,
// xd_eff_vel, starting_joint_vel, joint_values,WeightMatrix,config)
//
// Arguments    : const coder::array<double, 1U> &current_joint_velocity
//                const coder::array<double, 2U> &jacobi
//                const double xd_eff_vel[6]
//                const coder::array<double, 1U> &starting_joint_vel
//                const coder::array<double, 1U> &joint_values
//                const coder::array<double, 2U> &WeightMatrix
//                boolean_T config_useObjectiveNormInfinity
//                double config_weightNormInfinity
//                boolean_T config_useObjectiveNormL2
//                double config_weightNormL2
//                boolean_T config_useObjectiveTrajectoryFollowing
//                double config_weightTrajectoryFollowing
//                boolean_T config_useObjectiveJointAcceleration
//                double config_weightJointAcceleration
//                boolean_T config_useObjectiveManipulability
//                double config_weightManipulability
// Return Type  : double
//
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
    double config_weightManipulability)
{
  static rtEqualityCheckInfo b_emlrtECI{
      1,                       // nDims
      17,                      // lineNo
      127,                     // colNo
      "configurableObjective", // fName
      "/home/shobhit/ShobhitRobotModelling/robotmodelling/"
      "Matlab_implementation/functions_for_codegen/"
      "obstacleAvoidanceOptmisationModif"
      "ied/configurableObjective.m" // pName
  };
  static rtEqualityCheckInfo emlrtECI{
      1,                       // nDims
      17,                      // lineNo
      63,                      // colNo
      "configurableObjective", // fName
      "/home/shobhit/ShobhitRobotModelling/robotmodelling/"
      "Matlab_implementation/functions_for_codegen/"
      "obstacleAvoidanceOptmisationModif"
      "ied/configurableObjective.m" // pName
  };
  static rtRunTimeErrorInfo e_emlrtRTEI{
      13,    // lineNo
      "sqrt" // fName
  };
  coder::array<double, 2U> y;
  coder::array<double, 1U> b;
  double JJt[36];
  double x[36];
  double a;
  double absx;
  double objVal;
  double t;
  int aoffset;
  int i;
  int pipk;
  boolean_T isodd;
  // 'configurableObjective:2' objVal = 0;
  objVal = 0.0;
  // 'configurableObjective:4' if config.useObjectiveNormInfinity
  if (config_useObjectiveNormInfinity) {
    // 'configurableObjective:5' objVal = objVal + config.weightNormInfinity *
    // (norm(current_joint_velocity, Inf)^2) / 2;
    if (current_joint_velocity.size(0) == 0) {
      a = 0.0;
    } else {
      a = 0.0;
      i = current_joint_velocity.size(0);
      for (int k{0}; k < i; k++) {
        absx = std::abs(current_joint_velocity[k]);
        if (std::isnan(absx) || (absx > a)) {
          a = absx;
        }
      }
    }
    objVal = config_weightNormInfinity * (a * a) / 2.0;
  }
  // 'configurableObjective:8' if config.useObjectiveNormL2
  if (config_useObjectiveNormL2) {
    // 'configurableObjective:9' objVal = objVal + config.weightNormL2 *
    // (current_joint_velocity' * WeightMatrix * current_joint_velocity) / 2;
    if (current_joint_velocity.size(0) != WeightMatrix.size(0)) {
      if ((current_joint_velocity.size(0) == 1) ||
          ((WeightMatrix.size(0) == 1) && (WeightMatrix.size(1) == 1))) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
      }
    }
    coder::internal::blas::mtimes(current_joint_velocity, WeightMatrix, y);
    if (current_joint_velocity.size(0) != y.size(1)) {
      if ((y.size(1) == 1) || (current_joint_velocity.size(0) == 1)) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    pipk = y.size(1);
    for (i = 0; i < pipk; i++) {
      absx += y[i] * current_joint_velocity[i];
    }
    objVal += config_weightNormL2 * absx / 2.0;
  }
  // 'configurableObjective:12' if config.useObjectiveTrajectoryFollowing
  if (config_useObjectiveTrajectoryFollowing) {
    double b_y[6];
    double scale;
    // 'configurableObjective:13' objVal = objVal +
    // config.weightTrajectoryFollowing * norm((jacobi * current_joint_velocity
    // - xd_eff_vel), 2)^2 / 2;
    if (current_joint_velocity.size(0) != jacobi.size(1)) {
      if (current_joint_velocity.size(0) == 1) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
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
        b_y[b_i] += jacobi[aoffset + b_i] * current_joint_velocity[k];
      }
    }
    a = 0.0;
    scale = 3.3121686421112381E-170;
    for (int k{0}; k < 6; k++) {
      absx = b_y[k] - xd_eff_vel[k];
      b_y[k] = absx;
      absx = std::abs(absx);
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
    objVal += config_weightTrajectoryFollowing * (a * a) / 2.0;
  }
  // 'configurableObjective:16' if config.useObjectiveJointAcceleration
  if (config_useObjectiveJointAcceleration) {
    // 'configurableObjective:17' objVal =  objVal +
    // config.weightJointAcceleration * ((current_joint_velocity -
    // starting_joint_vel)' * WeightMatrix * (current_joint_velocity -
    // starting_joint_vel)) /2 ;
    isodd = ((current_joint_velocity.size(0) != starting_joint_vel.size(0)) &&
             ((current_joint_velocity.size(0) != 1) &&
              (starting_joint_vel.size(0) != 1)));
    if (isodd) {
      emlrtDimSizeImpxCheckR2021b(current_joint_velocity.size(0),
                                  starting_joint_vel.size(0), emlrtECI);
    }
    if (current_joint_velocity.size(0) == starting_joint_vel.size(0)) {
      b.set_size(current_joint_velocity.size(0));
      pipk = current_joint_velocity.size(0);
      for (i = 0; i < pipk; i++) {
        b[i] = current_joint_velocity[i] - starting_joint_vel[i];
      }
    } else {
      minus(b, current_joint_velocity, starting_joint_vel);
    }
    if (isodd) {
      emlrtDimSizeImpxCheckR2021b(current_joint_velocity.size(0),
                                  starting_joint_vel.size(0), b_emlrtECI);
    }
    if (b.size(0) != WeightMatrix.size(0)) {
      if ((b.size(0) == 1) ||
          ((WeightMatrix.size(0) == 1) && (WeightMatrix.size(1) == 1))) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
      }
    }
    coder::internal::blas::mtimes(b, WeightMatrix, y);
    if (b.size(0) != y.size(1)) {
      if ((y.size(1) == 1) || (b.size(0) == 1)) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    pipk = y.size(1);
    for (i = 0; i < pipk; i++) {
      absx += y[i] * b[i];
    }
    objVal += config_weightJointAcceleration * absx / 2.0;
  }
  // 'configurableObjective:20' if config.useObjectiveManipulability
  if (config_useObjectiveManipulability) {
    double H[294];
    double J[42];
    double inv_JJt[36];
    double Jm_transposed[7];
    int ipiv[6];
    int inv_JJt_tmp;
    signed char p[6];
    // 'configurableObjective:21' objVal  = objVal + config.weightManipulability
    // * (-computeManipulabilityJacobian(joint_values') *
    // current_joint_velocity);
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
        absx = 0.0;
        for (pipk = 0; pipk < 7; pipk++) {
          absx += J[i + 6 * pipk] * J[aoffset + 6 * pipk];
        }
        JJt[i + 6 * aoffset] = absx;
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
      b_rtErrorWithMessageID("sqrt", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
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
        absx = inv_JJt[i];
        if (absx != 0.0) {
          inv_JJt[i] = absx / x[k + aoffset];
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
          absx = 0.0;
          for (pipk = 0; pipk < 7; pipk++) {
            absx += J[i + 6 * pipk] * H[(aoffset + 6 * pipk) + 42 * b_i];
          }
          JJt[i + 6 * aoffset] = absx;
        }
      }
      absx = 0.0;
      for (i = 0; i < 36; i++) {
        absx += JJt[i] * inv_JJt[i];
      }
      Jm_transposed[b_i] = t * absx;
      //  Equation (11)
    }
    if (current_joint_velocity.size(0) != 7) {
      if (current_joint_velocity.size(0) == 1) {
        h_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      } else {
        g_rtErrorWithMessageID(emlrtRTEI.fName, emlrtRTEI.lineNo);
      }
    }
    absx = 0.0;
    for (i = 0; i < 7; i++) {
      absx += -Jm_transposed[i] * current_joint_velocity[i];
    }
    objVal += config_weightManipulability * absx;
  }
  //  You can add more terms similarly with their checks and weights
  return objVal;
}

//
// File trailer for configurableObjective.cpp
//
// [EOF]
//
