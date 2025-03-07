//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: relaxed.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "relaxed.h"
#include "addBoundToActiveSetMatrix_.h"
#include "driver1.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "sortLambdaQP.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Hessian
//                const array<double, 1U> &grad
//                j_struct_T &b_TrialState
//                struct_T &b_MeritFunction
//                h_struct_T &memspace
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                f_struct_T &b_CholManager
//                g_struct_T &QPObjective
//                k_struct_T &qpoptions
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void relaxed(const array<double, 2U> &Hessian, const array<double, 1U> &grad,
             j_struct_T &b_TrialState, struct_T &b_MeritFunction,
             h_struct_T &memspace, d_struct_T &WorkingSet,
             e_struct_T &b_QRManager, f_struct_T &b_CholManager,
             g_struct_T &QPObjective, k_struct_T &qpoptions)
{
  array<double, 2U> *y;
  double beta;
  double rho;
  double s;
  double smax;
  int b_mEq;
  int b_mIneq;
  int i;
  int ia;
  int idx;
  int idx_max;
  int idx_negative;
  int k;
  int mEq;
  int mIneq;
  int mLBOrig_tmp;
  int nVarOrig;
  boolean_T tf;
  nVarOrig = WorkingSet.nVar;
  mIneq = WorkingSet.sizes[2];
  mEq = WorkingSet.sizes[1];
  beta = 0.0;
  for (idx = 0; idx < nVarOrig; idx++) {
    beta += Hessian[idx + Hessian.size(0) * idx];
  }
  beta /= static_cast<double>(WorkingSet.nVar);
  if (b_TrialState.sqpIterations <= 1) {
    idx_negative = QPObjective.nvar;
    if (QPObjective.nvar < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (QPObjective.nvar > 1) {
        smax = std::abs(grad[0]);
        for (k = 2; k <= idx_negative; k++) {
          s = std::abs(grad[k - 1]);
          if (s > smax) {
            idx_max = k;
            smax = s;
          }
        }
      }
    }
    rho = 100.0 * std::fmax(1.0, std::abs(grad[idx_max - 1]));
  } else {
    idx_negative = WorkingSet.mConstr;
    if (WorkingSet.mConstr < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (WorkingSet.mConstr > 1) {
        smax = std::abs(b_TrialState.lambdasqp[0]);
        for (k = 2; k <= idx_negative; k++) {
          s = std::abs(b_TrialState.lambdasqp[k - 1]);
          if (s > smax) {
            idx_max = k;
            smax = s;
          }
        }
      }
    }
    rho = std::abs(b_TrialState.lambdasqp[idx_max - 1]);
  }
  QPObjective.nvar = WorkingSet.nVar;
  QPObjective.beta = beta;
  QPObjective.rho = rho;
  QPObjective.hasLinear = true;
  QPObjective.objtype = 4;
  qpactiveset::WorkingSet::setProblemType(WorkingSet, 2);
  b_mIneq = WorkingSet.sizes[2] - 1;
  b_mEq = WorkingSet.sizes[1] - 1;
  mLBOrig_tmp =
      (WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1)) - WorkingSet.sizes[2];
  for (k = 0; k <= b_mIneq; k++) {
    memspace.workspace_double[k] = WorkingSet.bineq[k];
  }
  idx_negative = WorkingSet.sizes[2];
  k = WorkingSet.ldA;
  y = &memspace.workspace_double;
  if ((nVarOrig != 0) && (idx_negative != 0)) {
    for (idx = 0; idx < idx_negative; idx++) {
      (*y)[idx] = -(*y)[idx];
    }
    idx = 0;
    i = k * (idx_negative - 1) + 1;
    for (idx_negative = 1; k < 0 ? idx_negative >= i : idx_negative <= i;
         idx_negative += k) {
      smax = 0.0;
      idx_max = (idx_negative + nVarOrig) - 1;
      for (ia = idx_negative; ia <= idx_max; ia++) {
        smax +=
            WorkingSet.Aineq[ia - 1] * b_TrialState.xstar[ia - idx_negative];
      }
      (*y)[idx] = (*y)[idx] + smax;
      idx++;
    }
  }
  for (idx = 0; idx <= b_mIneq; idx++) {
    b_TrialState.xstar[nVarOrig + idx] =
        static_cast<double>(memspace.workspace_double[idx] > 0.0) *
        memspace.workspace_double[idx];
  }
  for (k = 0; k <= b_mEq; k++) {
    memspace.workspace_double[k] = WorkingSet.beq.data[k];
  }
  idx_negative = WorkingSet.sizes[1];
  k = WorkingSet.ldA;
  y = &memspace.workspace_double;
  if ((nVarOrig != 0) && (idx_negative != 0)) {
    for (idx = 0; idx < idx_negative; idx++) {
      (*y)[idx] = -(*y)[idx];
    }
    idx = 0;
    i = k * (idx_negative - 1) + 1;
    for (idx_negative = 1; k < 0 ? idx_negative >= i : idx_negative <= i;
         idx_negative += k) {
      smax = 0.0;
      idx_max = (idx_negative + nVarOrig) - 1;
      for (ia = idx_negative; ia <= idx_max; ia++) {
        smax += WorkingSet.Aeq[ia - 1] * b_TrialState.xstar[ia - idx_negative];
      }
      (*y)[idx] = (*y)[idx] + smax;
      idx++;
    }
  }
  for (idx = 0; idx <= b_mEq; idx++) {
    idx_max = (b_mIneq + idx) + 1;
    idx_negative = ((b_mIneq + b_mEq) + idx) + 2;
    if (memspace.workspace_double[idx] <= 0.0) {
      b_TrialState.xstar[nVarOrig + idx_max] = 0.0;
      b_TrialState.xstar[nVarOrig + idx_negative] =
          -memspace.workspace_double[idx];
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig_tmp + idx_max) + 1);
      if (memspace.workspace_double[idx] >= -1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig_tmp + idx_negative) + 1);
      }
    } else {
      b_TrialState.xstar[nVarOrig + idx_max] = memspace.workspace_double[idx];
      b_TrialState.xstar[nVarOrig + idx_negative] = 0.0;
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig_tmp + idx_negative) + 1);
      if (memspace.workspace_double[idx] <= 1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig_tmp + idx_max) + 1);
      }
    }
  }
  k_struct_T b_qpoptions;
  idx_max = qpoptions.MaxIterations;
  qpoptions.MaxIterations =
      (qpoptions.MaxIterations + WorkingSet.nVar) - nVarOrig;
  b_qpoptions = qpoptions;
  ::coder::optim::coder::qpactiveset::driver(
      Hessian, grad, b_TrialState, memspace, WorkingSet, b_QRManager,
      b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
  qpoptions.MaxIterations = idx_max;
  b_mIneq = WorkingSet.sizes[2];
  b_mEq = WorkingSet.sizes[1];
  mLBOrig_tmp = WorkingSet.sizes[1] << 1;
  idx_max = mLBOrig_tmp + WorkingSet.sizes[2];
  idx_negative = WorkingSet.sizes[3] - 1;
  ia = 0;
  for (idx = 0; idx < b_mEq; idx++) {
    boolean_T b_tf;
    k = WorkingSet.isActiveIdx[3] + idx_negative;
    tf = WorkingSet.isActiveConstr[(k - (b_mEq << 1)) + idx];
    b_tf = WorkingSet.isActiveConstr[(k - b_mEq) + idx];
    memspace.workspace_int[idx] = tf;
    memspace.workspace_int[idx + b_mEq] = b_tf;
    ia = (ia + tf) + b_tf;
  }
  for (idx = 0; idx < b_mIneq; idx++) {
    tf = WorkingSet.isActiveConstr[((WorkingSet.isActiveIdx[3] + idx_negative) -
                                    idx_max) +
                                   idx];
    memspace.workspace_int[idx + mLBOrig_tmp] = tf;
    ia += tf;
  }
  if (b_TrialState.state != -6) {
    double penaltyParamTrial;
    mLBOrig_tmp = (WorkingSet.nVarMax - nVarOrig) - 1;
    idx_negative = nVarOrig + 1;
    smax = 0.0;
    s = 0.0;
    if (mLBOrig_tmp >= 1) {
      idx_max = nVarOrig + mLBOrig_tmp;
      for (k = idx_negative; k <= idx_max; k++) {
        smax += std::abs(b_TrialState.xstar[k - 1]);
      }
    }
    if (mLBOrig_tmp >= 1) {
      for (k = 0; k < mLBOrig_tmp; k++) {
        idx_max = nVarOrig + k;
        s += b_TrialState.xstar[idx_max] * b_TrialState.xstar[idx_max];
      }
    }
    beta = (b_TrialState.fstar - rho * smax) - beta / 2.0 * s;
    penaltyParamTrial = b_MeritFunction.penaltyParam;
    smax = 0.0;
    if (mEq >= 1) {
      i = static_cast<unsigned char>(mEq);
      for (k = 0; k < i; k++) {
        smax += std::abs(b_TrialState.cEq.data[k]);
      }
    }
    s = 0.0;
    for (idx = 0; idx < mIneq; idx++) {
      rho = b_TrialState.cIneq[idx];
      if (rho > 0.0) {
        s += rho;
      }
    }
    rho = smax + s;
    smax = b_MeritFunction.linearizedConstrViol;
    s = 0.0;
    if (mLBOrig_tmp >= 1) {
      idx_max = nVarOrig + mLBOrig_tmp;
      for (k = idx_negative; k <= idx_max; k++) {
        s += std::abs(b_TrialState.xstar[k - 1]);
      }
    }
    b_MeritFunction.linearizedConstrViol = s;
    smax = (rho + smax) - s;
    if ((smax > 2.2204460492503131E-16) && (beta > 0.0)) {
      if (b_TrialState.sqpFval == 0.0) {
        s = 1.0;
      } else {
        s = 1.5;
      }
      penaltyParamTrial = s * beta / smax;
    }
    if (penaltyParamTrial < b_MeritFunction.penaltyParam) {
      b_MeritFunction.phi = b_TrialState.sqpFval + penaltyParamTrial * rho;
      if ((b_MeritFunction.initFval +
           penaltyParamTrial * (b_MeritFunction.initConstrViolationEq +
                                b_MeritFunction.initConstrViolationIneq)) -
              b_MeritFunction.phi >
          static_cast<double>(b_MeritFunction.nPenaltyDecreases) *
              b_MeritFunction.threshold) {
        b_MeritFunction.nPenaltyDecreases++;
        if ((b_MeritFunction.nPenaltyDecreases << 1) >
            b_TrialState.sqpIterations) {
          b_MeritFunction.threshold *= 10.0;
        }
        b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
      } else {
        b_MeritFunction.phi =
            b_TrialState.sqpFval + b_MeritFunction.penaltyParam * rho;
      }
    } else {
      b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
      b_MeritFunction.phi =
          b_TrialState.sqpFval + b_MeritFunction.penaltyParam * rho;
    }
    b_MeritFunction.phiPrimePlus =
        std::fmin(beta - b_MeritFunction.penaltyParam * rho, 0.0);
    idx_max = WorkingSet.isActiveIdx[1] - 1;
    for (idx = 0; idx < mEq; idx++) {
      if ((memspace.workspace_int[idx] != 0) &&
          (memspace.workspace_int[idx + mEq] != 0)) {
        tf = true;
      } else {
        tf = false;
      }
      i = idx_max + idx;
      b_TrialState.lambda[i] = static_cast<double>(tf) * b_TrialState.lambda[i];
    }
    idx_max = WorkingSet.isActiveIdx[2];
    idx_negative = WorkingSet.nActiveConstr;
    for (idx = idx_max; idx <= idx_negative; idx++) {
      if (WorkingSet.Wid[idx - 1] == 3) {
        b_TrialState.lambda[idx - 1] =
            static_cast<double>(
                memspace.workspace_int
                    [(WorkingSet.Wlocalidx[idx - 1] + (mEq << 1)) - 1]) *
            b_TrialState.lambda[idx - 1];
      }
    }
  }
  idx_max = WorkingSet.sizes[0];
  mEq = WorkingSet.sizes[1];
  i = (WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1)) - WorkingSet.sizes[2];
  idx = WorkingSet.nActiveConstr;
  while ((idx > idx_max + mEq) && (ia > 0)) {
    if ((WorkingSet.Wid[idx - 1] == 4) && (WorkingSet.Wlocalidx[idx - 1] > i)) {
      idx_negative = WorkingSet.nActiveConstr - 1;
      smax = b_TrialState.lambda[idx_negative];
      b_TrialState.lambda[idx_negative] = 0.0;
      b_TrialState.lambda[idx - 1] = smax;
      qpactiveset::WorkingSet::removeConstr(WorkingSet, idx);
      ia--;
    }
    idx--;
  }
  QPObjective.nvar = nVarOrig;
  QPObjective.hasLinear = true;
  QPObjective.objtype = 3;
  qpactiveset::WorkingSet::setProblemType(WorkingSet, 3);
  qpactiveset::parseoutput::sortLambdaQP(
      b_TrialState.lambda, WorkingSet.nActiveConstr, WorkingSet.sizes,
      WorkingSet.isActiveIdx, WorkingSet.Wid, WorkingSet.Wlocalidx,
      memspace.workspace_double);
}

} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for relaxed.cpp
//
// [EOF]
//
