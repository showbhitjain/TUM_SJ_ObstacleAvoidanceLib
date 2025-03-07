//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compressBounds.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "compressBounds.h"
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
  int mLB;
  mLB = 0;
  mUB = 0;
  mFixed = 0;
  if (ub.size(0) != 0) {
    if (lb.size(0) != 0) {
      for (int idx{0}; idx < nVar; idx++) {
        double d;
        boolean_T guard1;
        d = lb[idx];
        guard1 = false;
        if ((!std::isinf(d)) && (!std::isnan(d))) {
          if (std::abs(d - ub[idx]) < 1.0E-6) {
            mFixed++;
            indexFixed[mFixed - 1] = idx + 1;
          } else {
            mLB++;
            indexLB[mLB - 1] = idx + 1;
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
        if (guard1) {
          d = ub[idx];
          if ((!std::isinf(d)) && (!std::isnan(d))) {
            mUB++;
            indexUB[mUB - 1] = idx + 1;
          }
        }
      }
    } else {
      for (int idx{0}; idx < nVar; idx++) {
        double d;
        d = ub[idx];
        if ((!std::isinf(d)) && (!std::isnan(d))) {
          mUB++;
          indexUB[mUB - 1] = idx + 1;
        }
      }
    }
  } else if (lb.size(0) != 0) {
    for (int idx{0}; idx < nVar; idx++) {
      double d;
      d = lb[idx];
      if ((!std::isinf(d)) && (!std::isnan(d))) {
        mLB++;
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
