//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: add_noise.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

// Include Files
#include "add_noise.h"
#include "rand.h"
#include "rt_nonfinite.h"
#include "utilsMatlab_data.h"
#include "utilsMatlab_initialize.h"
#include "coder_array.h"

// Function Declarations
static void binary_expand_op(coder::array<double, 1U> &in1,
                             const coder::array<double, 1U> &in2, double in3,
                             double in4);

// Function Definitions
//
// Arguments    : coder::array<double, 1U> &in1
//                const coder::array<double, 1U> &in2
//                double in3
//                double in4
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 1U> &in1,
                             const coder::array<double, 1U> &in2, double in3,
                             double in4)
{
  coder::array<double, 1U> b_in2;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  if (in1.size(0) == 1) {
    loop_ub = in2.size(0);
  } else {
    loop_ub = in1.size(0);
  }
  b_in2.set_size(loop_ub);
  stride_0_0 = (in2.size(0) != 1);
  stride_1_0 = (in1.size(0) != 1);
  for (int i{0}; i < loop_ub; i++) {
    b_in2[i] = in2[i * stride_0_0] + (in3 + in4 * in1[i * stride_1_0]);
  }
  in1.set_size(b_in2.size(0));
  loop_ub = b_in2.size(0);
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = b_in2[i];
  }
}

//
// Define the range of the uniform noise
//  noise_min = -0.005;
//  noise_max = 0.005;
//
// Arguments    : const coder::array<double, 1U> &joint_vector
//                double noise_min
//                double noise_max
//                coder::array<double, 1U> &noisy_joint_vector
// Return Type  : void
//
void add_noise(const coder::array<double, 1U> &joint_vector, double noise_min,
               double noise_max, coder::array<double, 1U> &noisy_joint_vector)
{
  double b_joint_vector[2];
  double a;
  if (!isInitialized_utilsMatlab) {
    utilsMatlab_initialize();
  }
  //  Generate uniform noise with the same size as the joint vector
  a = noise_max - noise_min;
  b_joint_vector[0] = joint_vector.size(0);
  b_joint_vector[1] = 1.0;
  coder::b_rand(b_joint_vector, noisy_joint_vector);
  //  Add the noise to the original joint vector
  if (joint_vector.size(0) == noisy_joint_vector.size(0)) {
    int loop_ub;
    noisy_joint_vector.set_size(joint_vector.size(0));
    loop_ub = joint_vector.size(0);
    for (int i{0}; i < loop_ub; i++) {
      noisy_joint_vector[i] =
          joint_vector[i] + (noise_min + a * noisy_joint_vector[i]);
    }
  } else {
    binary_expand_op(noisy_joint_vector, joint_vector, noise_min, a);
  }
}

//
// File trailer for add_noise.cpp
//
// [EOF]
//
