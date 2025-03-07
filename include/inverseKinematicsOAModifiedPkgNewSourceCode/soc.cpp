//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: soc.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "soc.h"
#include "addAeqConstr.h"
#include "addBoundToActiveSetMatrix_.h"
#include "driver1.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xnrm2.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Hessian
//                const array<double, 1U> &grad
//                j_struct_T &b_TrialState
//                h_struct_T &memspace
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                f_struct_T &b_CholManager
//                g_struct_T &QPObjective
//                const k_struct_T &qpoptions
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
boolean_T soc(const array<double, 2U> &Hessian, const array<double, 1U> &grad,
              j_struct_T &b_TrialState, h_struct_T &memspace,
              d_struct_T &WorkingSet, e_struct_T &b_QRManager,
              f_struct_T &b_CholManager, g_struct_T &QPObjective,
              const k_struct_T &qpoptions)
{
  array<double, 1U> y;
  double c;
  int i;
  int i1;
  int idxIneqOffset;
  int idx_Partition;
  int idx_lower;
  int idx_upper;
  int iy;
  int mConstrMax;
  int mIneq;
  int nVar_tmp_tmp;
  int nWIneq_old;
  int nWLower_old;
  int nWUpper_old;
  boolean_T success;
  nWIneq_old = WorkingSet.nWConstr[2];
  nWLower_old = WorkingSet.nWConstr[3];
  nWUpper_old = WorkingSet.nWConstr[4];
  nVar_tmp_tmp = WorkingSet.nVar - 1;
  mConstrMax = WorkingSet.mConstrMax - 1;
  for (idx_lower = 0; idx_lower <= nVar_tmp_tmp; idx_lower++) {
    b_TrialState.xstarsqp[idx_lower] = b_TrialState.xstarsqp_old[idx_lower];
    b_TrialState.socDirection[idx_lower] = b_TrialState.xstar[idx_lower];
  }
  for (idx_lower = 0; idx_lower <= mConstrMax; idx_lower++) {
    b_TrialState.lambdaStopTest[idx_lower] = b_TrialState.lambda[idx_lower];
  }
  mIneq = WorkingSet.sizes[2];
  idxIneqOffset = WorkingSet.isActiveIdx[2];
  if (WorkingSet.sizes[1] > 0) {
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    for (int idx{0}; idx < i; idx++) {
      WorkingSet.beq.data[idx] = -b_TrialState.cEq.data[idx];
    }
    idx_Partition = WorkingSet.ldA;
    y.set_size(WorkingSet.beq.size[0]);
    idx_upper = WorkingSet.beq.size[0];
    for (i1 = 0; i1 < idx_upper; i1++) {
      y[i1] = WorkingSet.beq.data[i1];
    }
    if (WorkingSet.nVar != 0) {
      iy = 0;
      i1 = WorkingSet.ldA * (WorkingSet.sizes[1] - 1) + 1;
      for (idx_upper = 1; idx_Partition < 0 ? idx_upper >= i1 : idx_upper <= i1;
           idx_upper += idx_Partition) {
        c = 0.0;
        idx_lower = (idx_upper + WorkingSet.nVar) - 1;
        for (int idx{idx_upper}; idx <= idx_lower; idx++) {
          c +=
              WorkingSet.Aeq[idx - 1] * b_TrialState.searchDir[idx - idx_upper];
        }
        y[iy] = y[iy] + c;
        iy++;
      }
    }
    WorkingSet.beq.size[0] = y.size(0);
    idx_upper = y.size(0);
    for (i1 = 0; i1 < idx_upper; i1++) {
      WorkingSet.beq.data[i1] = y[i1];
    }
    for (idx_lower = 0; idx_lower < i; idx_lower++) {
      WorkingSet.bwset[WorkingSet.sizes[0] + idx_lower] = y[idx_lower];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    for (int idx{0}; idx < mIneq; idx++) {
      WorkingSet.bineq[idx] = -b_TrialState.cIneq[idx];
    }
    idx_Partition = WorkingSet.ldA;
    if (WorkingSet.nVar != 0) {
      iy = 0;
      i = WorkingSet.ldA * (WorkingSet.sizes[2] - 1) + 1;
      for (idx_upper = 1; idx_Partition < 0 ? idx_upper >= i : idx_upper <= i;
           idx_upper += idx_Partition) {
        c = 0.0;
        i1 = (idx_upper + WorkingSet.nVar) - 1;
        for (int idx{idx_upper}; idx <= i1; idx++) {
          c += WorkingSet.Aineq[idx - 1] *
               b_TrialState.searchDir[idx - idx_upper];
        }
        WorkingSet.bineq[iy] = WorkingSet.bineq[iy] + c;
        iy++;
      }
    }
    iy = 1;
    idx_lower = WorkingSet.sizes[2] + 1;
    idx_upper = (WorkingSet.sizes[2] + WorkingSet.sizes[3]) + 1;
    i = WorkingSet.nActiveConstr;
    for (int idx{idxIneqOffset}; idx <= i; idx++) {
      switch (WorkingSet.Wid[idx - 1]) {
      case 3:
        idx_Partition = iy;
        iy++;
        WorkingSet.bwset[idx - 1] =
            WorkingSet.bineq[WorkingSet.Wlocalidx[idx - 1] - 1];
        break;
      case 4:
        idx_Partition = idx_lower;
        idx_lower++;
        break;
      default:
        idx_Partition = idx_upper;
        idx_upper++;
        break;
      }
      b_TrialState.workingset_old[idx_Partition - 1] =
          WorkingSet.Wlocalidx[idx - 1];
    }
  }
  for (idx_lower = 0; idx_lower <= nVar_tmp_tmp; idx_lower++) {
    b_TrialState.xstar[idx_lower] = b_TrialState.xstarsqp[idx_lower];
  }
  k_struct_T b_qpoptions;
  b_qpoptions = qpoptions;
  ::coder::optim::coder::qpactiveset::driver(
      Hessian, grad, b_TrialState, memspace, WorkingSet, b_QRManager,
      b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
  if (WorkingSet.indexEqRemoved.size[0] > 0) {
    while ((WorkingSet.mEqRemoved > 0) &&
           (WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1] >=
            b_TrialState.iNonEq0)) {
      qpactiveset::WorkingSet::addAeqConstr(
          WorkingSet,
          WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1]);
      WorkingSet.mEqRemoved--;
    }
  }
  for (int idx{0}; idx <= nVar_tmp_tmp; idx++) {
    double oldDirIdx;
    c = b_TrialState.socDirection[idx];
    oldDirIdx = c;
    c = b_TrialState.xstar[idx] - c;
    b_TrialState.socDirection[idx] = c;
    b_TrialState.xstar[idx] = oldDirIdx;
  }
  success = (::coder::internal::blas::xnrm2(nVar_tmp_tmp + 1,
                                            b_TrialState.socDirection) <=
             2.0 * ::coder::internal::blas::xnrm2(nVar_tmp_tmp + 1,
                                                  b_TrialState.xstar));
  mIneq = WorkingSet.sizes[2];
  idx_Partition = WorkingSet.sizes[3];
  if (WorkingSet.sizes[1] > 0) {
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    for (int idx{0}; idx < i; idx++) {
      WorkingSet.beq.data[idx] = -b_TrialState.cEq.data[idx];
      WorkingSet.bwset[WorkingSet.sizes[0] + idx] = WorkingSet.beq.data[idx];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    for (int idx{0}; idx < mIneq; idx++) {
      WorkingSet.bineq[idx] = -b_TrialState.cIneq[idx];
    }
    if (!success) {
      idx_upper = WorkingSet.nWConstr[0] + WorkingSet.nWConstr[1];
      iy = idx_upper + 1;
      idx_lower = WorkingSet.nActiveConstr;
      for (nVar_tmp_tmp = iy; nVar_tmp_tmp <= idx_lower; nVar_tmp_tmp++) {
        WorkingSet.isActiveConstr
            [(WorkingSet.isActiveIdx[WorkingSet.Wid[nVar_tmp_tmp - 1] - 1] +
              WorkingSet.Wlocalidx[nVar_tmp_tmp - 1]) -
             2] = false;
      }
      WorkingSet.nWConstr[2] = 0;
      WorkingSet.nWConstr[3] = 0;
      WorkingSet.nWConstr[4] = 0;
      WorkingSet.nActiveConstr = idx_upper;
      for (int idx{0}; idx < nWIneq_old; idx++) {
        iy = b_TrialState.workingset_old[idx];
        WorkingSet.nWConstr[2]++;
        WorkingSet.isActiveConstr[(WorkingSet.isActiveIdx[2] + iy) - 2] = true;
        WorkingSet.nActiveConstr++;
        i = WorkingSet.nActiveConstr - 1;
        WorkingSet.Wid[i] = 3;
        WorkingSet.Wlocalidx[i] = iy;
        idx_lower = WorkingSet.ldA * (iy - 1);
        idx_upper = WorkingSet.ldA * i;
        i1 = WorkingSet.nVar - 1;
        for (nVar_tmp_tmp = 0; nVar_tmp_tmp <= i1; nVar_tmp_tmp++) {
          WorkingSet.ATwset[idx_upper + nVar_tmp_tmp] =
              WorkingSet.Aineq[idx_lower + nVar_tmp_tmp];
        }
        WorkingSet.bwset[i] = WorkingSet.bineq[iy - 1];
      }
      for (int idx{0}; idx < nWLower_old; idx++) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, b_TrialState.workingset_old[idx + mIneq]);
      }
      for (int idx{0}; idx < nWUpper_old; idx++) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 5,
            b_TrialState.workingset_old[(idx + mIneq) + idx_Partition]);
      }
    }
  }
  if (!success) {
    for (idx_lower = 0; idx_lower <= mConstrMax; idx_lower++) {
      b_TrialState.lambda[idx_lower] = b_TrialState.lambdaStopTest[idx_lower];
    }
  } else {
    qpactiveset::parseoutput::sortLambdaQP(
        b_TrialState.lambda, WorkingSet.nActiveConstr, WorkingSet.sizes,
        WorkingSet.isActiveIdx, WorkingSet.Wid, WorkingSet.Wlocalidx,
        memspace.workspace_double);
  }
  return success;
}

} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for soc.cpp
//
// [EOF]
//
