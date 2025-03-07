//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: relaxed.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "relaxed.h"
#include "addBoundToActiveSetMatrix_.h"
#include "driver1.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "sortLambdaQP.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
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
  k_struct_T b_qpoptions;
  double beta;
  double penaltyParamTrial;
  double qpfvalQuadExcess;
  double s;
  double smax;
  int b_idx;
  int b_mEq;
  int b_mIneq;
  int b_nActiveLBArtificialPrime;
  int i;
  int idx;
  int idx_max;
  int idx_negative;
  int isAineqActive;
  int mEq;
  int mFiniteLB;
  int mIneq;
  int mLBOrig;
  int nActiveLBArtificial;
  int nActiveLBArtificialPrime;
  int nVarMax;
  int nVarOrig;
  boolean_T b_tf;
  boolean_T tf;
  nVarOrig = WorkingSet.nVar;
  nVarMax = WorkingSet.nVarMax;
  mIneq = WorkingSet.sizes[2];
  mEq = WorkingSet.sizes[1];
  beta = 0.0;
  for (idx = 0; idx < nVarOrig; idx++) {
    beta += Hessian[idx + Hessian.size(0) * idx];
  }
  beta /= static_cast<double>(WorkingSet.nVar);
  if (b_TrialState.sqpIterations <= 1) {
    mLBOrig = QPObjective.nvar;
    if (QPObjective.nvar < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (QPObjective.nvar > 1) {
        smax = std::abs(grad[0]);
        for (b_mIneq = 2; b_mIneq <= mLBOrig; b_mIneq++) {
          s = std::abs(grad[b_mIneq - 1]);
          if (s > smax) {
            idx_max = b_mIneq;
            smax = s;
          }
        }
      }
    }
    smax = 100.0 * std::fmax(1.0, std::abs(grad[idx_max - 1]));
  } else {
    mLBOrig = WorkingSet.mConstr;
    if (WorkingSet.mConstr < 1) {
      idx_max = 0;
    } else {
      idx_max = 1;
      if (WorkingSet.mConstr > 1) {
        smax = std::abs(b_TrialState.lambdasqp[0]);
        for (b_mIneq = 2; b_mIneq <= mLBOrig; b_mIneq++) {
          s = std::abs(b_TrialState.lambdasqp[b_mIneq - 1]);
          if (s > smax) {
            idx_max = b_mIneq;
            smax = s;
          }
        }
      }
    }
    smax = std::abs(b_TrialState.lambdasqp[idx_max - 1]);
  }
  QPObjective.nvar = WorkingSet.nVar;
  QPObjective.beta = beta;
  QPObjective.rho = smax;
  QPObjective.hasLinear = true;
  QPObjective.objtype = 4;
  qpactiveset::WorkingSet::setProblemType(WorkingSet, 2);
  b_mIneq = WorkingSet.sizes[2] - 1;
  b_mEq = WorkingSet.sizes[1] - 1;
  mLBOrig =
      (WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1)) - WorkingSet.sizes[2];
  i = (b_mIneq + 1 < 200);
  if (i) {
    for (isAineqActive = 0; isAineqActive <= b_mIneq; isAineqActive++) {
      memspace.workspace_double[isAineqActive] =
          WorkingSet.bineq[isAineqActive];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (isAineqActive = 0; isAineqActive <= b_mIneq; isAineqActive++) {
      memspace.workspace_double[isAineqActive] =
          WorkingSet.bineq[isAineqActive];
    }
  }
  ::coder::internal::blas::xgemv(nVarOrig, WorkingSet.sizes[2],
                                 WorkingSet.Aineq, WorkingSet.ldA,
                                 b_TrialState.xstar, memspace.workspace_double);
  if (i) {
    for (b_idx = 0; b_idx <= b_mIneq; b_idx++) {
      b_TrialState.xstar[nVarOrig + b_idx] =
          static_cast<double>(memspace.workspace_double[b_idx] > 0.0) *
          memspace.workspace_double[b_idx];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (b_idx = 0; b_idx <= b_mIneq; b_idx++) {
      b_TrialState.xstar[nVarOrig + b_idx] =
          static_cast<double>(memspace.workspace_double[b_idx] > 0.0) *
          memspace.workspace_double[b_idx];
    }
  }
  if (static_cast<int>(b_mEq + 1 < 200)) {
    for (isAineqActive = 0; isAineqActive <= b_mEq; isAineqActive++) {
      memspace.workspace_double[isAineqActive] =
          WorkingSet.beq.data[isAineqActive];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (isAineqActive = 0; isAineqActive <= b_mEq; isAineqActive++) {
      memspace.workspace_double[isAineqActive] =
          WorkingSet.beq.data[isAineqActive];
    }
  }
  ::coder::internal::blas::xgemv(nVarOrig, WorkingSet.sizes[1], WorkingSet.Aeq,
                                 WorkingSet.ldA, b_TrialState.xstar,
                                 memspace.workspace_double);
  for (idx = 0; idx <= b_mEq; idx++) {
    idx_max = (b_mIneq + idx) + 1;
    idx_negative = ((b_mIneq + b_mEq) + idx) + 2;
    if (memspace.workspace_double[idx] <= 0.0) {
      b_TrialState.xstar[nVarOrig + idx_max] = 0.0;
      b_TrialState.xstar[nVarOrig + idx_negative] =
          -memspace.workspace_double[idx];
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig + idx_max) + 1);
      if (memspace.workspace_double[idx] >= -OAGlobals::constraintTolerance) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig + idx_negative) + 1);
      }
    } else {
      b_TrialState.xstar[nVarOrig + idx_max] = memspace.workspace_double[idx];
      b_TrialState.xstar[nVarOrig + idx_negative] = 0.0;
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, 4, (mLBOrig + idx_negative) + 1);
      if (memspace.workspace_double[idx] <= OAGlobals::constraintTolerance) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, 4, (mLBOrig + idx_max) + 1);
      }
    }
  }
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
  idx_negative = WorkingSet.sizes[1] << 1;
  mLBOrig = idx_negative + WorkingSet.sizes[2];
  mFiniteLB = WorkingSet.sizes[3] - 1;
  nActiveLBArtificial = 0;
  if (static_cast<int>(b_mEq < 200)) {
    for (b_idx = 0; b_idx < b_mEq; b_idx++) {
      idx_max = WorkingSet.isActiveIdx[3] + mFiniteLB;
      tf = WorkingSet.isActiveConstr[(idx_max - (b_mEq << 1)) + b_idx];
      b_tf = WorkingSet.isActiveConstr[(idx_max - b_mEq) + b_idx];
      memspace.workspace_int[b_idx] = tf;
      memspace.workspace_int[b_idx + b_mEq] = b_tf;
      nActiveLBArtificial = (nActiveLBArtificial + tf) + b_tf;
    }
  } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(nActiveLBArtificialPrime, tf, b_tf)
    {
      nActiveLBArtificialPrime = 0;
#pragma omp for nowait
      for (b_idx = 0; b_idx < b_mEq; b_idx++) {
        tf =
            WorkingSet.isActiveConstr[((WorkingSet.isActiveIdx[3] + mFiniteLB) -
                                       (b_mEq << 1)) +
                                      b_idx];
        b_tf = WorkingSet.isActiveConstr
                   [((WorkingSet.isActiveIdx[3] + mFiniteLB) - b_mEq) + b_idx];
        memspace.workspace_int[b_idx] = tf;
        memspace.workspace_int[b_idx + b_mEq] = b_tf;
        nActiveLBArtificialPrime = (nActiveLBArtificialPrime + tf) + b_tf;
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        nActiveLBArtificial += nActiveLBArtificialPrime;
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  if (static_cast<int>(b_mIneq < 200)) {
    for (b_idx = 0; b_idx < b_mIneq; b_idx++) {
      tf = WorkingSet.isActiveConstr[((WorkingSet.isActiveIdx[3] + mFiniteLB) -
                                      mLBOrig) +
                                     b_idx];
      memspace.workspace_int[b_idx + idx_negative] = tf;
      nActiveLBArtificial += tf;
    }
  } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(b_nActiveLBArtificialPrime, tf)
    {
      b_nActiveLBArtificialPrime = 0;
#pragma omp for nowait
      for (b_idx = 0; b_idx < b_mIneq; b_idx++) {
        tf = WorkingSet.isActiveConstr
                 [((WorkingSet.isActiveIdx[3] + mFiniteLB) - mLBOrig) + b_idx];
        memspace.workspace_int[b_idx + idx_negative] = tf;
        b_nActiveLBArtificialPrime += tf;
      }
      omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      {

        nActiveLBArtificial += b_nActiveLBArtificialPrime;
      }
      omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
    }
  }
  if (b_TrialState.state != -6) {
    mLBOrig = (WorkingSet.nVarMax - nVarOrig) - 1;
    mFiniteLB = nVarOrig + 1;
    s = 0.0;
    qpfvalQuadExcess = 0.0;
    if (mLBOrig >= 1) {
      idx_negative = nVarOrig + mLBOrig;
      for (b_mIneq = mFiniteLB; b_mIneq <= idx_negative; b_mIneq++) {
        s += std::abs(b_TrialState.xstar[b_mIneq - 1]);
      }
    }
    if (mLBOrig >= 1) {
      for (b_mIneq = 0; b_mIneq < mLBOrig; b_mIneq++) {
        idx_max = nVarOrig + b_mIneq;
        qpfvalQuadExcess +=
            b_TrialState.xstar[idx_max] * b_TrialState.xstar[idx_max];
      }
    }
    beta = (b_TrialState.fstar - smax * s) - beta / 2.0 * qpfvalQuadExcess;
    mLBOrig = (nVarMax - nVarOrig) - 1;
    penaltyParamTrial = b_MeritFunction.penaltyParam;
    smax = 0.0;
    if (mEq >= 1) {
      i = static_cast<unsigned char>(mEq);
      for (b_mIneq = 0; b_mIneq < i; b_mIneq++) {
        smax += std::abs(b_TrialState.cEq.data[b_mIneq]);
      }
    }
    s = 0.0;
    for (idx = 0; idx < mIneq; idx++) {
      qpfvalQuadExcess = b_TrialState.cIneq[idx];
      if (qpfvalQuadExcess > 0.0) {
        s += qpfvalQuadExcess;
      }
    }
    qpfvalQuadExcess = smax + s;
    smax = b_MeritFunction.linearizedConstrViol;
    s = 0.0;
    if (mLBOrig >= 1) {
      idx_negative = nVarOrig + mLBOrig;
      for (b_mIneq = mFiniteLB; b_mIneq <= idx_negative; b_mIneq++) {
        s += std::abs(b_TrialState.xstar[b_mIneq - 1]);
      }
    }
    b_MeritFunction.linearizedConstrViol = s;
    smax = (qpfvalQuadExcess + smax) - s;
    if ((smax > 2.2204460492503131E-16) && (beta > 0.0)) {
      if (b_TrialState.sqpFval == 0.0) {
        s = 1.0;
      } else {
        s = 1.5;
      }
      penaltyParamTrial = s * beta / smax;
    }
    if (penaltyParamTrial < b_MeritFunction.penaltyParam) {
      b_MeritFunction.phi =
          b_TrialState.sqpFval + penaltyParamTrial * qpfvalQuadExcess;
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
        b_MeritFunction.phi = b_TrialState.sqpFval +
                              b_MeritFunction.penaltyParam * qpfvalQuadExcess;
      }
    } else {
      b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
      b_MeritFunction.phi = b_TrialState.sqpFval +
                            b_MeritFunction.penaltyParam * qpfvalQuadExcess;
    }
    b_MeritFunction.phiPrimePlus =
        std::fmin(beta - b_MeritFunction.penaltyParam * qpfvalQuadExcess, 0.0);
    idx_max = WorkingSet.isActiveIdx[1] - 1;
    if (static_cast<int>(mEq < 200)) {
      for (b_idx = 0; b_idx < mEq; b_idx++) {
        if ((memspace.workspace_int[b_idx] != 0) &&
            (memspace.workspace_int[b_idx + mEq] != 0)) {
          tf = true;
        } else {
          tf = false;
        }
        i = idx_max + b_idx;
        b_TrialState.lambda[i] =
            static_cast<double>(tf) * b_TrialState.lambda[i];
      }
    } else {
#pragma omp parallel for num_threads(4 > omp_get_max_threads()                 \
                                         ? omp_get_max_threads()               \
                                         : 4) private(tf, isAineqActive)

      for (b_idx = 0; b_idx < mEq; b_idx++) {
        if ((memspace.workspace_int[b_idx] != 0) &&
            (memspace.workspace_int[b_idx + mEq] != 0)) {
          tf = true;
        } else {
          tf = false;
        }
        isAineqActive = idx_max + b_idx;
        b_TrialState.lambda[isAineqActive] =
            static_cast<double>(tf) * b_TrialState.lambda[isAineqActive];
      }
    }
    idx_max = WorkingSet.isActiveIdx[2];
    idx_negative = WorkingSet.nActiveConstr;
    if (static_cast<int>((idx_negative - idx_max) + 1 < 200)) {
      for (b_idx = idx_max; b_idx <= idx_negative; b_idx++) {
        if (WorkingSet.Wid[b_idx - 1] == 3) {
          b_TrialState.lambda[b_idx - 1] =
              static_cast<double>(
                  memspace.workspace_int
                      [(WorkingSet.Wlocalidx[b_idx - 1] + (mEq << 1)) - 1]) *
              b_TrialState.lambda[b_idx - 1];
        }
      }
    } else {
#pragma omp parallel for num_threads(4 > omp_get_max_threads()                 \
                                         ? omp_get_max_threads()               \
                                         : 4) private(isAineqActive)

      for (b_idx = idx_max; b_idx <= idx_negative; b_idx++) {
        if (WorkingSet.Wid[b_idx - 1] == 3) {
          isAineqActive =
              memspace.workspace_int[(WorkingSet.Wlocalidx[b_idx - 1] +
                                      (mEq << 1)) -
                                     1];
          b_TrialState.lambda[b_idx - 1] = static_cast<double>(isAineqActive) *
                                           b_TrialState.lambda[b_idx - 1];
        }
      }
    }
  }
  idx_max = WorkingSet.sizes[0];
  mEq = WorkingSet.sizes[1];
  i = (WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1)) - WorkingSet.sizes[2];
  idx = WorkingSet.nActiveConstr;
  while ((idx > idx_max + mEq) && (nActiveLBArtificial > 0)) {
    if ((WorkingSet.Wid[idx - 1] == 4) && (WorkingSet.Wlocalidx[idx - 1] > i)) {
      idx_negative = WorkingSet.nActiveConstr - 1;
      smax = b_TrialState.lambda[idx_negative];
      b_TrialState.lambda[idx_negative] = 0.0;
      b_TrialState.lambda[idx - 1] = smax;
      qpactiveset::WorkingSet::removeConstr(WorkingSet, idx);
      nActiveLBArtificial--;
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
