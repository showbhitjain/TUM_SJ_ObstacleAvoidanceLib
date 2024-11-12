//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: unsafeSxfun.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "unsafeSxfun.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : coder::array<double, 1U> &in1
//                const coder::array<double, 1U> &in3
//                int in4
//                const coder::array<double, 1U> &in5
// Return Type  : void
//
void binary_expand_op(coder::array<double, 1U> &in1,
                      const coder::array<double, 1U> &in3, int in4,
                      const coder::array<double, 1U> &in5)
{
  coder::array<double, 1U> b_in3;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  if (in5.size(0) == 1) {
    loop_ub = in4 + 1;
  } else {
    loop_ub = in5.size(0);
  }
  b_in3.set_size(loop_ub);
  stride_0_0 = (in4 + 1 != 1);
  stride_1_0 = (in5.size(0) != 1);
  for (int i{0}; i < loop_ub; i++) {
    b_in3[i] = in3[i * stride_0_0] - in5[i * stride_1_0];
  }
  in1.set_size(b_in3.size(0));
  loop_ub = b_in3.size(0);
  for (int i{0}; i < loop_ub; i++) {
    double varargin_1;
    varargin_1 = b_in3[i];
    in1[i] = varargin_1 * varargin_1;
  }
}

//
// File trailer for unsafeSxfun.cpp
//
// [EOF]
//
