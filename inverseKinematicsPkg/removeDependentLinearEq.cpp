//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeDependentLinearEq.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "removeDependentLinearEq.h"
#include "ComputeNumDependentEq_.h"
#include "countsort.h"
#include "inverseKinematics_internal_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Aeq
//                double beq_data[]
//                const int &beq_size
//                const array<double, 1U> &bnd
//                array<int, 1U> &idxArray
//                g_struct_T &memspace
//                h_struct_T &b_TrialState
//                i_struct_T &WorkingSet
//                d_struct_T &b_QRManager
//                f_struct_T &QPObjective
// Return Type  : int
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace internal {
int removeDependentLinearEq(const array<double, 2U> &Aeq, double beq_data[],
                            const int &beq_size, const array<double, 1U> &bnd,
                            array<int, 1U> &idxArray, g_struct_T &memspace,
                            h_struct_T &b_TrialState, i_struct_T &WorkingSet,
                            d_struct_T &b_QRManager, f_struct_T &QPObjective)
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
    int idx_local;
    int k;
    for (idx_local = 0; idx_local < mFixed; idx_local++) {
      idx = 1;
      idxQR = idx_local;
      while (idx < WorkingSet.indexFixed[idx_local]) {
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
      WorkingSet.bwset[idx_local] = bnd[WorkingSet.indexFixed[idx_local] - 1];
    }
    for (idx_local = 0; idx_local <= mLinEq; idx_local++) {
      idxQR = mFixed + idx_local;
      for (k = 0; k < nVar_tmp; k++) {
        b_QRManager.QR[idxQR + k * b_QRManager.ldq] =
            Aeq[idx_local + k * (mLinEq + 1)];
      }
      WorkingSet.bwset[idxQR] = beq_data[idx_local];
    }
    nDepInd = qpactiveset::initialize::ComputeNumDependentEq_(
        b_QRManager, WorkingSet.bwset, mTotalLinEq, WorkingSet.nVar);
    if (nDepInd > 0) {
      int colOffsetEqRead;
      for (idx_local = 0; idx_local < mFixed; idx_local++) {
        idx = 1;
        idxQR = b_QRManager.ldq * idx_local;
        while (idx < WorkingSet.indexFixed[idx_local]) {
          b_QRManager.QR[(idx + idxQR) - 1] = 0.0;
          idx++;
        }
        b_QRManager.QR[(idx + idxQR) - 1] = 1.0;
        while (idx + 1 <= nVar_tmp) {
          b_QRManager.QR[idx + idxQR] = 0.0;
          idx++;
        }
      }
      for (idx_local = 0; idx_local <= mLinEq; idx_local++) {
        idxQR = WorkingSet.ldA * idx_local;
        colOffsetEqRead = b_QRManager.ldq * (mFixed + idx_local);
        for (k = 0; k < nVar_tmp; k++) {
          b_QRManager.QR[colOffsetEqRead + k] = WorkingSet.Aeq[idxQR + k];
        }
      }
      idxQR = WorkingSet.sizes[0];
      for (idx = 0; idx < idxQR; idx++) {
        b_QRManager.jpvt[idx] = 1;
      }
      idx_local = WorkingSet.sizes[0] + 1;
      for (idx = idx_local; idx <= mTotalLinEq; idx++) {
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
        idx_local = idxArray[idx - 1];
        k = (mLinEq + idx) - nDepInd;
        if (idx_local < k + 1) {
          idxQR = WorkingSet.ldA * (idx_local - 1);
          colOffsetEqRead = WorkingSet.ldA * k;
          for (mTotalLinEq = 0; mTotalLinEq < nVar_tmp; mTotalLinEq++) {
            WorkingSet.Aeq[mTotalLinEq + idxQR] =
                WorkingSet.Aeq[mTotalLinEq + colOffsetEqRead];
          }
          beq_data[idx_local - 1] = beq_data[k];
        }
      }
      idxQR = WorkingSet.sizes[1] - nDepInd;
      colOffsetEqRead = WorkingSet.sizes[3];
      mTotalLinEq = WorkingSet.sizes[4];
      WorkingSet.sizes[1] = idxQR;
      WorkingSet.sizes[2] = 0;
      WorkingSet.sizesPhaseOne[0] = mFixed;
      WorkingSet.sizesPhaseOne[1] = idxQR;
      WorkingSet.sizesPhaseOne[2] = 0;
      WorkingSet.sizesPhaseOne[3] = colOffsetEqRead + 1;
      WorkingSet.sizesPhaseOne[4] = mTotalLinEq;
      WorkingSet.sizesRegularized[0] = mFixed;
      WorkingSet.sizesRegularized[1] = idxQR;
      WorkingSet.sizesRegularized[2] = 0;
      idx_local = colOffsetEqRead + (idxQR << 1);
      WorkingSet.sizesRegularized[3] = idx_local;
      WorkingSet.sizesRegularized[4] = mTotalLinEq;
      WorkingSet.sizesRegPhaseOne[0] = mFixed;
      WorkingSet.sizesRegPhaseOne[1] = idxQR;
      WorkingSet.sizesRegPhaseOne[2] = 0;
      WorkingSet.sizesRegPhaseOne[3] = idx_local + 1;
      WorkingSet.sizesRegPhaseOne[4] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
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
      WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
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
      WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
      WorkingSet.isActiveIdxRegPhaseOne[4] = idx_local;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
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
      WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
      WorkingSet.isActiveIdxRegPhaseOne[4] = idx_local + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
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
