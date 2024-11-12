//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortLambdaQP.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "sortLambdaQP.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 1U> &lambda
//                int WorkingSet_nActiveConstr
//                const int WorkingSet_sizes[5]
//                const int WorkingSet_isActiveIdx[6]
//                const array<int, 1U> &WorkingSet_Wid
//                const array<int, 1U> &WorkingSet_Wlocalidx
//                array<double, 2U> &workspace
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace parseoutput {
void sortLambdaQP(array<double, 1U> &lambda, int WorkingSet_nActiveConstr,
                  const int WorkingSet_sizes[5],
                  const int WorkingSet_isActiveIdx[6],
                  const array<int, 1U> &WorkingSet_Wid,
                  const array<int, 1U> &WorkingSet_Wlocalidx,
                  array<double, 2U> &workspace)
{
  if ((lambda.size(0) != 0) && (WorkingSet_nActiveConstr != 0)) {
    int idx;
    int idxOffset;
    int mAll;
    mAll =
        (((WorkingSet_sizes[0] + WorkingSet_sizes[1]) + WorkingSet_sizes[3]) +
         WorkingSet_sizes[4]) -
        1;
    for (idx = 0; idx <= mAll; idx++) {
      workspace[idx] = lambda[idx];
      lambda[idx] = 0.0;
    }
    mAll = 0;
    idx = 0;
    while ((idx + 1 <= WorkingSet_nActiveConstr) &&
           (WorkingSet_Wid[idx] <= 2)) {
      if (WorkingSet_Wid[idx] == 1) {
        idxOffset = 1;
      } else {
        idxOffset = WorkingSet_isActiveIdx[1];
      }
      lambda[(idxOffset + WorkingSet_Wlocalidx[idx]) - 2] = workspace[mAll];
      mAll++;
      idx++;
    }
    while (idx + 1 <= WorkingSet_nActiveConstr) {
      switch (WorkingSet_Wid[idx]) {
      case 3:
        idxOffset = WorkingSet_isActiveIdx[2];
        break;
      case 4:
        idxOffset = WorkingSet_isActiveIdx[3];
        break;
      default:
        idxOffset = WorkingSet_isActiveIdx[4];
        break;
      }
      lambda[(idxOffset + WorkingSet_Wlocalidx[idx]) - 2] = workspace[mAll];
      mAll++;
      idx++;
    }
  }
}

} // namespace parseoutput
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for sortLambdaQP.cpp
//
// [EOF]
//
