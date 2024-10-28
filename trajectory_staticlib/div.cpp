//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: div.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

// Include Files
#include "div.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : double in1_data[]
//                int in1_size[2]
//                double in2
//                int in3
// Return Type  : void
//
namespace trajectoryGeneration {
void binary_expand_op_3(double in1_data[], int in1_size[2], double in2, int in3)
{
  double b_in1_data;
  int loop_ub;
  int stride_0_1;
  if (in3 == 1) {
    loop_ub = in1_size[1];
  } else {
    loop_ub = in3;
  }
  stride_0_1 = (in1_size[1] != 1);
  for (int i{0}; i < loop_ub; i++) {
    b_in1_data = in1_data[i * stride_0_1] / in2;
  }
  in1_size[0] = 1;
  in1_size[1] = loop_ub;
  for (int i{0}; i < loop_ub; i++) {
    in1_data[0] = b_in1_data;
  }
}

} // namespace trajectoryGeneration

//
// File trailer for div.cpp
//
// [EOF]
//
