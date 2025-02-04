//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

/*************************************************************************/
/* This automatically generated example C++ main file shows how to call  */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

// Include Files
#include "main.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void argInit_6x1_real_T(double result[6]);

static coder::array<double, 2U> argInit_6xUnbounded_real_T();

static coder::array<double, 1U> argInit_Unboundedx1_real_T();

static coder::array<double, 2U> argInit_UnboundedxUnbounded_real_T();

static boolean_T argInit_boolean_T();

static double argInit_real_T();

static void argInit_struct0_T(struct0_T &result);

static void j_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

// Function Definitions
//
// Arguments    : double result[6]
// Return Type  : void
//
static void argInit_6x1_real_T(double result[6])
{
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 6; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

//
// Arguments    : void
// Return Type  : coder::array<double, 2U>
//
static coder::array<double, 2U> argInit_6xUnbounded_real_T()
{
  coder::array<double, 2U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(6, 2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 6; idx0++) {
    for (int idx1{0}; idx1 < result.size(1); idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx0 + 6 * idx1] = argInit_real_T();
    }
  }
  return result;
}

//
// Arguments    : void
// Return Type  : coder::array<double, 1U>
//
static coder::array<double, 1U> argInit_Unboundedx1_real_T()
{
  coder::array<double, 1U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < result.size(0); idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
  return result;
}

//
// Arguments    : void
// Return Type  : coder::array<double, 2U>
//
static coder::array<double, 2U> argInit_UnboundedxUnbounded_real_T()
{
  coder::array<double, 2U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(2, 2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < result.size(0); idx0++) {
    for (int idx1{0}; idx1 < result.size(1); idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx0 + result.size(0) * idx1] = argInit_real_T();
    }
  }
  return result;
}

//
// Arguments    : void
// Return Type  : boolean_T
//
static boolean_T argInit_boolean_T()
{
  return false;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : struct0_T &result
// Return Type  : void
//
static void argInit_struct0_T(struct0_T &result)
{
  double b_result_tmp;
  boolean_T result_tmp;
  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  result_tmp = argInit_boolean_T();
  result.useObjectiveNormL2 = result_tmp;
  b_result_tmp = argInit_real_T();
  result.weightNormL2 = b_result_tmp;
  result.useObjectiveTrajectoryFollowing = result_tmp;
  result.weightTrajectoryFollowing = b_result_tmp;
  result.useObjectiveJointAcceleration = result_tmp;
  result.weightJointAcceleration = b_result_tmp;
  result.useObjectiveManipulability = result_tmp;
  result.weightManipulability = b_result_tmp;
  result.applyEqualityConstraints = result_tmp;
  result.applyInequalityConstraints = result_tmp;
  result.applySlack = result_tmp;
  argInit_6x1_real_T(result.Slacklowerbound);
  result.SlackObjectiveWeight = b_result_tmp;
  result.obstacleAvoidanceScheme = result_tmp;
  result.k = b_result_tmp;
  result.gamma = b_result_tmp;
  result.dynamicSlack = result_tmp;
  result.applyVelocityDamper = result_tmp;
  result.jointLimitActivationDistance = b_result_tmp;
  result.jointLimitStopDistance = b_result_tmp;
  result.jointLimitGain = b_result_tmp;
  result.useObjectiveNormInfinity = result_tmp;
  result.weightNormInfinity = b_result_tmp;
  argInit_6x1_real_T(result.SlackPenaltyWeight);
  for (int i{0}; i < 6; i++) {
    result.Slackupperbound[i] = result.Slacklowerbound[i];
  }
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void j_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Example main does not support command line arguments.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : int argc
//                char **argv
// Return Type  : int
//
int main(int argc, char **)
{
  static rtRunTimeErrorInfo c_emlrtRTEI{
      1,                            // lineNo
      "inverseKinematicsOAModified" // fName
  };
  OptimizationBasedIKWithOAModified *classInstance;
  classInstance = new OptimizationBasedIKWithOAModified;
  if (argc > 1) {
    j_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
  }
  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_inverseKinematicsOAModified(classInstance);
  delete classInstance;
  return 0;
}

//
// Arguments    : OptimizationBasedIKWithOAModified *instancePtr
// Return Type  : void
//
void main_inverseKinematicsOAModified(
    OptimizationBasedIKWithOAModified *instancePtr)
{
  coder::array<double, 2U> J_g_tmp;
  coder::array<double, 2U> jacobi;
  coder::array<double, 1U> joint_values_tmp;
  coder::array<double, 1U> optimal_joint_velocity;
  struct0_T r;
  double dv[6];
  double Exit_Flag;
  // Initialize function 'inverseKinematicsOAModified' input arguments.
  // Initialize function input argument 'joint_values'.
  joint_values_tmp = argInit_Unboundedx1_real_T();
  // Initialize function input argument 'jacobi'.
  jacobi = argInit_6xUnbounded_real_T();
  // Initialize function input argument 'xd_eff_vel'.
  // Initialize function input argument 'jointminvalues'.
  // Initialize function input argument 'jointmaxvalues'.
  // Initialize function input argument 'joint_min_vel'.
  // Initialize function input argument 'joint_max_vel'.
  // Initialize function input argument 'J_g'.
  J_g_tmp = argInit_UnboundedxUnbounded_real_T();
  // Initialize function input argument 'b_g'.
  // Initialize function input argument 'jointVelocityWeightMatrix'.
  // Initialize function input argument 'configInput'.
  // Call the entry-point 'inverseKinematicsOAModified'.
  argInit_6x1_real_T(dv);
  argInit_struct0_T(r);
  instancePtr->inverseKinematicsOAModified(
      joint_values_tmp, jacobi, dv, joint_values_tmp, joint_values_tmp,
      joint_values_tmp, joint_values_tmp, J_g_tmp, joint_values_tmp, J_g_tmp,
      &r, optimal_joint_velocity, &Exit_Flag);
}

//
// File trailer for main.cpp
//
// [EOF]
//
