//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: test_exit.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "test_exit.h"
#include "computeComplError.h"
#include "computeGradLag.h"
#include "computeQ_.h"
#include "inverseKinematics_internal_types.h"
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
//                g_struct_T &memspace
//                struct_T &MeritFunction
//                const i_struct_T &WorkingSet
//                h_struct_T &b_TrialState
//                d_struct_T &b_QRManager
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                int runTimeOptions_MaxFunctionEvaluations
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void b_test_exit(b_struct_T &Flags, g_struct_T &memspace,
                 struct_T &MeritFunction, const i_struct_T &WorkingSet,
                 h_struct_T &b_TrialState, d_struct_T &b_QRManager,
                 const array<double, 1U> &lb, const array<double, 1U> &ub,
                 int runTimeOptions_MaxFunctionEvaluations)
{
  array<double, 2U> *b_gradLag;
  array<double, 1U> *gradLag;
  double optimRelativeFactor;
  double s;
  double smax;
  int fullRank_R;
  int idx;
  int idx_max;
  int mEq;
  int mLB;
  int mLambda;
  int mLambda_tmp;
  int mUB;
  int nVar_tmp;
  boolean_T dxTooSmall;
  boolean_T exitg1;
  boolean_T isFeasible;
  nVar_tmp = WorkingSet.nVar;
  mEq = WorkingSet.sizes[1];
  mLB = WorkingSet.sizes[3];
  mUB = WorkingSet.sizes[4];
  mLambda_tmp = WorkingSet.sizes[0] + WorkingSet.sizes[1];
  mLambda = ((mLambda_tmp + WorkingSet.sizes[3]) + WorkingSet.sizes[4]) - 1;
  for (fullRank_R = 0; fullRank_R <= mLambda; fullRank_R++) {
    b_TrialState.lambdaStopTest[fullRank_R] =
        b_TrialState.lambdasqp[fullRank_R];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[1], WorkingSet.Aeq, WorkingSet.indexFixed,
      WorkingSet.sizes[0], WorkingSet.indexLB, WorkingSet.sizes[3],
      WorkingSet.indexUB, WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  if (WorkingSet.nVar < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (WorkingSet.nVar > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      for (fullRank_R = 2; fullRank_R <= nVar_tmp; fullRank_R++) {
        s = std::abs(b_TrialState.grad[fullRank_R - 1]);
        if (s > smax) {
          idx_max = fullRank_R;
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
  smax = 0.0;
  for (idx = 0; idx < mEq; idx++) {
    smax = std::fmax(smax, std::abs(b_TrialState.cEq.data[idx]));
  }
  for (idx = 0; idx < mLB; idx++) {
    mEq = WorkingSet.indexLB[idx] - 1;
    smax = std::fmax(smax, lb[mEq] - b_TrialState.xstarsqp[mEq]);
  }
  for (idx = 0; idx < mUB; idx++) {
    mEq = WorkingSet.indexUB[idx] - 1;
    smax = std::fmax(smax, b_TrialState.xstarsqp[mEq] - ub[mEq]);
  }
  MeritFunction.nlpPrimalFeasError = smax;
  if (b_TrialState.sqpIterations == 0) {
    MeritFunction.feasRelativeFactor = std::fmax(1.0, smax);
  }
  isFeasible = (smax <= 1.0E-6 * MeritFunction.feasRelativeFactor);
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
  MeritFunction.nlpDualFeasError = smax;
  if (!Flags.gradOK) {
    Flags.done = true;
    if (isFeasible) {
      b_TrialState.sqpExitFlag = 2;
    } else {
      b_TrialState.sqpExitFlag = -2;
    }
  } else {
    MeritFunction.nlpComplError = stopping::computeComplError(
        b_TrialState.xstarsqp, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
        WorkingSet.indexUB, WorkingSet.sizes[4], ub,
        b_TrialState.lambdaStopTest, mLambda_tmp + 1);
    smax =
        std::fmax(MeritFunction.nlpDualFeasError, MeritFunction.nlpComplError);
    MeritFunction.firstOrderOpt = smax;
    if (b_TrialState.sqpIterations > 1) {
      double d;
      double nlpComplErrorTmp;
      stopping::computeGradLag(
          memspace.workspace_double, WorkingSet.ldA, WorkingSet.nVar,
          b_TrialState.grad, WorkingSet.sizes[1], WorkingSet.Aeq,
          WorkingSet.indexFixed, WorkingSet.sizes[0], WorkingSet.indexLB,
          WorkingSet.sizes[3], WorkingSet.indexUB, WorkingSet.sizes[4],
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
          b_TrialState.xstarsqp, WorkingSet.indexLB, WorkingSet.sizes[3], lb,
          WorkingSet.indexUB, WorkingSet.sizes[4], ub,
          b_TrialState.lambdaStopTestPrev, mLambda_tmp + 1);
      d = std::fmax(s, nlpComplErrorTmp);
      if (d < smax) {
        MeritFunction.nlpDualFeasError = s;
        MeritFunction.nlpComplError = nlpComplErrorTmp;
        MeritFunction.firstOrderOpt = d;
        for (fullRank_R = 0; fullRank_R <= mLambda; fullRank_R++) {
          b_TrialState.lambdaStopTest[fullRank_R] =
              b_TrialState.lambdaStopTestPrev[fullRank_R];
        }
      } else {
        for (fullRank_R = 0; fullRank_R <= mLambda; fullRank_R++) {
          b_TrialState.lambdaStopTestPrev[fullRank_R] =
              b_TrialState.lambdaStopTest[fullRank_R];
        }
      }
    } else {
      for (fullRank_R = 0; fullRank_R <= mLambda; fullRank_R++) {
        b_TrialState.lambdaStopTestPrev[fullRank_R] =
            b_TrialState.lambdaStopTest[fullRank_R];
      }
    }
    if (isFeasible &&
        (MeritFunction.nlpDualFeasError <= 1.0E-6 * optimRelativeFactor) &&
        (MeritFunction.nlpComplError <= 1.0E-6 * optimRelativeFactor)) {
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
              mLB = WorkingSet.nActiveConstr - 1;
              if (WorkingSet.nActiveConstr > 0) {
                boolean_T guard2;
                for (fullRank_R = 0; fullRank_R <= mLB; fullRank_R++) {
                  b_TrialState.lambda[fullRank_R] = 0.0;
                }
                mUB = WorkingSet.nVar * WorkingSet.nActiveConstr;
                guard2 = false;
                if (mUB > 0) {
                  for (idx = 0; idx <= mLB; idx++) {
                    mEq = WorkingSet.ldA * idx;
                    idx_max = b_QRManager.ldq * idx;
                    for (fullRank_R = 0; fullRank_R < nVar_tmp; fullRank_R++) {
                      b_QRManager.QR[idx_max + fullRank_R] =
                          WorkingSet.ATwset[mEq + fullRank_R];
                    }
                  }
                  guard2 = true;
                } else if (mUB == 0) {
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
                  idx_max = WorkingSet.nVar;
                  mEq = WorkingSet.nActiveConstr;
                  if (idx_max <= mEq) {
                    mEq = idx_max;
                  }
                  b_QRManager.minRowCol = mEq;
                  ::coder::internal::lapack::xgeqp3(
                      b_QRManager.QR, WorkingSet.nVar, WorkingSet.nActiveConstr,
                      b_QRManager.jpvt, b_QRManager.tau);
                }
                QRManager::computeQ_(b_QRManager, b_QRManager.mrows);
                idx_max = WorkingSet.nVar;
                mEq = WorkingSet.nActiveConstr;
                if (idx_max >= mEq) {
                  mEq = idx_max;
                }
                smax = std::abs(b_QRManager.QR[0]) *
                       std::fmin(1.4901161193847656E-8,
                                 static_cast<double>(mEq) *
                                     2.2204460492503131E-16);
                idx_max = WorkingSet.nVar;
                fullRank_R = WorkingSet.nActiveConstr;
                if (idx_max <= fullRank_R) {
                  fullRank_R = idx_max;
                }
                mLB = 0;
                mEq = 0;
                while ((mLB < fullRank_R) &&
                       (std::abs(b_QRManager.QR[mEq]) > smax)) {
                  mLB++;
                  mEq = (mEq + b_QRManager.ldq) + 1;
                }
                idx_max = b_QRManager.ldq;
                b_gradLag = &memspace.workspace_double;
                if (nVar_tmp != 0) {
                  for (idx = 0; idx < nVar_tmp; idx++) {
                    (*b_gradLag)[idx] = 0.0;
                  }
                  idx = 0;
                  mUB = idx_max * (nVar_tmp - 1) + 1;
                  for (int iac{1}; idx_max < 0 ? iac >= mUB : iac <= mUB;
                       iac += idx_max) {
                    smax = 0.0;
                    mEq = (iac + nVar_tmp) - 1;
                    for (int ia{iac}; ia <= mEq; ia++) {
                      smax +=
                          b_QRManager.Q[ia - 1] * b_TrialState.grad[ia - iac];
                    }
                    (*b_gradLag)[idx] = (*b_gradLag)[idx] + smax;
                    idx++;
                  }
                }
                if (mLB != 0) {
                  for (mUB = mLB; mUB >= 1; mUB--) {
                    idx_max = (mUB + (mUB - 1) * b_QRManager.ldq) - 1;
                    memspace.workspace_double[mUB - 1] =
                        memspace.workspace_double[mUB - 1] /
                        b_QRManager.QR[idx_max];
                    for (idx = 0; idx <= mUB - 2; idx++) {
                      mEq = (mUB - idx) - 2;
                      memspace.workspace_double[mEq] =
                          memspace.workspace_double[mEq] -
                          memspace.workspace_double[mUB - 1] *
                              b_QRManager.QR[(idx_max - idx) - 1];
                    }
                  }
                }
                idx_max = WorkingSet.nActiveConstr;
                if (idx_max <= fullRank_R) {
                  fullRank_R = idx_max;
                }
                for (idx = 0; idx < fullRank_R; idx++) {
                  b_TrialState.lambda[b_QRManager.jpvt[idx] - 1] =
                      memspace.workspace_double[idx];
                }
                mUB = WorkingSet.sizes[0] + 1;
                for (idx = mUB; idx <= mLambda_tmp; idx++) {
                  b_TrialState.lambda[idx - 1] = -b_TrialState.lambda[idx - 1];
                }
                qpactiveset::parseoutput::sortLambdaQP(
                    b_TrialState.lambda, WorkingSet.nActiveConstr,
                    WorkingSet.sizes, WorkingSet.isActiveIdx, WorkingSet.Wid,
                    WorkingSet.Wlocalidx, memspace.workspace_double);
                stopping::computeGradLag(
                    memspace.workspace_double, WorkingSet.ldA, WorkingSet.nVar,
                    b_TrialState.grad, WorkingSet.sizes[1], WorkingSet.Aeq,
                    WorkingSet.indexFixed, WorkingSet.sizes[0],
                    WorkingSet.indexLB, WorkingSet.sizes[3], WorkingSet.indexUB,
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
                    b_TrialState.xstarsqp, WorkingSet.indexLB,
                    WorkingSet.sizes[3], lb, WorkingSet.indexUB,
                    WorkingSet.sizes[4], ub, b_TrialState.lambda,
                    WorkingSet.sizes[0] + 1);
                if ((smax <= 1.0E-6 * optimRelativeFactor) &&
                    (s <= 1.0E-6 * optimRelativeFactor)) {
                  MeritFunction.nlpDualFeasError = smax;
                  MeritFunction.nlpComplError = s;
                  MeritFunction.firstOrderOpt = std::fmax(smax, s);
                  for (fullRank_R = 0; fullRank_R <= mLambda; fullRank_R++) {
                    b_TrialState.lambdaStopTest[fullRank_R] =
                        b_TrialState.lambda[fullRank_R];
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
// Arguments    : struct_T &MeritFunction
//                const i_struct_T &WorkingSet
//                h_struct_T &b_TrialState
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
boolean_T test_exit(struct_T &MeritFunction, const i_struct_T &WorkingSet,
                    h_struct_T &b_TrialState, const array<double, 1U> &lb,
                    const array<double, 1U> &ub,
                    int runTimeOptions_MaxFunctionEvaluations,
                    boolean_T &Flags_fevalOK, boolean_T &Flags_done,
                    boolean_T &Flags_stepAccepted,
                    boolean_T &Flags_failedLineSearch, int &Flags_stepType)
{
  array<double, 1U> *gradLag;
  double s;
  double smax;
  int idx_max;
  int k;
  int mEq;
  int mLB;
  int mLambda;
  int mUB;
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
  mEq = WorkingSet.sizes[1];
  mLB = WorkingSet.sizes[3];
  mUB = WorkingSet.sizes[4];
  mLambda =
      (((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[3]) +
       WorkingSet.sizes[4]) -
      1;
  for (k = 0; k <= mLambda; k++) {
    b_TrialState.lambdaStopTest[k] = b_TrialState.lambdasqp[k];
  }
  stopping::computeGradLag(
      b_TrialState.gradLag, WorkingSet.ldA, WorkingSet.nVar, b_TrialState.grad,
      WorkingSet.sizes[1], WorkingSet.Aeq, WorkingSet.indexFixed,
      WorkingSet.sizes[0], WorkingSet.indexLB, WorkingSet.sizes[3],
      WorkingSet.indexUB, WorkingSet.sizes[4], b_TrialState.lambdaStopTest);
  if (WorkingSet.nVar < 1) {
    idx_max = 0;
  } else {
    idx_max = 1;
    if (WorkingSet.nVar > 1) {
      smax = std::abs(b_TrialState.grad[0]);
      for (k = 2; k <= nVar_tmp; k++) {
        s = std::abs(b_TrialState.grad[k - 1]);
        if (s > smax) {
          idx_max = k;
          smax = s;
        }
      }
    }
  }
  s = std::fmax(1.0, std::abs(b_TrialState.grad[idx_max - 1]));
  if (std::isinf(s)) {
    s = 1.0;
  }
  smax = 0.0;
  for (k = 0; k < mEq; k++) {
    smax = std::fmax(smax, std::abs(b_TrialState.cEq.data[k]));
  }
  for (k = 0; k < mLB; k++) {
    idx_max = WorkingSet.indexLB[k] - 1;
    smax = std::fmax(smax, lb[idx_max] - b_TrialState.xstarsqp[idx_max]);
  }
  for (k = 0; k < mUB; k++) {
    idx_max = WorkingSet.indexUB[k] - 1;
    smax = std::fmax(smax, b_TrialState.xstarsqp[idx_max] - ub[idx_max]);
  }
  MeritFunction.nlpPrimalFeasError = smax;
  MeritFunction.feasRelativeFactor = std::fmax(1.0, smax);
  isFeasible = (smax <= 1.0E-6 * MeritFunction.feasRelativeFactor);
  gradLag = &b_TrialState.gradLag;
  Flags_gradOK = true;
  smax = 0.0;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k <= nVar_tmp - 1)) {
    Flags_gradOK =
        ((!std::isinf((*gradLag)[k])) && (!std::isnan((*gradLag)[k])));
    if (!Flags_gradOK) {
      exitg1 = true;
    } else {
      smax = std::fmax(smax, std::abs((*gradLag)[k]));
      k++;
    }
  }
  MeritFunction.nlpDualFeasError = smax;
  if (!Flags_gradOK) {
    Flags_done = true;
    if (isFeasible) {
      b_TrialState.sqpExitFlag = 2;
    } else {
      b_TrialState.sqpExitFlag = -2;
    }
  } else {
    MeritFunction.nlpComplError = 0.0;
    MeritFunction.firstOrderOpt =
        std::fmax(MeritFunction.nlpDualFeasError, 0.0);
    for (k = 0; k <= mLambda; k++) {
      b_TrialState.lambdaStopTestPrev[k] = b_TrialState.lambdaStopTest[k];
    }
    if (isFeasible && (MeritFunction.nlpDualFeasError <= 1.0E-6 * s)) {
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
