//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: configurableObjective.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "configurableObjective.h"
#include "Hessian_final_link.h"
#include "Jacobi_final_link.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "xgetrf.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Definitions
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
  coder::array<double, 2U> b_joint_values;
  coder::array<double, 1U> b;
  double JJt[36];
  double inv_JJt[36];
  double absx;
  double objVal;
  double scale;
  double t;
  double t380;
  int aoffset;
  int i;
  int pipk;
  // 'configurableObjective:2' objVal = 0;
  objVal = 0.0;
  // 'configurableObjective:4' if config.useObjectiveNormInfinity
  if (config_useObjectiveNormInfinity) {
    // 'configurableObjective:5' objVal = objVal + config.weightNormInfinity *
    // (norm(current_joint_velocity, Inf)^2) / 2;
    if (current_joint_velocity.size(0) == 0) {
      t380 = 0.0;
    } else {
      t380 = 0.0;
      i = current_joint_velocity.size(0);
      for (int k{0}; k < i; k++) {
        absx = std::abs(current_joint_velocity[k]);
        if (std::isnan(absx) || (absx > t380)) {
          t380 = absx;
        }
      }
    }
    objVal = config_weightNormInfinity * (t380 * t380) / 2.0;
  }
  // 'configurableObjective:8' if config.useObjectiveNormL2
  if (config_useObjectiveNormL2) {
    // 'configurableObjective:9' objVal = objVal + config.weightNormL2 *
    // (current_joint_velocity' * WeightMatrix * current_joint_velocity) / 2;
    coder::internal::blas::mtimes(current_joint_velocity, WeightMatrix,
                                  b_joint_values);
    absx = 0.0;
    pipk = b_joint_values.size(1);
    for (i = 0; i < pipk; i++) {
      absx += b_joint_values[i] * current_joint_velocity[i];
    }
    objVal += config_weightNormL2 * absx / 2.0;
  }
  // 'configurableObjective:12' if config.useObjectiveTrajectoryFollowing
  if (config_useObjectiveTrajectoryFollowing) {
    double y[6];
    // 'configurableObjective:13' objVal = objVal +
    // config.weightTrajectoryFollowing * norm((jacobi * current_joint_velocity
    // - xd_eff_vel), 2)^2 / 2;
    pipk = jacobi.size(1);
    for (int b_i{0}; b_i < 6; b_i++) {
      y[b_i] = 0.0;
    }
    for (int k{0}; k < pipk; k++) {
      aoffset = k * 6;
      for (int b_i{0}; b_i < 6; b_i++) {
        y[b_i] += jacobi[aoffset + b_i] * current_joint_velocity[k];
      }
    }
    t380 = 0.0;
    scale = 3.3121686421112381E-170;
    for (int k{0}; k < 6; k++) {
      absx = y[k] - xd_eff_vel[k];
      y[k] = absx;
      absx = std::abs(absx);
      if (absx > scale) {
        t = scale / absx;
        t380 = t380 * t * t + 1.0;
        scale = absx;
      } else {
        t = absx / scale;
        t380 += t * t;
      }
    }
    t380 = scale * std::sqrt(t380);
    objVal += config_weightTrajectoryFollowing * (t380 * t380) / 2.0;
  }
  // 'configurableObjective:16' if config.useObjectiveJointAcceleration
  if (config_useObjectiveJointAcceleration) {
    // 'configurableObjective:17' objVal =  objVal +
    // config.weightJointAcceleration * ((current_joint_velocity -
    // starting_joint_vel)' * WeightMatrix * (current_joint_velocity -
    // starting_joint_vel)) /2 ;
    if (current_joint_velocity.size(0) == starting_joint_vel.size(0)) {
      b.set_size(current_joint_velocity.size(0));
      pipk = current_joint_velocity.size(0);
      for (i = 0; i < pipk; i++) {
        b[i] = current_joint_velocity[i] - starting_joint_vel[i];
      }
    } else {
      minus(b, current_joint_velocity, starting_joint_vel);
    }
    coder::internal::blas::mtimes(b, WeightMatrix, b_joint_values);
    absx = 0.0;
    pipk = b_joint_values.size(1);
    for (i = 0; i < pipk; i++) {
      absx += b_joint_values[i] * b[i];
    }
    objVal += config_weightJointAcceleration * absx / 2.0;
  }
  // 'configurableObjective:20' if config.useObjectiveManipulability
  if (config_useObjectiveManipulability) {
    double H[294];
    double J[42];
    int ipiv[6];
    int inv_JJt_tmp;
    signed char p[6];
    boolean_T isodd;
    // 'configurableObjective:21' objVal  = objVal + config.weightManipulability
    // * (-computeManipulabilityJacobian(joint_values') *
    // current_joint_velocity);
    //  Compute the Jacobian and Hessian first
    // [~, J, H] = computeKinematics(joint_values, mdhparams); because of syms
    // was too slow J = Jacobi_final_link1(joint_values); %for code generation
    // H = Hessian_final_link(joint_values); %for code generation
    // 'computeManipulabilityJacobian:7' J = Jacobi_final_link(joint_values);
    b_joint_values.set_size(1, joint_values.size(0));
    pipk = joint_values.size(0);
    for (i = 0; i < pipk; i++) {
      b_joint_values[i] = joint_values[i];
    }
    double b_t10[362];
    double t10;
    double t100;
    double t102;
    double t11;
    double t111;
    double t12;
    double t126;
    double t130;
    double t131;
    double t137;
    double t138;
    double t139;
    double t14;
    double t149;
    double t15;
    double t150;
    double t156;
    double t158;
    double t158_tmp;
    double t16;
    double t161;
    double t163;
    double t164;
    double t168;
    double t169;
    double t17;
    double t172;
    double t176;
    double t179;
    double t18;
    double t181;
    double t182;
    double t183;
    double t184;
    double t185;
    double t186;
    double t187;
    double t188;
    double t189;
    double t19;
    double t2;
    double t20;
    double t21;
    double t22;
    double t223;
    double t227;
    double t23;
    double t231;
    double t232;
    double t233;
    double t24;
    double t25;
    double t254;
    double t26;
    double t263;
    double t27;
    double t274;
    double t282;
    double t290;
    double t295;
    double t3;
    double t301;
    double t303;
    double t347;
    double t37;
    double t389;
    double t395;
    double t4;
    double t44;
    double t46;
    double t48;
    double t49;
    double t5;
    double t53;
    double t54;
    double t56;
    double t568;
    double t57;
    double t571;
    double t575;
    double t576;
    double t6;
    double t60;
    double t61;
    double t63;
    double t74;
    double t75;
    double t8;
    double t82;
    double t83;
    double t84;
    double t85;
    double t87;
    double t88;
    double t89;
    double t9;
    double t90;
    double t91;
    double t92;
    double t93;
    double t94;
    double t95;
    double t96;
    double t97;
    double t98;
    double t99;
    Jacobi_final_link(b_joint_values, J);
    // 'computeManipulabilityJacobian:8' H = Hessian_final_link(joint_values);
    // Hessian_final_link
    //     Hessian = Hessian_final_link(IN1)
    //     This function was generated by the Symbolic Math Toolbox
    //     version 23.2. 08-Nov-2024 19:20:56
    // 'Hessian_final_link:8' theta1 = in1(:,1);
    // 'Hessian_final_link:9' theta2 = in1(:,2);
    // 'Hessian_final_link:10' theta3 = in1(:,3);
    // 'Hessian_final_link:11' theta4 = in1(:,4);
    // 'Hessian_final_link:12' theta5 = in1(:,5);
    // 'Hessian_final_link:13' theta6 = in1(:,6);
    // 'Hessian_final_link:14' t2 = cos(theta1);
    t2 = std::cos(joint_values[0]);
    // 'Hessian_final_link:15' t3 = cos(theta2);
    t3 = std::cos(joint_values[1]);
    // 'Hessian_final_link:16' t4 = cos(theta3);
    t4 = std::cos(joint_values[2]);
    // 'Hessian_final_link:17' t5 = cos(theta4);
    t5 = std::cos(joint_values[3]);
    // 'Hessian_final_link:18' t6 = cos(theta5);
    t6 = std::cos(joint_values[4]);
    // 'Hessian_final_link:19' t7 = cos(theta6);
    // 'Hessian_final_link:20' t8 = sin(theta1);
    t8 = std::sin(joint_values[0]);
    // 'Hessian_final_link:21' t9 = sin(theta2);
    t9 = std::sin(joint_values[1]);
    // 'Hessian_final_link:22' t10 = sin(theta3);
    t10 = std::sin(joint_values[2]);
    // 'Hessian_final_link:23' t11 = sin(theta4);
    t11 = std::sin(joint_values[3]);
    // 'Hessian_final_link:24' t12 = sin(theta5);
    t12 = std::sin(joint_values[4]);
    // 'Hessian_final_link:25' t13 = sin(theta6);
    // 'Hessian_final_link:26' t14 = t2.*t3;
    t14 = t2 * t3;
    // 'Hessian_final_link:27' t15 = t2.*t4;
    t15 = t2 * t4;
    // 'Hessian_final_link:28' t16 = t3.*t4;
    t16 = t3 * t4;
    // 'Hessian_final_link:29' t17 = t2.*t9;
    t17 = t2 * t9;
    // 'Hessian_final_link:30' t18 = t3.*t8;
    t18 = t3 * t8;
    // 'Hessian_final_link:31' t19 = t2.*t10;
    t19 = t2 * t10;
    // 'Hessian_final_link:32' t20 = t4.*t8;
    t20 = t4 * t8;
    // 'Hessian_final_link:33' t21 = t3.*t10;
    t21 = t3 * t10;
    // 'Hessian_final_link:34' t22 = t4.*t9;
    t22 = t4 * t9;
    // 'Hessian_final_link:35' t23 = t5.*t9;
    t23 = t5 * t9;
    // 'Hessian_final_link:36' t24 = t8.*t9;
    t24 = t8 * t9;
    // 'Hessian_final_link:37' t25 = t8.*t10;
    t25 = t8 * t10;
    // 'Hessian_final_link:38' t26 = t9.*t10;
    t26 = t9 * t10;
    // 'Hessian_final_link:39' t27 = t9.*t11;
    t27 = t9 * t11;
    // 'Hessian_final_link:40' t44 = t2.*6.123233995736766e-17;
    t44 = t2 * 6.123233995736766E-17;
    // 'Hessian_final_link:41' t45 = t3.*6.123233995736766e-17;
    // 'Hessian_final_link:42' t46 = t4.*6.123233995736766e-17;
    t46 = t4 * 6.123233995736766E-17;
    // 'Hessian_final_link:43' t47 = t8.*6.123233995736766e-17;
    // 'Hessian_final_link:44' t48 = t9.*6.123233995736766e-17;
    t48 = t9 * 6.123233995736766E-17;
    // 'Hessian_final_link:45' t49 = t10.*6.123233995736766e-17;
    t49 = t10 * 6.123233995736766E-17;
    // 'Hessian_final_link:46' t74 = t4.*5.051668046482832e-18;
    t74 = t4 * 5.0516680464828323E-18;
    // 'Hessian_final_link:47' t75 = t10.*5.051668046482832e-18;
    t75 = t10 * 5.0516680464828323E-18;
    // 'Hessian_final_link:48' t120 = t2.*3.749399456654644e-33;
    // 'Hessian_final_link:49' t121 = t3.*3.749399456654644e-33;
    // 'Hessian_final_link:50' t122 = t4.*3.749399456654644e-33;
    // 'Hessian_final_link:51' t123 = t3+3.749399456654644e-33;
    // 'Hessian_final_link:52' t124 = t8.*3.749399456654644e-33;
    // 'Hessian_final_link:53' t125 = t9.*3.749399456654644e-33;
    // 'Hessian_final_link:54' t126 = t10.*3.749399456654644e-33;
    t126 = t10 * 3.749399456654644E-33;
    // 'Hessian_final_link:55' t218 = t2.*2.295845021658468e-49;
    // 'Hessian_final_link:56' t219 = t3.*2.295845021658468e-49;
    // 'Hessian_final_link:57' t220 = t4.*2.295845021658468e-49;
    // 'Hessian_final_link:58' t221 = t8.*2.295845021658468e-49;
    // 'Hessian_final_link:59' t222 = t9.*2.295845021658468e-49;
    // 'Hessian_final_link:60' t223 = t10.*2.295845021658468e-49;
    t223 = t10 * 2.2958450216584679E-49;
    // 'Hessian_final_link:61' t340 = t2.*1.405799628556214e-65;
    // 'Hessian_final_link:62' t341 = t8.*1.405799628556214e-65;
    // 'Hessian_final_link:63' t354 = t3.*1.504205602555149e-66;
    // 'Hessian_final_link:64' t355 = t4.*1.504205602555149e-66;
    // 'Hessian_final_link:65' t357 = t9.*1.504205602555149e-66;
    // 'Hessian_final_link:66' t358 = t10.*1.504205602555149e-66;
    // 'Hessian_final_link:67' et1 = t3;
    // 'Hessian_final_link:68' et2 = 1.439769391355383e-33;
    // 'Hessian_final_link:69' t409 = et1.*et2;
    // 'Hessian_final_link:70' et3 = t4;
    // 'Hessian_final_link:71' et4 = 1.439769391355383e-33;
    // 'Hessian_final_link:72' t410 = et3.*et4;
    // 'Hessian_final_link:73' et5 = t9;
    // 'Hessian_final_link:74' et6 = 1.439769391355383e-33;
    // 'Hessian_final_link:75' t411 = et5.*et6;
    // 'Hessian_final_link:76' et7 = t10;
    // 'Hessian_final_link:77' et8 = 1.439769391355383e-33;
    // 'Hessian_final_link:78' t412 = et7.*et8;
    // 'Hessian_final_link:79' et9 = t2;
    // 'Hessian_final_link:80' et10 = 9.210602882143395e-83;
    // 'Hessian_final_link:81' t469 = et9.*et10;
    // 'Hessian_final_link:82' et11 = t8;
    // 'Hessian_final_link:83' et12 = 9.210602882143395e-83;
    // 'Hessian_final_link:84' t471 = et11.*et12;
    // 'Hessian_final_link:85' et13 = t3;
    // 'Hessian_final_link:86' et14 = 3.16e-1;
    // 'Hessian_final_link:87' t506 = et13.*et14;
    // 'Hessian_final_link:88' et15 = t9;
    // 'Hessian_final_link:89' et16 = 3.16e-1;
    // 'Hessian_final_link:90' t507 = et15.*et16;
    // 'Hessian_final_link:91' et17 = t2;
    // 'Hessian_final_link:92' et18 = 8.816044883168516e-50;
    // 'Hessian_final_link:93' t510 = et17.*et18;
    // 'Hessian_final_link:94' et19 = t8;
    // 'Hessian_final_link:95' et20 = 8.816044883168516e-50;
    // 'Hessian_final_link:96' t511 = et19.*et20;
    // 'Hessian_final_link:97' t28 = -t15;
    // 'Hessian_final_link:98' t29 = -t23;
    // 'Hessian_final_link:99' t30 = -t24;
    // 'Hessian_final_link:100' t31 = -t26;
    // 'Hessian_final_link:101' t32 = t15.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:102' t33 = t16.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:103' t34 = t19.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:104' t35 = t20.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:105' t36 = t22.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:106' t37 = t25.*(3.3e+1./4.0e+2);
    t37 = t25 * 0.0825;
    // 'Hessian_final_link:107' t38 = t26.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:108' t39 = t27.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:109' t50 = -t46;
    // 'Hessian_final_link:110' t51 = -t47;
    // 'Hessian_final_link:111' t52 = -t49;
    // 'Hessian_final_link:112' t53 = t14.*6.123233995736766e-17;
    t53 = t14 * 6.123233995736766E-17;
    // 'Hessian_final_link:113' t54 = t15.*6.123233995736766e-17;
    t54 = t15 * 6.123233995736766E-17;
    // 'Hessian_final_link:114' t55 = t16.*6.123233995736766e-17;
    absx = t16 * 6.123233995736766E-17;
    // 'Hessian_final_link:115' t56 = t17.*6.123233995736766e-17;
    t56 = t17 * 6.123233995736766E-17;
    // 'Hessian_final_link:116' t57 = t18.*6.123233995736766e-17;
    t57 = t18 * 6.123233995736766E-17;
    // 'Hessian_final_link:117' t58 = t19.*6.123233995736766e-17;
    // 'Hessian_final_link:118' t59 = t20.*6.123233995736766e-17;
    // 'Hessian_final_link:119' t60 = t21.*6.123233995736766e-17;
    t60 = t21 * 6.123233995736766E-17;
    // 'Hessian_final_link:120' t61 = t22.*6.123233995736766e-17;
    t61 = t22 * 6.123233995736766E-17;
    // 'Hessian_final_link:121' t62 = t23.*6.123233995736766e-17;
    // 'Hessian_final_link:122' t63 = t24.*6.123233995736766e-17;
    t63 = t24 * 6.123233995736766E-17;
    // 'Hessian_final_link:123' t64 = t25.*6.123233995736766e-17;
    // 'Hessian_final_link:124' t65 = t26.*6.123233995736766e-17;
    // 'Hessian_final_link:125' t76 = -t74;
    // 'Hessian_final_link:126' t77 = t16.*5.051668046482832e-18;
    // 'Hessian_final_link:127' t78 = t21.*5.051668046482832e-18;
    // 'Hessian_final_link:128' t79 = t26.*5.051668046482832e-18;
    // 'Hessian_final_link:129' t127 = -t122;
    // 'Hessian_final_link:130' t128 = -t124;
    // 'Hessian_final_link:131' t129 = -t126;
    // 'Hessian_final_link:132' t130 = t14.*3.749399456654644e-33;
    t130 = t14 * 3.749399456654644E-33;
    // 'Hessian_final_link:133' t131 = t15.*3.749399456654644e-33;
    t131 = t15 * 3.749399456654644E-33;
    // 'Hessian_final_link:134' t132 = t16.*3.749399456654644e-33;
    // 'Hessian_final_link:135' t133 = t17.*3.749399456654644e-33;
    // 'Hessian_final_link:136' t134 = t18.*3.749399456654644e-33;
    // 'Hessian_final_link:137' t135 = t19.*3.749399456654644e-33;
    // 'Hessian_final_link:138' t136 = t20.*3.749399456654644e-33;
    // 'Hessian_final_link:139' t137 = t21.*3.749399456654644e-33;
    t137 = t21 * 3.749399456654644E-33;
    // 'Hessian_final_link:140' t138 = t22.*3.749399456654644e-33;
    t138 = t22 * 3.749399456654644E-33;
    // 'Hessian_final_link:141' t139 = t24.*3.749399456654644e-33;
    t139 = t24 * 3.749399456654644E-33;
    // 'Hessian_final_link:142' t140 = t25.*3.749399456654644e-33;
    // 'Hessian_final_link:143' t141 = t26.*3.749399456654644e-33;
    // 'Hessian_final_link:144' t148 = t23.*4.011857418620469e-34;
    // 'Hessian_final_link:145' t149 = t5.*t123;
    t149 = t5 * (t3 + 3.749399456654644E-33);
    // 'Hessian_final_link:146' t150 = t11.*t123;
    t150 = t11 * (t3 + 3.749399456654644E-33);
    // 'Hessian_final_link:147' t217 = t23.*3.84e-1;
    // 'Hessian_final_link:148' t224 = -t220;
    // 'Hessian_final_link:149' t225 = -t221;
    // 'Hessian_final_link:150' t226 = -t223;
    // 'Hessian_final_link:151' t227 = t14.*2.295845021658468e-49;
    t227 = t14 * 2.2958450216584679E-49;
    // 'Hessian_final_link:152' t228 = t16.*2.295845021658468e-49;
    // 'Hessian_final_link:153' t229 = t17.*2.295845021658468e-49;
    // 'Hessian_final_link:154' t230 = t18.*2.295845021658468e-49;
    // 'Hessian_final_link:155' t231 = t21.*2.295845021658468e-49;
    t231 = t21 * 2.2958450216584679E-49;
    // 'Hessian_final_link:156' t232 = t22.*2.295845021658468e-49;
    t232 = t22 * 2.2958450216584679E-49;
    // 'Hessian_final_link:157' t233 = t24.*2.295845021658468e-49;
    t233 = t24 * 2.2958450216584679E-49;
    // 'Hessian_final_link:158' t234 = t15.*2.45655417317456e-50;
    // 'Hessian_final_link:159' t236 = t19.*2.45655417317456e-50;
    // 'Hessian_final_link:160' t237 = t20.*2.45655417317456e-50;
    // 'Hessian_final_link:161' t238 = t21.*2.45655417317456e-50;
    // 'Hessian_final_link:162' t239 = t22.*2.45655417317456e-50;
    // 'Hessian_final_link:163' t242 = t25.*2.45655417317456e-50;
    // 'Hessian_final_link:164' t243 = t26.*2.45655417317456e-50;
    // 'Hessian_final_link:165' t295 = t15.*2.351321854362918e-17;
    t295 = t15 * 2.3513218543629179E-17;
    // 'Hessian_final_link:166' t296 = t19.*2.351321854362918e-17;
    // 'Hessian_final_link:167' t297 = t20.*2.351321854362918e-17;
    // 'Hessian_final_link:168' t298 = t21.*2.351321854362918e-17;
    // 'Hessian_final_link:169' t299 = t22.*2.351321854362918e-17;
    // 'Hessian_final_link:170' t301 = t25.*2.351321854362918e-17;
    t301 = t25 * 2.3513218543629179E-17;
    // 'Hessian_final_link:171' t302 = t26.*2.351321854362918e-17;
    // 'Hessian_final_link:172' t346 = -t341;
    // 'Hessian_final_link:173' t347 = t14.*1.405799628556214e-65;
    t347 = t14 * 1.405799628556214E-65;
    // 'Hessian_final_link:174' t348 = t17.*1.405799628556214e-65;
    // 'Hessian_final_link:175' t349 = t18.*1.405799628556214e-65;
    // 'Hessian_final_link:176' t350 = t24.*1.405799628556214e-65;
    // 'Hessian_final_link:177' t360 = -t355;
    // 'Hessian_final_link:178' t361 = t17.*1.504205602555149e-66;
    // 'Hessian_final_link:179' t362 = t18.*1.504205602555149e-66;
    // 'Hessian_final_link:180' t363 = t21.*1.504205602555149e-66;
    // 'Hessian_final_link:181' t364 = t22.*1.504205602555149e-66;
    // 'Hessian_final_link:182' t365 = t24.*1.504205602555149e-66;
    // 'Hessian_final_link:183' t367 = t14.*1.504205602555149e-66;
    // 'Hessian_final_link:184' t368 = t16.*1.504205602555149e-66;
    // 'Hessian_final_link:185' t413 = -t410;
    // 'Hessian_final_link:186' t414 = -t412;
    // 'Hessian_final_link:187' et21 = t17;
    // 'Hessian_final_link:188' et22 = 1.439769391355383e-33;
    // 'Hessian_final_link:189' t415 = et21.*et22;
    // 'Hessian_final_link:190' et23 = t18;
    // 'Hessian_final_link:191' et24 = 1.439769391355383e-33;
    // 'Hessian_final_link:192' t416 = et23.*et24;
    // 'Hessian_final_link:193' et25 = t21;
    // 'Hessian_final_link:194' et26 = 1.439769391355383e-33;
    // 'Hessian_final_link:195' t417 = et25.*et26;
    // 'Hessian_final_link:196' et27 = t22;
    // 'Hessian_final_link:197' et28 = 1.439769391355383e-33;
    // 'Hessian_final_link:198' t418 = et27.*et28;
    // 'Hessian_final_link:199' et29 = t24;
    // 'Hessian_final_link:200' et30 = 1.439769391355383e-33;
    // 'Hessian_final_link:201' t419 = et29.*et30;
    // 'Hessian_final_link:202' et31 = t14;
    // 'Hessian_final_link:203' et32 = 1.439769391355383e-33;
    // 'Hessian_final_link:204' t421 = et31.*et32;
    // 'Hessian_final_link:205' et33 = t16;
    // 'Hessian_final_link:206' et34 = 1.439769391355383e-33;
    // 'Hessian_final_link:207' t422 = et33.*et34;
    // 'Hessian_final_link:208' t476 = -t471;
    // 'Hessian_final_link:209' et35 = t14;
    // 'Hessian_final_link:210' et36 = 9.210602882143395e-83;
    // 'Hessian_final_link:211' t478 = et35.*et36;
    // 'Hessian_final_link:212' et37 = t17;
    // 'Hessian_final_link:213' et38 = 9.210602882143395e-83;
    // 'Hessian_final_link:214' t479 = et37.*et38;
    // 'Hessian_final_link:215' et39 = t18;
    // 'Hessian_final_link:216' et40 = 9.210602882143395e-83;
    // 'Hessian_final_link:217' t480 = et39.*et40;
    // 'Hessian_final_link:218' et41 = t24;
    // 'Hessian_final_link:219' et42 = 9.210602882143395e-83;
    // 'Hessian_final_link:220' t481 = et41.*et42;
    // 'Hessian_final_link:221' et43 = t14;
    // 'Hessian_final_link:222' et44 = 3.16e-1;
    // 'Hessian_final_link:223' t508 = et43.*et44;
    // 'Hessian_final_link:224' et45 = t18;
    // 'Hessian_final_link:225' et46 = 3.16e-1;
    // 'Hessian_final_link:226' t509 = et45.*et46;
    // 'Hessian_final_link:227' t512 = -t511;
    // 'Hessian_final_link:228' et47 = t14;
    // 'Hessian_final_link:229' et48 = 8.816044883168516e-50;
    // 'Hessian_final_link:230' t513 = et47.*et48;
    // 'Hessian_final_link:231' et49 = t17;
    // 'Hessian_final_link:232' et50 = 8.816044883168516e-50;
    // 'Hessian_final_link:233' t514 = et49.*et50;
    // 'Hessian_final_link:234' et51 = t18;
    // 'Hessian_final_link:235' et52 = 8.816044883168516e-50;
    // 'Hessian_final_link:236' t515 = et51.*et52;
    // 'Hessian_final_link:237' et53 = t24;
    // 'Hessian_final_link:238' et54 = 8.816044883168516e-50;
    // 'Hessian_final_link:239' t516 = et53.*et54;
    // 'Hessian_final_link:240' et55 = t14;
    // 'Hessian_final_link:241' et56 = 1.934941942652818e-17;
    // 'Hessian_final_link:242' t567 = et55.*et56;
    // 'Hessian_final_link:243' et57 = t17;
    // 'Hessian_final_link:244' et58 = 1.934941942652818e-17;
    // 'Hessian_final_link:245' t568 = et57.*et58;
    t568 = t17 * 1.9349419426528181E-17;
    // 'Hessian_final_link:246' et59 = t18;
    // 'Hessian_final_link:247' et60 = 1.934941942652818e-17;
    // 'Hessian_final_link:248' t569 = et59.*et60;
    // 'Hessian_final_link:249' et61 = t24;
    // 'Hessian_final_link:250' et62 = 1.934941942652818e-17;
    // 'Hessian_final_link:251' t570 = et61.*et62;
    // 'Hessian_final_link:252' t40 = -t36;
    // 'Hessian_final_link:253' t41 = -t37;
    // 'Hessian_final_link:254' t43 = -t39;
    // 'Hessian_final_link:255' t66 = -t53;
    // 'Hessian_final_link:256' t67 = -t54;
    // 'Hessian_final_link:257' t68 = -t55;
    // 'Hessian_final_link:258' t69 = -t60;
    // 'Hessian_final_link:259' t70 = -t61;
    // 'Hessian_final_link:260' t71 = -t63;
    // 'Hessian_final_link:261' t72 = -t64;
    // 'Hessian_final_link:262' t73 = -t65;
    // 'Hessian_final_link:263' t80 = -t78;
    // 'Hessian_final_link:264' t81 = -t79;
    // 'Hessian_final_link:265' t82 = t17+t57;
    t82 = t17 + t57;
    // 'Hessian_final_link:266' t83 = t18+t56;
    t83 = t18 + t56;
    // 'Hessian_final_link:267' t84 = t21+t61;
    t84 = t21 + t61;
    // 'Hessian_final_link:268' t87 = t30+t53;
    t87 = -t24 + t53;
    // 'Hessian_final_link:269' t142 = -t130;
    // 'Hessian_final_link:270' t143 = -t131;
    // 'Hessian_final_link:271' t144 = -t137;
    // 'Hessian_final_link:272' t145 = -t138;
    // 'Hessian_final_link:273' t146 = -t139;
    // 'Hessian_final_link:274' t147 = -t141;
    // 'Hessian_final_link:275' t151 = t149.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:276' t152 = t150.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:277' t172 = t22+t52+t60;
    t172 = (t22 - t49) + t60;
    // 'Hessian_final_link:278' t235 = -t227;
    // 'Hessian_final_link:279' t240 = -t231;
    // 'Hessian_final_link:280' t241 = -t232;
    // 'Hessian_final_link:281' t244 = -t233;
    // 'Hessian_final_link:282' t245 = -t234;
    // 'Hessian_final_link:283' t246 = -t239;
    // 'Hessian_final_link:284' t248 = -t243;
    // 'Hessian_final_link:285' t249 = t149.*6.123233995736766e-17;
    // 'Hessian_final_link:286' t250 = t150.*6.123233995736766e-17;
    // 'Hessian_final_link:287' t305 = -t295;
    // 'Hessian_final_link:288' t307 = -t301;
    // 'Hessian_final_link:289' t308 = -t302;
    // 'Hessian_final_link:290' t353 = -t347;
    // 'Hessian_final_link:291' t356 = -t350;
    // 'Hessian_final_link:292' t369 = -t363;
    // 'Hessian_final_link:293' t375 = t149.*4.011857418620469e-34;
    // 'Hessian_final_link:294' t376 = t150.*4.011857418620469e-34;
    // 'Hessian_final_link:295' t450 = t149.*3.84e-1;
    // 'Hessian_final_link:296' t451 = t150.*3.84e-1;
    // 'Hessian_final_link:297' t483 = -t478;
    // 'Hessian_final_link:298' t517 = -t513;
    // 'Hessian_final_link:299' t571 = t31+t45+t50+t55+2.295845021658468e-49;
    t571 = (((-t26 + t3 * 6.123233995736766E-17) - t46) + absx) +
           2.2958450216584679E-49;
    // 'Hessian_final_link:300' t572 = -t568;
    // 'Hessian_final_link:301' t573 = -t569;
    // 'Hessian_final_link:302' t574 = -t570;
    // 'Hessian_final_link:303' t85 = t14+t71;
    t85 = t14 - t63;
    // 'Hessian_final_link:304' t86 = t16+t73;
    t = t16 - t26 * 6.123233995736766E-17;
    // 'Hessian_final_link:305' t88 = t4.*t82;
    t88 = t4 * t82;
    // 'Hessian_final_link:306' t89 = t4.*t83;
    t89 = t4 * t83;
    // 'Hessian_final_link:307' t90 = t5.*t83;
    t90 = t5 * t83;
    // 'Hessian_final_link:308' t91 = t10.*t82;
    t91 = t10 * t82;
    // 'Hessian_final_link:309' t92 = t10.*t83;
    t92 = t10 * t83;
    // 'Hessian_final_link:310' t93 = t11.*t83;
    t93 = t11 * t83;
    // 'Hessian_final_link:311' t100 = t4.*t87;
    t100 = t4 * t87;
    // 'Hessian_final_link:312' t102 = t10.*t87;
    t102 = t10 * t87;
    // 'Hessian_final_link:313' t154 = -t152;
    // 'Hessian_final_link:314' t155 = t46.*t82;
    // 'Hessian_final_link:315' t156 = t46.*t83;
    t156 = t46 * t83;
    // 'Hessian_final_link:316' t158 = t5.*t84.*6.123233995736766e-17;
    t158_tmp = t5 * t84;
    t158 = t158_tmp * 6.123233995736766E-17;
    // 'Hessian_final_link:317' t159 = t49.*t82;
    // 'Hessian_final_link:318' t160 = t49.*t83;
    // 'Hessian_final_link:319' t161 = t48+t84;
    t161 = t48 + t84;
    // 'Hessian_final_link:320' t162 = t11.*t84.*6.123233995736766e-17;
    // 'Hessian_final_link:321' t163 = t24+t44+t66;
    t163 = (t24 + t44) - t53;
    // 'Hessian_final_link:322' t166 = t26+t46+t68;
    absx = (t26 + t46) - absx;
    // 'Hessian_final_link:323' t168 = t51+t82;
    t168 = -(t8 * 6.123233995736766E-17) + t82;
    // 'Hessian_final_link:324' t175 = t46.*t87;
    // 'Hessian_final_link:325' t176 = t49.*t87;
    t176 = t49 * t87;
    // 'Hessian_final_link:326' t185 = t5.*t172;
    t185 = t5 * t172;
    // 'Hessian_final_link:327' t187 = t6.*t172;
    t187 = t6 * t172;
    // 'Hessian_final_link:328' t189 = t11.*t172;
    t189 = t11 * t172;
    // 'Hessian_final_link:329' t190 = t12.*t172;
    // 'Hessian_final_link:330' t191 = t74.*t82;
    // 'Hessian_final_link:331' t192 = t75.*t82;
    // 'Hessian_final_link:332' t193 = t75.*t83;
    // 'Hessian_final_link:333' t194 = t11.*t84.*5.051668046482832e-18;
    // 'Hessian_final_link:334' t202 = t74.*t87;
    // 'Hessian_final_link:335' t204 = t75.*t87;
    // 'Hessian_final_link:336' t251 = -t250;
    // 'Hessian_final_link:337' t254 = t5.*t84.*3.749399456654644e-33;
    t254 = t158_tmp * 3.749399456654644E-33;
    // 'Hessian_final_link:338' t377 = -t376;
    // 'Hessian_final_link:339' t382 = t5.*t84.*2.45655417317456e-50;
    // 'Hessian_final_link:340' t482 = t5.*t84.*2.351321854362918e-17;
    // 'Hessian_final_link:341' t575 = t6.*t571;
    t575 = t6 * t571;
    // 'Hessian_final_link:342' t576 = t12.*t571;
    t576 = t12 * t571;
    // 'Hessian_final_link:343' t94 = t4.*t85;
    t94 = t4 * t85;
    // 'Hessian_final_link:344' t95 = t5.*t85;
    t95 = t5 * t85;
    // 'Hessian_final_link:345' t96 = t5.*t86;
    t96 = t5 * t;
    // 'Hessian_final_link:346' t97 = t10.*t85;
    t97 = t10 * t85;
    // 'Hessian_final_link:347' t98 = t11.*t85;
    t98 = t11 * t85;
    // 'Hessian_final_link:348' t99 = t11.*t86;
    t99 = t11 * t;
    // 'Hessian_final_link:349' t101 = -t90;
    // 'Hessian_final_link:350' t103 = -t91;
    // 'Hessian_final_link:351' t104 = -t92;
    // 'Hessian_final_link:352' t108 = t88.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:353' t109 = t89.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:354' t110 = t92.*(3.3e+1./4.0e+2);
    t = t92 * 0.0825;
    // 'Hessian_final_link:355' t111 = t93.*(3.3e+1./4.0e+2);
    t111 = t93 * 0.0825;
    // 'Hessian_final_link:356' t118 = t100.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:357' t157 = t90.*6.123233995736766e-17;
    // 'Hessian_final_link:358' t164 = t46.*t85;
    t164 = t46 * t85;
    // 'Hessian_final_link:359' t167 = -t158;
    // 'Hessian_final_link:360' t169 = t49.*t85;
    t169 = t49 * t85;
    // 'Hessian_final_link:361' t170 = t91.*(-6.123233995736766e-17);
    // 'Hessian_final_link:362' t171 = t92.*(-6.123233995736766e-17);
    // 'Hessian_final_link:363' t174 = -t162;
    // 'Hessian_final_link:364' t178 = t100.*(-6.123233995736766e-17);
    // 'Hessian_final_link:365' t179 = t6.*t161;
    t179 = t6 * t161;
    // 'Hessian_final_link:366' t180 = t12.*t161;
    // 'Hessian_final_link:367' t181 = t5.*t163;
    t181 = t5 * t163;
    // 'Hessian_final_link:368' t182 = t5.*t166;
    t182 = t5 * absx;
    // 'Hessian_final_link:369' t183 = t5.*t168;
    t183 = t5 * t168;
    // 'Hessian_final_link:370' t184 = t11.*t163;
    t184 = t11 * t163;
    // 'Hessian_final_link:371' t186 = t11.*t166;
    t186 = t11 * absx;
    // 'Hessian_final_link:372' t188 = t11.*t168;
    t188 = t11 * t168;
    // 'Hessian_final_link:373' t195 = t88.*(-5.051668046482832e-18);
    // 'Hessian_final_link:374' t197 = t91.*(-5.051668046482832e-18);
    // 'Hessian_final_link:375' t198 = t92.*(-5.051668046482832e-18);
    // 'Hessian_final_link:376' t199 = -t185;
    // 'Hessian_final_link:377' t205 = t100.*(-5.051668046482832e-18);
    // 'Hessian_final_link:378' t210 = t185.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:379' t212 = t189.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:380' t252 = t88.*3.749399456654644e-33;
    // 'Hessian_final_link:381' t253 = t89.*3.749399456654644e-33;
    // 'Hessian_final_link:382' t255 = t91.*3.749399456654644e-33;
    // 'Hessian_final_link:383' t256 = t92.*3.749399456654644e-33;
    // 'Hessian_final_link:384' t258 = -t254;
    // 'Hessian_final_link:385' t261 = t90.*4.011857418620469e-34;
    // 'Hessian_final_link:386' t263 = t100.*3.749399456654644e-33;
    t263 = t100 * 3.749399456654644E-33;
    // 'Hessian_final_link:387' t264 = t102.*3.749399456654644e-33;
    // 'Hessian_final_link:388' t274 = t185.*6.123233995736766e-17;
    t274 = t185 * 6.123233995736766E-17;
    // 'Hessian_final_link:389' t277 = t189.*6.123233995736766e-17;
    // 'Hessian_final_link:390' t282 = t187.*6.55186037543834e-18;
    t282 = t187 * 6.55186037543834E-18;
    // 'Hessian_final_link:391' t286 = t189.*5.051668046482832e-18;
    // 'Hessian_final_link:392' t290 = t102+t156;
    t290 = t102 + t156;
    // 'Hessian_final_link:393' t303 = t5.*(t100+t52.*t83);
    absx = t100 + -t49 * t83;
    t303 = t5 * absx;
    // 'Hessian_final_link:394' t304 = t11.*(t100+t52.*t83);
    absx *= t11;
    // 'Hessian_final_link:395' t306 = t19+t89+t176;
    scale = (t19 + t89) + t176;
    // 'Hessian_final_link:396' t366 = t90.*3.84e-1;
    // 'Hessian_final_link:397' t378 = t88.*2.295845021658468e-49;
    // 'Hessian_final_link:398' t379 = t89.*2.295845021658468e-49;
    // 'Hessian_final_link:399' t380 = t91.*2.295845021658468e-49;
    t380 = t91 * 2.2958450216584679E-49;
    // 'Hessian_final_link:400' t381 = t89.*2.45655417317456e-50;
    // 'Hessian_final_link:401' t384 = t91.*2.45655417317456e-50;
    // 'Hessian_final_link:402' t385 = t92.*2.45655417317456e-50;
    // 'Hessian_final_link:403' t388 = -t382;
    // 'Hessian_final_link:404' t389 = t100.*2.295845021658468e-49;
    t389 = t100 * 2.2958450216584679E-49;
    // 'Hessian_final_link:405' t391 = t102.*2.295845021658468e-49;
    // 'Hessian_final_link:406' t393 = t102.*2.45655417317456e-50;
    // 'Hessian_final_link:407' t395 = t185.*3.749399456654644e-33;
    t395 = t185 * 3.749399456654644E-33;
    // 'Hessian_final_link:408' t402 = t185.*4.011857418620469e-34;
    // 'Hessian_final_link:409' t405 = t189.*4.011857418620469e-34;
    // 'Hessian_final_link:410' t431 =
    // t5.*(t91+t50.*t85).*6.123233995736766e-17; 'Hessian_final_link:411' t440
    // = t11.*(t91+t50.*t85).*(-5.051668046482832e-18); 'Hessian_final_link:412'
    // t463 = t89.*2.351321854362918e-17; 'Hessian_final_link:413' t465 =
    // t91.*2.351321854362918e-17; 'Hessian_final_link:414' t466 =
    // t92.*2.351321854362918e-17; 'Hessian_final_link:415' t477 =
    // t102.*2.351321854362918e-17; 'Hessian_final_link:416' t484 = -t482;
    // 'Hessian_final_link:417' t488 = t185.*3.84e-1;
    // 'Hessian_final_link:418' t491 = t189.*3.84e-1;
    // 'Hessian_final_link:419' t493 = t91.*1.504205602555149e-66;
    // 'Hessian_final_link:420' t494 = t88.*1.504205602555149e-66;
    // 'Hessian_final_link:421' t495 = t89.*1.504205602555149e-66;
    // 'Hessian_final_link:422' t498 = t102.*1.504205602555149e-66;
    // 'Hessian_final_link:423' t500 = t100.*1.504205602555149e-66;
    // 'Hessian_final_link:424' t503 = t185.*2.45655417317456e-50;
    // 'Hessian_final_link:425' t523 =
    // t5.*(t91+t50.*t85).*3.749399456654644e-33; 'Hessian_final_link:426' et63
    // = t91; 'Hessian_final_link:427' et64 = 1.439769391355383e-33;
    // 'Hessian_final_link:428' t550 = et63.*et64;
    // 'Hessian_final_link:429' et65 = t88;
    // 'Hessian_final_link:430' et66 = 1.439769391355383e-33;
    // 'Hessian_final_link:431' t552 = et65.*et66;
    // 'Hessian_final_link:432' et67 = t89;
    // 'Hessian_final_link:433' et68 = 1.439769391355383e-33;
    // 'Hessian_final_link:434' t553 = et67.*et68;
    // 'Hessian_final_link:435' et69 = t102;
    // 'Hessian_final_link:436' et70 = 1.439769391355383e-33;
    // 'Hessian_final_link:437' t557 = et69.*et70;
    // 'Hessian_final_link:438' et71 = t100;
    // 'Hessian_final_link:439' et72 = 1.439769391355383e-33;
    // 'Hessian_final_link:440' t559 = et71.*et72;
    // 'Hessian_final_link:441' t563 = t185.*2.351321854362918e-17;
    // 'Hessian_final_link:442' t579 = -t576;
    // 'Hessian_final_link:443' t617 = t575.*6.55186037543834e-18;
    // 'Hessian_final_link:444' t618 = t576.*6.55186037543834e-18;
    // 'Hessian_final_link:445' t105 = -t94;
    // 'Hessian_final_link:446' t107 = -t99;
    // 'Hessian_final_link:447' t112 = t94.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:448' t113 = t96.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:449' t114 = t97.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:450' t115 = -t110;
    // 'Hessian_final_link:451' t116 = t98.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:452' t117 = -t111;
    // 'Hessian_final_link:453' t165 = t95.*6.123233995736766e-17;
    // 'Hessian_final_link:454' t173 = t99.*6.123233995736766e-17;
    // 'Hessian_final_link:455' t200 = -t186;
    // 'Hessian_final_link:456' t201 = -t188;
    // 'Hessian_final_link:457' t203 = t97.*(-5.051668046482832e-18);
    // 'Hessian_final_link:458' t206 = t181.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:459' t207 = t182.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:460' t208 = t183.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:461' t209 = t184.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:462' t211 = t188.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:463' t257 = t94.*3.749399456654644e-33;
    // 'Hessian_final_link:464' t259 = t97.*3.749399456654644e-33;
    // 'Hessian_final_link:465' t260 = -t255;
    // 'Hessian_final_link:466' t265 = t95.*4.011857418620469e-34;
    // 'Hessian_final_link:467' t266 = t99.*4.011857418620469e-34;
    // 'Hessian_final_link:468' t267 = -t263;
    // 'Hessian_final_link:469' t270 = t181.*6.123233995736766e-17;
    // 'Hessian_final_link:470' t271 = t182.*6.123233995736766e-17;
    // 'Hessian_final_link:471' t272 = t183.*6.123233995736766e-17;
    // 'Hessian_final_link:472' t273 = t184.*6.123233995736766e-17;
    // 'Hessian_final_link:473' t275 = t186.*6.123233995736766e-17;
    // 'Hessian_final_link:474' t276 = t188.*6.123233995736766e-17;
    // 'Hessian_final_link:475' t278 = t179.*6.55186037543834e-18;
    // 'Hessian_final_link:476' t279 = -t274;
    // 'Hessian_final_link:477' t283 = -t282;
    // 'Hessian_final_link:478' t284 = t182.*5.051668046482832e-18;
    // 'Hessian_final_link:479' t285 = t186.*5.051668046482832e-18;
    // 'Hessian_final_link:480' t289 = t88+t169;
    // 'Hessian_final_link:481' t292 = t103+t164;
    // 'Hessian_final_link:482' t300 = t20+t97+t155;
    // 'Hessian_final_link:483' t309 = t27+t96+t174;
    // 'Hessian_final_link:484' t310 = t29+t99+t158;
    // 'Hessian_final_link:485' t312 = -t304;
    // 'Hessian_final_link:486' t315 = t15+t104+t175;
    // 'Hessian_final_link:487' t316 = t5.*t306;
    // 'Hessian_final_link:488' t317 = t6.*t306;
    // 'Hessian_final_link:489' t318 = t11.*t306;
    // 'Hessian_final_link:490' t319 = t12.*t306;
    // 'Hessian_final_link:491' t331 = t303.*(3.3e+1./4.0e+2);
    // 'Hessian_final_link:492' t370 = t95.*3.84e-1;
    // 'Hessian_final_link:493' t371 = t99.*3.84e-1;
    // 'Hessian_final_link:494' t383 = t94.*2.295845021658468e-49;
    // 'Hessian_final_link:495' t386 = -t380;
    // 'Hessian_final_link:496' t387 = t94.*2.45655417317456e-50;
    // 'Hessian_final_link:497' t390 = t97.*2.45655417317456e-50;
    // 'Hessian_final_link:498' t392 = -t389;
    // 'Hessian_final_link:499' t394 = t182.*3.749399456654644e-33;
    // 'Hessian_final_link:500' t396 = t186.*3.749399456654644e-33;
    // 'Hessian_final_link:501' t397 = -t395;
    // 'Hessian_final_link:502' t399 = t181.*4.011857418620469e-34;
    // 'Hessian_final_link:503' t400 = t183.*4.011857418620469e-34;
    // 'Hessian_final_link:504' t401 = t184.*4.011857418620469e-34;
    // 'Hessian_final_link:505' t403 = t186.*4.011857418620469e-34;
    // 'Hessian_final_link:506' t404 = t188.*4.011857418620469e-34;
    // 'Hessian_final_link:507' t423 = t5.*t290.*6.123233995736766e-17;
    // 'Hessian_final_link:508' Hessian =
    // ft_1({t10,t100,t101,t102,t105,t107,t108,t109,t11,t110,t111,t112,t113,t114,t115,t116,t117,t118,t12,t120,t121,t123,t125,t126,t127,t128,t129,t13,t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t14,t140,t142,t143,t144,t145,t146,t147,t148,t149,t15,t150,t151,t154,t156,t157,t159,t160,t161,t163,t164,t165,t167,t168,t169,t17,t170,t171,t172,t173,t176,t178,t179,t18,t180,t181,t182,t183,t184,t185,t186,t187,t188,t189,t19,t190,t191,t192,t193,t194,t195,t197,t198,t199,t2,t20,t200,t201,t202,t203,t204,t205,t206,t207,t208,t209,t21,t210,t211,t212,t217,t218,t219,t22,t222,t223,t224,t225,t226,t227,t228,t229,t23,t230,t231,t232,t233,t235,t236,t237,t238,t24,t240,t241,t242,t244,t245,t246,t248,t249,t25,t251,t252,t253,t254,t256,t257,t258,t259,t260,t261,t263,t264,t265,t266,t267,t270,t271,t272,t273,t274,t275,t276,t277,t278,t279,t28,t282,t283,t284,t285,t286,t289,t290,t292,t295,t296,t297,t298,t299,t300,t301,t303,t304,t305,t306,t307,t308,t309,t310,t312,t315,t316,t317,t318,t319,t32,t33,t331,t34,t340,t346,t347,t348,t349,t35,t353,t354,t356,t357,t358,t360,t361,t362,t364,t365,t366,t367,t368,t369,t37,t370,t371,t375,t377,t378,t379,t38,t380,t381,t383,t384,t385,t386,t387,t388,t389,t390,t391,t392,t393,t394,t395,t396,t397,t399,t40,t400,t401,t402,t403,t404,t405,t409,t41,t411,t413,t414,t415,t416,t417,t418,t419,t421,t422,t423,t43,t431,t44,t440,t450,t451,t463,t465,t466,t469,t476,t477,t479,t48,t480,t481,t483,t484,t488,t49,t491,t493,t494,t495,t498,t5,t50,t500,t503,t506,t507,t508,t509,t510,t512,t514,t515,t516,t517,t523,t53,t54,t550,t552,t553,t557,t559,t56,t563,t567,t568,t57,t571,t572,t573,t574,t575,t576,t579,t58,t59,t6,t60,t61,t617,t618,t62,t63,t66,t67,t69,t7,t70,t71,t72,t73,t75,t76,t77,t8,t80,t81,t83,t84,t85,t87,t88,t89,t9,t90,t91,t92,t93,t94,t95,t97,t98,t99});
    b_t10[0] = t10;
    b_t10[1] = t100;
    b_t10[2] = -t90;
    b_t10[3] = t102;
    b_t10[4] = -t94;
    b_t10[5] = -t99;
    b_t10[6] = t88 * 0.0825;
    b_t10[7] = t89 * 0.0825;
    b_t10[8] = t11;
    b_t10[9] = t;
    b_t10[10] = t111;
    b_t10[11] = t94 * 0.0825;
    b_t10[12] = t96 * 0.0825;
    b_t10[13] = t97 * 0.0825;
    b_t10[14] = -t;
    b_t10[15] = t98 * 0.0825;
    b_t10[16] = -t111;
    b_t10[17] = t100 * 0.0825;
    b_t10[18] = t12;
    b_t10[19] = t2 * 3.749399456654644E-33;
    b_t10[20] = t3 * 3.749399456654644E-33;
    b_t10[21] = t3 + 3.749399456654644E-33;
    b_t10[22] = t9 * 3.749399456654644E-33;
    b_t10[23] = t126;
    b_t10[24] = -(t4 * 3.749399456654644E-33);
    b_t10[25] = -(t8 * 3.749399456654644E-33);
    b_t10[26] = -t126;
    b_t10[27] = std::sin(joint_values[5]);
    b_t10[28] = t130;
    b_t10[29] = t131;
    b_t10[30] = t16 * 3.749399456654644E-33;
    b_t10[31] = t17 * 3.749399456654644E-33;
    b_t10[32] = t18 * 3.749399456654644E-33;
    b_t10[33] = t19 * 3.749399456654644E-33;
    b_t10[34] = t20 * 3.749399456654644E-33;
    b_t10[35] = t137;
    b_t10[36] = t138;
    b_t10[37] = t139;
    b_t10[38] = t14;
    b_t10[39] = t25 * 3.749399456654644E-33;
    b_t10[40] = -t130;
    b_t10[41] = -t131;
    b_t10[42] = -t137;
    b_t10[43] = -t138;
    b_t10[44] = -t139;
    b_t10[45] = -(t26 * 3.749399456654644E-33);
    b_t10[46] = t23 * 4.011857418620469E-34;
    b_t10[47] = t149;
    b_t10[48] = t15;
    b_t10[49] = t150;
    b_t10[50] = t149 * 0.0825;
    b_t10[51] = -(t150 * 0.0825);
    b_t10[52] = t156;
    b_t10[53] = t90 * 6.123233995736766E-17;
    b_t10[54] = t49 * t82;
    b_t10[55] = t49 * t83;
    b_t10[56] = t161;
    b_t10[57] = t163;
    b_t10[58] = t164;
    b_t10[59] = t95 * 6.123233995736766E-17;
    b_t10[60] = -t158;
    b_t10[61] = t168;
    b_t10[62] = t169;
    b_t10[63] = t17;
    b_t10[64] = t91 * -6.123233995736766E-17;
    b_t10[65] = t92 * -6.123233995736766E-17;
    b_t10[66] = t172;
    b_t10[67] = t99 * 6.123233995736766E-17;
    b_t10[68] = t176;
    b_t10[69] = t100 * -6.123233995736766E-17;
    b_t10[70] = t179;
    b_t10[71] = t18;
    b_t10[72] = t12 * t161;
    b_t10[73] = t181;
    b_t10[74] = t182;
    b_t10[75] = t183;
    b_t10[76] = t184;
    b_t10[77] = t185;
    b_t10[78] = t186;
    b_t10[79] = t187;
    b_t10[80] = t188;
    b_t10[81] = t189;
    b_t10[82] = t19;
    b_t10[83] = t12 * t172;
    b_t10[84] = t74 * t82;
    b_t10[85] = t75 * t82;
    b_t10[86] = t75 * t83;
    t = t11 * t84;
    b_t10[87] = t * 5.0516680464828323E-18;
    b_t10[88] = t88 * -5.0516680464828323E-18;
    b_t10[89] = t91 * -5.0516680464828323E-18;
    b_t10[90] = t92 * -5.0516680464828323E-18;
    b_t10[91] = -t185;
    b_t10[92] = t2;
    b_t10[93] = t20;
    b_t10[94] = -t186;
    b_t10[95] = -t188;
    b_t10[96] = t74 * t87;
    b_t10[97] = t97 * -5.0516680464828323E-18;
    b_t10[98] = t75 * t87;
    b_t10[99] = t100 * -5.0516680464828323E-18;
    b_t10[100] = t181 * 0.0825;
    b_t10[101] = t182 * 0.0825;
    b_t10[102] = t183 * 0.0825;
    b_t10[103] = t184 * 0.0825;
    b_t10[104] = t21;
    b_t10[105] = t185 * 0.0825;
    b_t10[106] = t188 * 0.0825;
    b_t10[107] = t189 * 0.0825;
    b_t10[108] = t23 * 0.384;
    b_t10[109] = t2 * 2.2958450216584679E-49;
    b_t10[110] = t3 * 2.2958450216584679E-49;
    b_t10[111] = t22;
    b_t10[112] = t9 * 2.2958450216584679E-49;
    b_t10[113] = t223;
    b_t10[114] = -(t4 * 2.2958450216584679E-49);
    b_t10[115] = -(t8 * 2.2958450216584679E-49);
    b_t10[116] = -t223;
    b_t10[117] = t227;
    b_t10[118] = t16 * 2.2958450216584679E-49;
    b_t10[119] = t17 * 2.2958450216584679E-49;
    b_t10[120] = t23;
    b_t10[121] = t18 * 2.2958450216584679E-49;
    b_t10[122] = t231;
    b_t10[123] = t232;
    b_t10[124] = t233;
    b_t10[125] = -t227;
    b_t10[126] = t19 * 2.45655417317456E-50;
    b_t10[127] = t20 * 2.45655417317456E-50;
    b_t10[128] = t21 * 2.45655417317456E-50;
    b_t10[129] = t24;
    b_t10[130] = -t231;
    b_t10[131] = -t232;
    b_t10[132] = t25 * 2.45655417317456E-50;
    b_t10[133] = -t233;
    b_t10[134] = -(t15 * 2.45655417317456E-50);
    b_t10[135] = -(t22 * 2.45655417317456E-50);
    b_t10[136] = -(t26 * 2.45655417317456E-50);
    b_t10[137] = t149 * 6.123233995736766E-17;
    b_t10[138] = t25;
    b_t10[139] = -(t150 * 6.123233995736766E-17);
    b_t10[140] = t88 * 3.749399456654644E-33;
    b_t10[141] = t89 * 3.749399456654644E-33;
    b_t10[142] = t254;
    b_t10[143] = t92 * 3.749399456654644E-33;
    b_t10[144] = t94 * 3.749399456654644E-33;
    b_t10[145] = -t254;
    b_t10[146] = t97 * 3.749399456654644E-33;
    b_t10[147] = -(t91 * 3.749399456654644E-33);
    b_t10[148] = t90 * 4.011857418620469E-34;
    b_t10[149] = t263;
    b_t10[150] = t102 * 3.749399456654644E-33;
    b_t10[151] = t95 * 4.011857418620469E-34;
    b_t10[152] = t99 * 4.011857418620469E-34;
    b_t10[153] = -t263;
    b_t10[154] = t181 * 6.123233995736766E-17;
    b_t10[155] = t182 * 6.123233995736766E-17;
    b_t10[156] = t183 * 6.123233995736766E-17;
    b_t10[157] = t184 * 6.123233995736766E-17;
    b_t10[158] = t274;
    b_t10[159] = t186 * 6.123233995736766E-17;
    b_t10[160] = t188 * 6.123233995736766E-17;
    b_t10[161] = t189 * 6.123233995736766E-17;
    b_t10[162] = t179 * 6.55186037543834E-18;
    b_t10[163] = -t274;
    b_t10[164] = -t15;
    b_t10[165] = t282;
    b_t10[166] = -t282;
    b_t10[167] = t182 * 5.0516680464828323E-18;
    b_t10[168] = t186 * 5.0516680464828323E-18;
    b_t10[169] = t189 * 5.0516680464828323E-18;
    b_t10[170] = t88 + t169;
    b_t10[171] = t290;
    b_t10[172] = -t91 + t164;
    b_t10[173] = t295;
    b_t10[174] = t19 * 2.3513218543629179E-17;
    b_t10[175] = t20 * 2.3513218543629179E-17;
    b_t10[176] = t21 * 2.3513218543629179E-17;
    b_t10[177] = t22 * 2.3513218543629179E-17;
    b_t10[178] = (t20 + t97) + t46 * t82;
    b_t10[179] = t301;
    b_t10[180] = t303;
    b_t10[181] = absx;
    b_t10[182] = -t295;
    b_t10[183] = scale;
    b_t10[184] = -t301;
    b_t10[185] = -(t26 * 2.3513218543629179E-17);
    b_t10[186] = (t27 + t96) - t * 6.123233995736766E-17;
    b_t10[187] = (-t23 + t99) + t158;
    b_t10[188] = -absx;
    b_t10[189] = (t15 - t92) + t46 * t87;
    b_t10[190] = t5 * scale;
    b_t10[191] = t6 * scale;
    b_t10[192] = t11 * scale;
    b_t10[193] = t12 * scale;
    b_t10[194] = t15 * 0.0825;
    b_t10[195] = t16 * 0.0825;
    b_t10[196] = t303 * 0.0825;
    b_t10[197] = t19 * 0.0825;
    b_t10[198] = t2 * 1.405799628556214E-65;
    b_t10[199] = -(t8 * 1.405799628556214E-65);
    b_t10[200] = t347;
    b_t10[201] = t17 * 1.405799628556214E-65;
    b_t10[202] = t18 * 1.405799628556214E-65;
    b_t10[203] = t20 * 0.0825;
    b_t10[204] = -t347;
    b_t10[205] = t3 * 1.504205602555149E-66;
    b_t10[206] = -(t24 * 1.405799628556214E-65);
    b_t10[207] = t9 * 1.504205602555149E-66;
    b_t10[208] = t10 * 1.504205602555149E-66;
    b_t10[209] = -(t4 * 1.504205602555149E-66);
    b_t10[210] = t17 * 1.504205602555149E-66;
    b_t10[211] = t18 * 1.504205602555149E-66;
    b_t10[212] = t22 * 1.504205602555149E-66;
    b_t10[213] = t24 * 1.504205602555149E-66;
    b_t10[214] = t90 * 0.384;
    b_t10[215] = t14 * 1.504205602555149E-66;
    b_t10[216] = t16 * 1.504205602555149E-66;
    b_t10[217] = -(t21 * 1.504205602555149E-66);
    b_t10[218] = t37;
    b_t10[219] = t95 * 0.384;
    b_t10[220] = t99 * 0.384;
    b_t10[221] = t149 * 4.011857418620469E-34;
    b_t10[222] = -(t150 * 4.011857418620469E-34);
    b_t10[223] = t88 * 2.2958450216584679E-49;
    b_t10[224] = t89 * 2.2958450216584679E-49;
    b_t10[225] = t26 * 0.0825;
    b_t10[226] = t380;
    b_t10[227] = t89 * 2.45655417317456E-50;
    b_t10[228] = t94 * 2.2958450216584679E-49;
    b_t10[229] = t91 * 2.45655417317456E-50;
    b_t10[230] = t92 * 2.45655417317456E-50;
    b_t10[231] = -t380;
    b_t10[232] = t94 * 2.45655417317456E-50;
    b_t10[233] = -(t158_tmp * 2.45655417317456E-50);
    b_t10[234] = t389;
    b_t10[235] = t97 * 2.45655417317456E-50;
    b_t10[236] = t102 * 2.2958450216584679E-49;
    b_t10[237] = -t389;
    b_t10[238] = t102 * 2.45655417317456E-50;
    b_t10[239] = t182 * 3.749399456654644E-33;
    b_t10[240] = t395;
    b_t10[241] = t186 * 3.749399456654644E-33;
    b_t10[242] = -t395;
    b_t10[243] = t181 * 4.011857418620469E-34;
    b_t10[244] = -(t22 * 0.0825);
    b_t10[245] = t183 * 4.011857418620469E-34;
    b_t10[246] = t184 * 4.011857418620469E-34;
    b_t10[247] = t185 * 4.011857418620469E-34;
    b_t10[248] = t186 * 4.011857418620469E-34;
    b_t10[249] = t188 * 4.011857418620469E-34;
    b_t10[250] = t189 * 4.011857418620469E-34;
    b_t10[251] = t3 * 1.439769391355383E-33;
    b_t10[252] = -t37;
    b_t10[253] = t9 * 1.439769391355383E-33;
    b_t10[254] = -(t4 * 1.439769391355383E-33);
    b_t10[255] = -(t10 * 1.439769391355383E-33);
    b_t10[256] = t17 * 1.439769391355383E-33;
    b_t10[257] = t18 * 1.439769391355383E-33;
    b_t10[258] = t21 * 1.439769391355383E-33;
    b_t10[259] = t22 * 1.439769391355383E-33;
    b_t10[260] = t24 * 1.439769391355383E-33;
    b_t10[261] = t14 * 1.439769391355383E-33;
    b_t10[262] = t16 * 1.439769391355383E-33;
    b_t10[263] = t5 * t290 * 6.123233995736766E-17;
    b_t10[264] = -(t27 * 0.0825);
    t = t91 + -t46 * t85;
    absx = t5 * t;
    b_t10[265] = absx * 6.123233995736766E-17;
    b_t10[266] = t44;
    b_t10[267] = t11 * t * -5.0516680464828323E-18;
    b_t10[268] = t149 * 0.384;
    b_t10[269] = t150 * 0.384;
    b_t10[270] = t89 * 2.3513218543629179E-17;
    b_t10[271] = t91 * 2.3513218543629179E-17;
    b_t10[272] = t92 * 2.3513218543629179E-17;
    b_t10[273] = t2 * 9.2106028821433953E-83;
    b_t10[274] = -(t8 * 9.2106028821433953E-83);
    b_t10[275] = t102 * 2.3513218543629179E-17;
    b_t10[276] = t17 * 9.2106028821433953E-83;
    b_t10[277] = t48;
    b_t10[278] = t18 * 9.2106028821433953E-83;
    b_t10[279] = t24 * 9.2106028821433953E-83;
    b_t10[280] = -(t14 * 9.2106028821433953E-83);
    b_t10[281] = -(t158_tmp * 2.3513218543629179E-17);
    b_t10[282] = t185 * 0.384;
    b_t10[283] = t49;
    b_t10[284] = t189 * 0.384;
    b_t10[285] = t91 * 1.504205602555149E-66;
    b_t10[286] = t88 * 1.504205602555149E-66;
    b_t10[287] = t89 * 1.504205602555149E-66;
    b_t10[288] = t102 * 1.504205602555149E-66;
    b_t10[289] = t5;
    b_t10[290] = -t46;
    b_t10[291] = t100 * 1.504205602555149E-66;
    b_t10[292] = t185 * 2.45655417317456E-50;
    b_t10[293] = t3 * 0.316;
    b_t10[294] = t9 * 0.316;
    b_t10[295] = t14 * 0.316;
    b_t10[296] = t18 * 0.316;
    b_t10[297] = t2 * 8.8160448831685167E-50;
    b_t10[298] = -(t8 * 8.8160448831685167E-50);
    b_t10[299] = t17 * 8.8160448831685167E-50;
    b_t10[300] = t18 * 8.8160448831685167E-50;
    b_t10[301] = t24 * 8.8160448831685167E-50;
    b_t10[302] = -(t14 * 8.8160448831685167E-50);
    b_t10[303] = absx * 3.749399456654644E-33;
    b_t10[304] = t53;
    b_t10[305] = t54;
    b_t10[306] = t91 * 1.439769391355383E-33;
    b_t10[307] = t88 * 1.439769391355383E-33;
    b_t10[308] = t89 * 1.439769391355383E-33;
    b_t10[309] = t102 * 1.439769391355383E-33;
    b_t10[310] = t100 * 1.439769391355383E-33;
    b_t10[311] = t56;
    b_t10[312] = t185 * 2.3513218543629179E-17;
    b_t10[313] = t14 * 1.9349419426528181E-17;
    b_t10[314] = t568;
    b_t10[315] = t57;
    b_t10[316] = t571;
    b_t10[317] = -t568;
    b_t10[318] = -(t18 * 1.9349419426528181E-17);
    b_t10[319] = -(t24 * 1.9349419426528181E-17);
    b_t10[320] = t575;
    b_t10[321] = t576;
    b_t10[322] = -t576;
    b_t10[323] = t19 * 6.123233995736766E-17;
    b_t10[324] = t20 * 6.123233995736766E-17;
    b_t10[325] = t6;
    b_t10[326] = t60;
    b_t10[327] = t61;
    b_t10[328] = t575 * 6.55186037543834E-18;
    b_t10[329] = t576 * 6.55186037543834E-18;
    b_t10[330] = t23 * 6.123233995736766E-17;
    b_t10[331] = t63;
    b_t10[332] = -t53;
    b_t10[333] = -t54;
    b_t10[334] = -t60;
    b_t10[335] = std::cos(joint_values[5]);
    b_t10[336] = -t61;
    b_t10[337] = -t63;
    b_t10[338] = -(t25 * 6.123233995736766E-17);
    b_t10[339] = -(t26 * 6.123233995736766E-17);
    b_t10[340] = t75;
    b_t10[341] = -t74;
    b_t10[342] = t16 * 5.0516680464828323E-18;
    b_t10[343] = t8;
    b_t10[344] = -(t21 * 5.0516680464828323E-18);
    b_t10[345] = -(t26 * 5.0516680464828323E-18);
    b_t10[346] = t83;
    b_t10[347] = t84;
    b_t10[348] = t85;
    b_t10[349] = t87;
    b_t10[350] = t88;
    b_t10[351] = t89;
    b_t10[352] = t9;
    b_t10[353] = t90;
    b_t10[354] = t91;
    b_t10[355] = t92;
    b_t10[356] = t93;
    b_t10[357] = t94;
    b_t10[358] = t95;
    b_t10[359] = t97;
    b_t10[360] = t98;
    b_t10[361] = t99;
    ft_1(b_t10, H);
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
            inv_JJt[pipk] -= inv_JJt[i] * JJt[(b_i + 6 * (j - 1)) - 1];
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
          inv_JJt[i] = absx / JJt[k + aoffset];
          for (int b_i{0}; b_i < k; b_i++) {
            inv_JJt_tmp = b_i + pipk;
            inv_JJt[inv_JJt_tmp] -= inv_JJt[i] * JJt[b_i + aoffset];
          }
        }
      }
    }
    //
    //  Pre-compute the pseudo-inverse of JJt
    // pseudo_inv_JJt = pinv(JJt);
    // 'computeManipulabilityJacobian:26' for i = 1:n
    scale = 0.0;
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
      //  Equation (11)
      scale += -(t * absx) * current_joint_velocity[b_i];
    }
    objVal += config_weightManipulability * scale;
  }
  //  You can add more terms similarly with their checks and weights
  return objVal;
}

//
// File trailer for configurableObjective.cpp
//
// [EOF]
//
