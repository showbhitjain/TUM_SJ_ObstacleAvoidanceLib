//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: configurableObjective.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "configurableObjective.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : coder::array<double, 1U> &in1
//                const double in2_data[]
//                const int &in2_size
//                const double in3_data[]
//                const int &in3_size
// Return Type  : void
//
void binary_expand_op_1(coder::array<double, 1U> &in1, const double in2_data[],
                        const int &in2_size, const double in3_data[],
                        const int &in3_size)
{
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  if (in3_size == 1) {
    loop_ub = in2_size;
  } else {
    loop_ub = in3_size;
  }
  in1.set_size(loop_ub);
  stride_0_0 = (in2_size != 1);
  stride_1_0 = (in3_size != 1);
  for (int i{0}; i < loop_ub; i++) {
    in1[i] = in2_data[i * stride_0_0] - in3_data[i * stride_1_0];
  }
}

//
// File trailer for configurableObjective.cpp
//
// [EOF]
//
