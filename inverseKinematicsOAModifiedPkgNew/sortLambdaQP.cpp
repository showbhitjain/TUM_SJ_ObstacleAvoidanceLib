//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sortLambdaQP.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "sortLambdaQP.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
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
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,             // iFirst
      -1,             // iLast
      1,              // lineNo
      1,              // colNo
      "",             // aName
      "sortLambdaQP", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+parseoutput/sortLambdaQP.p", // pName
      0                              // checkKind
  };
  if (WorkingSet_nActiveConstr != 0) {
    int currentMplier;
    int i;
    int idx;
    int mAll;
    boolean_T exitg1;
    mAll =
        (((WorkingSet_sizes[0] + WorkingSet_sizes[1]) + WorkingSet_sizes[3]) +
         WorkingSet_sizes[4]) +
        WorkingSet_sizes[2];
    if (mAll > 2147483646) {
      check_forloop_overflow_error();
    }
    for (currentMplier = 0; currentMplier < mAll; currentMplier++) {
      workspace[currentMplier] = lambda[currentMplier];
    }
    for (currentMplier = 0; currentMplier < mAll; currentMplier++) {
      lambda[currentMplier] = 0.0;
    }
    currentMplier = 1;
    idx = 1;
    exitg1 = false;
    while ((!exitg1) && (idx <= WorkingSet_nActiveConstr)) {
      if ((idx < 1) || (idx > WorkingSet_Wid.size(0))) {
        rtDynamicBoundsError(idx, 1, WorkingSet_Wid.size(0), w_emlrtBCI);
      }
      i = WorkingSet_Wid[idx - 1];
      if (i <= 2) {
        if (idx > WorkingSet_Wlocalidx.size(0)) {
          rtDynamicBoundsError(idx, 1, WorkingSet_Wlocalidx.size(0),
                               w_emlrtBCI);
        }
        if (idx > WorkingSet_Wid.size(0)) {
          rtDynamicBoundsError(idx, 1, WorkingSet_Wid.size(0), w_emlrtBCI);
        }
        if (i == 1) {
          mAll = 0;
        } else {
          mAll = WorkingSet_isActiveIdx[1] - 1;
        }
        i = workspace.size(0) * workspace.size(1);
        if ((currentMplier < 1) || (currentMplier > i)) {
          rtDynamicBoundsError(currentMplier, 1, i, w_emlrtBCI);
        }
        i = lambda.size(0);
        mAll += WorkingSet_Wlocalidx[idx - 1];
        if ((mAll < 1) || (mAll > i)) {
          rtDynamicBoundsError(mAll, 1, i, w_emlrtBCI);
        }
        lambda[mAll - 1] = workspace[currentMplier - 1];
        currentMplier++;
        idx++;
      } else {
        exitg1 = true;
      }
    }
    while (idx <= WorkingSet_nActiveConstr) {
      if ((idx < 1) || (idx > WorkingSet_Wlocalidx.size(0))) {
        rtDynamicBoundsError(idx, 1, WorkingSet_Wlocalidx.size(0), w_emlrtBCI);
      }
      if (idx > WorkingSet_Wid.size(0)) {
        rtDynamicBoundsError(idx, 1, WorkingSet_Wid.size(0), w_emlrtBCI);
      }
      switch (WorkingSet_Wid[idx - 1]) {
      case 3:
        mAll = WorkingSet_isActiveIdx[2];
        break;
      case 4:
        mAll = WorkingSet_isActiveIdx[3];
        break;
      default:
        mAll = WorkingSet_isActiveIdx[4];
        break;
      }
      i = workspace.size(0) * workspace.size(1);
      if ((currentMplier < 1) || (currentMplier > i)) {
        rtDynamicBoundsError(currentMplier, 1, i, w_emlrtBCI);
      }
      i = lambda.size(0);
      mAll = (mAll + WorkingSet_Wlocalidx[idx - 1]) - 1;
      if ((mAll < 1) || (mAll > i)) {
        rtDynamicBoundsError(mAll, 1, i, w_emlrtBCI);
      }
      lambda[mAll - 1] = workspace[currentMplier - 1];
      currentMplier++;
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
