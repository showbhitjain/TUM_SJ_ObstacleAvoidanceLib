//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_exit.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "test_exit.h"
#include "computeComplError.h"
#include "computeGradLag.h"
#include "computePrimalFeasError.h"
#include "computeQ_.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

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
  array<double, 2U> *b_gradLag;
  array<double, 1U> *gradLag;
  double optimRelativeFactor;
  double s;
  double smax;
  int idx;
  int idx_max;
  int mLambda;
  int mLambda_tmp;
  int nVar_tmp;
  int rankR;
  boolean_T dxTooSmall;
  boolean_T exitg1;
  boolean_T isFeasible;
  nVar_tmp = WorkingSet.nVar;
  mLambda_tmp = WorkingSet.sizes[0] + WorkingSet.sizes[1];
  mLambda = (((mLambda_tmp + WorkingSet.sizes[2]) + WorkingSet.sizes[3]) +
             WorkingSet.sizes[4]) -
            1;
  for (rankR = 0; rankR <= mLambda; rankR++) {
    b_TrialState.lambdaStopTest[rankR] = b_TrialState.lambdasqp[rankR];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[2], WorkingSet.Aineq, WorkingSet.sizes[1],
      WorkingSet.Aeq, WorkingSet.indexFixed, WorkingSet.sizes[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], WorkingSet.indexUB,
      WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  if (WorkingSet.nVar < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (WorkingSet.nVar > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      for (rankR = 2; rankR <= nVar_tmp; rankR++) {
        s = std::abs(b_TrialState.grad[rankR - 1]);
        if (s > smax) {
          idx_max = rankR;
          smax = s;
        }
      }
    }
  }
  optimRelativeFactor =
      std::fmax(1.0, std::abs(b_TrialState.grad[idx_max - 1]));
  if (std::isinf(optimRelativeFactor)) {
    optimRelativeFactor = 1.0;
  }
  b_MeritFunction.nlpPrimalFeasError = stopping::computePrimalFeasError(
      b_TrialState.xstarsqp, WorkingSet.sizes[2], b_TrialState.cIneq,
      WorkingSet.sizes[1], b_TrialState.cEq.data, WorkingSet.indexLB,
      WorkingSet.sizes[3], lb, WorkingSet.indexUB, WorkingSet.sizes[4], ub);
  if (b_TrialState.sqpIterations == 0) {
    b_MeritFunction.feasRelativeFactor =
        std::fmax(1.0, b_MeritFunction.nlpPrimalFeasError);
  }
  isFeasible = (b_MeritFunction.nlpPrimalFeasError <=
                1.0E-6 * b_MeritFunction.feasRelativeFactor);
  gradLag = &b_TrialState.gradLag;
  dxTooSmall = true;
  smax = 0.0;
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= nVar_tmp - 1)) {
    dxTooSmall =
        ((!std::isinf((*gradLag)[idx])) && (!std::isnan((*gradLag)[idx])));
    if (!dxTooSmall) {
      exitg1 = true;
    } else {
      smax = std::fmax(smax, std::abs((*gradLag)[idx]));
      idx++;
    }
  }
  Flags.gradOK = dxTooSmall;
  b_MeritFunction.nlpDualFeasError = smax;
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
        b_TrialState.lambdaStopTest, mLambda_tmp + 1);
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
      b_gradLag = &memspace.workspace_double;
      s = 0.0;
      idx = 0;
      exitg1 = false;
      while ((!exitg1) && (idx <= nVar_tmp - 1)) {
        dxTooSmall = ((!std::isinf((*b_gradLag)[idx])) &&
                      (!std::isnan((*b_gradLag)[idx])));
        if (!dxTooSmall) {
          exitg1 = true;
        } else {
          s = std::fmax(s, std::abs((*b_gradLag)[idx]));
          idx++;
        }
      }
      nlpComplErrorTmp = stopping::computeComplError(
          fscales_lineq_constraint, b_TrialState.xstarsqp, WorkingSet.sizes[2],
          b_TrialState.cIneq, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
          WorkingSet.indexUB, WorkingSet.sizes[4], ub,
          b_TrialState.lambdaStopTestPrev, mLambda_tmp + 1);
      d = std::fmax(s, nlpComplErrorTmp);
      if (d < smax) {
        b_MeritFunction.nlpDualFeasError = s;
        b_MeritFunction.nlpComplError = nlpComplErrorTmp;
        b_MeritFunction.firstOrderOpt = d;
        for (rankR = 0; rankR <= mLambda; rankR++) {
          b_TrialState.lambdaStopTest[rankR] =
              b_TrialState.lambdaStopTestPrev[rankR];
        }
      } else {
        for (rankR = 0; rankR <= mLambda; rankR++) {
          b_TrialState.lambdaStopTestPrev[rankR] =
              b_TrialState.lambdaStopTest[rankR];
        }
      }
    } else {
      for (rankR = 0; rankR <= mLambda; rankR++) {
        b_TrialState.lambdaStopTestPrev[rankR] =
            b_TrialState.lambdaStopTest[rankR];
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
          dxTooSmall = true;
          idx = 0;
          exitg1 = false;
          while ((!exitg1) && (idx <= nVar_tmp - 1)) {
            if (1.0E-6 * std::fmax(1.0, std::abs(b_TrialState.xstarsqp[idx])) <=
                std::abs(b_TrialState.delta_x[idx])) {
              dxTooSmall = false;
              exitg1 = true;
            } else {
              idx++;
            }
          }
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
                int fullRank_R;
                int ix;
                boolean_T guard2;
                for (rankR = 0; rankR <= nActiveConstr; rankR++) {
                  b_TrialState.lambda[rankR] = 0.0;
                }
                ix = WorkingSet.nVar * WorkingSet.nActiveConstr;
                guard2 = false;
                if (ix > 0) {
                  for (idx = 0; idx <= nActiveConstr; idx++) {
                    idx_max = WorkingSet.ldA * idx;
                    ix = b_QRManager.ldq * idx;
                    for (rankR = 0; rankR < nVar_tmp; rankR++) {
                      b_QRManager.QR[ix + rankR] =
                          WorkingSet.ATwset[idx_max + rankR];
                    }
                  }
                  guard2 = true;
                } else if (ix == 0) {
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
                  ix = WorkingSet.nVar;
                  idx_max = WorkingSet.nActiveConstr;
                  if (ix <= idx_max) {
                    idx_max = ix;
                  }
                  b_QRManager.minRowCol = idx_max;
                  ::coder::internal::lapack::xgeqp3(
                      b_QRManager.QR, WorkingSet.nVar, WorkingSet.nActiveConstr,
                      b_QRManager.jpvt, b_QRManager.tau);
                }
                QRManager::computeQ_(b_QRManager, b_QRManager.mrows);
                ix = WorkingSet.nVar;
                idx_max = WorkingSet.nActiveConstr;
                if (ix >= idx_max) {
                  idx_max = ix;
                }
                smax = std::abs(b_QRManager.QR[0]) *
                       std::fmin(1.4901161193847656E-8,
                                 static_cast<double>(idx_max) *
                                     2.2204460492503131E-16);
                ix = WorkingSet.nVar;
                fullRank_R = WorkingSet.nActiveConstr;
                if (ix <= fullRank_R) {
                  fullRank_R = ix;
                }
                rankR = 0;
                idx_max = 0;
                while ((rankR < fullRank_R) &&
                       (std::abs(b_QRManager.QR[idx_max]) > smax)) {
                  rankR++;
                  idx_max = (idx_max + b_QRManager.ldq) + 1;
                }
                idx_max = b_QRManager.ldq;
                b_gradLag = &memspace.workspace_double;
                if (nVar_tmp != 0) {
                  for (idx = 0; idx < nVar_tmp; idx++) {
                    (*b_gradLag)[idx] = 0.0;
                  }
                  idx = 0;
                  ix = idx_max * (nVar_tmp - 1) + 1;
                  for (int iac{1}; idx_max < 0 ? iac >= ix : iac <= ix;
                       iac += idx_max) {
                    smax = 0.0;
                    nActiveConstr = (iac + nVar_tmp) - 1;
                    for (int ia{iac}; ia <= nActiveConstr; ia++) {
                      smax +=
                          b_QRManager.Q[ia - 1] * b_TrialState.grad[ia - iac];
                    }
                    (*b_gradLag)[idx] = (*b_gradLag)[idx] + smax;
                    idx++;
                  }
                }
                if (rankR != 0) {
                  for (nActiveConstr = rankR; nActiveConstr >= 1;
                       nActiveConstr--) {
                    idx_max = (nActiveConstr +
                               (nActiveConstr - 1) * b_QRManager.ldq) -
                              1;
                    memspace.workspace_double[nActiveConstr - 1] =
                        memspace.workspace_double[nActiveConstr - 1] /
                        b_QRManager.QR[idx_max];
                    for (idx = 0; idx <= nActiveConstr - 2; idx++) {
                      ix = (nActiveConstr - idx) - 2;
                      memspace.workspace_double[ix] =
                          memspace.workspace_double[ix] -
                          memspace.workspace_double[nActiveConstr - 1] *
                              b_QRManager.QR[(idx_max - idx) - 1];
                    }
                  }
                }
                ix = WorkingSet.nActiveConstr;
                if (ix <= fullRank_R) {
                  fullRank_R = ix;
                }
                for (idx = 0; idx < fullRank_R; idx++) {
                  b_TrialState.lambda[b_QRManager.jpvt[idx] - 1] =
                      memspace.workspace_double[idx];
                }
                ix = WorkingSet.sizes[0] + 1;
                for (idx = ix; idx <= mLambda_tmp; idx++) {
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
                b_gradLag = &memspace.workspace_double;
                smax = 0.0;
                idx = 0;
                exitg1 = false;
                while ((!exitg1) && (idx <= nVar_tmp - 1)) {
                  dxTooSmall = ((!std::isinf((*b_gradLag)[idx])) &&
                                (!std::isnan((*b_gradLag)[idx])));
                  if (!dxTooSmall) {
                    exitg1 = true;
                  } else {
                    smax = std::fmax(smax, std::abs((*b_gradLag)[idx]));
                    idx++;
                  }
                }
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
                  for (rankR = 0; rankR <= mLambda; rankR++) {
                    b_TrialState.lambdaStopTest[rankR] =
                        b_TrialState.lambda[rankR];
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
boolean_T test_exit(struct_T &b_MeritFunction, const d_struct_T &WorkingSet,
                    j_struct_T &b_TrialState, const array<double, 2U> &lb,
                    const array<double, 2U> &ub,
                    int runTimeOptions_MaxFunctionEvaluations,
                    boolean_T &Flags_fevalOK, boolean_T &Flags_done,
                    boolean_T &Flags_stepAccepted,
                    boolean_T &Flags_failedLineSearch, int &Flags_stepType)
{
  array<double, 1U> *gradLag;
  double s;
  double smax;
  int idx_max;
  int mLambda;
  int nVar_tmp;
  boolean_T Flags_gradOK;
  boolean_T exitg1;
  boolean_T isFeasible;
  Flags_fevalOK = true;
  Flags_done = false;
  Flags_stepAccepted = false;
  Flags_failedLineSearch = false;
  Flags_stepType = 1;
  nVar_tmp = WorkingSet.nVar;
  mLambda =
      ((((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[2]) +
        WorkingSet.sizes[3]) +
       WorkingSet.sizes[4]) -
      1;
  for (int k{0}; k <= mLambda; k++) {
    b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[2], WorkingSet.Aineq, WorkingSet.sizes[1],
      WorkingSet.Aeq, WorkingSet.indexFixed, WorkingSet.sizes[0],
      WorkingSet.indexLB, WorkingSet.sizes[3], WorkingSet.indexUB,
      WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  if (WorkingSet.nVar < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (WorkingSet.nVar > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      for (int k{2}; k <= nVar_tmp; k++) {
        s = std::abs(b_TrialState.grad[k - 1]);
        if (s > smax) {
          idx_max = k;
          smax = s;
        }
      }
    }
  }
  smax = std::fmax(1.0, std::abs(b_TrialState.grad[idx_max - 1]));
  if (std::isinf(smax)) {
    smax = 1.0;
  }
  b_MeritFunction.nlpPrimalFeasError = stopping::computePrimalFeasError(
      b_TrialState.xstarsqp, WorkingSet.sizes[2], b_TrialState.cIneq,
      WorkingSet.sizes[1], b_TrialState.cEq.data, WorkingSet.indexLB,
      WorkingSet.sizes[3], lb, WorkingSet.indexUB, WorkingSet.sizes[4], ub);
  b_MeritFunction.feasRelativeFactor =
      std::fmax(1.0, b_MeritFunction.nlpPrimalFeasError);
  isFeasible = (b_MeritFunction.nlpPrimalFeasError <=
                1.0E-6 * b_MeritFunction.feasRelativeFactor);
  gradLag = &b_TrialState.gradLag;
  Flags_gradOK = true;
  s = 0.0;
  idx_max = 0;
  exitg1 = false;
  while ((!exitg1) && (idx_max <= nVar_tmp - 1)) {
    Flags_gradOK = ((!std::isinf((*gradLag)[idx_max])) &&
                    (!std::isnan((*gradLag)[idx_max])));
    if (!Flags_gradOK) {
      exitg1 = true;
    } else {
      s = std::fmax(s, std::abs((*gradLag)[idx_max]));
      idx_max++;
    }
  }
  b_MeritFunction.nlpDualFeasError = s;
  if (!Flags_gradOK) {
    Flags_done = true;
    if (isFeasible) {
      b_TrialState.sqpExitFlag = 2;
    } else {
      b_TrialState.sqpExitFlag = -2;
    }
  } else {
    b_MeritFunction.nlpComplError = 0.0;
    b_MeritFunction.firstOrderOpt =
        std::fmax(b_MeritFunction.nlpDualFeasError, 0.0);
    for (int k{0}; k <= mLambda; k++) {
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
