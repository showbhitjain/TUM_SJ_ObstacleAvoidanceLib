//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFval.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "computeFval.h"
#include "inverseKinematics_internal_types.h"
#include "linearForm_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const f_struct_T &obj
//                array<double, 2U> &workspace
//                const array<double, 2U> &H
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
double computeFval(const f_struct_T &obj, array<double, 2U> &workspace,
                   const array<double, 2U> &H, const array<double, 1U> &f,
                   const array<double, 1U> &x)
{
  double val;
  switch (obj.objtype) {
  case 5:
    val = obj.gammaScalar * x[obj.nvar - 1];
    break;
  case 3: {
    linearForm_(obj.hasLinear, obj.nvar, workspace, H, f, x);
    val = 0.0;
    if (obj.nvar >= 1) {
      int ixlast;
      ixlast = obj.nvar;
      for (int k{0}; k < ixlast; k++) {
        val += x[k] * workspace[k];
      }
    }
  } break;
  default: {
    int ixlast;
    int k;
    linearForm_(obj.hasLinear, obj.nvar, workspace, H, f, x);
    ixlast = obj.nvar + 1;
    k = obj.maxVar - 1;
    for (int idx{ixlast}; idx <= k; idx++) {
      workspace[idx - 1] = 0.5 * obj.beta * x[idx - 1] + obj.rho;
    }
    val = 0.0;
    if (k >= 1) {
      ixlast = obj.maxVar;
      for (k = 0; k <= ixlast - 2; k++) {
        val += x[k] * workspace[k];
      }
    }
  } break;
  }
  return val;
}

} // namespace Objective
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeFval.cpp
//
// [EOF]
//
