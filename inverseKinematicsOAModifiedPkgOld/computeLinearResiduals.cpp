//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeLinearResiduals.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "computeLinearResiduals.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &x
//                int nVar
//                array<double, 1U> &workspaceIneq
//                int mLinIneq
//                const array<double, 1U> &AineqT
//                const array<double, 2U> &bineq
//                int ldAi
//                double workspaceEq_data[]
//                int &workspaceEq_size
//                int mLinEq
//                const array<double, 1U> &AeqT
//                const double beq_data[]
//                int ldAe
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace internal {
void computeLinearResiduals(const array<double, 1U> &x, int nVar,
                            array<double, 1U> &workspaceIneq, int mLinIneq,
                            const array<double, 1U> &AineqT,
                            const array<double, 2U> &bineq, int ldAi,
                            double workspaceEq_data[], int &workspaceEq_size,
                            int mLinEq, const array<double, 1U> &AeqT,
                            const double beq_data[], int ldAe)
{
  array<double, 1U> y;
  int k;
  if (mLinIneq > 0) {
    if (mLinIneq > 2147483646) {
      check_forloop_overflow_error();
    }
    for (k = 0; k < mLinIneq; k++) {
      workspaceIneq[k] = bineq[k];
    }
    ::coder::internal::blas::xgemv(nVar, mLinIneq, AineqT, ldAi, x,
                                   workspaceIneq);
  }
  if (mLinEq > 0) {
    y.set_size(workspaceEq_size);
    for (int i{0}; i < workspaceEq_size; i++) {
      y[i] = workspaceEq_data[i];
    }
    if (mLinEq > 2147483646) {
      check_forloop_overflow_error();
    }
    for (k = 0; k < mLinEq; k++) {
      y[k] = beq_data[k];
    }
    ::coder::internal::blas::xgemv(nVar, mLinEq, AeqT, ldAe, x, y);
    workspaceEq_size = y.size(0);
    k = y.size(0);
    for (int i{0}; i < k; i++) {
      workspaceEq_data[i] = y[i];
    }
  }
}

} // namespace internal
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeLinearResiduals.cpp
//
// [EOF]
//
