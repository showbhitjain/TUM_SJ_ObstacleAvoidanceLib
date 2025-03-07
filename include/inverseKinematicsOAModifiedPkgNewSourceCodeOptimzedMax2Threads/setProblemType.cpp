//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: setProblemType.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "setProblemType.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "modifyOverheadPhaseOne_.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
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
    obj.nVar = obj.nVarOrig;
    obj.mConstr = obj.mConstrOrig;
    if (obj.nWConstr[4] > 0) {
      int i;
      i = obj.sizesNormal[4];
      for (int mIneq{0}; mIneq < i; mIneq++) {
        obj.isActiveConstr[(obj.isActiveIdxNormal[4] + mIneq) - 1] =
            obj.isActiveConstr[(obj.isActiveIdx[4] + mIneq) - 1];
      }
    }
    obj.sizes[0] = obj.sizesNormal[0];
    obj.sizes[1] = obj.sizesNormal[1];
    obj.sizes[2] = obj.sizesNormal[2];
    obj.sizes[3] = obj.sizesNormal[3];
    obj.sizes[4] = obj.sizesNormal[4];
    obj.isActiveIdx[0] = obj.isActiveIdxNormal[0];
    obj.isActiveIdx[1] = obj.isActiveIdxNormal[1];
    obj.isActiveIdx[2] = obj.isActiveIdxNormal[2];
    obj.isActiveIdx[3] = obj.isActiveIdxNormal[3];
    obj.isActiveIdx[4] = obj.isActiveIdxNormal[4];
    obj.isActiveIdx[5] = obj.isActiveIdxNormal[5];
  } break;
  case 1:
    obj.nVar = obj.nVarOrig + 1;
    obj.mConstr = obj.mConstrOrig + 1;
    obj.sizes[0] = obj.sizesPhaseOne[0];
    obj.sizes[1] = obj.sizesPhaseOne[1];
    obj.sizes[2] = obj.sizesPhaseOne[2];
    obj.sizes[3] = obj.sizesPhaseOne[3];
    obj.sizes[4] = obj.sizesPhaseOne[4];
    obj.isActiveIdx[0] = obj.isActiveIdxPhaseOne[0];
    obj.isActiveIdx[1] = obj.isActiveIdxPhaseOne[1];
    obj.isActiveIdx[2] = obj.isActiveIdxPhaseOne[2];
    obj.isActiveIdx[3] = obj.isActiveIdxPhaseOne[3];
    obj.isActiveIdx[4] = obj.isActiveIdxPhaseOne[4];
    obj.isActiveIdx[5] = obj.isActiveIdxPhaseOne[5];
    modifyOverheadPhaseOne_(obj);
    break;
  case 2: {
    obj.nVar = obj.nVarMax - 1;
    obj.mConstr = obj.mConstrMax - 1;
    obj.sizes[0] = obj.sizesRegularized[0];
    obj.sizes[1] = obj.sizesRegularized[1];
    obj.sizes[2] = obj.sizesRegularized[2];
    obj.sizes[3] = obj.sizesRegularized[3];
    obj.sizes[4] = obj.sizesRegularized[4];
    if (obj.probType != 4) {
      int colOffsetATw;
      int colOffsetAineq;
      int i;
      int i1;
      int mEq;
      int mIneq;
      int offsetEq1_tmp_tmp;
      int offsetEq2;
      int offsetIneq;
      mIneq = obj.sizes[2];
      mEq = obj.sizes[1];
      offsetIneq = obj.nVarOrig + 1;
      offsetEq1_tmp_tmp = obj.nVarOrig + obj.sizes[2];
      offsetEq2 = offsetEq1_tmp_tmp + obj.sizes[1];
      i = obj.sizes[0];
      for (int idx_col{0}; idx_col < i; idx_col++) {
        colOffsetATw = obj.ldA * idx_col;
        i1 = obj.nVar;
        for (int idx_row{offsetIneq}; idx_row <= i1; idx_row++) {
          obj.ATwset[(idx_row + colOffsetATw) - 1] = 0.0;
        }
      }
      for (int idx_col{0}; idx_col < mIneq; idx_col++) {
        colOffsetAineq = obj.ldA * idx_col - 1;
        i = offsetIneq + idx_col;
        i1 = i - 1;
        for (int idx_row{offsetIneq}; idx_row <= i1; idx_row++) {
          obj.Aineq[idx_row + colOffsetAineq] = 0.0;
        }
        obj.Aineq[i + colOffsetAineq] = -1.0;
        i++;
        i1 = obj.nVar;
        for (int idx_row{i}; idx_row <= i1; idx_row++) {
          obj.Aineq[idx_row + colOffsetAineq] = 0.0;
        }
      }
      for (int idx_col{0}; idx_col < mEq; idx_col++) {
        colOffsetAineq = obj.ldA * idx_col - 1;
        colOffsetATw = colOffsetAineq + obj.ldA * (obj.isActiveIdx[1] - 1);
        for (int idx_row{offsetIneq}; idx_row <= offsetEq1_tmp_tmp; idx_row++) {
          obj.Aeq[idx_row + colOffsetAineq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        i = offsetEq1_tmp_tmp + 1;
        i1 = (offsetEq1_tmp_tmp + idx_col) + 1;
        mIneq = i1 - 1;
        for (int idx_row{i}; idx_row <= mIneq; idx_row++) {
          obj.Aeq[idx_row + colOffsetAineq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        obj.Aeq[i1 + colOffsetAineq] = -1.0;
        obj.ATwset[i1 + colOffsetATw] = -1.0;
        i = i1 + 1;
        for (int idx_row{i}; idx_row <= offsetEq2; idx_row++) {
          obj.Aeq[idx_row + colOffsetAineq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        i = offsetEq2 + 1;
        i1 = (offsetEq2 + idx_col) + 1;
        mIneq = i1 - 1;
        for (int idx_row{i}; idx_row <= mIneq; idx_row++) {
          obj.Aeq[idx_row + colOffsetAineq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
        obj.Aeq[i1 + colOffsetAineq] = 1.0;
        obj.ATwset[i1 + colOffsetATw] = 1.0;
        i = i1 + 1;
        i1 = obj.nVar;
        for (int idx_row{i}; idx_row <= i1; idx_row++) {
          obj.Aeq[idx_row + colOffsetAineq] = 0.0;
          obj.ATwset[idx_row + colOffsetATw] = 0.0;
        }
      }
      colOffsetAineq = obj.nVarOrig;
      i = obj.sizesNormal[3] + 1;
      i1 = obj.sizesRegularized[3];
      for (mIneq = i; mIneq <= i1; mIneq++) {
        colOffsetAineq++;
        obj.indexLB[mIneq - 1] = colOffsetAineq;
      }
      if (obj.nWConstr[4] > 0) {
        i = obj.sizesRegularized[4];
        for (mIneq = 0; mIneq < i; mIneq++) {
          obj.isActiveConstr[obj.isActiveIdxRegularized[4] + mIneq] =
              obj.isActiveConstr[(obj.isActiveIdx[4] + mIneq) - 1];
        }
      }
      i = obj.isActiveIdx[4];
      i1 = obj.isActiveIdxRegularized[4] - 1;
      if (static_cast<int>((i1 - i) + 1 < 200)) {
        for (int idx{i}; idx <= i1; idx++) {
          obj.isActiveConstr[idx - 1] = false;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = i; idx <= i1; idx++) {
          obj.isActiveConstr[idx - 1] = false;
        }
      }
      i = obj.nVarOrig + 1;
      i1 = (obj.nVarOrig + obj.sizes[2]) + (obj.sizes[1] << 1);
      if (static_cast<int>((i1 - i) + 1 < 200)) {
        for (int idx{i}; idx <= i1; idx++) {
          obj.lb[idx - 1] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = i; idx <= i1; idx++) {
          obj.lb[idx - 1] = 0.0;
        }
      }
      colOffsetAineq = obj.isActiveIdx[2];
      i = obj.nActiveConstr;
      for (int idx_col{colOffsetAineq}; idx_col <= i; idx_col++) {
        colOffsetATw = obj.ldA * (idx_col - 1) - 1;
        if (obj.Wid[idx_col - 1] == 3) {
          i1 = offsetIneq + obj.Wlocalidx[idx_col - 1];
          mIneq = i1 - 2;
          for (int idx_row{offsetIneq}; idx_row <= mIneq; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
          obj.ATwset[(i1 + colOffsetATw) - 1] = -1.0;
          mIneq = obj.nVar;
          for (int idx_row{i1}; idx_row <= mIneq; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
        } else {
          i1 = obj.nVar;
          for (int idx_row{offsetIneq}; idx_row <= i1; idx_row++) {
            obj.ATwset[idx_row + colOffsetATw] = 0.0;
          }
        }
      }
    }
    obj.isActiveIdx[0] = obj.isActiveIdxRegularized[0];
    obj.isActiveIdx[1] = obj.isActiveIdxRegularized[1];
    obj.isActiveIdx[2] = obj.isActiveIdxRegularized[2];
    obj.isActiveIdx[3] = obj.isActiveIdxRegularized[3];
    obj.isActiveIdx[4] = obj.isActiveIdxRegularized[4];
    obj.isActiveIdx[5] = obj.isActiveIdxRegularized[5];
  } break;
  default:
    obj.nVar = obj.nVarMax;
    obj.mConstr = obj.mConstrMax;
    obj.sizes[0] = obj.sizesRegPhaseOne[0];
    obj.sizes[1] = obj.sizesRegPhaseOne[1];
    obj.sizes[2] = obj.sizesRegPhaseOne[2];
    obj.sizes[3] = obj.sizesRegPhaseOne[3];
    obj.sizes[4] = obj.sizesRegPhaseOne[4];
    obj.isActiveIdx[0] = obj.isActiveIdxRegPhaseOne[0];
    obj.isActiveIdx[1] = obj.isActiveIdxRegPhaseOne[1];
    obj.isActiveIdx[2] = obj.isActiveIdxRegPhaseOne[2];
    obj.isActiveIdx[3] = obj.isActiveIdxRegPhaseOne[3];
    obj.isActiveIdx[4] = obj.isActiveIdxRegPhaseOne[4];
    obj.isActiveIdx[5] = obj.isActiveIdxRegPhaseOne[5];
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
