//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeGradLag.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
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
                    const array<double, 1U> &grad, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int idx;
  int ix;
  for (ix = 0; ix < nVar; ix++) {
    workspace[ix] = grad[ix];
  }
  for (idx = 0; idx < mFixed; idx++) {
    workspace[finiteFixed[idx] - 1] =
        workspace[finiteFixed[idx] - 1] + lambda[idx];
  }
  if ((nVar != 0) && (mEq != 0)) {
    ix = mFixed;
    idx = ldA * (mEq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= idx : iac <= idx; iac += ldA) {
      int i;
      i = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i; ia++) {
        int i1;
        i1 = ia - iac;
        workspace[i1] = workspace[i1] + AeqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  ix = mFixed + mEq;
  for (idx = 0; idx < mLB; idx++) {
    workspace[finiteLB[idx] - 1] =
        workspace[finiteLB[idx] - 1] - lambda[ix + idx];
  }
  if (mLB - 1 >= 0) {
    ix += mLB;
  }
  for (idx = 0; idx < mUB; idx++) {
    workspace[finiteUB[idx] - 1] =
        workspace[finiteUB[idx] - 1] + lambda[ix + idx];
  }
}

//
// Arguments    : array<double, 2U> &workspace
//                int ldA
//                int nVar
//                const array<double, 1U> &grad
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
                    const array<double, 1U> &grad, int mEq,
                    const array<double, 1U> &AeqTrans,
                    const array<int, 1U> &finiteFixed, int mFixed,
                    const array<int, 1U> &finiteLB, int mLB,
                    const array<int, 1U> &finiteUB, int mUB,
                    const array<double, 1U> &lambda)
{
  int idx;
  int ix;
  for (ix = 0; ix < nVar; ix++) {
    workspace[ix] = grad[ix];
  }
  for (idx = 0; idx < mFixed; idx++) {
    workspace[finiteFixed[idx] - 1] =
        workspace[finiteFixed[idx] - 1] + lambda[idx];
  }
  if ((nVar != 0) && (mEq != 0)) {
    ix = mFixed;
    idx = ldA * (mEq - 1) + 1;
    for (int iac{1}; ldA < 0 ? iac >= idx : iac <= idx; iac += ldA) {
      int i;
      i = (iac + nVar) - 1;
      for (int ia{iac}; ia <= i; ia++) {
        int i1;
        i1 = ia - iac;
        workspace[i1] = workspace[i1] + AeqTrans[ia - 1] * lambda[ix];
      }
      ix++;
    }
  }
  ix = mFixed + mEq;
  for (idx = 0; idx < mLB; idx++) {
    workspace[finiteLB[idx] - 1] =
        workspace[finiteLB[idx] - 1] - lambda[ix + idx];
  }
  if (mLB - 1 >= 0) {
    ix += mLB;
  }
  for (idx = 0; idx < mUB; idx++) {
    workspace[finiteUB[idx] - 1] =
        workspace[finiteUB[idx] - 1] + lambda[ix + idx];
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
