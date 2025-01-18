//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: eye.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "eye.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : double b_I[36]
// Return Type  : void
//
namespace coder {
void eye(double b_I[36])
{
  std::memset(&b_I[0], 0, 36U * sizeof(double));
  for (int k{0}; k < 6; k++) {
    b_I[k + 6 * k] = 1.0;
  }
}

} // namespace coder

//
// File trailer for eye.cpp
//
// [EOF]
//
