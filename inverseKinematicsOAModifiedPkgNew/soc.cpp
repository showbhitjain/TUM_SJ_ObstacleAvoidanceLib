//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: soc.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "soc.h"
#include "addAeqConstr.h"
#include "addBoundToActiveSetMatrix_.h"
#include "driver1.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "removeAllIneqConstr.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xgemv.h"
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
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,    // iFirst
      -1,    // iLast
      1,     // lineNo
      1,     // colNo
      "",    // aName
      "soc", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "soc.p", // pName
      0        // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                 // iFirst
      -1,                 // iLast
      1,                  // lineNo
      1,                  // colNo
      "",                 // aName
      "updateWorkingSet", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "+soc/updateWorkingSet.p", // pName
      0                          // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "restoreWorkingSet", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "+soc/restoreWorkingSet.p", // pName
      0                           // checkKind
  };
  array<double, 1U> r;
  double lenSOC;
  int b;
  int i;
  int i1;
  int i2;
  int idxIneqOffset;
  int idx_Aineq;
  int idx_Partition;
  int idx_lower;
  int idx_upper;
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
  nVar = WorkingSet.nVar;
  mConstrMax = WorkingSet.mConstrMax;
  if (WorkingSet.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx_Aineq = 0; idx_Aineq < nVar; idx_Aineq++) {
    b_TrialState.xstarsqp[idx_Aineq] = b_TrialState.xstarsqp_old[idx_Aineq];
  }
  if (WorkingSet.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx_Aineq = 0; idx_Aineq < nVar; idx_Aineq++) {
    i = b_TrialState.xstar.size(0);
    if ((idx_Aineq + 1 < 1) || (idx_Aineq + 1 > i)) {
      rtDynamicBoundsError(idx_Aineq + 1, 1, i, w_emlrtBCI);
    }
    i = b_TrialState.socDirection.size(0);
    if (idx_Aineq + 1 > i) {
      rtDynamicBoundsError(idx_Aineq + 1, 1, i, w_emlrtBCI);
    }
    b_TrialState.socDirection[idx_Aineq] = b_TrialState.xstar[idx_Aineq];
  }
  if (WorkingSet.mConstrMax > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx_Aineq = 0; idx_Aineq < mConstrMax; idx_Aineq++) {
    b_TrialState.lambdaStopTest[idx_Aineq] = b_TrialState.lambda[idx_Aineq];
  }
  mIneq = WorkingSet.sizes[2];
  idxIneqOffset = WorkingSet.isActiveIdx[2];
  if (WorkingSet.sizes[1] > 0) {
    if (WorkingSet.sizes[1] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    i1 = b_TrialState.cEq.size[0];
    for (int idx{0}; idx < i; idx++) {
      if (idx + 1 > i1) {
        rtDynamicBoundsError(idx + 1, 1, i1, x_emlrtBCI);
      }
      i2 = WorkingSet.beq.size[0];
      if (idx + 1 > i2) {
        rtDynamicBoundsError(idx + 1, 1, i2, x_emlrtBCI);
      }
      WorkingSet.beq.data[idx] = -b_TrialState.cEq.data[idx];
    }
    r.set_size(WorkingSet.beq.size[0]);
    idx_Aineq = WorkingSet.beq.size[0];
    for (i1 = 0; i1 < idx_Aineq; i1++) {
      r[i1] = WorkingSet.beq.data[i1];
    }
    ::coder::internal::blas::b_xgemv(WorkingSet.nVar, WorkingSet.sizes[1],
                                     WorkingSet.Aeq, WorkingSet.ldA,
                                     b_TrialState.searchDir, r);
    WorkingSet.beq.size[0] = r.size(0);
    idx_Aineq = r.size(0);
    for (i1 = 0; i1 < idx_Aineq; i1++) {
      WorkingSet.beq.data[i1] = r[i1];
    }
    if (WorkingSet.sizes[1] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx_Aineq = 0; idx_Aineq < i; idx_Aineq++) {
      WorkingSet.bwset[WorkingSet.sizes[0] + idx_Aineq] =
          WorkingSet.beq.data[idx_Aineq];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    if (WorkingSet.sizes[2] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = b_TrialState.cIneq.size(0);
    for (int idx{0}; idx < mIneq; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      i1 = WorkingSet.bineq.size(0);
      if (idx + 1 > i1) {
        rtDynamicBoundsError(idx + 1, 1, i1, x_emlrtBCI);
      }
      WorkingSet.bineq[idx] = -b_TrialState.cIneq[idx];
    }
    ::coder::internal::blas::b_xgemv(WorkingSet.nVar, WorkingSet.sizes[2],
                                     WorkingSet.Aineq, WorkingSet.ldA,
                                     b_TrialState.searchDir, WorkingSet.bineq);
    idx_Aineq = 1;
    idx_lower = WorkingSet.sizes[2] + 1;
    idx_upper = (WorkingSet.sizes[2] + WorkingSet.sizes[3]) + 1;
    b = WorkingSet.nActiveConstr;
    if ((WorkingSet.isActiveIdx[2] <= WorkingSet.nActiveConstr) &&
        (WorkingSet.nActiveConstr > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx{idxIneqOffset}; idx <= b; idx++) {
      i = WorkingSet.Wid.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
      }
      i = WorkingSet.Wlocalidx.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
      }
      switch (WorkingSet.Wid[idx - 1]) {
      case 3:
        idx_Partition = idx_Aineq;
        idx_Aineq++;
        i = WorkingSet.bineq.size(0);
        i1 = WorkingSet.Wlocalidx[idx - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(WorkingSet.Wlocalidx[idx - 1], 1, i, x_emlrtBCI);
        }
        i = WorkingSet.bwset.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
        }
        WorkingSet.bwset[idx - 1] = WorkingSet.bineq[i1 - 1];
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
      i = b_TrialState.workingset_old.size(0);
      if ((idx_Partition < 1) || (idx_Partition > i)) {
        rtDynamicBoundsError(idx_Partition, 1, i, x_emlrtBCI);
      }
      b_TrialState.workingset_old[idx_Partition - 1] =
          WorkingSet.Wlocalidx[idx - 1];
    }
  }
  if (WorkingSet.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx_Aineq = 0; idx_Aineq < nVar; idx_Aineq++) {
    b_TrialState.xstar[idx_Aineq] = b_TrialState.xstarsqp[idx_Aineq];
  }
  k_struct_T b_qpoptions;
  b_qpoptions = qpoptions;
  ::coder::optim::coder::qpactiveset::driver(
      Hessian, grad, b_TrialState, memspace, WorkingSet, b_QRManager,
      b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
  if (WorkingSet.indexEqRemoved.size[0] > 0) {
    boolean_T exitg1;
    exitg1 = false;
    while ((!exitg1) && (WorkingSet.mEqRemoved > 0)) {
      i = WorkingSet.indexEqRemoved.size[0];
      if ((WorkingSet.mEqRemoved < 1) || (WorkingSet.mEqRemoved > i)) {
        rtDynamicBoundsError(WorkingSet.mEqRemoved, 1, i, w_emlrtBCI);
      }
      if (WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1] >=
          b_TrialState.iNonEq0) {
        i = WorkingSet.indexEqRemoved.size[0];
        if ((WorkingSet.mEqRemoved < 1) || (WorkingSet.mEqRemoved > i)) {
          rtDynamicBoundsError(WorkingSet.mEqRemoved, 1, i, w_emlrtBCI);
        }
        qpactiveset::WorkingSet::addAeqConstr(
            WorkingSet,
            WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1]);
        WorkingSet.mEqRemoved--;
      } else {
        exitg1 = true;
      }
    }
  }
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < nVar; idx++) {
    i = b_TrialState.socDirection.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    lenSOC = b_TrialState.socDirection[idx];
    i = b_TrialState.xstar.size(0);
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    i = b_TrialState.socDirection.size(0);
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    b_TrialState.socDirection[idx] = b_TrialState.xstar[idx] - lenSOC;
    i = b_TrialState.xstar.size(0);
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    b_TrialState.xstar[idx] = lenSOC;
  }
  double lenQPNormal;
  lenSOC = ::coder::internal::blas::xnrm2(nVar, b_TrialState.socDirection);
  lenQPNormal = ::coder::internal::blas::xnrm2(nVar, b_TrialState.xstar);
  success = (lenSOC <= 2.0 * lenQPNormal);
  mIneq = WorkingSet.sizes[2] + 1;
  idxIneqOffset = WorkingSet.sizes[3];
  if (WorkingSet.sizes[1] > 0) {
    if (WorkingSet.sizes[1] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = static_cast<unsigned char>(WorkingSet.sizes[1]);
    i1 = b_TrialState.cEq.size[0];
    for (int idx{0}; idx < i; idx++) {
      if (idx + 1 > i1) {
        rtDynamicBoundsError(idx + 1, 1, i1, y_emlrtBCI);
      }
      i2 = WorkingSet.beq.size[0];
      if (idx + 1 > i2) {
        rtDynamicBoundsError(idx + 1, 1, i2, y_emlrtBCI);
      }
      WorkingSet.beq.data[idx] = -b_TrialState.cEq.data[idx];
    }
    if (WorkingSet.sizes[1] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx_Aineq = 0; idx_Aineq < i; idx_Aineq++) {
      WorkingSet.bwset[WorkingSet.sizes[0] + idx_Aineq] =
          WorkingSet.beq.data[idx_Aineq];
    }
  }
  if (WorkingSet.sizes[2] > 0) {
    if (WorkingSet.sizes[2] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = b_TrialState.cIneq.size(0);
    for (int idx{0}; idx <= mIneq - 2; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, y_emlrtBCI);
      }
      i1 = WorkingSet.bineq.size(0);
      if (idx + 1 > i1) {
        rtDynamicBoundsError(idx + 1, 1, i1, y_emlrtBCI);
      }
      WorkingSet.bineq[idx] = -b_TrialState.cIneq[idx];
    }
    if (!success) {
      qpactiveset::WorkingSet::removeAllIneqConstr(WorkingSet);
      if (nWIneq_old > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < nWIneq_old; idx++) {
        i = b_TrialState.workingset_old.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, y_emlrtBCI);
        }
        idx_Aineq = b_TrialState.workingset_old[idx];
        WorkingSet.nWConstr[2]++;
        i = WorkingSet.isActiveConstr.size(0);
        i1 = (WorkingSet.isActiveIdx[2] + idx_Aineq) - 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, q_emlrtBCI);
        }
        WorkingSet.isActiveConstr[i1 - 1] = true;
        WorkingSet.nActiveConstr++;
        i = WorkingSet.Wid.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i, q_emlrtBCI);
        }
        i = WorkingSet.nActiveConstr - 1;
        WorkingSet.Wid[i] = 3;
        i1 = WorkingSet.Wlocalidx.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i1)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i1, q_emlrtBCI);
        }
        WorkingSet.Wlocalidx[i] = idx_Aineq;
        idx_lower = WorkingSet.ldA * (idx_Aineq - 1) + 1;
        idx_upper = WorkingSet.ldA * i + 1;
        i1 = WorkingSet.nVar - 1;
        for (nVar = 0; nVar <= i1; nVar++) {
          i2 = WorkingSet.Aineq.size(0);
          b = idx_lower + nVar;
          if ((b < 1) || (b > i2)) {
            rtDynamicBoundsError(b, 1, i2, r_emlrtBCI);
          }
          i2 = WorkingSet.ATwset.size(0);
          idx_Partition = idx_upper + nVar;
          if ((idx_Partition < 1) || (idx_Partition > i2)) {
            rtDynamicBoundsError(idx_Partition, 1, i2, r_emlrtBCI);
          }
          WorkingSet.ATwset[idx_Partition - 1] = WorkingSet.Aineq[b - 1];
        }
        i1 = WorkingSet.bineq.size(0);
        if ((idx_Aineq < 1) || (idx_Aineq > i1)) {
          rtDynamicBoundsError(idx_Aineq, 1, i1, r_emlrtBCI);
        }
        i1 = WorkingSet.bwset.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i1)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i1, r_emlrtBCI);
        }
        WorkingSet.bwset[i] = WorkingSet.bineq[idx_Aineq - 1];
      }
      if (nWLower_old > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < nWLower_old; idx++) {
        i = b_TrialState.workingset_old.size(0);
        i1 = idx + mIneq;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, y_emlrtBCI);
        }
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, b_TrialState.workingset_old[i1 - 1]);
      }
      if (nWUpper_old > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < nWUpper_old; idx++) {
        i = b_TrialState.workingset_old.size(0);
        i1 = (idx + mIneq) + idxIneqOffset;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, y_emlrtBCI);
        }
        qpactiveset::WorkingSet::b_addBoundToActiveSetMatrix_(
            WorkingSet, b_TrialState.workingset_old[i1 - 1]);
      }
    }
  }
  if (!success) {
    if (mConstrMax > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx_Aineq = 0; idx_Aineq < mConstrMax; idx_Aineq++) {
      b_TrialState.lambda[idx_Aineq] = b_TrialState.lambdaStopTest[idx_Aineq];
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
