//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGrad_StoreHx.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "computeGrad_StoreHx.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : g_struct_T &obj
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
void computeGrad_StoreHx(g_struct_T &obj, const array<double, 2U> &H,
                         const array<double, 1U> &f, const array<double, 1U> &x)
{
  int iy;
  switch (obj.objtype) {
  case 5: {
    int i;
    i = obj.nvar;
    if (static_cast<int>(i - 1 < 200)) {
      for (iy = 0; iy <= i - 2; iy++) {
        obj.grad[iy] = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (iy = 0; iy <= i - 2; iy++) {
        obj.grad[iy] = 0.0;
      }
    }
    obj.grad[obj.nvar - 1] = obj.gammaScalar;
  } break;
  case 3: {
    int i;
    int ix;
    int lda;
    int m_tmp_tmp;
    m_tmp_tmp = obj.nvar - 1;
    lda = obj.nvar;
    if (obj.nvar != 0) {
      if (static_cast<int>(m_tmp_tmp + 1 < 200)) {
        for (iy = 0; iy <= m_tmp_tmp; iy++) {
          obj.Hx[iy] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (iy = 0; iy <= m_tmp_tmp; iy++) {
          obj.Hx[iy] = 0.0;
        }
      }
      ix = 0;
      i = obj.nvar * (obj.nvar - 1) + 1;
      for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        int i1;
        i1 = iac + m_tmp_tmp;
        for (int ia{iac}; ia <= i1; ia++) {
          int b_iy;
          b_iy = ia - iac;
          obj.Hx[b_iy] = obj.Hx[b_iy] + H[ia - 1] * x[ix];
        }
        ix++;
      }
    }
    i = obj.nvar;
    if (static_cast<int>(i < 200)) {
      for (iy = 0; iy < i; iy++) {
        obj.grad[iy] = obj.Hx[iy];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (iy = 0; iy < i; iy++) {
        obj.grad[iy] = obj.Hx[iy];
      }
    }
    if (obj.hasLinear && (obj.nvar >= 1)) {
      ix = obj.nvar - 1;
      if (static_cast<int>(ix + 1 < 200)) {
        for (int k{0}; k <= ix; k++) {
          obj.grad[k] = obj.grad[k] + f[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k <= ix; k++) {
          obj.grad[k] = obj.grad[k] + f[k];
        }
      }
    }
  } break;
  default: {
    int b_iy;
    int i;
    int i1;
    int ix;
    int lda;
    int m_tmp_tmp;
    int maxRegVar;
    maxRegVar = obj.maxVar - 1;
    m_tmp_tmp = obj.nvar - 1;
    lda = obj.nvar;
    if (obj.nvar != 0) {
      if (static_cast<int>(m_tmp_tmp + 1 < 200)) {
        for (iy = 0; iy <= m_tmp_tmp; iy++) {
          obj.Hx[iy] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (iy = 0; iy <= m_tmp_tmp; iy++) {
          obj.Hx[iy] = 0.0;
        }
      }
      ix = 0;
      i = obj.nvar * (obj.nvar - 1) + 1;
      for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        i1 = iac + m_tmp_tmp;
        for (int ia{iac}; ia <= i1; ia++) {
          b_iy = ia - iac;
          obj.Hx[b_iy] = obj.Hx[b_iy] + H[ia - 1] * x[ix];
        }
        ix++;
      }
    }
    i = obj.nvar + 1;
    if (static_cast<int>((maxRegVar - i) + 1 < 200)) {
      for (iy = i; iy <= maxRegVar; iy++) {
        obj.Hx[iy - 1] = obj.beta * x[iy - 1];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (iy = i; iy <= maxRegVar; iy++) {
        obj.Hx[iy - 1] = obj.beta * x[iy - 1];
      }
    }
    if (static_cast<int>(maxRegVar < 200)) {
      for (iy = 0; iy < maxRegVar; iy++) {
        obj.grad[iy] = obj.Hx[iy];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (iy = 0; iy < maxRegVar; iy++) {
        obj.grad[iy] = obj.Hx[iy];
      }
    }
    if (obj.hasLinear && (obj.nvar >= 1)) {
      ix = obj.nvar - 1;
      if (static_cast<int>(ix + 1 < 200)) {
        for (int k{0}; k <= ix; k++) {
          obj.grad[k] = obj.grad[k] + f[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k <= ix; k++) {
          obj.grad[k] = obj.grad[k] + f[k];
        }
      }
    }
    ix = (obj.maxVar - obj.nvar) - 1;
    if (ix >= 1) {
      b_iy = obj.nvar;
      i = ix - 1;
      if (static_cast<int>(ix < 200)) {
        for (int k{0}; k <= i; k++) {
          i1 = b_iy + k;
          obj.grad[i1] = obj.grad[i1] + obj.rho;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(iy)

        for (int k = 0; k <= i; k++) {
          iy = b_iy + k;
          obj.grad[iy] = obj.grad[iy] + obj.rho;
        }
      }
    }
  } break;
  }
}

} // namespace Objective
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeGrad_StoreHx.cpp
//
// [EOF]
//
