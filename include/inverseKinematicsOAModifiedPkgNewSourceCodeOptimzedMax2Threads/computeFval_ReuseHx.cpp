//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeFval_ReuseHx.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "computeFval_ReuseHx.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const g_struct_T &obj
//                array<double, 2U> &workspace
//                const array<double, 1U> &f
//                const array<double, 1U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace Objective {
double computeFval_ReuseHx(const g_struct_T &obj, array<double, 2U> &workspace,
                           const array<double, 1U> &f,
                           const array<double, 1U> &x)
{
  double val;
  switch (obj.objtype) {
  case 5:
    val = obj.gammaScalar * x[obj.nvar - 1];
    break;
  case 3: {
    if (obj.hasLinear) {
      int ixlast;
      ixlast = obj.nvar;
      if (static_cast<int>(ixlast < 200)) {
        for (int i{0}; i < ixlast; i++) {
          workspace[i] = 0.5 * obj.Hx[i] + f[i];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int i = 0; i < ixlast; i++) {
          workspace[i] = 0.5 * obj.Hx[i] + f[i];
        }
      }
      val = 0.0;
      if (obj.nvar >= 1) {
        ixlast = obj.nvar;
        for (int k{0}; k < ixlast; k++) {
          val += x[k] * workspace[k];
        }
      }
    } else {
      val = 0.0;
      if (obj.nvar >= 1) {
        int ixlast;
        ixlast = obj.nvar;
        for (int k{0}; k < ixlast; k++) {
          val += x[k] * obj.Hx[k];
        }
      }
      val *= 0.5;
    }
  } break;
  default: {
    int maxRegVar;
    maxRegVar = obj.maxVar - 1;
    if (obj.hasLinear) {
      int ixlast;
      ixlast = obj.nvar;
      if (static_cast<int>(ixlast < 200)) {
        for (int i{0}; i < ixlast; i++) {
          workspace[i] = f[i];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int i = 0; i < ixlast; i++) {
          workspace[i] = f[i];
        }
      }
      ixlast = obj.maxVar - obj.nvar;
      for (int k{0}; k <= ixlast - 2; k++) {
        workspace[obj.nvar + k] = obj.rho;
      }
      if (static_cast<int>(maxRegVar < 200)) {
        for (int i{0}; i < maxRegVar; i++) {
          workspace[i] = workspace[i] + 0.5 * obj.Hx[i];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int i = 0; i < maxRegVar; i++) {
          workspace[i] = workspace[i] + 0.5 * obj.Hx[i];
        }
      }
      val = 0.0;
      if (obj.maxVar - 1 >= 1) {
        ixlast = obj.maxVar;
        for (int k{0}; k <= ixlast - 2; k++) {
          val += x[k] * workspace[k];
        }
      }
    } else {
      int ixlast;
      val = 0.0;
      if (maxRegVar >= 1) {
        ixlast = obj.maxVar;
        for (int k{0}; k <= ixlast - 2; k++) {
          val += x[k] * obj.Hx[k];
        }
      }
      val *= 0.5;
      ixlast = obj.nvar + 1;
      for (int k{ixlast}; k <= maxRegVar; k++) {
        val += x[k - 1] * obj.rho;
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
// File trailer for computeFval_ReuseHx.cpp
//
// [EOF]
//
