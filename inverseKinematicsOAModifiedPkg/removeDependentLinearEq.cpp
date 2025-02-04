//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeDependentLinearEq.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "removeDependentLinearEq.h"
#include "ComputeNumDependentEq_.h"
#include "IndexOfDependentEq_.h"
#include "countsort.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
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
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                        // iFirst
      -1,                        // iLast
      1,                         // lineNo
      1,                         // colNo
      "",                        // aName
      "removeDependentLinearEq", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+internal/removeDependentLinearEq.p", // pName
      0                                      // checkKind
  };
  int mFixed;
  int mLinEq;
  int mTotalLinEq;
  int nDepInd;
  int nVar_tmp;
  nVar_tmp = WorkingSet.nVar;
  mFixed = WorkingSet.sizes[0];
  mLinEq = beq_size;
  mTotalLinEq = beq_size + WorkingSet.sizes[0];
  nDepInd = 0;
  if (beq_size > 0) {
    int colOffsetEqRead;
    int i;
    int i1;
    int idx;
    int idxQR;
    int k;
    if (WorkingSet.sizes[0] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (colOffsetEqRead = 0; colOffsetEqRead < mFixed; colOffsetEqRead++) {
      i = WorkingSet.indexFixed.size(0);
      if ((colOffsetEqRead + 1 < 1) || (colOffsetEqRead + 1 > i)) {
        rtDynamicBoundsError(colOffsetEqRead + 1, 1, i, w_emlrtBCI);
      }
      idx = 1;
      idxQR = colOffsetEqRead + 1;
      while (idx < WorkingSet.indexFixed[colOffsetEqRead]) {
        i = b_QRManager.QR.size(0) * b_QRManager.QR.size(1);
        if ((idxQR < 1) || (idxQR > i)) {
          rtDynamicBoundsError(idxQR, 1, i, w_emlrtBCI);
        }
        b_QRManager.QR[idxQR - 1] = 0.0;
        idx++;
        idxQR += b_QRManager.ldq;
      }
      i = b_QRManager.QR.size(0) * b_QRManager.QR.size(1);
      if ((idxQR < 1) || (idxQR > i)) {
        rtDynamicBoundsError(idxQR, 1, i, w_emlrtBCI);
      }
      b_QRManager.QR[idxQR - 1] = 1.0;
      while (idx + 1 <= nVar_tmp) {
        idxQR += b_QRManager.ldq;
        if ((idxQR < 1) || (idxQR > i)) {
          rtDynamicBoundsError(idxQR, 1, i, w_emlrtBCI);
        }
        b_QRManager.QR[idxQR - 1] = 0.0;
        idx++;
      }
      i = WorkingSet.indexFixed[colOffsetEqRead];
      if ((i < 1) || (i > bnd.size(0))) {
        rtDynamicBoundsError(i, 1, bnd.size(0), w_emlrtBCI);
      }
      i1 = WorkingSet.bwset.size(0);
      if ((colOffsetEqRead + 1 < 1) || (colOffsetEqRead + 1 > i1)) {
        rtDynamicBoundsError(colOffsetEqRead + 1, 1, i1, w_emlrtBCI);
      }
      WorkingSet.bwset[colOffsetEqRead] = bnd[i - 1];
    }
    for (colOffsetEqRead = 0; colOffsetEqRead < mLinEq; colOffsetEqRead++) {
      idxQR = (mFixed + colOffsetEqRead) + 1;
      if (WorkingSet.nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (k = 0; k < nVar_tmp; k++) {
        b_QRManager.QR[(idxQR + k * b_QRManager.ldq) - 1] =
            Aeq[colOffsetEqRead + k * mLinEq];
      }
      i = beq_size;
      if (colOffsetEqRead + 1 > i) {
        rtDynamicBoundsError(colOffsetEqRead + 1, 1, i, w_emlrtBCI);
      }
      i = WorkingSet.bwset.size(0);
      if ((idxQR < 1) || (idxQR > i)) {
        rtDynamicBoundsError(idxQR, 1, i, w_emlrtBCI);
      }
      WorkingSet.bwset[idxQR - 1] = beq_data[colOffsetEqRead];
    }
    nDepInd = qpactiveset::initialize::ComputeNumDependentEq_(
        b_QRManager, WorkingSet.bwset, mTotalLinEq, WorkingSet.nVar);
    if (nDepInd > 0) {
      int colOffsetEqWrite;
      boolean_T overflow;
      if (WorkingSet.sizes[0] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (colOffsetEqRead = 0; colOffsetEqRead < mFixed; colOffsetEqRead++) {
        i = WorkingSet.indexFixed.size(0);
        if ((colOffsetEqRead + 1 < 1) || (colOffsetEqRead + 1 > i)) {
          rtDynamicBoundsError(colOffsetEqRead + 1, 1, i, w_emlrtBCI);
        }
        idx = 1;
        idxQR = b_QRManager.ldq * colOffsetEqRead;
        while (idx < WorkingSet.indexFixed[colOffsetEqRead]) {
          i = b_QRManager.QR.size(0) * b_QRManager.QR.size(1);
          i1 = idx + idxQR;
          if ((i1 < 1) || (i1 > i)) {
            rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
          }
          b_QRManager.QR[i1 - 1] = 0.0;
          idx++;
        }
        i = b_QRManager.QR.size(0) * b_QRManager.QR.size(1);
        i1 = idx + idxQR;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
        }
        b_QRManager.QR[i1 - 1] = 1.0;
        for (idx++; idx <= nVar_tmp; idx++) {
          i1 = idx + idxQR;
          if ((i1 < 1) || (i1 > i)) {
            rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
          }
          b_QRManager.QR[i1 - 1] = 0.0;
        }
      }
      overflow = (nVar_tmp > 2147483646);
      for (colOffsetEqRead = 0; colOffsetEqRead < mLinEq; colOffsetEqRead++) {
        idxQR = WorkingSet.ldA * colOffsetEqRead;
        colOffsetEqWrite = b_QRManager.ldq * (mFixed + colOffsetEqRead);
        if (overflow) {
          check_forloop_overflow_error();
        }
        for (k = 0; k < nVar_tmp; k++) {
          b_QRManager.QR[colOffsetEqWrite + k] = WorkingSet.Aeq[idxQR + k];
        }
      }
      qpactiveset::initialize::IndexOfDependentEq_(
          idxArray, WorkingSet.sizes[0], nDepInd, b_QRManager, WorkingSet.nVar,
          mTotalLinEq);
      utils::countsort(idxArray, nDepInd, memspace.workspace_sort, mTotalLinEq);
      i = idxArray.size(0);
      for (idx = nDepInd; idx >= 1; idx--) {
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
        }
        i1 = idxArray[idx - 1];
        idxQR = (mLinEq + idx) - nDepInd;
        if (i1 < idxQR) {
          colOffsetEqWrite = WorkingSet.ldA * (i1 - 1);
          colOffsetEqRead = WorkingSet.ldA * (idxQR - 1);
          if (nVar_tmp > 2147483646) {
            check_forloop_overflow_error();
          }
          for (int row{0}; row < nVar_tmp; row++) {
            int i2;
            mTotalLinEq = WorkingSet.Aeq.size(0);
            k = (row + colOffsetEqRead) + 1;
            if ((k < 1) || (k > mTotalLinEq)) {
              rtDynamicBoundsError(k, 1, mTotalLinEq, w_emlrtBCI);
            }
            mTotalLinEq = WorkingSet.Aeq.size(0);
            i2 = (row + colOffsetEqWrite) + 1;
            if ((i2 < 1) || (i2 > mTotalLinEq)) {
              rtDynamicBoundsError(i2, 1, mTotalLinEq, w_emlrtBCI);
            }
            WorkingSet.Aeq[i2 - 1] = WorkingSet.Aeq[k - 1];
          }
          mTotalLinEq = beq_size;
          if ((idxQR < 1) || (idxQR > mTotalLinEq)) {
            rtDynamicBoundsError(idxQR, 1, mTotalLinEq, w_emlrtBCI);
          }
          if ((i1 < 1) || (i1 > mTotalLinEq)) {
            rtDynamicBoundsError(i1, 1, mTotalLinEq, w_emlrtBCI);
          }
          beq_data[i1 - 1] = beq_data[idxQR - 1];
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
      i = (colOffsetEqRead + colOffsetEqWrite) + (idxQR << 1);
      WorkingSet.sizesRegularized[3] = i;
      WorkingSet.sizesRegularized[4] = mTotalLinEq;
      WorkingSet.sizesRegPhaseOne[0] = mFixed;
      WorkingSet.sizesRegPhaseOne[1] = idxQR;
      WorkingSet.sizesRegPhaseOne[2] = colOffsetEqWrite;
      WorkingSet.sizesRegPhaseOne[3] = i + 1;
      WorkingSet.sizesRegPhaseOne[4] = mTotalLinEq;
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      for (k = 0; k < 5; k++) {
        WorkingSet.sizesNormal[k] = WorkingSet.sizes[k];
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (i1 = 0; i1 < 6; i1++) {
        WorkingSet.isActiveIdx[i1] = WorkingSet.isActiveIdxRegPhaseOne[i1];
        WorkingSet.isActiveIdxNormal[i1] =
            WorkingSet.isActiveIdxRegPhaseOne[i1];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = colOffsetEqRead + 1;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      for (k = 0; k < 5; k++) {
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (i1 = 0; i1 < 6; i1++) {
        WorkingSet.isActiveIdxPhaseOne[i1] =
            WorkingSet.isActiveIdxRegPhaseOne[i1];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = i;
      WorkingSet.isActiveIdxRegPhaseOne[5] = mTotalLinEq;
      for (k = 0; k < 5; k++) {
        WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
            WorkingSet.isActiveIdxRegPhaseOne[k];
      }
      for (i1 = 0; i1 < 6; i1++) {
        WorkingSet.isActiveIdxRegularized[i1] =
            WorkingSet.isActiveIdxRegPhaseOne[i1];
      }
      WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
      WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
      WorkingSet.isActiveIdxRegPhaseOne[2] = idxQR;
      WorkingSet.isActiveIdxRegPhaseOne[3] = colOffsetEqWrite;
      WorkingSet.isActiveIdxRegPhaseOne[4] = i + 1;
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
