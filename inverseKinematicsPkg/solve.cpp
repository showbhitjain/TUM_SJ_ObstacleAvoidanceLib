//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: solve.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "solve.h"
#include "inverseKinematics_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const e_struct_T &obj
//                array<double, 1U> &rhs
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace CholManager {
void solve(const e_struct_T &obj, array<double, 1U> &rhs)
{
  int jA;
  int n_tmp;
  n_tmp = obj.ndims;
  if ((rhs.size(0) != 0) && (obj.ndims != 0)) {
    for (int j{0}; j < n_tmp; j++) {
      double temp;
      jA = j * obj.ldm;
      temp = rhs[j];
      for (int i{0}; i < j; i++) {
        temp -= obj.FMat[jA + i] * rhs[i];
      }
      rhs[j] = temp / obj.FMat[jA + j];
    }
  }
  if ((rhs.size(0) != 0) && (obj.ndims != 0)) {
    for (int j{n_tmp}; j >= 1; j--) {
      jA = (j + (j - 1) * obj.ldm) - 1;
      rhs[j - 1] = rhs[j - 1] / obj.FMat[jA];
      for (int i{0}; i <= j - 2; i++) {
        int ix;
        ix = (j - i) - 2;
        rhs[ix] = rhs[ix] - rhs[j - 1] * obj.FMat[(jA - i) - 1];
      }
    }
  }
}

} // namespace CholManager
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for solve.cpp
//
// [EOF]
//
