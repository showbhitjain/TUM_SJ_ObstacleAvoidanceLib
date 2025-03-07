//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: linearForm_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "linearForm_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : boolean_T obj_hasLinear
//                int obj_nvar
//                array<double, 2U> &workspace
//                const array<double, 2U> &H
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
void linearForm_(boolean_T obj_hasLinear, int obj_nvar,
                 array<double, 2U> &workspace, const array<double, 2U> &H,
                 const array<double, 1U> &f, const array<double, 1U> &x)
{
  int ix;
  ix = 0;
  if (obj_hasLinear) {
    if (static_cast<int>(obj_nvar < 200)) {
      for (int i{0}; i < obj_nvar; i++) {
        workspace[i] = f[i];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int i = 0; i < obj_nvar; i++) {
        workspace[i] = f[i];
      }
    }
    ix = 1;
  }
  if (obj_nvar != 0) {
    int b_i;
    if (ix != 1) {
      if (static_cast<int>(obj_nvar < 200)) {
        for (int i{0}; i < obj_nvar; i++) {
          workspace[i] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int i = 0; i < obj_nvar; i++) {
          workspace[i] = 0.0;
        }
      }
    }
    ix = 0;
    b_i = obj_nvar * (obj_nvar - 1) + 1;
    for (int iac{1}; obj_nvar < 0 ? iac >= b_i : iac <= b_i; iac += obj_nvar) {
      double c;
      int i1;
      c = 0.5 * x[ix];
      i1 = (iac + obj_nvar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        int i2;
        i2 = ia - iac;
        workspace[i2] = workspace[i2] + H[ia - 1] * c;
      }
      ix++;
    }
  }
}

} // namespace Objective
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for linearForm_.cpp
//
// [EOF]
//
