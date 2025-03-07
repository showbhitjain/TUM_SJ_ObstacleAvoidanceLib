//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: soc.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
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
#include "omp.h"
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
  k_struct_T b_qpoptions;
  double c;
  double d;
  double oldDirIdx;
  int i;
  int i1;
  int i2;
  int idxIneqOffset;
  int idx_Partition;
  int idx_lower;
  int idx_upper;
  int iy;
  int mConstrMax;
  int mIneq;
  int nVar;
  int nWIneq_old;
  int nWLower_old;
  int nWUpper_old;
  boolean_T success;
  nWIneq_old = WorkingSet.nWConstr[2];
  nWLower_old = WorkingSet.nWConstr[3];
  nWUpper_old = WorkingSet.nWConstr[4];
  nVar = WorkingSet.nVar - 1;
  mConstrMax = WorkingSet.mConstrMax - 1;
  i = (nVar + 1 < 200);
  if (i) {
    for (int k{0}; k <= nVar; k++) {
      b_TrialState.xstarsqp[k] = b_TrialState.xstarsqp_old[k];
      b_TrialState.socDirection[k] = b_TrialState.xstar[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= nVar; k++) {
      b_TrialState.xstarsqp[k] = b_TrialState.xstarsqp_old[k];
      b_TrialState.socDirection[k] = b_TrialState.xstar[k];
    }
  }
  i1 = (mConstrMax + 1 < 200);
  if (i1) {
    for (int k{0}; k <= mConstrMax; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= mConstrMax; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
    }
  }
  mIneq = WorkingSet.sizes[2];
  idxIneqOffset = WorkingSet.isActiveIdx[2];
  if (WorkingSet.sizes[1] > 0) {
    i2 = static_cast<unsigned char>(WorkingSet.sizes[1]);
    if (static_cast<int>(i2 < 200)) {
      for (int k{0}; k < i2; k++) {
        WorkingSet.beq.data[k] = -b_TrialState.cEq.data[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < i2; k++) {
        WorkingSet.beq.data[k] = -b_TrialState.cEq.data[k];
      }
    }
    idx_Partition = WorkingSet.ldA;
    y.set_size(WorkingSet.beq.size[0]);
    idx_upper = WorkingSet.beq.size[0];
    for (i2 = 0; i2 < idx_upper; i2++) {
      y[i2] = WorkingSet.beq.data[i2];
    }
    if (WorkingSet.nVar != 0) {
      iy = 0;
      i2 = WorkingSet.ldA * (WorkingSet.sizes[1] - 1) + 1;
      for (idx_upper = 1; idx_Partition < 0 ? idx_upper >= i2 : idx_upper <= i2;
           idx_upper += idx_Partition) {
        c = 0.0;
        idx_lower = (idx_upper + WorkingSet.nVar) - 1;
        for (int ia{idx_upper}; ia <= idx_lower; ia++) {
          c += WorkingSet.Aeq[ia - 1] * b_TrialState.searchDir[ia - idx_upper];
        }
        y[iy] = y[iy] + c;
        iy++;
      }
    }
    WorkingSet.beq.size[0] = y.size(0);
    idx_upper = y.size(0);
    for (i2 = 0; i2 < idx_upper; i2++) {
      WorkingSet.beq.data[i2] = y[i2];
    }
    i2 = static_cast<unsigned char>(WorkingSet.sizes[1]);
    for (idx_lower = 0; idx_lower < i2; idx_lower++) {
      WorkingSet.bwset[WorkingSet.sizes[0] + idx_lower] = y[idx_lower];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    if (static_cast<int>(mIneq < 200)) {
      for (int k{0}; k < mIneq; k++) {
        WorkingSet.bineq[k] = -b_TrialState.cIneq[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < mIneq; k++) {
        WorkingSet.bineq[k] = -b_TrialState.cIneq[k];
      }
    }
    idx_Partition = WorkingSet.ldA;
    if (WorkingSet.nVar != 0) {
      iy = 0;
      i2 = WorkingSet.ldA * (WorkingSet.sizes[2] - 1) + 1;
      for (idx_upper = 1; idx_Partition < 0 ? idx_upper >= i2 : idx_upper <= i2;
           idx_upper += idx_Partition) {
        c = 0.0;
        idx_lower = (idx_upper + WorkingSet.nVar) - 1;
        for (int ia{idx_upper}; ia <= idx_lower; ia++) {
          c +=
              WorkingSet.Aineq[ia - 1] * b_TrialState.searchDir[ia - idx_upper];
        }
        WorkingSet.bineq[iy] = WorkingSet.bineq[iy] + c;
        iy++;
      }
    }
    iy = 1;
    idx_lower = WorkingSet.sizes[2] + 1;
    idx_upper = (WorkingSet.sizes[2] + WorkingSet.sizes[3]) + 1;
    i2 = WorkingSet.nActiveConstr;
    for (int ia{idxIneqOffset}; ia <= i2; ia++) {
      switch (WorkingSet.Wid[ia - 1]) {
      case 3:
        idx_Partition = iy;
        iy++;
        WorkingSet.bwset[ia - 1] =
            WorkingSet.bineq[WorkingSet.Wlocalidx[ia - 1] - 1];
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
          WorkingSet.Wlocalidx[ia - 1];
    }
  }
  if (i) {
    for (int k{0}; k <= nVar; k++) {
      b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= nVar; k++) {
      b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
    }
  }
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
  if (i) {
    for (int k{0}; k <= nVar; k++) {
      d = b_TrialState.socDirection[k];
      oldDirIdx = d;
      d = b_TrialState.xstar[k] - d;
      b_TrialState.socDirection[k] = d;
      b_TrialState.xstar[k] = oldDirIdx;
    }
  } else {
#pragma omp parallel for num_threads(4 > omp_get_max_threads()                 \
                                         ? omp_get_max_threads()               \
                                         : 4) private(oldDirIdx, d)

    for (int k = 0; k <= nVar; k++) {
      d = b_TrialState.socDirection[k];
      oldDirIdx = d;
      d = b_TrialState.xstar[k] - d;
      b_TrialState.socDirection[k] = d;
      b_TrialState.xstar[k] = oldDirIdx;
    }
  }
  success =
      (::coder::internal::blas::xnrm2(nVar + 1, b_TrialState.socDirection) <=
       2.0 * ::coder::internal::blas::xnrm2(nVar + 1, b_TrialState.xstar));
  mIneq = WorkingSet.sizes[2];
  idxIneqOffset = WorkingSet.sizes[3];
  if (WorkingSet.sizes[1] > 0) {
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    if (static_cast<int>(i < 200)) {
      for (int k{0}; k < i; k++) {
        WorkingSet.beq.data[k] = -b_TrialState.cEq.data[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < i; k++) {
        WorkingSet.beq.data[k] = -b_TrialState.cEq.data[k];
      }
    }
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    for (idx_lower = 0; idx_lower < i; idx_lower++) {
      WorkingSet.bwset[WorkingSet.sizes[0] + idx_lower] =
          WorkingSet.beq.data[idx_lower];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    if (static_cast<int>(mIneq < 200)) {
      for (int k{0}; k < mIneq; k++) {
        WorkingSet.bineq[k] = -b_TrialState.cIneq[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k < mIneq; k++) {
        WorkingSet.bineq[k] = -b_TrialState.cIneq[k];
      }
    }
    if (!success) {
      idx_upper = WorkingSet.nWConstr[0] + WorkingSet.nWConstr[1];
      iy = idx_upper + 1;
      idx_lower = WorkingSet.nActiveConstr;
      for (idx_Partition = iy; idx_Partition <= idx_lower; idx_Partition++) {
        WorkingSet.isActiveConstr
            [(WorkingSet.isActiveIdx[WorkingSet.Wid[idx_Partition - 1] - 1] +
              WorkingSet.Wlocalidx[idx_Partition - 1]) -
             2] = false;
      }
      WorkingSet.nWConstr[2] = 0;
      WorkingSet.nWConstr[3] = 0;
      WorkingSet.nWConstr[4] = 0;
      WorkingSet.nActiveConstr = idx_upper;
      for (int ia{0}; ia < nWIneq_old; ia++) {
        iy = b_TrialState.workingset_old[ia];
        WorkingSet.nWConstr[2]++;
        WorkingSet.isActiveConstr[(WorkingSet.isActiveIdx[2] + iy) - 2] = true;
        WorkingSet.nActiveConstr++;
        i = WorkingSet.nActiveConstr - 1;
        WorkingSet.Wid[i] = 3;
        WorkingSet.Wlocalidx[i] = iy;
        idx_lower = WorkingSet.ldA * (iy - 1);
        idx_upper = WorkingSet.ldA * i;
        i2 = WorkingSet.nVar - 1;
        for (idx_Partition = 0; idx_Partition <= i2; idx_Partition++) {
          WorkingSet.ATwset[idx_upper + idx_Partition] =
              WorkingSet.Aineq[idx_lower + idx_Partition];
        }
        WorkingSet.bwset[i] = WorkingSet.bineq[iy - 1];
      }
      for (int ia{0}; ia < nWLower_old; ia++) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, b_TrialState.workingset_old[ia + mIneq]);
      }
      for (int ia{0}; ia < nWUpper_old; ia++) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 5,
            b_TrialState.workingset_old[(ia + mIneq) + idxIneqOffset]);
      }
    }
  }
  if (!success) {
    if (i1) {
      for (int k{0}; k <= mConstrMax; k++) {
        b_TrialState.lambda[k] = b_TrialState.lambdaStopTest[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= mConstrMax; k++) {
        b_TrialState.lambda[k] = b_TrialState.lambdaStopTest[k];
      }
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
