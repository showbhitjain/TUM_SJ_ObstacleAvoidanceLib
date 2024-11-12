//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fmincon.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "fmincon.h"
#include "anonymous_function.h"
#include "compressBounds.h"
#include "computeFiniteDifferences.h"
#include "driver.h"
#include "factoryConstruct.h"
#include "factoryConstruct1.h"
#include "factoryConstruct2.h"
#include "initActiveSet.h"
#include "inverseKinematicsWithConstraints.h"
#include "inverseKinematics_internal_types.h"
#include "inverseKinematics_internal_types1.h"
#include "inverseKinematics_types.h"
#include "removeDependentLinearEq.h"
#include "rt_nonfinite.h"
#include "stickyStruct.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const anonymous_function &fun
//                array<double, 1U> &x0
//                const array<double, 2U> &Aeq
//                const double beq_data[]
//                int beq_size
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                double *exitflag
// Return Type  : double
//
namespace coder {
double fmincon(const anonymous_function &fun, array<double, 1U> &x0,
               const array<double, 2U> &Aeq, const double beq_data[],
               int beq_size, const array<double, 1U> &lb,
               const array<double, 1U> &ub, double *exitflag)
{
  array<double, 1U> *A;
  array<double, 1U> *x;
  internal::i_stickyStruct FcnEvaluator;
  array<double, 2U> Hessian;
  array<double, 1U> y;
  array<int, 1U> idxDepEq;
  d_struct_T QRManager;
  e_struct_T CholManager;
  f_struct_T QPObjective;
  g_struct_T memspace;
  h_struct_T TrialState;
  i_struct_T WorkingSet;
  k_struct_T FiniteDifferences;
  struct_T obj;
  double beqFiltered_data[6];
  double fval;
  int i;
  int k;
  int mConstrMax;
  int mConstrMax_tmp_tmp;
  int mFixed;
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
  mConstrMax_tmp_tmp = beq_size << 1;
  mConstrMax =
      (((beq_size + lb.size(0)) + ub.size(0)) + mConstrMax_tmp_tmp) + 1;
  nVarMax = (x0.size(0) + mConstrMax_tmp_tmp) + 1;
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
    double c;
    int mLB;
    boolean_T hasLB;
    boolean_T hasUB;
    optim::coder::fminconsqp::TrialState::factoryConstruct(
        nVarMax, mConstrMax, beq_size, x0, TrialState);
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
    optim::coder::qpactiveset::WorkingSet::factoryConstruct(
        beq_size, x0.size(0), nVarMax, mConstrMax, WorkingSet);
    mLB = optim::coder::qpactiveset::initialize::compressBounds(
        x0.size(0), WorkingSet.indexLB, WorkingSet.indexUB,
        WorkingSet.indexFixed, lb, ub, mUB, mFixed);
    nVar = ((beq_size + mLB) + mUB) + mFixed;
    WorkingSet.mConstr = nVar;
    WorkingSet.mConstrOrig = nVar;
    WorkingSet.mConstrMax = mConstrMax;
    WorkingSet.sizes[0] = mFixed;
    WorkingSet.sizes[1] = beq_size;
    WorkingSet.sizes[2] = 0;
    WorkingSet.sizes[3] = mLB;
    WorkingSet.sizes[4] = mUB;
    WorkingSet.sizesPhaseOne[0] = mFixed;
    WorkingSet.sizesPhaseOne[1] = beq_size;
    WorkingSet.sizesPhaseOne[2] = 0;
    WorkingSet.sizesPhaseOne[3] = mLB + 1;
    WorkingSet.sizesPhaseOne[4] = mUB;
    WorkingSet.sizesRegularized[0] = mFixed;
    WorkingSet.sizesRegularized[1] = beq_size;
    WorkingSet.sizesRegularized[2] = 0;
    i = mLB + mConstrMax_tmp_tmp;
    WorkingSet.sizesRegularized[3] = i;
    WorkingSet.sizesRegularized[4] = mUB;
    WorkingSet.sizesRegPhaseOne[0] = mFixed;
    WorkingSet.sizesRegPhaseOne[1] = beq_size;
    WorkingSet.sizesRegPhaseOne[2] = 0;
    WorkingSet.sizesRegPhaseOne[3] = i + 1;
    WorkingSet.sizesRegPhaseOne[4] = mUB;
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = beq_size;
    WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
    WorkingSet.isActiveIdxRegPhaseOne[4] = mLB;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.sizesNormal[k] = WorkingSet.sizes[k];
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (mConstrMax_tmp_tmp = 0; mConstrMax_tmp_tmp < 6; mConstrMax_tmp_tmp++) {
      WorkingSet.isActiveIdx[mConstrMax_tmp_tmp] =
          WorkingSet.isActiveIdxRegPhaseOne[mConstrMax_tmp_tmp];
      WorkingSet.isActiveIdxNormal[mConstrMax_tmp_tmp] =
          WorkingSet.isActiveIdxRegPhaseOne[mConstrMax_tmp_tmp];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = beq_size;
    WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
    WorkingSet.isActiveIdxRegPhaseOne[4] = mLB + 1;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (mConstrMax_tmp_tmp = 0; mConstrMax_tmp_tmp < 6; mConstrMax_tmp_tmp++) {
      WorkingSet.isActiveIdxPhaseOne[mConstrMax_tmp_tmp] =
          WorkingSet.isActiveIdxRegPhaseOne[mConstrMax_tmp_tmp];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = beq_size;
    WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
    WorkingSet.isActiveIdxRegPhaseOne[4] = i;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    for (mConstrMax_tmp_tmp = 0; mConstrMax_tmp_tmp < 6; mConstrMax_tmp_tmp++) {
      WorkingSet.isActiveIdxRegularized[mConstrMax_tmp_tmp] =
          WorkingSet.isActiveIdxRegPhaseOne[mConstrMax_tmp_tmp];
    }
    WorkingSet.isActiveIdxRegPhaseOne[0] = 1;
    WorkingSet.isActiveIdxRegPhaseOne[1] = mFixed;
    WorkingSet.isActiveIdxRegPhaseOne[2] = beq_size;
    WorkingSet.isActiveIdxRegPhaseOne[3] = 0;
    WorkingSet.isActiveIdxRegPhaseOne[4] = i + 1;
    WorkingSet.isActiveIdxRegPhaseOne[5] = mUB;
    for (k = 0; k < 5; k++) {
      WorkingSet.isActiveIdxRegPhaseOne[k + 1] +=
          WorkingSet.isActiveIdxRegPhaseOne[k];
    }
    if (beq_size > 0) {
      i = WorkingSet.nVar;
      for (nVar = 0; nVar < beq_size; nVar++) {
        for (nVarMax = 0; nVarMax < i; nVarMax++) {
          WorkingSet.Aeq[nVarMax + WorkingSet.ldA * nVar] =
              Aeq[nVar + beq_size * nVarMax];
        }
      }
    }
    y.set_size(beq_size);
    for (i = 0; i < beq_size; i++) {
      y[i] = beqFiltered_data[i];
    }
    for (k = 0; k < beq_size; k++) {
      y[k] = beq_data[k];
    }
    idxDepEq.set_size(x0.size(0) + beq_size);
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
    maxDims = beq_size - nVar;
    if (lb.size(0) != 0) {
      for (nVar = 0; nVar < mLB; nVar++) {
        TrialState.xstarsqp[WorkingSet.indexLB[nVar] - 1] =
            std::fmax(TrialState.xstarsqp[WorkingSet.indexLB[nVar] - 1],
                      lb[WorkingSet.indexLB[nVar] - 1]);
      }
    }
    if (ub.size(0) != 0) {
      for (nVar = 0; nVar < mUB; nVar++) {
        TrialState.xstarsqp[WorkingSet.indexUB[nVar] - 1] =
            std::fmin(TrialState.xstarsqp[WorkingSet.indexUB[nVar] - 1],
                      ub[WorkingSet.indexUB[nVar] - 1]);
      }
      for (nVar = 0; nVar < mFixed; nVar++) {
        TrialState.xstarsqp[WorkingSet.indexFixed[nVar] - 1] =
            ub[WorkingSet.indexFixed[nVar] - 1];
      }
    }
    fval = inverseKinematics_anonFcn1(
        fun.workspace.jacobi, fun.workspace.xd_eff_vel.data,
        fun.workspace.xd_eff_vel.size[0], fun.workspace.starting_joint_vel,
        fun.workspace.joint_values, fun.workspace.config.useObjective1,
        fun.workspace.config.weight1, fun.workspace.config.useObjective2,
        fun.workspace.config.weight2, fun.workspace.config.useObjective3,
        fun.workspace.config.weight3, fun.workspace.config.useObjective4,
        fun.workspace.config.weight4, fun.workspace.config.useObjective5,
        fun.workspace.config.weight5, fun.workspace.config.useObjective6,
        fun.workspace.config.weight6, TrialState.xstarsqp);
    TrialState.sqpFval = fval;
    optim::coder::utils::FiniteDifferences::computeFiniteDifferences(
        FiniteDifferences, fval, TrialState.xstarsqp, TrialState.grad, lb, ub);
    TrialState.FunctionEvaluations = FiniteDifferences.numEvals + 1;
    if (maxDims > 0) {
      y.set_size(TrialState.cEq.size[0]);
      k = TrialState.cEq.size[0];
      for (i = 0; i < k; i++) {
        y[i] = TrialState.cEq.data[i];
      }
      for (k = 0; k < maxDims; k++) {
        y[k] = beqFiltered_data[k];
      }
      nVar = x0.size(0);
      A = &WorkingSet.Aeq;
      nVarMax = WorkingSet.ldA;
      x = &TrialState.xstarsqp;
      if (nVar != 0) {
        for (mConstrMax = 0; mConstrMax < maxDims; mConstrMax++) {
          y[mConstrMax] = -y[mConstrMax];
        }
        mConstrMax = 0;
        i = nVarMax * (maxDims - 1) + 1;
        for (k = 1; nVarMax < 0 ? k >= i : k <= i; k += nVarMax) {
          c = 0.0;
          mConstrMax_tmp_tmp = (k + nVar) - 1;
          for (int ia{k}; ia <= mConstrMax_tmp_tmp; ia++) {
            c += (*A)[ia - 1] * (*x)[ia - k];
          }
          y[mConstrMax] = y[mConstrMax] + c;
          mConstrMax++;
        }
      }
      TrialState.cEq.size[0] = y.size(0);
      k = y.size(0);
      for (i = 0; i < k; i++) {
        TrialState.cEq.data[i] = y[i];
      }
    }
    for (nVar = 0; nVar < maxDims; nVar++) {
      WorkingSet.beq.data[nVar] = -TrialState.cEq.data[nVar];
      WorkingSet.bwset[mFixed + nVar] = WorkingSet.beq.data[nVar];
    }
    hasLB = (lb.size(0) != 0);
    hasUB = (ub.size(0) != 0);
    if (hasLB) {
      for (nVar = 0; nVar < mLB; nVar++) {
        WorkingSet.lb[WorkingSet.indexLB[nVar] - 1] =
            -lb[WorkingSet.indexLB[nVar] - 1] +
            x0[WorkingSet.indexLB[nVar] - 1];
      }
    }
    if (hasUB) {
      for (nVar = 0; nVar < mUB; nVar++) {
        WorkingSet.ub[WorkingSet.indexUB[nVar] - 1] =
            ub[WorkingSet.indexUB[nVar] - 1] - x0[WorkingSet.indexUB[nVar] - 1];
      }
    }
    if (hasLB && hasUB) {
      for (nVar = 0; nVar < mFixed; nVar++) {
        c = ub[WorkingSet.indexFixed[nVar] - 1] -
            x0[WorkingSet.indexFixed[nVar] - 1];
        WorkingSet.ub[WorkingSet.indexFixed[nVar] - 1] = c;
        WorkingSet.bwset[nVar] = c;
      }
    }
    optim::coder::qpactiveset::WorkingSet::initActiveSet(WorkingSet);
    obj.penaltyParam = 1.0;
    obj.threshold = 0.0001;
    obj.nPenaltyDecreases = 0;
    obj.linearizedConstrViol = 0.0;
    obj.initFval = fval;
    c = 0.0;
    if (maxDims >= 1) {
      i = static_cast<unsigned char>(maxDims);
      for (k = 0; k < i; k++) {
        c += std::abs(TrialState.cEq.data[k]);
      }
    }
    obj.initConstrViolationEq = c;
    obj.initConstrViolationIneq = 0.0;
    obj.phi = 0.0;
    obj.phiPrimePlus = 0.0;
    obj.phiFullStep = 0.0;
    obj.feasRelativeFactor = 0.0;
    obj.nlpPrimalFeasError = 0.0;
    obj.nlpDualFeasError = 0.0;
    obj.nlpComplError = 0.0;
    obj.firstOrderOpt = 0.0;
    obj.hasObjective = true;
    optim::coder::fminconsqp::driver(
        Hessian, beqFiltered_data, lb, ub, TrialState, obj, FcnEvaluator,
        FiniteDifferences, memspace, WorkingSet, QRManager, CholManager,
        QPObjective, 100 * x0.size(0));
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
