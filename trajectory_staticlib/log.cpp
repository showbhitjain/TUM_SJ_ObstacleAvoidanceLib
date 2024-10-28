//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: log.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

// Include Files
#include "log.h"
#include "quaternion.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : double &in1
//                const coder::quaternion *in3
//                const int in4_size[2]
//                const double in5_data[]
//                const int in5_size[2]
// Return Type  : void
//
namespace trajectoryGeneration {
void binary_expand_op(double &in1, const coder::quaternion *in3,
                      const int in4_size[2], const double in5_data[],
                      const int in5_size[2])
{
  int loop_ub;
  if (in5_size[1] == 1) {
    loop_ub = in4_size[1];
  } else {
    loop_ub = in5_size[1];
  }
  for (int i{0}; i < loop_ub; i++) {
    in1 = in3->d * in5_data[0];
  }
}

//
// Arguments    : double &in1
//                const coder::quaternion *in3
//                const int in4_size[2]
//                const double in5_data[]
//                const int in5_size[2]
// Return Type  : void
//
void binary_expand_op_1(double &in1, const coder::quaternion *in3,
                        const int in4_size[2], const double in5_data[],
                        const int in5_size[2])
{
  int loop_ub;
  if (in5_size[1] == 1) {
    loop_ub = in4_size[1];
  } else {
    loop_ub = in5_size[1];
  }
  for (int i{0}; i < loop_ub; i++) {
    in1 = in3->c * in5_data[0];
  }
}

//
// Arguments    : double &in1
//                const coder::quaternion *in3
//                const int in4_size[2]
//                const double in5_data[]
//                const int in5_size[2]
// Return Type  : void
//
void binary_expand_op_2(double &in1, const coder::quaternion *in3,
                        const int in4_size[2], const double in5_data[],
                        const int in5_size[2])
{
  int loop_ub;
  if (in5_size[1] == 1) {
    loop_ub = in4_size[1];
  } else {
    loop_ub = in5_size[1];
  }
  for (int i{0}; i < loop_ub; i++) {
    in1 = in3->b * in5_data[0];
  }
}

} // namespace trajectoryGeneration

//
// File trailer for log.cpp
//
// [EOF]
//
