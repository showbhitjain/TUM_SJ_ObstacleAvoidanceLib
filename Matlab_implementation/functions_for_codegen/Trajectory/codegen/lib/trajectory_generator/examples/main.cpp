//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
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
#include "Trajectory.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Declarations
static coder::array<char, 2U> argInit_1xUnbounded_char_T();

static coder::array<double, 2U> argInit_1xUnbounded_real_T();

static coder::array<double, 2U> argInit_3xUnbounded_real_T();

static char argInit_char_T();

static coder::array<double, 2U> argInit_d4xUnbounded_real_T();

static double argInit_real_T();

// Function Definitions
//
// Arguments    : void
// Return Type  : coder::array<char, 2U>
//
static coder::array<char, 2U> argInit_1xUnbounded_char_T()
{
  coder::array<char, 2U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(1, 2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 1; idx0++) {
    for (int idx1{0}; idx1 < result.size(1); idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx1] = argInit_char_T();
    }
  }
  return result;
}

//
// Arguments    : void
// Return Type  : coder::array<double, 2U>
//
static coder::array<double, 2U> argInit_1xUnbounded_real_T()
{
  coder::array<double, 2U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(1, 2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 1; idx0++) {
    for (int idx1{0}; idx1 < result.size(1); idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx1] = argInit_real_T();
    }
  }
  return result;
}

//
// Arguments    : void
// Return Type  : coder::array<double, 2U>
//
static coder::array<double, 2U> argInit_3xUnbounded_real_T()
{
  coder::array<double, 2U> result;
  // Set the size of the array.
  // Change this size to the value that the application requires.
  result.set_size(3, 2);
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 3; idx0++) {
    for (int idx1{0}; idx1 < result.size(1); idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result[idx0 + 3 * idx1] = argInit_real_T();
    }
  }
  return result;
}

//
// Arguments    : void
// Return Type  : char
//
static char argInit_char_T()
{
  return '?';
}

//
// Arguments    : void
// Return Type  : coder::array<double, 2U>
//
static coder::array<double, 2U> argInit_d4xUnbounded_real_T()
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
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : int argc
//                char **argv
// Return Type  : int
//
int main(int, char **)
{
  TrajectoryGeneration::Trajectory *classInstance;
  classInstance = new TrajectoryGeneration::Trajectory;
  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_cartesian_trajectory(classInstance);
  main_orientation_trajectory(classInstance);
  delete classInstance;
  return 0;
}

//
// Arguments    : TrajectoryGeneration::Trajectory *instancePtr
// Return Type  : void
//
void main_cartesian_trajectory(TrajectoryGeneration::Trajectory *instancePtr)
{
  coder::array<double, 2U> waypointAccels;
  coder::array<double, 2U> waypointTimes;
  coder::array<double, 2U> waypointVels;
  coder::array<double, 2U> waypoints_tmp;
  coder::array<double, 2U> xd;
  coder::array<double, 2U> xd_vel;
  coder::array<double, 2U> xdd;
  coder::array<char, 2U> trajType;
  // Initialize function 'cartesian_trajectory' input arguments.
  // Initialize function input argument 'waypoints'.
  waypoints_tmp = argInit_3xUnbounded_real_T();
  // Initialize function input argument 'waypointTimes'.
  waypointTimes = argInit_1xUnbounded_real_T();
  // Initialize function input argument 'trajType'.
  trajType = argInit_1xUnbounded_char_T();
  // Initialize function input argument 'waypointVels'.
  waypointVels = waypoints_tmp;
  // Initialize function input argument 'waypointAccels'.
  waypointAccels = waypoints_tmp;
  // Call the entry-point 'cartesian_trajectory'.
  instancePtr->cartesian_trajectory(waypoints_tmp, waypointTimes,
                                    argInit_real_T(), trajType, waypointVels,
                                    waypointAccels, xd, xd_vel, xdd);
}

//
// Arguments    : TrajectoryGeneration::Trajectory *instancePtr
// Return Type  : void
//
void main_orientation_trajectory(TrajectoryGeneration::Trajectory *instancePtr)
{
  coder::array<double, 2U> desired_angular_accel;
  coder::array<double, 2U> desired_angular_velocity;
  coder::array<double, 2U> desired_quaternions;
  coder::array<double, 2U> orientations;
  coder::array<double, 2U> waypointTimes;
  coder::array<char, 2U> trajType;
  // Initialize function 'orientation_trajectory' input arguments.
  // Initialize function input argument 'orientations'.
  orientations = argInit_d4xUnbounded_real_T();
  // Initialize function input argument 'waypointTimes'.
  waypointTimes = argInit_1xUnbounded_real_T();
  // Initialize function input argument 'trajType'.
  trajType = argInit_1xUnbounded_char_T();
  // Call the entry-point 'orientation_trajectory'.
  instancePtr->orientation_trajectory(
      orientations, waypointTimes, argInit_real_T(), trajType,
      desired_quaternions, desired_angular_velocity, desired_angular_accel);
}

//
// File trailer for main.cpp
//
// [EOF]
//
