//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: updateWorkingSetForNewQP.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "updateWorkingSetForNewQP.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &xk
//                d_struct_T &WorkingSet
//                int mIneq
//                const array<double, 1U> &cIneq
//                int mEq
//                const double cEq_data[]
//                int mLB
//                const array<double, 1U> &lb
//                int mUB
//                const array<double, 1U> &ub
//                int mFixed
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace internal {
void updateWorkingSetForNewQP(const array<double, 1U> &xk,
                              d_struct_T &WorkingSet, int mIneq,
                              const array<double, 1U> &cIneq, int mEq,
                              const double cEq_data[], int mLB,
                              const array<double, 1U> &lb, int mUB,
                              const array<double, 1U> &ub, int mFixed)
{
  int i;
  boolean_T hasLB;
  boolean_T hasUB;
  for (int idx{0}; idx < mEq; idx++) {
    WorkingSet.beq.data[idx] = -cEq_data[idx];
    WorkingSet.bwset[mFixed + idx] = WorkingSet.beq.data[idx];
  }
  for (int idx{0}; idx < mIneq; idx++) {
    WorkingSet.bineq[idx] = -cIneq[idx];
  }
  hasLB = (lb.size(0) != 0);
  hasUB = (ub.size(0) != 0);
  if (hasLB) {
    for (int idx{0}; idx < mLB; idx++) {
      WorkingSet.lb[WorkingSet.indexLB[idx] - 1] =
          -lb[WorkingSet.indexLB[idx] - 1] + xk[WorkingSet.indexLB[idx] - 1];
    }
  }
  if (hasUB) {
    for (int idx{0}; idx < mUB; idx++) {
      WorkingSet.ub[WorkingSet.indexUB[idx] - 1] =
          ub[WorkingSet.indexUB[idx] - 1] - xk[WorkingSet.indexUB[idx] - 1];
    }
  }
  if (hasLB && hasUB) {
    for (int idx{0}; idx < mFixed; idx++) {
      double d;
      d = ub[WorkingSet.indexFixed[idx] - 1] -
          xk[WorkingSet.indexFixed[idx] - 1];
      WorkingSet.ub[WorkingSet.indexFixed[idx] - 1] = d;
      WorkingSet.bwset[idx] = d;
    }
  }
  i = mFixed + mEq;
  if (WorkingSet.nActiveConstr > i) {
    int ineqStart;
    ineqStart = i + 1;
    if (ineqStart < 1) {
      ineqStart = 1;
    }
    i = WorkingSet.nActiveConstr;
    for (int idx{ineqStart}; idx <= i; idx++) {
      switch (WorkingSet.Wid[idx - 1]) {
      case 4:
        WorkingSet.bwset[idx - 1] =
            WorkingSet
                .lb[WorkingSet.indexLB[WorkingSet.Wlocalidx[idx - 1] - 1] - 1];
        break;
      case 5:
        WorkingSet.bwset[idx - 1] =
            WorkingSet
                .ub[WorkingSet.indexUB[WorkingSet.Wlocalidx[idx - 1] - 1] - 1];
        break;
      default:
        WorkingSet.bwset[idx - 1] =
            WorkingSet.bineq[WorkingSet.Wlocalidx[idx - 1] - 1];
        break;
      }
    }
  }
}

} // namespace internal
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for updateWorkingSetForNewQP.cpp
//
// [EOF]
//
