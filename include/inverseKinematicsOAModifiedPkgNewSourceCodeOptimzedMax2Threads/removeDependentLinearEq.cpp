//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeDependentLinearEq.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "removeDependentLinearEq.h"
#include "ComputeNumDependentEq_.h"
#include "countsort.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Aeq
//                double beq_data[]
//                const int &beq_size
//                const array<double, 1U> &bnd
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
                            const int &beq_size, const array<double, 1U> &bnd,
                            array<int, 1U> &idxArray, h_struct_T &memspace,
                            j_struct_T &b_TrialState, d_struct_T &WorkingSet,
                            e_struct_T &b_QRManager, g_struct_T &QPObjective)
{
  int mFixed;
  int mLinEq;
  int mTotalLinEq;
  int nDepInd;
  int nVar;
  nVar = WorkingSet.nVar;
  mFixed = WorkingSet.sizes[0];
  mLinEq = beq_size - 1;
  mTotalLinEq = beq_size + WorkingSet.sizes[0];
  nDepInd = 0;
  if (beq_size > 0) {
    int colOffsetEqRead;
    int idx;
    int idxQR;
    int idx_local;
    for (idx_local = 0; idx_local < mFixed; idx_local++) {
      idx = 1;
      idxQR = idx_local;
      while (idx < WorkingSet.indexFixed[idx_local]) {
        b_QRManager.QR[idxQR] = 0.0;
        idx++;
        idxQR += b_QRManager.ldq;
      }
      b_QRManager.QR[idxQR] = 1.0;
      while (idx + 1 <= nVar) {
        idxQR += b_QRManager.ldq;
        b_QRManager.QR[idxQR] = 0.0;
        idx++;
      }
      WorkingSet.bwset[idx_local] = bnd[WorkingSet.indexFixed[idx_local] - 1];
    }
    for (idx_local = 0; idx_local <= mLinEq; idx_local++) {
      idxQR = mFixed + idx_local;
      for (colOffsetEqRead = 0; colOffsetEqRead < nVar; colOffsetEqRead++) {
        b_QRManager.QR[idxQR + colOffsetEqRead * b_QRManager.ldq] =
            Aeq[idx_local + colOffsetEqRead * (mLinEq + 1)];
      }
      WorkingSet.bwset[idxQR] = beq_data[idx_local];
    }
    nDepInd = qpactiveset::initialize::ComputeNumDependentEq_(
        b_QRManager, WorkingSet.bwset, mTotalLinEq, WorkingSet.nVar);
    if (nDepInd > 0) {
      int colOffsetEqWrite;
      for (idx_local = 0; idx_local < mFixed; idx_local++) {
        idx = 1;
        idxQR = b_QRManager.ldq * idx_local;
        while (idx < WorkingSet.indexFixed[idx_local]) {
          b_QRManager.QR[(idx + idxQR) - 1] = 0.0;
          idx++;
        }
        b_QRManager.QR[(idx + idxQR) - 1] = 1.0;
        while (idx + 1 <= nVar) {
          b_QRManager.QR[idx + idxQR] = 0.0;
          idx++;
        }
      }
      for (idx_local = 0; idx_local <= mLinEq; idx_local++) {
        idxQR = WorkingSet.ldA * idx_local;
        colOffsetEqWrite = b_QRManager.ldq * (mFixed + idx_local);
        for (colOffsetEqRead = 0; colOffsetEqRead < nVar; colOffsetEqRead++) {
          b_QRManager.QR[colOffsetEqWrite + colOffsetEqRead] =
              WorkingSet.Aeq[idxQR + colOffsetEqRead];
        }
      }
      idxQR = WorkingSet.sizes[0];
      if (static_cast<int>(idxQR < 200)) {
        for (int b_idx{0}; b_idx < idxQR; b_idx++) {
          b_QRManager.jpvt[b_idx] = 1;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int b_idx = 0; b_idx < idxQR; b_idx++) {
          b_QRManager.jpvt[b_idx] = 1;
        }
      }
      idx_local = WorkingSet.sizes[0] + 1;
      if (static_cast<int>((mTotalLinEq - idx_local) + 1 < 200)) {
        for (int b_idx{idx_local}; b_idx <= mTotalLinEq; b_idx++) {
          b_QRManager.jpvt[b_idx - 1] = 0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int b_idx = idx_local; b_idx <= mTotalLinEq; b_idx++) {
          b_QRManager.jpvt[b_idx - 1] = 0;
        }
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
      if (static_cast<int>(nDepInd < 200)) {
        for (int b_idx{0}; b_idx < nDepInd; b_idx++) {
          idxArray[b_idx] = b_QRManager.jpvt[(mTotalLinEq - nDepInd) + b_idx];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int b_idx = 0; b_idx < nDepInd; b_idx++) {
          idxArray[b_idx] = b_QRManager.jpvt[(mTotalLinEq - nDepInd) + b_idx];
        }
      }
      utils::countsort(idxArray, nDepInd, memspace.workspace_sort, 1,
                       mTotalLinEq);
      for (idx = nDepInd; idx >= 1; idx--) {
        idx_local = idxArray[idx - 1];
        idxQR = (mLinEq + idx) - nDepInd;
        if (idx_local < idxQR + 1) {
          colOffsetEqWrite = WorkingSet.ldA * (idx_local - 1);
          colOffsetEqRead = WorkingSet.ldA * idxQR;
          for (mTotalLinEq = 0; mTotalLinEq < nVar; mTotalLinEq++) {
            WorkingSet.Aeq[mTotalLinEq + colOffsetEqWrite] =
                WorkingSet.Aeq[mTotalLinEq + colOffsetEqRead];
          }
          beq_data[idx_local - 1] = beq_data[idxQR];
        }
      }
      idxQR = WorkingSet.sizes[1] - nDepInd;
      colOffsetEqWrite = WorkingSet.sizes[2];
      colOffsetEqRead = WorkingSet.sizes[3];
      mTotalLinEq = WorkingSet.sizes[4];
      WorkingSet.sizes[1] = idxQR;
      WorkingSet.sizesPhaseOne[0] = mFixed;
      WorkingSet.sizesPhaseOne[1] = idxQR;
      WorkingSet.sizesPhaseOne[2] = colOffsetEqWrite;
      WorkingSet.sizesPhaseOne[3] = colOffsetEqRead + 1;
      WorkingSet.sizesPhaseOne[4] = mTotalLinEq;
      WorkingSet.sizesRegularized[0] = mFixed;
      WorkingSet.sizesRegularized[1] = idxQR;
      WorkingSet.sizesRegularized[2] = colOffsetEqWrite;
      idx_local = (colOffsetEqRead + colOffsetEqWrite) + (idxQR << 1);
      WorkingSet.sizesRegularized[3] = idx_local;
      WorkingSet.sizesRegularized[4] = mTotalLinEq;
      WorkingSet.sizesRegPhaseOne[0] = mFixed;
      WorkingSet.sizesRegPhaseOne[1] = idxQR;
      WorkingSet.sizesRegPhaseOne[2] = colOffsetEqWrite;
      WorkingSet.sizesRegPhaseOne[3] = idx_local + 1;
      WorkingSet.sizesRegPhaseOne[4] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      WorkingSet.sizesNormal[0] = WorkingSet.sizes[0];
      WorkingSet.isActiveIdxRegPhaseOne[1] +=
          WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.sizesNormal[1] = WorkingSet.sizes[1];
      WorkingSet.isActiveIdxRegPhaseOne[2] +=
          WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.sizesNormal[2] = WorkingSet.sizes[2];
      WorkingSet.isActiveIdxRegPhaseOne[3] +=
          WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.sizesNormal[3] = WorkingSet.sizes[3];
      WorkingSet.isActiveIdxRegPhaseOne[4] +=
          WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.sizesNormal[4] = WorkingSet.sizes[4];
      WorkingSet.isActiveIdxRegPhaseOne[5] +=
          WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdx[0] = WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxNormal[0] = WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdx[1] = WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxNormal[1] = WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdx[2] = WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxNormal[2] = WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdx[3] = WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxNormal[3] = WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdx[4] = WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdxNormal[4] = WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdx[5] = WorkingSet.isActiveIdxRegPhaseOne[5];
      WorkingSet.isActiveIdxNormal[5] = WorkingSet.isActiveIdxRegPhaseOne[5];
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[1] +=
          WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxRegPhaseOne[2] +=
          WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxRegPhaseOne[3] +=
          WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxRegPhaseOne[4] +=
          WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxRegPhaseOne[5] +=
          WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdxPhaseOne[0] = WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxPhaseOne[1] = WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxPhaseOne[2] = WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxPhaseOne[3] = WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxPhaseOne[4] = WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdxPhaseOne[5] = WorkingSet.isActiveIdxRegPhaseOne[5];
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = idx_local;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[1] +=
          WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxRegPhaseOne[2] +=
          WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxRegPhaseOne[3] +=
          WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxRegPhaseOne[4] +=
          WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxRegPhaseOne[5] +=
          WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdxRegularized[0] =
          WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxRegularized[1] =
          WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxRegularized[2] =
          WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxRegularized[3] =
          WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxRegularized[4] =
          WorkingSet.isActiveIdxRegPhaseOne[4];
      WorkingSet.isActiveIdxRegularized[5] =
          WorkingSet.isActiveIdxRegPhaseOne[5];
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = idx_local + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[1] +=
          WorkingSet.isActiveIdxRegPhaseOne[0];
      WorkingSet.isActiveIdxRegPhaseOne[2] +=
          WorkingSet.isActiveIdxRegPhaseOne[1];
      WorkingSet.isActiveIdxRegPhaseOne[3] +=
          WorkingSet.isActiveIdxRegPhaseOne[2];
      WorkingSet.isActiveIdxRegPhaseOne[4] +=
          WorkingSet.isActiveIdxRegPhaseOne[3];
      WorkingSet.isActiveIdxRegPhaseOne[5] +=
          WorkingSet.isActiveIdxRegPhaseOne[4];
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
