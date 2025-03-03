//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: setProblemType.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "setProblemType.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "modifyOverheadPhaseOne_.h"
#include "modifyOverheadRegularized_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int PROBLEM_TYPE
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void setProblemType(d_struct_T &obj, int PROBLEM_TYPE)
{
  switch (PROBLEM_TYPE) {
  case 3: {
    int i;
    obj.nVar = obj.nVarOrig;
    obj.mConstr = obj.mConstrOrig;
    if (obj.nWConstr[4] > 0) {
      int b;
      b = obj.sizesNormal[4];
      if (obj.sizesNormal[4] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < b; idx++) {
        int i1;
        int i2;
        i = obj.isActiveConstr.size(0);
        i1 = obj.isActiveIdx[4] + idx;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, d_emlrtBCI);
        }
        i = obj.isActiveConstr.size(0);
        i2 = obj.isActiveIdxNormal[4] + idx;
        if ((i2 < 1) || (i2 > i)) {
          rtDynamicBoundsError(i2, 1, i, d_emlrtBCI);
        }
        obj.isActiveConstr[i2 - 1] = obj.isActiveConstr[i1 - 1];
      }
    }
    for (i = 0; i < 5; i++) {
      obj.sizes[i] = obj.sizesNormal[i];
    }
    for (i = 0; i < 6; i++) {
      obj.isActiveIdx[i] = obj.isActiveIdxNormal[i];
    }
  } break;
  case 1:
    obj.nVar = obj.nVarOrig + 1;
    obj.mConstr = obj.mConstrOrig + 1;
    for (int i{0}; i < 5; i++) {
      obj.sizes[i] = obj.sizesPhaseOne[i];
    }
    for (int i{0}; i < 6; i++) {
      obj.isActiveIdx[i] = obj.isActiveIdxPhaseOne[i];
    }
    modifyOverheadPhaseOne_(obj);
    break;
  case 2:
    obj.nVar = obj.nVarMax - 1;
    obj.mConstr = obj.mConstrMax - 1;
    for (int i{0}; i < 5; i++) {
      obj.sizes[i] = obj.sizesRegularized[i];
    }
    if (obj.probType != 4) {
      modifyOverheadRegularized_(obj);
    }
    for (int i{0}; i < 6; i++) {
      obj.isActiveIdx[i] = obj.isActiveIdxRegularized[i];
    }
    break;
  default:
    obj.nVar = obj.nVarMax;
    obj.mConstr = obj.mConstrMax;
    for (int i{0}; i < 5; i++) {
      obj.sizes[i] = obj.sizesRegPhaseOne[i];
    }
    for (int i{0}; i < 6; i++) {
      obj.isActiveIdx[i] = obj.isActiveIdxRegPhaseOne[i];
    }
    modifyOverheadPhaseOne_(obj);
    break;
  }
  obj.probType = PROBLEM_TYPE;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for setProblemType.cpp
//
// [EOF]
//
