//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGrad_StoreHx.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "computeGrad_StoreHx.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
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
  switch (obj.objtype) {
  case 5: {
    int i;
    i = obj.nvar;
    for (int ix{0}; ix <= i - 2; ix++) {
      obj.grad[ix] = 0.0;
    }
    obj.grad[obj.nvar - 1] = obj.gammaScalar;
  } break;
  case 3: {
    int i;
    int ix;
    int lda;
    int m_tmp;
    m_tmp = obj.nvar - 1;
    lda = obj.nvar;
    if (obj.nvar != 0) {
      int iy;
      for (iy = 0; iy <= m_tmp; iy++) {
        obj.Hx[iy] = 0.0;
      }
      ix = 0;
      i = obj.nvar * m_tmp + 1;
      for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        int i1;
        i1 = iac + m_tmp;
        for (int ia{iac}; ia <= i1; ia++) {
          iy = ia - iac;
          obj.Hx[iy] = obj.Hx[iy] + H[ia - 1] * x[ix];
        }
        ix++;
      }
    }
    i = obj.nvar;
    for (ix = 0; ix < i; ix++) {
      obj.grad[ix] = obj.Hx[ix];
    }
    if (obj.hasLinear && (obj.nvar >= 1)) {
      for (ix = 0; ix <= m_tmp; ix++) {
        obj.grad[ix] = obj.grad[ix] + f[ix];
      }
    }
  } break;
  default: {
    int i;
    int i1;
    int ix;
    int iy;
    int lda;
    int m_tmp;
    int maxRegVar;
    maxRegVar = obj.maxVar - 1;
    m_tmp = obj.nvar - 1;
    lda = obj.nvar;
    if (obj.nvar != 0) {
      for (iy = 0; iy <= m_tmp; iy++) {
        obj.Hx[iy] = 0.0;
      }
      ix = 0;
      i = obj.nvar * (obj.nvar - 1) + 1;
      for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
        i1 = iac + m_tmp;
        for (int ia{iac}; ia <= i1; ia++) {
          iy = ia - iac;
          obj.Hx[iy] = obj.Hx[iy] + H[ia - 1] * x[ix];
        }
        ix++;
      }
    }
    i = obj.nvar + 1;
    for (ix = i; ix <= maxRegVar; ix++) {
      obj.Hx[ix - 1] = obj.beta * x[ix - 1];
    }
    for (ix = 0; ix < maxRegVar; ix++) {
      obj.grad[ix] = obj.Hx[ix];
    }
    if (obj.hasLinear && (obj.nvar >= 1)) {
      for (ix = 0; ix <= m_tmp; ix++) {
        obj.grad[ix] = obj.grad[ix] + f[ix];
      }
    }
    ix = (obj.maxVar - obj.nvar) - 1;
    if (ix >= 1) {
      iy = obj.nvar;
      i = ix - 1;
      for (ix = 0; ix <= i; ix++) {
        i1 = iy + ix;
        obj.grad[i1] = obj.grad[i1] + obj.rho;
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
