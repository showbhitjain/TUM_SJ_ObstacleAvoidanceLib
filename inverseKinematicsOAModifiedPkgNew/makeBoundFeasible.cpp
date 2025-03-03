//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: makeBoundFeasible.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "makeBoundFeasible.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 1U> &xk
//                const d_struct_T &WorkingSet
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void makeBoundFeasible(array<double, 1U> &xk, const d_struct_T &WorkingSet,
                       const array<double, 2U> &lb, const array<double, 2U> &ub)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "makeBoundFeasible", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "makeBoundFeasible.p", // pName
      0                      // checkKind
  };
  double d;
  int i;
  int mLB;
  int mUB;
  boolean_T b;
  mLB = WorkingSet.sizes[3] - 1;
  mUB = WorkingSet.sizes[4] - 1;
  if (lb.size(0) != 0) {
    if (ub.size(0) == 0) {
      if (WorkingSet.sizes[3] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mLB; idx++) {
        i = WorkingSet.indexLB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = xk.size(0);
        b = ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.lb.size(0);
        b = ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
        }
        d = WorkingSet.lb[WorkingSet.indexLB[idx] - 1];
        if (-xk[WorkingSet.indexLB[idx] - 1] > d) {
          xk[WorkingSet.indexLB[idx] - 1] = -d + std::abs(d);
        }
      }
    } else {
      if (WorkingSet.sizes[3] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mLB; idx++) {
        i = WorkingSet.indexLB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = xk.size(0);
        b = ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.lb.size(0);
        b = ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
        }
        d = WorkingSet.lb[WorkingSet.indexLB[idx] - 1];
        if (-xk[WorkingSet.indexLB[idx] - 1] > d) {
          if ((WorkingSet.indexLB[idx] < 1) ||
              (WorkingSet.indexLB[idx] > ub.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, ub.size(0),
                                 w_emlrtBCI);
          }
          if (std::isinf(ub[WorkingSet.indexLB[idx] - 1])) {
            xk[WorkingSet.indexLB[idx] - 1] = -d + std::abs(d);
          } else {
            i = WorkingSet.ub.size(0);
            if ((WorkingSet.indexLB[idx] < 1) ||
                (WorkingSet.indexLB[idx] > i)) {
              rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, w_emlrtBCI);
            }
            xk[WorkingSet.indexLB[idx] - 1] =
                (WorkingSet.ub[WorkingSet.indexLB[idx] - 1] - d) / 2.0;
          }
        }
      }
    }
  }
  if (ub.size(0) != 0) {
    if (lb.size(0) == 0) {
      if (WorkingSet.sizes[4] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mUB; idx++) {
        i = WorkingSet.indexUB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = xk.size(0);
        b = ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.ub.size(0);
        b = ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
        }
        d = WorkingSet.ub[WorkingSet.indexUB[idx] - 1];
        if (xk[WorkingSet.indexUB[idx] - 1] > d) {
          xk[WorkingSet.indexUB[idx] - 1] = d - std::abs(d);
        }
      }
    } else {
      if (WorkingSet.sizes[4] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mUB; idx++) {
        i = WorkingSet.indexUB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = xk.size(0);
        b = ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
        }
        i = WorkingSet.ub.size(0);
        b = ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i));
        if (b) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
        }
        d = WorkingSet.ub[WorkingSet.indexUB[idx] - 1];
        if (xk[WorkingSet.indexUB[idx] - 1] > d) {
          if ((WorkingSet.indexUB[idx] < 1) ||
              (WorkingSet.indexUB[idx] > lb.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, lb.size(0),
                                 w_emlrtBCI);
          }
          if (std::isinf(lb[WorkingSet.indexUB[idx] - 1])) {
            xk[WorkingSet.indexUB[idx] - 1] = d - std::abs(d);
          } else {
            i = WorkingSet.lb.size(0);
            if ((WorkingSet.indexUB[idx] < 1) ||
                (WorkingSet.indexUB[idx] > i)) {
              rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, w_emlrtBCI);
            }
            xk[WorkingSet.indexUB[idx] - 1] =
                (d - WorkingSet.lb[WorkingSet.indexUB[idx] - 1]) / 2.0;
          }
        }
      }
    }
  }
}

} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for makeBoundFeasible.cpp
//
// [EOF]
//
