//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGradLag.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "computeGradLag.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 1U> &workspace
//                int ldA
//                int nVar
//                const array<double, 1U> &grad
//                int mIneq
//                const array<double, 1U> &AineqTrans
//                int mEq
//                const array<double, 1U> &AeqTrans
//                const array<int, 1U> &finiteFixed
//                int mFixed
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &lambda
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace stopping {
void computeGradLag(array<double, 1U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int i;
  int i1;
  int i2;
  int iL0;
  int ix;
  for (ix = 0; ix < nVar; ix++) {
    workspace[ix] = grad[ix];
  }
  for (ix = 0; ix < mFixed; ix++) {
    workspace[finiteFixed[ix] - 1] =
        workspace[finiteFixed[ix] - 1] + lambda[ix];
  }
  if ((nVar != 0) && (mEq != 0)) {
    ix = mFixed;
    i = ldA * (mEq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= i : iac <= i; iac += ldA) {
      i1 = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        i2 = ia - iac;
        workspace[i2] = workspace[i2] + AeqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  iL0 = mFixed + mEq;
  if ((nVar != 0) && (mIneq != 0)) {
    ix = iL0;
    i = ldA * (mIneq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= i : iac <= i; iac += ldA) {
      i1 = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        i2 = ia - iac;
        workspace[i2] = workspace[i2] + AineqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  iL0 += mIneq;
  for (ix = 0; ix < mLB; ix++) {
    workspace[finiteLB[ix] - 1] =
        workspace[finiteLB[ix] - 1] - lambda[iL0 + ix];
  }
  if (mLB - 1 >= 0) {
    iL0 += mLB;
  }
  for (ix = 0; ix < mUB; ix++) {
    workspace[finiteUB[ix] - 1] =
        workspace[finiteUB[ix] - 1] + lambda[iL0 + ix];
  }
}

//
// Arguments    : array<double, 2U> &workspace
//                int ldA
//                int nVar
//                const array<double, 1U> &grad
//                int mIneq
//                const array<double, 1U> &AineqTrans
//                int mEq
//                const array<double, 1U> &AeqTrans
//                const array<int, 1U> &finiteFixed
//                int mFixed
//                const array<int, 1U> &finiteLB
//                int mLB
//                const array<int, 1U> &finiteUB
//                int mUB
//                const array<double, 1U> &lambda
// Return Type  : void
//
void computeGradLag(array<double, 2U> &workspace, int ldA, int nVar,
                    const array<double, 1U> &grad, int mIneq,
                    const array<double, 1U> &AineqTrans, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int i;
  int i1;
  int i2;
  int iL0;
  int ix;
  for (ix = 0; ix < nVar; ix++) {
    workspace[ix] = grad[ix];
  }
  for (ix = 0; ix < mFixed; ix++) {
    workspace[finiteFixed[ix] - 1] =
        workspace[finiteFixed[ix] - 1] + lambda[ix];
  }
  if ((nVar != 0) && (mEq != 0)) {
    ix = mFixed;
    i = ldA * (mEq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= i : iac <= i; iac += ldA) {
      i1 = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        i2 = ia - iac;
        workspace[i2] = workspace[i2] + AeqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  iL0 = mFixed + mEq;
  if ((nVar != 0) && (mIneq != 0)) {
    ix = iL0;
    i = ldA * (mIneq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= i : iac <= i; iac += ldA) {
      i1 = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        i2 = ia - iac;
        workspace[i2] = workspace[i2] + AineqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  iL0 += mIneq;
  for (ix = 0; ix < mLB; ix++) {
    workspace[finiteLB[ix] - 1] =
        workspace[finiteLB[ix] - 1] - lambda[iL0 + ix];
  }
  if (mLB - 1 >= 0) {
    iL0 += mLB;
  }
  for (ix = 0; ix < mUB; ix++) {
    workspace[finiteUB[ix] - 1] =
        workspace[finiteUB[ix] - 1] + lambda[iL0 + ix];
  }
}

} // namespace stopping
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeGradLag.cpp
//
// [EOF]
//
