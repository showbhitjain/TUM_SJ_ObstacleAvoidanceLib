//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeDependentLinearEq.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "removeDependentLinearEq.h"
#include "ComputeNumDependentEq_.h"
#include "countsort.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Aeq
//                double beq_data[]
//                const int &beq_size
//                const array<double, 2U> &bnd
//                array<int, 1U> &idxArray
//                h_struct_T &memspace
//                j_struct_T &b_TrialState
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                g_struct_T &QPObjective
// Return Type  : int
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace internal {
int removeDependentLinearEq(const array<double, 2U> &Aeq, double beq_data[],
                            const int &beq_size, const array<double, 2U> &bnd,
                            array<int, 1U> &idxArray, h_struct_T &memspace,
                            j_struct_T &b_TrialState, d_struct_T &WorkingSet,
                            e_struct_T &b_QRManager, g_struct_T &QPObjective)
{
  int mFixed;
  int mLinEq;
  int mTotalLinEq;
  int nDepInd;
  int nVar_tmp;
  nVar_tmp = WorkingSet.nVar;
  mFixed = WorkingSet.sizes[0];
  mLinEq = beq_size - 1;
  mTotalLinEq = beq_size + WorkingSet.sizes[0];
  nDepInd = 0;
  if (beq_size > 0) {
    int idx;
    int idxQR;
    int k;
    int mUB;
    for (mUB = 0; mUB < mFixed; mUB++) {
      idx = 1;
      idxQR = mUB;
      while (idx < WorkingSet.indexFixed[mUB]) {
        b_QRManager.QR[idxQR] = 0.0;
        idx++;
        idxQR += b_QRManager.ldq;
      }
      b_QRManager.QR[idxQR] = 1.0;
      while (idx + 1 <= nVar_tmp) {
        idxQR += b_QRManager.ldq;
        b_QRManager.QR[idxQR] = 0.0;
        idx++;
      }
      WorkingSet.bwset[mUB] = bnd[WorkingSet.indexFixed[mUB] - 1];
    }
    for (mUB = 0; mUB <= mLinEq; mUB++) {
      idxQR = mFixed + mUB;
      for (k = 0; k < nVar_tmp; k++) {
        b_QRManager.QR[idxQR + k * b_QRManager.ldq] =
            Aeq[mUB + k * (mLinEq + 1)];
      }
      WorkingSet.bwset[idxQR] = beq_data[mUB];
    }
    nDepInd = qpactiveset::initialize::ComputeNumDependentEq_(
        b_QRManager, WorkingSet.bwset, mTotalLinEq, WorkingSet.nVar);
    if (nDepInd > 0) {
      int colOffsetEqRead;
      int i;
      for (mUB = 0; mUB < mFixed; mUB++) {
        idx = 1;
        idxQR = b_QRManager.ldq * mUB;
        while (idx < WorkingSet.indexFixed[mUB]) {
          b_QRManager.QR[(idx + idxQR) - 1] = 0.0;
          idx++;
        }
        b_QRManager.QR[(idx + idxQR) - 1] = 1.0;
        while (idx + 1 <= nVar_tmp) {
          b_QRManager.QR[idx + idxQR] = 0.0;
          idx++;
        }
      }
      for (mUB = 0; mUB <= mLinEq; mUB++) {
        idxQR = WorkingSet.ldA * mUB;
        colOffsetEqRead = b_QRManager.ldq * (mFixed + mUB);
        for (k = 0; k < nVar_tmp; k++) {
          b_QRManager.QR[colOffsetEqRead + k] = WorkingSet.Aeq[idxQR + k];
        }
      }
      idxQR = WorkingSet.sizes[0];
      for (idx = 0; idx < idxQR; idx++) {
        b_QRManager.jpvt[idx] = 1;
      }
      i = WorkingSet.sizes[0] + 1;
      for (idx = i; idx <= mTotalLinEq; idx++) {
        b_QRManager.jpvt[idx - 1] = 0;
      }
      if (WorkingSet.nVar * mTotalLinEq == 0) {
        b_QRManager.mrows = WorkingSet.nVar;
        b_QRManager.ncols = mTotalLinEq;
        b_QRManager.minRowCol = 0;
      } else {
        b_QRManager.usedPivoting = true;
        b_QRManager.mrows = WorkingSet.nVar;
        b_QRManager.ncols = mTotalLinEq;
        idxQR = WorkingSet.nVar;
        if (idxQR > mTotalLinEq) {
          idxQR = mTotalLinEq;
        }
        b_QRManager.minRowCol = idxQR;
        ::coder::internal::lapack::xgeqp3(b_QRManager.QR, WorkingSet.nVar,
                                          mTotalLinEq, b_QRManager.jpvt,
                                          b_QRManager.tau);
      }
      for (idx = 0; idx < nDepInd; idx++) {
        idxArray[idx] = b_QRManager.jpvt[(mTotalLinEq - nDepInd) + idx];
      }
      utils::countsort(idxArray, nDepInd, memspace.workspace_sort, 1,
                       mTotalLinEq);
      for (idx = nDepInd; idx >= 1; idx--) {
        i = idxArray[idx - 1];
        k = (mLinEq + idx) - nDepInd;
        if (i < k + 1) {
          idxQR = WorkingSet.ldA * (i - 1);
          colOffsetEqRead = WorkingSet.ldA * k;
          for (mUB = 0; mUB < nVar_tmp; mUB++) {
            WorkingSet.Aeq[mUB + idxQR] = WorkingSet.Aeq[mUB + colOffsetEqRead];
          }
          beq_data[i - 1] = beq_data[k];
        }
      }
      idxQR = WorkingSet.sizes[1] - nDepInd;
      colOffsetEqRead = WorkingSet.sizes[2];
      mTotalLinEq = WorkingSet.sizes[3];
      mUB = WorkingSet.sizes[4];
      WorkingSet.sizes[1] = idxQR;
      WorkingSet.sizesPhaseOne[0] = mFixed;
      WorkingSet.sizesPhaseOne[1] = idxQR;
      WorkingSet.sizesPhaseOne[2] = colOffsetEqRead;
      WorkingSet.sizesPhaseOne[3] = mTotalLinEq + 1;
      WorkingSet.sizesPhaseOne[4] = mUB;
      WorkingSet.sizesRegularized[0] = mFixed;
      WorkingSet.sizesRegularized[1] = idxQR;
      WorkingSet.sizesRegularized[2] = colOffsetEqRead;
      i = (mTotalLinEq + colOffsetEqRead) + (idxQR << 1);
      WorkingSet.sizesRegularized[3] = i;
      WorkingSet.sizesRegularized[4] = mUB;
      WorkingSet.sizesRegPhaseOne[0] = mFixed;
      WorkingSet.sizesRegPhaseOne[1] = idxQR;
      WorkingSet.sizesRegPhaseOne[2] = colOffsetEqRead;
      WorkingSet.sizesRegPhaseOne[3] = i + 1;
      WorkingSet.sizesRegPhaseOne[4] = mUB;
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[4] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
      for (k = 0; k < 5; k++) {
        WorkingSet.sizesNormal[k] = WorkingSet.sizes[k];
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (k = 0; k < 6; k++) {
        WorkingSet.isActiveIdx[k] = WorkingSet.isActiveIdxRegPhaseOne[k];
        WorkingSet.isActiveIdxNormal[k] = WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[4] = mTotalLinEq + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
      for (k = 0; k < 5; k++) {
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (k = 0; k < 6; k++) {
        WorkingSet.isActiveIdxPhaseOne[k] =
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[4] = i;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
      for (k = 0; k < 5; k++) {
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (k = 0; k < 6; k++) {
        WorkingSet.isActiveIdxRegularized[k] =
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[4] = i + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
      for (k = 0; k < 5; k++) {
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      idxQR = nDepInd << 1;
      WorkingSet.nVarMax -= idxQR;
      WorkingSet.mConstr -= nDepInd;
      WorkingSet.mConstrMax -= 3 * nDepInd;
      WorkingSet.mConstrOrig -= nDepInd;
      QPObjective.maxVar -= idxQR;
      b_TrialState.nVarMax -= idxQR;
      b_TrialState.mEq -= nDepInd;
      b_TrialState.iNonEq0 -= nDepInd;
    }
  }
  return nDepInd;
}

} // namespace internal
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for removeDependentLinearEq.cpp
//
// [EOF]
//
