//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compressBounds.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "compressBounds.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int nVar
//                array<int, 1U> &indexLB
//                array<int, 1U> &indexUB
//                array<int, 1U> &indexFixed
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                int &mUB
//                int &mFixed
// Return Type  : int
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
int compressBounds(int nVar, array<int, 1U> &indexLB, array<int, 1U> &indexUB,
                   array<int, 1U> &indexFixed, const array<double, 2U> &lb,
                   const array<double, 2U> &ub, int &mUB, int &mFixed)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,               // iFirst
      -1,               // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "compressBounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+initialize/compressBounds.p", // pName
      0                               // checkKind
  };
  int mLB;
  mLB = 0;
  mUB = 0;
  mFixed = 0;
  if (ub.size(0) != 0) {
    if (lb.size(0) != 0) {
      if (nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < nVar; idx++) {
        double d;
        int i;
        boolean_T guard1;
        if ((idx + 1 < 1) || (idx + 1 > lb.size(0))) {
          rtDynamicBoundsError(idx + 1, 1, lb.size(0), w_emlrtBCI);
        }
        d = lb[idx];
        guard1 = false;
        if ((!std::isinf(d)) && (!std::isnan(d))) {
          if (idx + 1 > lb.size(0)) {
            rtDynamicBoundsError(idx + 1, 1, lb.size(0), w_emlrtBCI);
          }
          if (idx + 1 > ub.size(0)) {
            rtDynamicBoundsError(idx + 1, 1, ub.size(0), w_emlrtBCI);
          }
          if (std::abs(d - ub[idx]) < 1.0E-6) {
            mFixed++;
            i = indexFixed.size(0);
            if ((mFixed < 1) || (mFixed > i)) {
              rtDynamicBoundsError(mFixed, 1, i, w_emlrtBCI);
            }
            indexFixed[mFixed - 1] = idx + 1;
            if (idx + 1 > ub.size(0)) {
              rtDynamicBoundsError(idx + 1, 1, ub.size(0), w_emlrtBCI);
            }
            if (idx + 1 > lb.size(0)) {
              rtDynamicBoundsError(idx + 1, 1, lb.size(0), w_emlrtBCI);
            }
          } else {
            mLB++;
            i = indexLB.size(0);
            if ((mLB < 1) || (mLB > i)) {
              rtDynamicBoundsError(mLB, 1, i, w_emlrtBCI);
            }
            indexLB[mLB - 1] = idx + 1;
            if (idx + 1 > lb.size(0)) {
              rtDynamicBoundsError(idx + 1, 1, lb.size(0), w_emlrtBCI);
            }
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
        if (guard1) {
          if (idx + 1 > ub.size(0)) {
            rtDynamicBoundsError(idx + 1, 1, ub.size(0), w_emlrtBCI);
          }
          d = ub[idx];
          if ((!std::isinf(d)) && (!std::isnan(d))) {
            mUB++;
            i = indexUB.size(0);
            if ((mUB < 1) || (mUB > i)) {
              rtDynamicBoundsError(mUB, 1, i, w_emlrtBCI);
            }
            indexUB[mUB - 1] = idx + 1;
            if (idx + 1 > ub.size(0)) {
              rtDynamicBoundsError(idx + 1, 1, ub.size(0), w_emlrtBCI);
            }
          }
        }
      }
    } else {
      if (nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < nVar; idx++) {
        boolean_T b;
        b = ((idx + 1 < 1) || (idx + 1 > ub.size(0)));
        if (b) {
          rtDynamicBoundsError(idx + 1, 1, ub.size(0), w_emlrtBCI);
        }
        if ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx]))) {
          int i;
          mUB++;
          i = indexUB.size(0);
          if ((mUB < 1) || (mUB > i)) {
            rtDynamicBoundsError(mUB, 1, i, w_emlrtBCI);
          }
          indexUB[mUB - 1] = idx + 1;
        }
      }
    }
  } else if (lb.size(0) != 0) {
    if (nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < nVar; idx++) {
      boolean_T b;
      b = ((idx + 1 < 1) || (idx + 1 > lb.size(0)));
      if (b) {
        rtDynamicBoundsError(idx + 1, 1, lb.size(0), w_emlrtBCI);
      }
      if ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx]))) {
        int i;
        mLB++;
        i = indexLB.size(0);
        if ((mLB < 1) || (mLB > i)) {
          rtDynamicBoundsError(mLB, 1, i, w_emlrtBCI);
        }
        indexLB[mLB - 1] = idx + 1;
      }
    }
  }
  return mLB;
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for compressBounds.cpp
//
// [EOF]
//
