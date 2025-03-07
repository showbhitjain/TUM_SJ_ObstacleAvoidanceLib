//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fmincon.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "fmincon.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "compressBounds.h"
#include "computeFiniteDifferences.h"
#include "driver.h"
#include "factoryConstruct.h"
#include "factoryConstruct1.h"
#include "factoryConstruct2.h"
#include "initActiveSet.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_types.h"
#include "removeDependentLinearEq.h"
#include "rt_nonfinite.h"
#include "stickyStruct.h"
#include "updateWorkingSetForNewQP.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
// Function Definitions
//
// Arguments    : const anonymous_function &fun
//                array<double, 1U> &x0
//                const array<double, 2U> &Aineq
//                const array<double, 2U> &bineq
//                const array<double, 2U> &Aeq
//                const double beq_data[]
//                const int beq_size[2]
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                double *exitflag
// Return Type  : double
//
namespace coder {
double fmincon(const anonymous_function &fun, array<double, 1U> &x0,
               const array<double, 2U> &Aineq, const array<double, 2U> &bineq,
               const array<double, 2U> &Aeq, const double beq_data[],
               const int beq_size[2], const array<double, 1U> &lb,
               const array<double, 1U> &ub, double *exitflag)
{
  internal::i_stickyStruct FcnEvaluator;
  array<double, 2U> Hessian;
  array<double, 1U> fscales_lineq_constraint;
  array<double, 1U> y;
  array<int, 1U> idxDepEq;
  d_struct_T WorkingSet;
  e_struct_T QRManager;
  f_struct_T CholManager;
  g_struct_T QPObjective;
  h_struct_T memspace;
  i_struct_T expl_temp;
  j_struct_T TrialState;
  l_struct_T FiniteDifferences;
  struct_T MeritFunction;
  double beqFiltered_data[6];
  double fval;
  int i;
  int k;
  int mConstrMax;
  int mConstrMax_tmp;
  int mConstrMax_tmp_tmp;
  int mFixed;
  int mLinEq;
  int mLinIneq;
  int mUB;
  int maxDims;
  int nVar;
  int nVarMax;
  nVar = x0.size(0) - 1;
  *exitflag = rtInf;
  if ((lb.size(0) != 0) && (ub.size(0) != 0)) {
    boolean_T exitg1;
    k = 0;
    exitg1 = false;
    while ((!exitg1) && (k <= x0.size(0) - 1)) {
      if (lb[k] > ub[k]) {
        *exitflag = -2.0;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  expl_temp.FiniteDifferenceStepSize.set_size(x0.size(0));
  expl_temp.TypicalX.set_size(x0.size(0));
  k = x0.size(0);
  for (i = 0; i < k; i++) {
    expl_temp.TypicalX[i] = 1.0;
  }
  for (nVarMax = 0; nVarMax <= nVar; nVarMax++) {
    expl_temp.FiniteDifferenceStepSize[nVarMax] = 1.4901161193847656E-8;
  }
  mLinEq = beq_size[0] * beq_size[1];
  mLinIneq = bineq.size(0) * bineq.size(1);
  mConstrMax_tmp_tmp = mLinEq << 1;
  mConstrMax_tmp = mLinIneq + mLinEq;
  mConstrMax =
      ((((mConstrMax_tmp + lb.size(0)) + ub.size(0)) + mConstrMax_tmp_tmp) +
       mLinIneq) +
      1;
  nVarMax = ((x0.size(0) + mConstrMax_tmp_tmp) + mLinIneq) + 1;
  if (nVarMax >= mConstrMax) {
    maxDims = nVarMax;
  } else {
    maxDims = mConstrMax;
  }
  Hessian.set_size(x0.size(0), x0.size(0));
  k = x0.size(0) * x0.size(0);
  for (i = 0; i < k; i++) {
    Hessian[i] = 0.0;
  }
  for (k = 0; k <= nVar; k++) {
    Hessian[k + Hessian.size(0) * k] = 1.0;
  }
  if (*exitflag == -2.0) {
    fval = rtInf;
  } else {
    double normResid;
    optim::coder::fminconsqp::TrialState::factoryConstruct(
        nVarMax, mConstrMax, mLinIneq, mLinEq, x0, TrialState);
    for (k = 0; k <= nVar; k++) {
      TrialState.xstarsqp[k] = x0[k];
    }
    FcnEvaluator.next.next.next.next.next.next.next.next.value = fun;
    optim::coder::utils::FiniteDifferences::factoryConstruct(
        fun, x0.size(0), lb, ub, FiniteDifferences);
    QRManager.ldq = maxDims;
    QRManager.QR.set_size(maxDims, maxDims);
    QRManager.Q.set_size(maxDims, maxDims);
    k = maxDims * maxDims;
    for (i = 0; i < k; i++) {
      QRManager.Q[i] = 0.0;
    }
    QRManager.jpvt.set_size(maxDims);
    for (i = 0; i < maxDims; i++) {
      QRManager.jpvt[i] = 0;
    }
    QRManager.mrows = 0;
    QRManager.ncols = 0;
    QRManager.tau.set_size(maxDims);
    QRManager.minRowCol = 0;
    QRManager.usedPivoting = false;
    CholManager.FMat.set_size(maxDims, maxDims);
    CholManager.ldm = maxDims;
    CholManager.ndims = 0;
    CholManager.info = 0;
    CholManager.scaleFactor = 0.0;
    CholManager.ConvexCheck = true;
    CholManager.regTol_ = rtInf;
    CholManager.workspace_ = rtInf;
    CholManager.workspace2_ = rtInf;
    QPObjective.grad.set_size(nVarMax);
    QPObjective.Hx.set_size(nVarMax - 1);
    QPObjective.maxVar = nVarMax;
    QPObjective.beta = 0.0;
    QPObjective.rho = 0.0;
    QPObjective.prev_objtype = 3;
    QPObjective.prev_nvar = 0;
    QPObjective.prev_hasLinear = false;
    QPObjective.gammaScalar = 0.0;
    QPObjective.nvar = x0.size(0);
    QPObjective.hasLinear = true;
    QPObjective.objtype = 3;
    if (nVarMax >= 2) {
      nVar = nVarMax;
    } else {
      nVar = 2;
    }
    memspace.workspace_double.set_size(maxDims, nVar);
    memspace.workspace_int.set_size(maxDims);
    memspace.workspace_sort.set_size(maxDims);
    fscales_lineq_constraint.set_size(mLinIneq);
    for (i = 0; i < mLinIneq; i++) {
      fscales_lineq_constraint[i] = 1.0;
    }
    optim::coder::qpactiveset::WorkingSet::factoryConstruct(
        mLinIneq, mLinEq, x0.size(0), nVarMax, mConstrMax, WorkingSet);
    maxDims = optim::coder::qpactiveset::initialize::compressBounds(
        x0.size(0), WorkingSet.indexLB, WorkingSet.indexUB,
        WorkingSet.indexFixed, lb, ub, mUB, mFixed);
    nVar = ((mConstrMax_tmp + maxDims) + mUB) + mFixed;
    WorkingSet.mConstr = nVar;
    WorkingSet.mConstrOrig = nVar;
    WorkingSet.mConstrMax = mConstrMax;
    WorkingSet.sizes[0] = mFixed;
    WorkingSet.sizes[1] = mLinEq;
    WorkingSet.sizes[2] = mLinIneq;
    WorkingSet.sizes[3] = maxDims;
    WorkingSet.sizes[4] = mUB;
    WorkingSet.sizesPhaseOne[0] = mFixed;
    WorkingSet.sizesPhaseOne[1] = mLinEq;
    WorkingSet.sizesPhaseOne[2] = mLinIneq;
    WorkingSet.sizesPhaseOne[3] = maxDims + 1;
    WorkingSet.sizesPhaseOne[4] = mUB;
    WorkingSet.sizesRegularized[0] = mFixed;
    WorkingSet.sizesRegularized[1] = mLinEq;
    WorkingSet.sizesRegularized[2] = mLinIneq;
    i = (maxDims + mLinIneq) + mConstrMax_tmp_tmp;
    WorkingSet.sizesRegularized[3] = i;
    WorkingSet.sizesRegularized[4] = mUB;
    WorkingSet.sizesRegPhaseOne[0] = mFixed;
    WorkingSet.sizesRegPhaseOne[1] = mLinEq;
    WorkingSet.sizesRegPhaseOne[2] = mLinIneq;
    WorkingSet.sizesRegPhaseOne[3] = i + 1;
    WorkingSet.sizesRegPhaseOne[4] = mUB;
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = mLinEq;
    WorkingSet.isActiveIdxRegPhaseOne[3] = mLinIneq;
    WorkingSet.isActiveIdxRegPhaseOne[4] = maxDims;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.sizesNormal[k] = WorkingSet.sizes[k];
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (nVar = 0; nVar < 6; nVar++) {
      WorkingSet.isActiveIdx[nVar] = WorkingSet.isActiveIdxRegPhaseOne[nVar];
      WorkingSet.isActiveIdxNormal[nVar] =
          WorkingSet.isActiveIdxRegPhaseOne[nVar];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = mLinEq;
    WorkingSet.isActiveIdxRegPhaseOne[3] = mLinIneq;
    WorkingSet.isActiveIdxRegPhaseOne[4] = maxDims + 1;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (nVar = 0; nVar < 6; nVar++) {
      WorkingSet.isActiveIdxPhaseOne[nVar] =
          WorkingSet.isActiveIdxRegPhaseOne[nVar];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = mLinEq;
    WorkingSet.isActiveIdxRegPhaseOne[3] = mLinIneq;
    WorkingSet.isActiveIdxRegPhaseOne[4] = i;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (nVar = 0; nVar < 6; nVar++) {
      WorkingSet.isActiveIdxRegularized[nVar] =
          WorkingSet.isActiveIdxRegPhaseOne[nVar];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = mLinEq;
    WorkingSet.isActiveIdxRegPhaseOne[3] = mLinIneq;
    WorkingSet.isActiveIdxRegPhaseOne[4] = i + 1;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    if (mLinIneq > 0) {
      i = WorkingSet.nVar;
      for (nVar = 0; nVar < mLinIneq; nVar++) {
        for (nVarMax = 0; nVarMax < i; nVarMax++) {
          WorkingSet.Aineq[nVarMax + WorkingSet.ldA * nVar] =
              Aineq[nVar + mLinIneq * nVarMax];
        }
      }
    }
    if (mLinEq > 0) {
      i = WorkingSet.nVar;
      for (nVar = 0; nVar < mLinEq; nVar++) {
        for (nVarMax = 0; nVarMax < i; nVarMax++) {
          WorkingSet.Aeq[nVarMax + WorkingSet.ldA * nVar] =
              Aeq[nVar + mLinEq * nVarMax];
        }
      }
    }
    y.set_size(mLinEq);
    for (i = 0; i < mLinEq; i++) {
      y[i] = beqFiltered_data[i];
    }
    for (k = 0; k < mLinEq; k++) {
      y[k] = beq_data[k];
    }
    idxDepEq.set_size(x0.size(0) + mLinEq);
    nVar = y.size(0);
    k = y.size(0);
    for (i = 0; i < k; i++) {
      beqFiltered_data[i] = y[i];
    }
    nVar = optim::coder::fminconsqp::internal::removeDependentLinearEq(
        Aeq, beqFiltered_data, nVar, ub, idxDepEq, memspace, TrialState,
        WorkingSet, QRManager, QPObjective);
    if (nVar < 0) {
      nVar = 0;
    }
    nVar = mLinEq - nVar;
    if (lb.size(0) != 0) {
      for (nVarMax = 0; nVarMax < maxDims; nVarMax++) {
        TrialState.xstarsqp[WorkingSet.indexLB[nVarMax] - 1] =
            std::fmax(TrialState.xstarsqp[WorkingSet.indexLB[nVarMax] - 1],
                      lb[WorkingSet.indexLB[nVarMax] - 1]);
      }
    }
    if (ub.size(0) != 0) {
      for (nVarMax = 0; nVarMax < mUB; nVarMax++) {
        TrialState.xstarsqp[WorkingSet.indexUB[nVarMax] - 1] =
            std::fmin(TrialState.xstarsqp[WorkingSet.indexUB[nVarMax] - 1],
                      ub[WorkingSet.indexUB[nVarMax] - 1]);
      }
      for (nVarMax = 0; nVarMax < mFixed; nVarMax++) {
        TrialState.xstarsqp[WorkingSet.indexFixed[nVarMax] - 1] =
            ub[WorkingSet.indexFixed[nVarMax] - 1];
      }
    }
    fval = inverseKinematicsOAModified_anonFcn1(
        fun.workspace.jacobi, fun.workspace.xd_eff_vel,
        fun.workspace.starting_joint_vel, fun.workspace.joint_values,
        fun.workspace.jointVelocityWeightMatrix, fun.workspace.config,
        fun.workspace.slackIsUsed, TrialState.xstarsqp);
    TrialState.sqpFval = fval;
    optim::coder::utils::FiniteDifferences::computeFiniteDifferences(
        FiniteDifferences, fval, TrialState.xstarsqp, TrialState.grad, lb, ub);
    TrialState.FunctionEvaluations = FiniteDifferences.numEvals + 1;
    if (mLinIneq > 0) {
      for (k = 0; k < mLinIneq; k++) {
        TrialState.cIneq[k] = bineq[k];
      }
      internal::blas::xgemv(x0.size(0), mLinIneq, WorkingSet.Aineq,
                            WorkingSet.ldA, TrialState.xstarsqp,
                            TrialState.cIneq);
    }
    if (nVar > 0) {
      y.set_size(TrialState.cEq.size[0]);
      k = TrialState.cEq.size[0];
      for (i = 0; i < k; i++) {
        y[i] = TrialState.cEq.data[i];
      }
      for (k = 0; k < nVar; k++) {
        y[k] = beqFiltered_data[k];
      }
      internal::blas::xgemv(x0.size(0), nVar, WorkingSet.Aeq, WorkingSet.ldA,
                            TrialState.xstarsqp, y);
      TrialState.cEq.size[0] = y.size(0);
      k = y.size(0);
      for (i = 0; i < k; i++) {
        TrialState.cEq.data[i] = y[i];
      }
    }
    optim::coder::fminconsqp::internal::updateWorkingSetForNewQP(
        x0, WorkingSet, mLinIneq, TrialState.cIneq, nVar, TrialState.cEq.data,
        maxDims, lb, mUB, ub, mFixed);
    optim::coder::qpactiveset::WorkingSet::initActiveSet(WorkingSet);
    MeritFunction.penaltyParam = 1.0;
    MeritFunction.threshold = 0.0001;
    MeritFunction.nPenaltyDecreases = 0;
    MeritFunction.linearizedConstrViol = 0.0;
    MeritFunction.initFval = fval;
    normResid = 0.0;
    if (nVar >= 1) {
      i = static_cast<unsigned char>(nVar);
      for (k = 0; k < i; k++) {
        normResid += std::abs(TrialState.cEq.data[k]);
      }
    }
    MeritFunction.initConstrViolationEq = normResid;
    normResid = 0.0;
    for (nVarMax = 0; nVarMax < mLinIneq; nVarMax++) {
      if (TrialState.cIneq[nVarMax] > 0.0) {
        normResid += TrialState.cIneq[nVarMax];
      }
    }
    MeritFunction.initConstrViolationIneq = normResid;
    MeritFunction.phi = 0.0;
    MeritFunction.phiPrimePlus = 0.0;
    MeritFunction.phiFullStep = 0.0;
    MeritFunction.feasRelativeFactor = 0.0;
    MeritFunction.nlpPrimalFeasError = 0.0;
    MeritFunction.nlpDualFeasError = 0.0;
    MeritFunction.nlpComplError = 0.0;
    MeritFunction.firstOrderOpt = 0.0;
    MeritFunction.hasObjective = true;
    expl_temp.ConstrRelTolFactor = 1.0;
    expl_temp.MaxFunctionEvaluations = 100 * x0.size(0);
    expl_temp.MaxIterations = OAGlobals::maxIterations;
    optim::coder::fminconsqp::driver(
        Hessian, bineq, beqFiltered_data, lb, ub, TrialState, MeritFunction,
        FcnEvaluator, FiniteDifferences, memspace, WorkingSet, QRManager,
        CholManager, QPObjective, fscales_lineq_constraint, expl_temp);
    x0.set_size(TrialState.xstarsqp.size(0));
    k = TrialState.xstarsqp.size(0);
    for (i = 0; i < k; i++) {
      x0[i] = TrialState.xstarsqp[i];
    }
    fval = TrialState.sqpFval;
    *exitflag = TrialState.sqpExitFlag;
  }
  return fval;
}

} // namespace coder

//
// File trailer for fmincon.cpp
//
// [EOF]
//
