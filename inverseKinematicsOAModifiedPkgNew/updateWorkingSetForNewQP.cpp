//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: updateWorkingSetForNewQP.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "updateWorkingSetForNewQP.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
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
//                int cEq_size
//                int mLB
//                const array<double, 2U> &lb
//                int mUB
//                const array<double, 2U> &ub
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
                              const double cEq_data[], int cEq_size, int mLB,
                              const array<double, 2U> &lb, int mUB,
                              const array<double, 2U> &ub, int mFixed)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                         // iFirst
      -1,                         // iLast
      1,                          // lineNo
      1,                          // colNo
      "",                         // aName
      "updateWorkingSetForNewQP", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+internal/updateWorkingSetForNewQP.p", // pName
      0                                       // checkKind
  };
  int i;
  int i1;
  boolean_T hasLB;
  boolean_T hasUB;
  if (mEq > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mEq; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > cEq_size)) {
      rtDynamicBoundsError(idx + 1, 1, cEq_size, w_emlrtBCI);
    }
    i = WorkingSet.beq.size[0];
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    WorkingSet.beq.data[idx] = -cEq_data[idx];
    i = WorkingSet.beq.size[0];
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    i = WorkingSet.bwset.size(0);
    i1 = (mFixed + idx) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    WorkingSet.bwset[i1 - 1] = WorkingSet.beq.data[idx];
  }
  if (mIneq > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mIneq; idx++) {
    if ((idx + 1 < 1) || (idx + 1 > cIneq.size(0))) {
      rtDynamicBoundsError(idx + 1, 1, cIneq.size(0), w_emlrtBCI);
    }
    i = WorkingSet.bineq.size(0);
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    WorkingSet.bineq[idx] = -cIneq[idx];
  }
  hasLB = (lb.size(0) != 0);
  hasUB = (ub.size(0) != 0);
  if (hasLB) {
    if (mLB > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mLB; idx++) {
      i = WorkingSet.indexLB.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
      }
      if ((WorkingSet.indexLB[idx] < 1) ||
          (WorkingSet.indexLB[idx] > lb.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, lb.size(0),
                             w_emlrtBCI);
      }
      if ((WorkingSet.indexLB[idx] < 1) ||
          (WorkingSet.indexLB[idx] > xk.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, xk.size(0),
                             w_emlrtBCI);
      }
      i = WorkingSet.lb.size(0);
      if ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i)) {
        rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
      }
      WorkingSet.lb[WorkingSet.indexLB[idx] - 1] =
          -lb[WorkingSet.indexLB[idx] - 1] + xk[WorkingSet.indexLB[idx] - 1];
    }
  }
  if (hasUB) {
    if (mUB > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mUB; idx++) {
      i = WorkingSet.indexUB.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
      }
      if ((WorkingSet.indexUB[idx] < 1) ||
          (WorkingSet.indexUB[idx] > ub.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, ub.size(0),
                             w_emlrtBCI);
      }
      if ((WorkingSet.indexUB[idx] < 1) ||
          (WorkingSet.indexUB[idx] > xk.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, xk.size(0),
                             w_emlrtBCI);
      }
      i = WorkingSet.ub.size(0);
      if ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i)) {
        rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
      }
      WorkingSet.ub[WorkingSet.indexUB[idx] - 1] =
          ub[WorkingSet.indexUB[idx] - 1] - xk[WorkingSet.indexUB[idx] - 1];
    }
  }
  if (hasLB && hasUB) {
    if (mFixed > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < mFixed; idx++) {
      double d;
      i = WorkingSet.indexFixed.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
      }
      if ((WorkingSet.indexFixed[idx] < 1) ||
          (WorkingSet.indexFixed[idx] > ub.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexFixed[idx], 1, ub.size(0),
                             w_emlrtBCI);
      }
      if ((WorkingSet.indexFixed[idx] < 1) ||
          (WorkingSet.indexFixed[idx] > xk.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexFixed[idx], 1, xk.size(0),
                             w_emlrtBCI);
      }
      i = WorkingSet.ub.size(0);
      if ((WorkingSet.indexFixed[idx] < 1) ||
          (WorkingSet.indexFixed[idx] > i)) {
        rtDynamicBoundsError(WorkingSet.indexFixed[idx], 1, i, w_emlrtBCI);
      }
      d = ub[WorkingSet.indexFixed[idx] - 1] -
          xk[WorkingSet.indexFixed[idx] - 1];
      WorkingSet.ub[WorkingSet.indexFixed[idx] - 1] = d;
      if ((WorkingSet.indexFixed[idx] < 1) ||
          (WorkingSet.indexFixed[idx] > ub.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexFixed[idx], 1, ub.size(0),
                             w_emlrtBCI);
      }
      if ((WorkingSet.indexFixed[idx] < 1) ||
          (WorkingSet.indexFixed[idx] > xk.size(0))) {
        rtDynamicBoundsError(WorkingSet.indexFixed[idx], 1, xk.size(0),
                             w_emlrtBCI);
      }
      i = WorkingSet.bwset.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
      }
      WorkingSet.bwset[idx] = d;
    }
  }
  i = mFixed + mEq;
  if (WorkingSet.nActiveConstr > i) {
    int b;
    int ineqStart;
    ineqStart = i + 1;
    if (ineqStart < 1) {
      ineqStart = 1;
    }
    b = WorkingSet.nActiveConstr;
    if ((ineqStart <= WorkingSet.nActiveConstr) &&
        (WorkingSet.nActiveConstr > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx{ineqStart}; idx <= b; idx++) {
      i = WorkingSet.Wlocalidx.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
      }
      i = WorkingSet.Wid.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
      }
      switch (WorkingSet.Wid[idx - 1]) {
      case 4:
        i = WorkingSet.indexLB.size(0);
        i1 = WorkingSet.Wlocalidx[idx - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(WorkingSet.Wlocalidx[idx - 1], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.lb.size(0);
        i1 = WorkingSet.indexLB[i1 - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        i = WorkingSet.bwset.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
        }
        WorkingSet.bwset[idx - 1] = WorkingSet.lb[i1 - 1];
        break;
      case 5:
        i = WorkingSet.indexUB.size(0);
        i1 = WorkingSet.Wlocalidx[idx - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(WorkingSet.Wlocalidx[idx - 1], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.ub.size(0);
        i1 = WorkingSet.indexUB[WorkingSet.Wlocalidx[idx - 1] - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        i = WorkingSet.bwset.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
        }
        WorkingSet.bwset[idx - 1] = WorkingSet.ub[i1 - 1];
        break;
      default:
        i = WorkingSet.bineq.size(0);
        i1 = WorkingSet.Wlocalidx[idx - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(WorkingSet.Wlocalidx[idx - 1], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.bwset.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
        }
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
