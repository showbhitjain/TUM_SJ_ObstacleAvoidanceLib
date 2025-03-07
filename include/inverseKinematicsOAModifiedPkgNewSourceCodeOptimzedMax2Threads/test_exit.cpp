//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_exit.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "test_exit.h"
#include "computeComplError.h"
#include "computeGradLag.h"
#include "computePrimalFeasError.h"
#include "computeQ_.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xgemv.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"

// Function Definitions
//
// Arguments    : b_struct_T &Flags
//                h_struct_T &memspace
//                struct_T &b_MeritFunction
//                const array<double, 1U> &fscales_lineq_constraint
//                const d_struct_T &WorkingSet
//                j_struct_T &b_TrialState
//                e_struct_T &b_QRManager
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
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
                 e_struct_T &b_QRManager, const array<double, 1U> &lb,
                 const array<double, 1U> &ub,
                 int runTimeOptions_MaxFunctionEvaluations)
{
  array<double, 2U> *b_gradLag;
  array<double, 1U> *gradLag;
  double optimRelativeFactor;
  double s;
  double smax;
  int i;
  int idx;
  int idx_max;
  int mLambda;
  int nVar;
  boolean_T dxTooSmall;
  boolean_T exitg1;
  boolean_T isFeasible;
  nVar = WorkingSet.nVar;
  mLambda =
      ((((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[2]) +
        WorkingSet.sizes[3]) +
       WorkingSet.sizes[4]) -
      1;
  i = (mLambda + 1 < 200);
  if (i) {
    for (int k{0}; k <= mLambda; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= mLambda; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
    }
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
      for (int b_k{2}; b_k <= nVar; b_k++) {
        s = std::abs(b_TrialState.grad[b_k - 1]);
        if (s > smax) {
          idx_max = b_k;
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
                OAGlobals::constraintTolerance * b_MeritFunction.feasRelativeFactor);
  idx_max = WorkingSet.nVar;
  gradLag = &b_TrialState.gradLag;
  dxTooSmall = true;
  smax = 0.0;
  idx = 0;
  exitg1 = false;
  while ((!exitg1) && (idx <= idx_max - 1)) {
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
      idx_max = WorkingSet.nVar;
      b_gradLag = &memspace.workspace_double;
      s = 0.0;
      idx = 0;
      exitg1 = false;
      while ((!exitg1) && (idx <= idx_max - 1)) {
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
          b_TrialState.lambdaStopTestPrev,
          (WorkingSet.sizes[0] + WorkingSet.sizes[1]) + 1);
      d = std::fmax(s, nlpComplErrorTmp);
      if (d < smax) {
        b_MeritFunction.nlpDualFeasError = s;
        b_MeritFunction.nlpComplError = nlpComplErrorTmp;
        b_MeritFunction.firstOrderOpt = d;
        if (i) {
          for (int k{0}; k <= mLambda; k++) {
            b_TrialState.lambdaStopTest[k] = b_TrialState.lambdaStopTestPrev[k];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int k = 0; k <= mLambda; k++) {
            b_TrialState.lambdaStopTest[k] = b_TrialState.lambdaStopTestPrev[k];
          }
        }
      } else if (i) {
        for (int k{0}; k <= mLambda; k++) {
          b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = 0; k <= mLambda; k++) {
          b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
        }
      }
    } else if (i) {
      for (int k{0}; k <= mLambda; k++) {
        b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= mLambda; k++) {
        b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
      }
    }
    if (isFeasible &&
        (b_MeritFunction.nlpDualFeasError <= OAGlobals::optimalityTolerance * optimRelativeFactor) &&
        (b_MeritFunction.nlpComplError <= OAGlobals::optimalityTolerance * optimRelativeFactor)) {
      Flags.done = true;
      b_TrialState.sqpExitFlag = 1;
    } else {
      Flags.done = false;
      if (isFeasible && (b_TrialState.sqpFval < -1.0E+6)) {
        Flags.done = true;
        b_TrialState.sqpExitFlag = -3;
      } else {
        boolean_T guard1;
        guard1 = false;
        if (b_TrialState.sqpIterations > 0) {
          dxTooSmall = true;
          idx = 0;
          exitg1 = false;
          while ((!exitg1) && (idx <= nVar - 1)) {
            if (OAGlobals::stepTolerance *
                    std::fmax(1.0, std::abs(b_TrialState.xstarsqp[idx])) <=
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
                int ix;
                boolean_T guard2;
                if (static_cast<int>(nActiveConstr + 1 < 200)) {
                  for (int k{0}; k <= nActiveConstr; k++) {
                    b_TrialState.lambda[k] = 0.0;
                  }
                } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

                  for (int k = 0; k <= nActiveConstr; k++) {
                    b_TrialState.lambda[k] = 0.0;
                  }
                }
                ix = WorkingSet.nVar * WorkingSet.nActiveConstr;
                guard2 = false;
                if (ix > 0) {
                  for (idx = 0; idx <= nActiveConstr; idx++) {
                    idx_max = WorkingSet.ldA * idx;
                    ix = b_QRManager.ldq * idx;
                    for (int b_k{0}; b_k < nVar; b_k++) {
                      b_QRManager.QR[ix + b_k] =
                          WorkingSet.ATwset[idx_max + b_k];
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
                nVar = WorkingSet.nActiveConstr;
                if (ix <= nVar) {
                  nVar = ix;
                }
                nActiveConstr = 0;
                idx_max = 0;
                while ((nActiveConstr < nVar) &&
                       (std::abs(b_QRManager.QR[idx_max]) > smax)) {
                  nActiveConstr++;
                  idx_max = (idx_max + b_QRManager.ldq) + 1;
                }
                ::coder::internal::blas::xgemv(WorkingSet.nVar, WorkingSet.nVar,
                                               b_QRManager.Q, b_QRManager.ldq,
                                               b_TrialState.grad,
                                               memspace.workspace_double);
                if (nActiveConstr != 0) {
                  for (int b_k{nActiveConstr}; b_k >= 1; b_k--) {
                    idx_max = (b_k + (b_k - 1) * b_QRManager.ldq) - 1;
                    memspace.workspace_double[b_k - 1] =
                        memspace.workspace_double[b_k - 1] /
                        b_QRManager.QR[idx_max];
                    for (idx = 0; idx <= b_k - 2; idx++) {
                      ix = (b_k - idx) - 2;
                      memspace.workspace_double[ix] =
                          memspace.workspace_double[ix] -
                          memspace.workspace_double[b_k - 1] *
                              b_QRManager.QR[(idx_max - idx) - 1];
                    }
                  }
                }
                ix = WorkingSet.nActiveConstr;
                if (ix <= nVar) {
                  nVar = ix;
                }
                for (idx = 0; idx < nVar; idx++) {
                  b_TrialState.lambda[b_QRManager.jpvt[idx] - 1] =
                      memspace.workspace_double[idx];
                }
                ix = WorkingSet.sizes[0] + 1;
                idx_max = WorkingSet.sizes[0] + WorkingSet.sizes[1];
                if (static_cast<int>((idx_max - ix) + 1 < 200)) {
                  for (int k{ix}; k <= idx_max; k++) {
                    b_TrialState.lambda[k - 1] = -b_TrialState.lambda[k - 1];
                  }
                } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

                  for (int k = ix; k <= idx_max; k++) {
                    b_TrialState.lambda[k - 1] = -b_TrialState.lambda[k - 1];
                  }
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
                nVar = WorkingSet.nVar;
                b_gradLag = &memspace.workspace_double;
                smax = 0.0;
                idx = 0;
                exitg1 = false;
                while ((!exitg1) && (idx <= nVar - 1)) {
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
                if ((smax <= OAGlobals::optimalityTolerance * optimRelativeFactor) &&
                    (s <= OAGlobals::optimalityTolerance * optimRelativeFactor)) {
                  b_MeritFunction.nlpDualFeasError = smax;
                  b_MeritFunction.nlpComplError = s;
                  b_MeritFunction.firstOrderOpt = std::fmax(smax, s);
                  if (i) {
                    for (int k{0}; k <= mLambda; k++) {
                      b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
                    }
                  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

                    for (int k = 0; k <= mLambda; k++) {
                      b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
                    }
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
          if (b_TrialState.sqpIterations >= OAGlobals::maxIterations) {
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
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                int runTimeOptions_MaxFunctionEvaluations
//                boolean_T &Flags_fevalOK
//                boolean_T &Flags_done
//                boolean_T &Flags_stepAccepted
//                boolean_T &Flags_failedLineSearch
//                int &Flags_stepType
// Return Type  : boolean_T
//
boolean_T test_exit(struct_T &b_MeritFunction, const d_struct_T &WorkingSet,
                    j_struct_T &b_TrialState, const array<double, 1U> &lb,
                    const array<double, 1U> &ub,
                    int runTimeOptions_MaxFunctionEvaluations,
                    boolean_T &Flags_fevalOK, boolean_T &Flags_done,
                    boolean_T &Flags_stepAccepted,
                    boolean_T &Flags_failedLineSearch, int &Flags_stepType)
{
  array<double, 1U> *gradLag;
  double s;
  double smax;
  int i;
  int idx_max;
  int mLambda;
  int nVar;
  boolean_T Flags_gradOK;
  boolean_T exitg1;
  boolean_T isFeasible;
  Flags_fevalOK = true;
  Flags_done = false;
  Flags_stepAccepted = false;
  Flags_failedLineSearch = false;
  Flags_stepType = 1;
  nVar = WorkingSet.nVar;
  mLambda =
      ((((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[2]) +
        WorkingSet.sizes[3]) +
       WorkingSet.sizes[4]) -
      1;
  i = (mLambda + 1 < 200);
  if (i) {
    for (int k{0}; k <= mLambda; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= mLambda; k++) {
      b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
    }
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
      for (int b_k{2}; b_k <= nVar; b_k++) {
        s = std::abs(b_TrialState.grad[b_k - 1]);
        if (s > smax) {
          idx_max = b_k;
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
                OAGlobals::constraintTolerance * b_MeritFunction.feasRelativeFactor);
  nVar = WorkingSet.nVar;
  gradLag = &b_TrialState.gradLag;
  Flags_gradOK = true;
  s = 0.0;
  idx_max = 0;
  exitg1 = false;
  while ((!exitg1) && (idx_max <= nVar - 1)) {
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
    if (i) {
      for (int k{0}; k <= mLambda; k++) {
        b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= mLambda; k++) {
        b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
      }
    }
    if (isFeasible && (b_MeritFunction.nlpDualFeasError <= OAGlobals::optimalityTolerance * smax)) {
      Flags_done = true;
      b_TrialState.sqpExitFlag = 1;
    } else if (isFeasible && (b_TrialState.sqpFval < -1.0E+6)) {
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
