//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fmincon.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "fmincon.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "checkLinearInputs.h"
#include "compressBounds.h"
#include "computeFiniteDifferences.h"
#include "computeLinearResiduals.h"
#include "driver.h"
#include "eml_int_forloop_overflow_check.h"
#include "factoryConstruct.h"
#include "factoryConstruct1.h"
#include "factoryConstruct2.h"
#include "initActiveSet.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "loadProblem.h"
#include "removeDependentLinearEq.h"
#include "rt_nonfinite.h"
#include "stickyStruct.h"
#include "updateWorkingSetForNewQP.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void k_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void l_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

// Function Definitions
//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void k_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Objective or nonlinear constraint undefined at initial point. "
               "Fmincon cannot continue.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void l_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Initial point must be non-empty.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const anonymous_function &fun
//                array<double, 1U> &x0
//                const array<double, 2U> &Aineq
//                const array<double, 2U> &bineq
//                const array<double, 2U> &Aeq
//                const double beq_data[]
//                const int beq_size[2]
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                double *exitflag
// Return Type  : double
//
namespace coder {
double fmincon(const anonymous_function &fun, array<double, 1U> &x0,
               const array<double, 2U> &Aineq, const array<double, 2U> &bineq,
               const array<double, 2U> &Aeq, const double beq_data[],
               const int beq_size[2], const array<double, 2U> &lb,
               const array<double, 2U> &ub, double *exitflag)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                               // iFirst
      -1,                               // iLast
      1,                                // lineNo
      1,                                // colNo
      "",                               // aName
      "convertFminconOptionsForSolver", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+options/"
      "convertFminconOptionsForSolver.p", // pName
      0                                   // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                                                     // iFirst
      -1,                                                     // iLast
      1,                                                      // lineNo
      1,                                                      // colNo
      "",                                                     // aName
      "fmincon",                                              // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/eml/fmincon.p", // pName
      0                                                       // checkKind
  };
  static rtRunTimeErrorInfo c_emlrtRTEI{
      1,        // lineNo
      "checkX0" // fName
  };
  static rtRunTimeErrorInfo d_emlrtRTEI{
      1,        // lineNo
      "fmincon" // fName
  };
  internal::i_stickyStruct FcnEvaluator;
  array<double, 2U> Hessian;
  array<double, 1U> b_y;
  array<double, 1U> fscales_lineq_constraint;
  array<int, 1U> idxDepEq;
  array<boolean_T, 1U> b;
  d_struct_T WorkingSet;
  e_struct_T QRManager;
  f_struct_T CholManager;
  g_struct_T QPObjective;
  h_struct_T memspace;
  i_struct_T expl_temp;
  j_struct_T TrialState;
  l_struct_T FiniteDifferences;
  struct_T MeritFunction;
  double d;
  double fval;
  int i;
  int mConstrMax;
  int mLinEq;
  int mLinIneq;
  int maxDims;
  int nDepEq;
  int nVar;
  int nVarMax;
  int varargin_1;
  boolean_T exitg1;
  boolean_T y;
  if (x0.size(0) == 0) {
    l_rtErrorWithMessageID(c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
  }
  b.set_size(x0.size(0));
  nDepEq = x0.size(0);
  for (i = 0; i < nDepEq; i++) {
    d = x0[i];
    b[i] = ((!std::isinf(d)) && (!std::isnan(d)));
  }
  y = true;
  if (b.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  nDepEq = 1;
  exitg1 = false;
  while ((!exitg1) && (nDepEq <= b.size(0))) {
    if (!b[nDepEq - 1]) {
      y = false;
      exitg1 = true;
    } else {
      nDepEq++;
    }
  }
  if (!y) {
    rtErrorWithMessageID("x0", c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
  }
  nVar = x0.size(0) - 1;
  *exitflag = optim::coder::validate::checkLinearInputs(
      x0.size(0), Aineq, bineq, Aeq, beq_data, beq_size, lb, ub);
  expl_temp.FiniteDifferenceStepSize.set_size(x0.size(0));
  expl_temp.TypicalX.set_size(x0.size(0));
  nDepEq = x0.size(0);
  for (i = 0; i < nDepEq; i++) {
    expl_temp.TypicalX[i] = 1.0;
  }
  if (x0.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  for (mConstrMax = 0; mConstrMax <= nVar; mConstrMax++) {
    if (mConstrMax + 1 > expl_temp.FiniteDifferenceStepSize.size(0)) {
      rtDynamicBoundsError(mConstrMax + 1, 1,
                           expl_temp.FiniteDifferenceStepSize.size(0),
                           w_emlrtBCI);
    }
    expl_temp.FiniteDifferenceStepSize[mConstrMax] = 1.4901161193847656E-8;
  }
  mLinEq = beq_size[0] * beq_size[1];
  mLinIneq = bineq.size(0) * bineq.size(1);
  nDepEq = (mLinEq << 1) + 1;
  mConstrMax =
      ((((mLinIneq + mLinEq) + lb.size(0)) + ub.size(0)) + nDepEq) + mLinIneq;
  nVarMax = (x0.size(0) + nDepEq) + mLinIneq;
  if (nVarMax >= mConstrMax) {
    maxDims = nVarMax;
  } else {
    maxDims = mConstrMax;
  }
  varargin_1 = x0.size(0);
  Hessian.set_size(x0.size(0), x0.size(0));
  nDepEq = x0.size(0) * x0.size(0);
  for (i = 0; i < nDepEq; i++) {
    Hessian[i] = 0.0;
  }
  if (x0.size(0) > 0) {
    if (x0.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    for (nDepEq = 0; nDepEq < varargin_1; nDepEq++) {
      Hessian[nDepEq + Hessian.size(0) * nDepEq] = 1.0;
    }
  }
  if (*exitflag == -2.0) {
    if (x0.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    fval = rtInf;
  } else {
    double normResid;
    optim::coder::fminconsqp::TrialState::factoryConstruct(
        nVarMax, mConstrMax, mLinIneq, mLinEq, x0, TrialState);
    if (x0.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    for (nDepEq = 0; nDepEq <= nVar; nDepEq++) {
      TrialState.xstarsqp[nDepEq] = x0[nDepEq];
    }
    FcnEvaluator.next.next.next.next.next.next.next.next.value = fun;
    optim::coder::utils::FiniteDifferences::factoryConstruct(
        fun, x0.size(0), lb, ub, FiniteDifferences);
    QRManager.ldq = maxDims;
    QRManager.QR.set_size(maxDims, maxDims);
    QRManager.Q.set_size(maxDims, maxDims);
    nDepEq = maxDims * maxDims;
    for (i = 0; i < nDepEq; i++) {
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
      nDepEq = nVarMax;
    } else {
      nDepEq = 2;
    }
    memspace.workspace_double.set_size(maxDims, nDepEq);
    memspace.workspace_int.set_size(maxDims);
    memspace.workspace_sort.set_size(maxDims);
    fscales_lineq_constraint.set_size(mLinIneq);
    for (i = 0; i < mLinIneq; i++) {
      fscales_lineq_constraint[i] = 1.0;
    }
    optim::coder::qpactiveset::WorkingSet::factoryConstruct(
        mLinIneq, mLinEq, x0.size(0), nVarMax, mConstrMax, WorkingSet);
    nVarMax = optim::coder::qpactiveset::initialize::compressBounds(
        x0.size(0), WorkingSet.indexLB, WorkingSet.indexUB,
        WorkingSet.indexFixed, lb, ub, maxDims, nVar);
    optim::coder::qpactiveset::WorkingSet::loadProblem(
        WorkingSet, mLinIneq, mLinIneq, Aineq, mLinEq, mLinEq, Aeq, nVarMax,
        maxDims, nVar, mConstrMax);
    b_y.set_size(mLinEq);
    for (nDepEq = 0; nDepEq < mLinEq; nDepEq++) {
      b_y[nDepEq] = beq_data[nDepEq];
    }
    idxDepEq.set_size(x0.size(0) + mLinEq);
    b_y.reserve(6);
    nDepEq = optim::coder::fminconsqp::internal::removeDependentLinearEq(
        Aeq, (double *)b_y.data(), (*(int(*)[1])b_y.size())[0], ub, idxDepEq,
        memspace, TrialState, WorkingSet, QRManager, QPObjective);
    b_y.set_size(b_y.size(0));
    if (nDepEq < 0) {
      nDepEq = 0;
    }
    varargin_1 = mLinEq - nDepEq;
    if (lb.size(0) != 0) {
      if (nVarMax > 2147483646) {
        check_forloop_overflow_error();
      }
      for (mConstrMax = 0; mConstrMax < nVarMax; mConstrMax++) {
        if ((mConstrMax + 1 < 1) ||
            (mConstrMax + 1 > WorkingSet.indexLB.size(0))) {
          rtDynamicBoundsError(mConstrMax + 1, 1, WorkingSet.indexLB.size(0),
                               x_emlrtBCI);
        }
        nDepEq = WorkingSet.indexLB[mConstrMax] - 1;
        y = ((WorkingSet.indexLB[mConstrMax] < 1) ||
             (WorkingSet.indexLB[mConstrMax] > TrialState.xstarsqp.size(0)));
        if (y) {
          rtDynamicBoundsError(WorkingSet.indexLB[mConstrMax], 1,
                               TrialState.xstarsqp.size(0), x_emlrtBCI);
        }
        if ((WorkingSet.indexLB[mConstrMax] < 1) ||
            (WorkingSet.indexLB[mConstrMax] > lb.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexLB[mConstrMax], 1, lb.size(0),
                               x_emlrtBCI);
        }
        TrialState.xstarsqp[WorkingSet.indexLB[mConstrMax] - 1] =
            std::fmax(TrialState.xstarsqp[nDepEq], lb[nDepEq]);
      }
    }
    if (ub.size(0) != 0) {
      if (maxDims > 2147483646) {
        check_forloop_overflow_error();
      }
      for (mConstrMax = 0; mConstrMax < maxDims; mConstrMax++) {
        if ((mConstrMax + 1 < 1) ||
            (mConstrMax + 1 > WorkingSet.indexUB.size(0))) {
          rtDynamicBoundsError(mConstrMax + 1, 1, WorkingSet.indexUB.size(0),
                               x_emlrtBCI);
        }
        nDepEq = WorkingSet.indexUB[mConstrMax] - 1;
        y = ((WorkingSet.indexUB[mConstrMax] < 1) ||
             (WorkingSet.indexUB[mConstrMax] > TrialState.xstarsqp.size(0)));
        if (y) {
          rtDynamicBoundsError(WorkingSet.indexUB[mConstrMax], 1,
                               TrialState.xstarsqp.size(0), x_emlrtBCI);
        }
        if ((WorkingSet.indexUB[mConstrMax] < 1) ||
            (WorkingSet.indexUB[mConstrMax] > ub.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexUB[mConstrMax], 1, ub.size(0),
                               x_emlrtBCI);
        }
        TrialState.xstarsqp[WorkingSet.indexUB[mConstrMax] - 1] =
            std::fmin(TrialState.xstarsqp[nDepEq], ub[nDepEq]);
      }
      if (nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (mConstrMax = 0; mConstrMax < nVar; mConstrMax++) {
        if ((mConstrMax + 1 < 1) ||
            (mConstrMax + 1 > WorkingSet.indexFixed.size(0))) {
          rtDynamicBoundsError(mConstrMax + 1, 1, WorkingSet.indexFixed.size(0),
                               x_emlrtBCI);
        }
        if ((WorkingSet.indexFixed[mConstrMax] < 1) ||
            (WorkingSet.indexFixed[mConstrMax] > ub.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexFixed[mConstrMax], 1, ub.size(0),
                               x_emlrtBCI);
        }
        if ((WorkingSet.indexFixed[mConstrMax] < 1) ||
            (WorkingSet.indexFixed[mConstrMax] > TrialState.xstarsqp.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexFixed[mConstrMax], 1,
                               TrialState.xstarsqp.size(0), x_emlrtBCI);
        }
        TrialState.xstarsqp[WorkingSet.indexFixed[mConstrMax] - 1] =
            ub[WorkingSet.indexFixed[mConstrMax] - 1];
      }
    }
    if (TrialState.xstarsqp.size(0) == 0) {
      d_rtErrorWithMessageID("input", emlrtRTEI.fName, emlrtRTEI.lineNo);
    }
    fval = inverseKinematicsOAModified_anonFcn1(
        fun.workspace.jacobi, fun.workspace.xd_eff_vel,
        fun.workspace.starting_joint_vel, fun.workspace.joint_values,
        fun.workspace.jointVelocityWeightMatrix, fun.workspace.config,
        fun.workspace.slackIsUsed, TrialState.xstarsqp);
    nDepEq = 1;
    y = std::isnan(fval);
    if (std::isinf(fval) || y) {
      if (y) {
        nDepEq = -3;
      } else if (fval < 0.0) {
        nDepEq = -1;
      } else {
        nDepEq = -2;
      }
    }
    TrialState.sqpFval = fval;
    if (nDepEq == 1) {
      nDepEq = 1;
    }
    if (nDepEq != 1) {
      k_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    optim::coder::utils::FiniteDifferences::computeFiniteDifferences(
        FiniteDifferences, fval, TrialState.xstarsqp, TrialState.grad, lb, ub,
        expl_temp.FiniteDifferenceStepSize, expl_temp.TypicalX);
    TrialState.FunctionEvaluations = FiniteDifferences.numEvals + 1;
    optim::coder::fminconsqp::internal::computeLinearResiduals(
        TrialState.xstarsqp, x0.size(0), TrialState.cIneq, mLinIneq,
        WorkingSet.Aineq, bineq, WorkingSet.ldA, TrialState.cEq.data,
        TrialState.cEq.size[0], varargin_1, WorkingSet.Aeq,
        (const double *)b_y.data(), WorkingSet.ldA);
    optim::coder::fminconsqp::internal::updateWorkingSetForNewQP(
        x0, WorkingSet, mLinIneq, TrialState.cIneq, varargin_1,
        TrialState.cEq.data, TrialState.cEq.size[0], nVarMax, lb, maxDims, ub,
        nVar);
    optim::coder::qpactiveset::WorkingSet::initActiveSet(WorkingSet);
    MeritFunction.penaltyParam = 1.0;
    MeritFunction.threshold = 0.0001;
    MeritFunction.nPenaltyDecreases = 0;
    MeritFunction.linearizedConstrViol = 0.0;
    MeritFunction.initFval = fval;
    normResid = 0.0;
    if (varargin_1 >= 1) {
      if (varargin_1 > 2147483646) {
        check_forloop_overflow_error();
      }
      i = static_cast<unsigned char>(varargin_1);
      for (nDepEq = 0; nDepEq < i; nDepEq++) {
        normResid += std::abs(TrialState.cEq.data[nDepEq]);
      }
    }
    MeritFunction.initConstrViolationEq = normResid;
    normResid = 0.0;
    if (mLinIneq > 2147483646) {
      check_forloop_overflow_error();
    }
    for (mConstrMax = 0; mConstrMax < mLinIneq; mConstrMax++) {
      if (mConstrMax + 1 > TrialState.cIneq.size(0)) {
        rtDynamicBoundsError(mConstrMax + 1, 1, TrialState.cIneq.size(0),
                             b_emlrtBCI);
      }
      d = TrialState.cIneq[mConstrMax];
      if (d > 0.0) {
        if (mConstrMax + 1 > TrialState.cIneq.size(0)) {
          rtDynamicBoundsError(mConstrMax + 1, 1, TrialState.cIneq.size(0),
                               b_emlrtBCI);
        }
        normResid += d;
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
    expl_temp.MaxIterations = 1000;
    optim::coder::fminconsqp::driver(
        Hessian, bineq, (const double *)b_y.data(), lb, ub, TrialState,
        MeritFunction, FcnEvaluator, FiniteDifferences, memspace, WorkingSet,
        QRManager, CholManager, QPObjective, fscales_lineq_constraint,
        expl_temp);
    x0.set_size(TrialState.xstarsqp.size(0));
    nDepEq = TrialState.xstarsqp.size(0);
    for (i = 0; i < nDepEq; i++) {
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
