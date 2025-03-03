//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_exit.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "test_exit.h"
#include "computeComplError.h"
#include "computeDualFeasError.h"
#include "computeGradLag.h"
#include "computePrimalFeasError.h"
#include "computeQ_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "isDeltaXTooSmall.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xgemv.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo e_emlrtBCI{
    -1,          // iFirst
    -1,          // iLast
    1,           // lineNo
    1,           // colNo
    "",          // aName
    "test_exit", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
    "test_exit.p", // pName
    0              // checkKind
};

// Function Definitions
//
// Arguments    : b_struct_T &Flags
//                h_struct_T &memspace
//                struct_T &b_MeritFunction
//                const array<double, 1U> &fscales_lineq_constraint
//                const d_struct_T &WorkingSet
//                j_struct_T &b_TrialState
//                e_struct_T &b_QRManager
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                int runTimeOptions_MaxFunctionEvaluations
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void b_test_exit(b_struct_T &Flags, h_struct_T &memspace,
                 struct_T &b_MeritFunction,
                 const array<double, 1U> &fscales_lineq_constraint,
                 const d_struct_T &WorkingSet, j_struct_T &b_TrialState,
                 e_struct_T &b_QRManager, const array<double, 2U> &lb,
                 const array<double, 2U> &ub,
                 int runTimeOptions_MaxFunctionEvaluations)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                 // iFirst
      -1,                 // iLast
      1,                  // lineNo
      1,                  // colNo
      "",                 // aName
      "computeLambdaLSQ", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+stopping/computeLambdaLSQ.p", // pName
      0                               // checkKind
  };
  double optimRelativeFactor;
  double s;
  double smax;
  int fullRank_R;
  int idx_max;
  int k;
  int mLambda;
  int mLambda_tmp;
  boolean_T isFeasible;
  fullRank_R = WorkingSet.nVar;
  mLambda_tmp = WorkingSet.sizes[0] + WorkingSet.sizes[1];
  mLambda = ((mLambda_tmp + WorkingSet.sizes[2]) + WorkingSet.sizes[3]) +
            WorkingSet.sizes[4];
  if (mLambda > 2147483646) {
    check_forloop_overflow_error();
  }
  for (k = 0; k < mLambda; k++) {
    b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[2], WorkingSet.Aineq, WorkingSet.sizes[1],
      WorkingSet.Aeq, WorkingSet.indexFixed, WorkingSet.sizes[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], WorkingSet.indexUB,
      WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  if (fullRank_R < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (fullRank_R > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      if (fullRank_R > 2147483646) {
        check_forloop_overflow_error();
      }
      for (k = 2; k <= fullRank_R; k++) {
        s = std::abs(b_TrialState.grad[k - 1]);
        if (s > smax) {
          idx_max = k;
          smax = s;
        }
      }
    }
  }
  k = b_TrialState.grad.size(0);
  if ((idx_max < 1) || (idx_max > k)) {
    rtDynamicBoundsError(idx_max, 1, k, e_emlrtBCI);
  }
  optimRelativeFactor =
      std::fmax(1.0, std::abs(b_TrialState.grad[idx_max - 1]));
  if (std::isinf(optimRelativeFactor)) {
    optimRelativeFactor = 1.0;
  }
  b_MeritFunction.nlpPrimalFeasError = stopping::computePrimalFeasError(
      b_TrialState.xstarsqp, WorkingSet.sizes[2], b_TrialState.cIneq,
      WorkingSet.sizes[1], b_TrialState.cEq.data, b_TrialState.cEq.size[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], lb, WorkingSet.indexUB,
      WorkingSet.sizes[4], ub);
  if (b_TrialState.sqpIterations == 0) {
    b_MeritFunction.feasRelativeFactor =
        std::fmax(1.0, b_MeritFunction.nlpPrimalFeasError);
  }
  isFeasible = (b_MeritFunction.nlpPrimalFeasError <=
                1.0E-6 * b_MeritFunction.feasRelativeFactor);
  Flags.gradOK = stopping::computeDualFeasError(
      WorkingSet.nVar, b_TrialState.gradLag, b_MeritFunction.nlpDualFeasError);
  if (!Flags.gradOK) {
    Flags.done = true;
    if (isFeasible) {
      b_TrialState.sqpExitFlag = 2;
    } else {
      b_TrialState.sqpExitFlag = -2;
    }
  } else {
    b_MeritFunction.nlpComplError = stopping::computeComplError(
        fscales_lineq_constraint, b_TrialState.xstarsqp, WorkingSet.sizes[2],
        b_TrialState.cIneq, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
        WorkingSet.indexUB, WorkingSet.sizes[4], ub,
        b_TrialState.lambdaStopTest,
        (WorkingSet.sizes[0] + WorkingSet.sizes[1]) + 1);
    smax = std::fmax(b_MeritFunction.nlpDualFeasError,
                     b_MeritFunction.nlpComplError);
    b_MeritFunction.firstOrderOpt = smax;
    if (b_TrialState.sqpIterations > 1) {
      double d;
      double nlpComplErrorTmp;
      stopping::computeGradLag(
          memspace.workspace_double, WorkingSet.ldA, WorkingSet.nVar,
          b_TrialState.grad, WorkingSet.sizes[2], WorkingSet.Aineq,
          WorkingSet.sizes[1], WorkingSet.Aeq, WorkingSet.indexFixed,
          WorkingSet.sizes[0], WorkingSet.indexLB, WorkingSet.sizes[3],
          WorkingSet.indexUB, WorkingSet.sizes[4],
          b_TrialState.lambdaStopTestPrev);
      stopping::computeDualFeasError(WorkingSet.nVar, memspace.workspace_double,
                                     s);
      nlpComplErrorTmp = stopping::computeComplError(
          fscales_lineq_constraint, b_TrialState.xstarsqp, WorkingSet.sizes[2],
          b_TrialState.cIneq, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
          WorkingSet.indexUB, WorkingSet.sizes[4], ub,
          b_TrialState.lambdaStopTestPrev,
          (WorkingSet.sizes[0] + WorkingSet.sizes[1]) + 1);
      d = std::fmax(s, nlpComplErrorTmp);
      if (d < smax) {
        b_MeritFunction.nlpDualFeasError = s;
        b_MeritFunction.nlpComplError = nlpComplErrorTmp;
        b_MeritFunction.firstOrderOpt = d;
        for (k = 0; k < mLambda; k++) {
          b_TrialState.lambdaStopTest[k] = b_TrialState.lambdaStopTestPrev[k];
        }
      } else {
        for (k = 0; k < mLambda; k++) {
          b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
        }
      }
    } else {
      for (k = 0; k < mLambda; k++) {
        b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
      }
    }
    if (isFeasible &&
        (b_MeritFunction.nlpDualFeasError <= 1.0E-6 * optimRelativeFactor) &&
        (b_MeritFunction.nlpComplError <= 1.0E-6 * optimRelativeFactor)) {
      Flags.done = true;
      b_TrialState.sqpExitFlag = 1;
    } else {
      Flags.done = false;
      if (isFeasible && (b_TrialState.sqpFval < -1.0E+20)) {
        Flags.done = true;
        b_TrialState.sqpExitFlag = -3;
      } else {
        boolean_T guard1;
        guard1 = false;
        if (b_TrialState.sqpIterations > 0) {
          boolean_T dxTooSmall;
          dxTooSmall = stopping::isDeltaXTooSmall(
              b_TrialState.xstarsqp, b_TrialState.delta_x, WorkingSet.nVar);
          if (dxTooSmall) {
            if (!isFeasible) {
              if (Flags.stepType != 2) {
                Flags.stepType = 2;
                Flags.failedLineSearch = false;
                Flags.stepAccepted = false;
                guard1 = true;
              } else {
                Flags.done = true;
                b_TrialState.sqpExitFlag = -2;
              }
            } else {
              int nActiveConstr;
              nActiveConstr = WorkingSet.nActiveConstr - 1;
              if (WorkingSet.nActiveConstr > 0) {
                int iQR_diag;
                boolean_T exitg1;
                boolean_T guard2;
                if (WorkingSet.nActiveConstr > 2147483646) {
                  check_forloop_overflow_error();
                }
                for (k = 0; k <= nActiveConstr; k++) {
                  b_TrialState.lambda[k] = 0.0;
                }
                k = WorkingSet.nVar * WorkingSet.nActiveConstr;
                guard2 = false;
                if (k > 0) {
                  if (WorkingSet.nActiveConstr > 2147483646) {
                    check_forloop_overflow_error();
                  }
                  for (int idx{0}; idx <= nActiveConstr; idx++) {
                    idx_max = WorkingSet.ldA * idx;
                    iQR_diag = b_QRManager.ldq * idx;
                    for (k = 0; k < fullRank_R; k++) {
                      b_QRManager.QR[iQR_diag + k] =
                          WorkingSet.ATwset[idx_max + k];
                    }
                  }
                  guard2 = true;
                } else if (k == 0) {
                  b_QRManager.mrows = WorkingSet.nVar;
                  b_QRManager.ncols = WorkingSet.nActiveConstr;
                  b_QRManager.minRowCol = 0;
                } else {
                  guard2 = true;
                }
                if (guard2) {
                  b_QRManager.usedPivoting = true;
                  b_QRManager.mrows = WorkingSet.nVar;
                  b_QRManager.ncols = WorkingSet.nActiveConstr;
                  iQR_diag = WorkingSet.nVar;
                  idx_max = WorkingSet.nActiveConstr;
                  if (iQR_diag <= idx_max) {
                    idx_max = iQR_diag;
                  }
                  b_QRManager.minRowCol = idx_max;
                  ::coder::internal::lapack::xgeqp3(
                      b_QRManager.QR, WorkingSet.nVar, WorkingSet.nActiveConstr,
                      b_QRManager.jpvt, b_QRManager.tau);
                }
                QRManager::computeQ_(b_QRManager, b_QRManager.mrows);
                ::coder::internal::blas::xgemv(WorkingSet.nVar, WorkingSet.nVar,
                                               b_QRManager.Q, b_QRManager.ldq,
                                               b_TrialState.grad,
                                               memspace.workspace_double);
                iQR_diag = WorkingSet.nVar;
                idx_max = WorkingSet.nActiveConstr;
                if (iQR_diag >= idx_max) {
                  idx_max = iQR_diag;
                }
                smax = std::abs(b_QRManager.QR[0]) *
                       std::fmin(1.4901161193847656E-8,
                                 static_cast<double>(idx_max) *
                                     2.2204460492503131E-16);
                iQR_diag = WorkingSet.nVar;
                fullRank_R = WorkingSet.nActiveConstr;
                if (iQR_diag <= fullRank_R) {
                  fullRank_R = iQR_diag;
                }
                nActiveConstr = 0;
                iQR_diag = 1;
                exitg1 = false;
                while ((!exitg1) && (nActiveConstr < fullRank_R)) {
                  k = b_QRManager.QR.size(0) * b_QRManager.QR.size(1);
                  if ((iQR_diag < 1) || (iQR_diag > k)) {
                    rtDynamicBoundsError(iQR_diag, 1, k, w_emlrtBCI);
                  }
                  if (std::abs(b_QRManager.QR[iQR_diag - 1]) > smax) {
                    nActiveConstr++;
                    iQR_diag = (iQR_diag + b_QRManager.ldq) + 1;
                  } else {
                    exitg1 = true;
                  }
                }
                if (nActiveConstr != 0) {
                  for (k = nActiveConstr; k >= 1; k--) {
                    idx_max = (k + (k - 1) * b_QRManager.ldq) - 1;
                    memspace.workspace_double[k - 1] =
                        memspace.workspace_double[k - 1] /
                        b_QRManager.QR[idx_max];
                    for (int idx{0}; idx <= k - 2; idx++) {
                      iQR_diag = (k - idx) - 2;
                      memspace.workspace_double[iQR_diag] =
                          memspace.workspace_double[iQR_diag] -
                          memspace.workspace_double[k - 1] *
                              b_QRManager.QR[(idx_max - idx) - 1];
                    }
                  }
                }
                iQR_diag = WorkingSet.nActiveConstr;
                if (iQR_diag <= fullRank_R) {
                  fullRank_R = iQR_diag;
                }
                if (fullRank_R > 2147483646) {
                  check_forloop_overflow_error();
                }
                for (int idx{0}; idx < fullRank_R; idx++) {
                  k = b_QRManager.jpvt.size(0);
                  if ((idx + 1 < 1) || (idx + 1 > k)) {
                    rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
                  }
                  k = memspace.workspace_double.size(0) *
                      memspace.workspace_double.size(1);
                  if ((idx + 1 < 1) || (idx + 1 > k)) {
                    rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
                  }
                  k = b_TrialState.lambda.size(0);
                  idx_max = b_QRManager.jpvt[idx];
                  if ((idx_max < 1) || (idx_max > k)) {
                    rtDynamicBoundsError(idx_max, 1, k, w_emlrtBCI);
                  }
                  b_TrialState.lambda[idx_max - 1] =
                      memspace.workspace_double[idx];
                }
                idx_max = WorkingSet.sizes[0] + 1;
                if ((WorkingSet.sizes[0] + 1 <= mLambda_tmp) &&
                    (mLambda_tmp > 2147483646)) {
                  check_forloop_overflow_error();
                }
                for (int idx{idx_max}; idx <= mLambda_tmp; idx++) {
                  k = b_TrialState.lambda.size(0);
                  if ((idx < 1) || (idx > k)) {
                    rtDynamicBoundsError(idx, 1, k, e_emlrtBCI);
                  }
                  k = b_TrialState.lambda.size(0);
                  if (idx > k) {
                    rtDynamicBoundsError(idx, 1, k, e_emlrtBCI);
                  }
                  b_TrialState.lambda[idx - 1] = -b_TrialState.lambda[idx - 1];
                }
                qpactiveset::parseoutput::sortLambdaQP(
                    b_TrialState.lambda, WorkingSet.nActiveConstr,
                    WorkingSet.sizes, WorkingSet.isActiveIdx, WorkingSet.Wid,
                    WorkingSet.Wlocalidx, memspace.workspace_double);
                stopping::computeGradLag(
                    memspace.workspace_double, WorkingSet.ldA, WorkingSet.nVar,
                    b_TrialState.grad, WorkingSet.sizes[2], WorkingSet.Aineq,
                    WorkingSet.sizes[1], WorkingSet.Aeq, WorkingSet.indexFixed,
                    WorkingSet.sizes[0], WorkingSet.indexLB,
                    WorkingSet.sizes[3], WorkingSet.indexUB,
                    WorkingSet.sizes[4], b_TrialState.lambda);
                stopping::computeDualFeasError(WorkingSet.nVar,
                                               memspace.workspace_double, smax);
                s = stopping::computeComplError(
                    fscales_lineq_constraint, b_TrialState.xstarsqp,
                    WorkingSet.sizes[2], b_TrialState.cIneq, WorkingSet.indexLB,
                    WorkingSet.sizes[3], lb, WorkingSet.indexUB,
                    WorkingSet.sizes[4], ub, b_TrialState.lambda,
                    WorkingSet.sizes[0] + 1);
                if ((smax <= 1.0E-6 * optimRelativeFactor) &&
                    (s <= 1.0E-6 * optimRelativeFactor)) {
                  b_MeritFunction.nlpDualFeasError = smax;
                  b_MeritFunction.nlpComplError = s;
                  b_MeritFunction.firstOrderOpt = std::fmax(smax, s);
                  for (k = 0; k < mLambda; k++) {
                    b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
                  }
                  Flags.done = true;
                  b_TrialState.sqpExitFlag = 1;
                } else {
                  Flags.done = true;
                  b_TrialState.sqpExitFlag = 2;
                }
              } else {
                Flags.done = true;
                b_TrialState.sqpExitFlag = 2;
              }
            }
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
        if (guard1) {
          if (b_TrialState.sqpIterations >= 1000) {
            Flags.done = true;
            b_TrialState.sqpExitFlag = 0;
          } else if (b_TrialState.FunctionEvaluations >=
                     runTimeOptions_MaxFunctionEvaluations) {
            Flags.done = true;
            b_TrialState.sqpExitFlag = 0;
          }
        }
      }
    }
  }
}

//
// Arguments    : struct_T &b_MeritFunction
//                const array<double, 1U> &fscales_lineq_constraint
//                const d_struct_T &WorkingSet
//                j_struct_T &b_TrialState
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                int runTimeOptions_MaxFunctionEvaluations
//                boolean_T &Flags_fevalOK
//                boolean_T &Flags_done
//                boolean_T &Flags_stepAccepted
//                boolean_T &Flags_failedLineSearch
//                int &Flags_stepType
// Return Type  : boolean_T
//
boolean_T test_exit(struct_T &b_MeritFunction,
                    const array<double, 1U> &fscales_lineq_constraint,
                    const d_struct_T &WorkingSet, j_struct_T &b_TrialState,
                    const array<double, 2U> &lb, const array<double, 2U> &ub,
                    int runTimeOptions_MaxFunctionEvaluations,
                    boolean_T &Flags_fevalOK, boolean_T &Flags_done,
                    boolean_T &Flags_stepAccepted,
                    boolean_T &Flags_failedLineSearch, int &Flags_stepType)
{
  double smax;
  int idx_max;
  int mLambda;
  int n;
  boolean_T Flags_gradOK;
  boolean_T isFeasible;
  Flags_fevalOK = true;
  Flags_done = false;
  Flags_stepAccepted = false;
  Flags_failedLineSearch = false;
  Flags_stepType = 1;
  mLambda =
      (((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[2]) +
       WorkingSet.sizes[3]) +
      WorkingSet.sizes[4];
  if (mLambda > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int k{0}; k < mLambda; k++) {
    b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[2], WorkingSet.Aineq, WorkingSet.sizes[1],
      WorkingSet.Aeq, WorkingSet.indexFixed, WorkingSet.sizes[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], WorkingSet.indexUB,
      WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  n = WorkingSet.nVar;
  if (n < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (n > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      if (n > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{2}; k <= n; k++) {
        double s;
        s = std::abs(b_TrialState.grad[k - 1]);
        if (s > smax) {
          idx_max = k;
          smax = s;
        }
      }
    }
  }
  n = b_TrialState.grad.size(0);
  if ((idx_max < 1) || (idx_max > n)) {
    rtDynamicBoundsError(idx_max, 1, n, e_emlrtBCI);
  }
  smax = std::fmax(1.0, std::abs(b_TrialState.grad[idx_max - 1]));
  if (std::isinf(smax)) {
    smax = 1.0;
  }
  b_MeritFunction.nlpPrimalFeasError = stopping::computePrimalFeasError(
      b_TrialState.xstarsqp, WorkingSet.sizes[2], b_TrialState.cIneq,
      WorkingSet.sizes[1], b_TrialState.cEq.data, b_TrialState.cEq.size[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], lb, WorkingSet.indexUB,
      WorkingSet.sizes[4], ub);
  b_MeritFunction.feasRelativeFactor =
      std::fmax(1.0, b_MeritFunction.nlpPrimalFeasError);
  isFeasible = (b_MeritFunction.nlpPrimalFeasError <=
                1.0E-6 * b_MeritFunction.feasRelativeFactor);
  Flags_gradOK = stopping::computeDualFeasError(
      WorkingSet.nVar, b_TrialState.gradLag, b_MeritFunction.nlpDualFeasError);
  if (!Flags_gradOK) {
    Flags_done = true;
    if (isFeasible) {
      b_TrialState.sqpExitFlag = 2;
    } else {
      b_TrialState.sqpExitFlag = -2;
    }
  } else {
    stopping::computeComplError(
        fscales_lineq_constraint, b_TrialState.xstarsqp, WorkingSet.sizes[2],
        b_TrialState.cIneq, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
        WorkingSet.indexUB, WorkingSet.sizes[4], ub,
        b_TrialState.lambdaStopTest,
        (WorkingSet.sizes[0] + WorkingSet.sizes[1]) + 1);
    b_MeritFunction.nlpComplError = 0.0;
    b_MeritFunction.firstOrderOpt =
        std::fmax(b_MeritFunction.nlpDualFeasError, 0.0);
    for (int k{0}; k < mLambda; k++) {
      b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
    }
    if (isFeasible && (b_MeritFunction.nlpDualFeasError <= 1.0E-6 * smax)) {
      Flags_done = true;
      b_TrialState.sqpExitFlag = 1;
    } else if (isFeasible && (b_TrialState.sqpFval < -1.0E+20)) {
      Flags_done = true;
      b_TrialState.sqpExitFlag = -3;
    } else if (b_TrialState.FunctionEvaluations >=
               runTimeOptions_MaxFunctionEvaluations) {
      Flags_done = true;
      b_TrialState.sqpExitFlag = 0;
    }
  }
  return Flags_gradOK;
}

} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for test_exit.cpp
//
// [EOF]
//
