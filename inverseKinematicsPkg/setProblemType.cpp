//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: setProblemType.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "setProblemType.h"
#include "inverseKinematics_internal_types.h"
#include "modifyOverheadPhaseOne_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : i_struct_T &obj
//                int PROBLEM_TYPE
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void setProblemType(i_struct_T &obj, int PROBLEM_TYPE)
{
  switch (PROBLEM_TYPE) {
  case 3: {
    int i;
    obj.nVar = obj.nVarOrig;
    obj.mConstr = obj.mConstrOrig;
    if (obj.nWConstr[4] > 0) {
      i = obj.sizesNormal[4];
      for (int offsetEq1{0}; offsetEq1 < i; offsetEq1++) {
        obj.isActiveConstr[(obj.isActiveIdxNormal[4] + offsetEq1) - 1] =
            obj.isActiveConstr[(obj.isActiveIdx[4] + offsetEq1) - 1];
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
  case 2: {
    int i;
    obj.nVar = obj.nVarMax - 1;
    obj.mConstr = obj.mConstrMax - 1;
    for (i = 0; i < 5; i++) {
      obj.sizes[i] = obj.sizesRegularized[i];
    }
    if (obj.probType != 4) {
      int colOffsetATw;
      int i1;
      int i2;
      int mEq;
      int offsetEq1;
      int offsetEq2;
      int offsetIneq_tmp;
      mEq = obj.sizes[1];
      offsetIneq_tmp = obj.nVarOrig + 1;
      offsetEq1 = obj.nVarOrig;
      offsetEq2 = obj.nVarOrig + obj.sizes[1];
      i = obj.sizes[0];
      for (int idx_col{0}; idx_col < i; idx_col++) {
        colOffsetATw = obj.ldA * idx_col;
        i1 = obj.nVar;
        for (int idx_row{offsetIneq_tmp}; idx_row <= i1; idx_row++) {
          obj.ATwset[(idx_row + colOffsetATw) - 1] = 0.0;
        }
      }
      for (int idx_col{0}; idx_col < mEq; idx_col++) {
        int colOffsetAeq;
        colOffsetAeq = obj.ldA * idx_col - 1;
        colOffsetATw = colOffsetAeq + obj.ldA * (obj.isActiveIdx[1] - 1);
        for (int idx_row{offsetIneq_tmp}; idx_row <= offsetEq1; idx_row++) {
          obj.Aeq[idx_row + colOffsetAeq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        i = offsetEq1 + 1;
        i1 = (offsetEq1 + idx_col) + 1;
        i2 = i1 - 1;
        for (int idx_row{i}; idx_row <= i2; idx_row++) {
          obj.Aeq[idx_row + colOffsetAeq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        obj.Aeq[i1 + colOffsetAeq] = -1.0;
        obj.ATwset[i1 + colOffsetATw] = -1.0;
        i = i1 + 1;
        for (int idx_row{i}; idx_row <= offsetEq2; idx_row++) {
          obj.Aeq[idx_row + colOffsetAeq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        i = offsetEq2 + 1;
        i1 = (offsetEq2 + idx_col) + 1;
        i2 = i1 - 1;
        for (int idx_row{i}; idx_row <= i2; idx_row++) {
          obj.Aeq[idx_row + colOffsetAeq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        obj.Aeq[i1 + colOffsetAeq] = 1.0;
        obj.ATwset[i1 + colOffsetATw] = 1.0;
        i = i1 + 1;
        i1 = obj.nVar;
        for (int idx_row{i}; idx_row <= i1; idx_row++) {
          obj.Aeq[idx_row + colOffsetAeq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
      }
      mEq = obj.nVarOrig;
      i = obj.sizesNormal[3] + 1;
      i1 = obj.sizesRegularized[3];
      for (offsetEq1 = i; offsetEq1 <= i1; offsetEq1++) {
        mEq++;
        obj.indexLB[offsetEq1 - 1] = mEq;
      }
      if (obj.nWConstr[4] > 0) {
        i = obj.sizesRegularized[4];
        for (offsetEq1 = 0; offsetEq1 < i; offsetEq1++) {
          obj.isActiveConstr[obj.isActiveIdxRegularized[4] + offsetEq1] =
              obj.isActiveConstr[(obj.isActiveIdx[4] + offsetEq1) - 1];
        }
      }
      i = obj.isActiveIdx[4];
      i1 = obj.isActiveIdxRegularized[4] - 1;
      for (offsetEq1 = i; offsetEq1 <= i1; offsetEq1++) {
        obj.isActiveConstr[offsetEq1 - 1] = false;
      }
      i = obj.nVarOrig + (obj.sizes[1] << 1);
      for (offsetEq1 = offsetIneq_tmp; offsetEq1 <= i; offsetEq1++) {
        obj.lb[offsetEq1 - 1] = 0.0;
      }
      mEq = obj.isActiveIdx[2];
      i = obj.nActiveConstr;
      for (int idx_col{mEq}; idx_col <= i; idx_col++) {
        colOffsetATw = obj.ldA * (idx_col - 1) - 1;
        if (obj.Wid[idx_col - 1] == 3) {
          i1 = offsetIneq_tmp + obj.Wlocalidx[idx_col - 1];
          i2 = i1 - 2;
          for (int idx_row{offsetIneq_tmp}; idx_row <= i2; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
          obj.ATwset[(i1 + colOffsetATw) - 1] = -1.0;
          i2 = obj.nVar;
          for (int idx_row{i1}; idx_row <= i2; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
        } else {
          i1 = obj.nVar;
          for (int idx_row{offsetIneq_tmp}; idx_row <= i1; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
        }
      }
    }
    for (i = 0; i < 6; i++) {
      obj.isActiveIdx[i] = obj.isActiveIdxRegularized[i];
    }
  } break;
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
