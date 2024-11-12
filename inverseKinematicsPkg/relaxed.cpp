//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: relaxed.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "relaxed.h"
#include "addBoundToActiveSetMatrix_.h"
#include "driver1.h"
#include "inverseKinematics_internal_types.h"
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
//                h_struct_T &b_TrialState
//                struct_T &MeritFunction
//                g_struct_T &memspace
//                i_struct_T &WorkingSet
//                d_struct_T &b_QRManager
//                e_struct_T &b_CholManager
//                f_struct_T &QPObjective
//                j_struct_T &qpoptions
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void relaxed(const array<double, 2U> &Hessian, const array<double, 1U> &grad,
             h_struct_T &b_TrialState, struct_T &MeritFunction,
             g_struct_T &memspace, i_struct_T &WorkingSet,
             d_struct_T &b_QRManager, e_struct_T &b_CholManager,
             f_struct_T &QPObjective, j_struct_T &qpoptions)
{
  array<double, 2U> *y;
  array<double, 1U> b_WorkingSet;
  double beta;
  double rho;
  double s;
  double smax;
  int b_mEq;
  int i;
  int iIneqEnd;
  int idx;
  int idx_max;
  int mEq;
  int mLBOrig_tmp;
  int nVarOrig;
  boolean_T tf;
  nVarOrig = WorkingSet.nVar;
  mEq = WorkingSet.sizes[1];
  beta = 0.0;
  for (idx = 0; idx < nVarOrig; idx++) {
    beta += Hessian[idx + Hessian.size(0) * idx];
  }
  beta /= static_cast<double>(WorkingSet.nVar);
  if (b_TrialState.sqpIterations <= 1) {
    iIneqEnd = QPObjective.nvar;
    if (QPObjective.nvar < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (QPObjective.nvar > 1) {
        smax = std::abs(grad[0]);
        for (int k{2}; k <= iIneqEnd; k++) {
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
    iIneqEnd = WorkingSet.mConstr;
    if (WorkingSet.mConstr < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (WorkingSet.mConstr > 1) {
        smax = std::abs(b_TrialState.lambdasqp[0]);
        for (int k{2}; k <= iIneqEnd; k++) {
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
  b_mEq = WorkingSet.sizes[1] - 1;
  mLBOrig_tmp = WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1);
  b_WorkingSet.set(nullptr, WorkingSet.Aineq.size[0]);
  for (int k{0}; k <= b_mEq; k++) {
    memspace.workspace_double[k] = WorkingSet.beq.data[k];
  }
  iIneqEnd = WorkingSet.sizes[1];
  idx = WorkingSet.ldA;
  y = &memspace.workspace_double;
  if ((nVarOrig != 0) && (iIneqEnd != 0)) {
    int iy;
    for (iy = 0; iy < iIneqEnd; iy++) {
      (*y)[iy] = -(*y)[iy];
    }
    iy = 0;
    i = idx * (iIneqEnd - 1) + 1;
    for (iIneqEnd = 1; idx < 0 ? iIneqEnd >= i : iIneqEnd <= i;
         iIneqEnd += idx) {
      smax = 0.0;
      idx_max = (iIneqEnd + nVarOrig) - 1;
      for (int k{iIneqEnd}; k <= idx_max; k++) {
        smax += WorkingSet.Aeq[k - 1] * b_TrialState.xstar[k - iIneqEnd];
      }
      (*y)[iy] = (*y)[iy] + smax;
      iy++;
    }
  }
  for (idx = 0; idx <= b_mEq; idx++) {
    idx_max = (b_mEq + idx) + 1;
    if (memspace.workspace_double[idx] <= 0.0) {
      b_TrialState.xstar[nVarOrig + idx] = 0.0;
      b_TrialState.xstar[nVarOrig + idx_max] = -memspace.workspace_double[idx];
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig_tmp + idx) + 1);
      if (memspace.workspace_double[idx] >= -1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig_tmp + idx_max) + 1);
      }
    } else {
      b_TrialState.xstar[nVarOrig + idx] = memspace.workspace_double[idx];
      b_TrialState.xstar[nVarOrig + idx_max] = 0.0;
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig_tmp + idx_max) + 1);
      if (memspace.workspace_double[idx] <= 1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig_tmp + idx) + 1);
      }
    }
  }
  j_struct_T b_qpoptions;
  idx_max = qpoptions.MaxIterations;
  qpoptions.MaxIterations =
      (qpoptions.MaxIterations + WorkingSet.nVar) - nVarOrig;
  b_qpoptions = qpoptions;
  ::coder::optim::coder::qpactiveset::driver(
      Hessian, grad, b_TrialState, memspace, WorkingSet, b_QRManager,
      b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
  qpoptions.MaxIterations = idx_max;
  b_mEq = WorkingSet.sizes[1];
  idx_max = WorkingSet.sizes[3] - 1;
  mLBOrig_tmp = 0;
  for (idx = 0; idx < b_mEq; idx++) {
    boolean_T b_tf;
    iIneqEnd = WorkingSet.isActiveIdx[3] + idx_max;
    tf = WorkingSet.isActiveConstr[(iIneqEnd - (b_mEq << 1)) + idx];
    b_tf = WorkingSet.isActiveConstr[(iIneqEnd - b_mEq) + idx];
    memspace.workspace_int[idx] = tf;
    memspace.workspace_int[idx + b_mEq] = b_tf;
    mLBOrig_tmp = (mLBOrig_tmp + tf) + b_tf;
  }
  if (b_TrialState.state != -6) {
    double constrViolationEq;
    iIneqEnd = (WorkingSet.nVarMax - nVarOrig) - 1;
    idx = nVarOrig + 1;
    smax = 0.0;
    s = 0.0;
    if (iIneqEnd >= 1) {
      idx_max = nVarOrig + iIneqEnd;
      for (int k{idx}; k <= idx_max; k++) {
        smax += std::abs(b_TrialState.xstar[k - 1]);
      }
    }
    if (iIneqEnd >= 1) {
      for (int k{0}; k < iIneqEnd; k++) {
        idx_max = nVarOrig + k;
        s += b_TrialState.xstar[idx_max] * b_TrialState.xstar[idx_max];
      }
    }
    rho = (b_TrialState.fstar - rho * smax) - beta / 2.0 * s;
    beta = MeritFunction.penaltyParam;
    constrViolationEq = 0.0;
    if (mEq >= 1) {
      i = static_cast<unsigned char>(mEq);
      for (int k{0}; k < i; k++) {
        constrViolationEq += std::abs(b_TrialState.cEq.data[k]);
      }
    }
    smax = MeritFunction.linearizedConstrViol;
    s = 0.0;
    if (iIneqEnd >= 1) {
      idx_max = nVarOrig + iIneqEnd;
      for (int k{idx}; k <= idx_max; k++) {
        s += std::abs(b_TrialState.xstar[k - 1]);
      }
    }
    MeritFunction.linearizedConstrViol = s;
    smax = (constrViolationEq + smax) - s;
    if ((smax > 2.2204460492503131E-16) && (rho > 0.0)) {
      if (b_TrialState.sqpFval == 0.0) {
        beta = 1.0;
      } else {
        beta = 1.5;
      }
      beta = beta * rho / smax;
    }
    if (beta < MeritFunction.penaltyParam) {
      MeritFunction.phi = b_TrialState.sqpFval + beta * constrViolationEq;
      if ((MeritFunction.initFval +
           beta * MeritFunction.initConstrViolationEq) -
              MeritFunction.phi >
          static_cast<double>(MeritFunction.nPenaltyDecreases) *
              MeritFunction.threshold) {
        MeritFunction.nPenaltyDecreases++;
        if ((MeritFunction.nPenaltyDecreases << 1) >
            b_TrialState.sqpIterations) {
          MeritFunction.threshold *= 10.0;
        }
        MeritFunction.penaltyParam = std::fmax(beta, 1.0E-10);
      } else {
        MeritFunction.phi = b_TrialState.sqpFval +
                            MeritFunction.penaltyParam * constrViolationEq;
      }
    } else {
      MeritFunction.penaltyParam = std::fmax(beta, 1.0E-10);
      MeritFunction.phi =
          b_TrialState.sqpFval + MeritFunction.penaltyParam * constrViolationEq;
    }
    MeritFunction.phiPrimePlus =
        std::fmin(rho - MeritFunction.penaltyParam * constrViolationEq, 0.0);
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
    iIneqEnd = WorkingSet.nActiveConstr;
    for (idx = idx_max; idx <= iIneqEnd; idx++) {
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
  i = WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1);
  idx = WorkingSet.nActiveConstr;
  while ((idx > idx_max + mEq) && (mLBOrig_tmp > 0)) {
    if ((WorkingSet.Wid[idx - 1] == 4) && (WorkingSet.Wlocalidx[idx - 1] > i)) {
      iIneqEnd = WorkingSet.nActiveConstr - 1;
      smax = b_TrialState.lambda[iIneqEnd];
      b_TrialState.lambda[iIneqEnd] = 0.0;
      b_TrialState.lambda[idx - 1] = smax;
      qpactiveset::WorkingSet::removeConstr(WorkingSet, idx);
      mLBOrig_tmp--;
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
