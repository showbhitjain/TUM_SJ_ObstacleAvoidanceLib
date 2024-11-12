//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "driver.h"
#include "BFGSUpdate.h"
#include "computeFiniteDifferences.h"
#include "evalObjAndConstr.h"
#include "inverseKinematics_internal_types.h"
#include "rt_nonfinite.h"
#include "step.h"
#include "stickyStruct.h"
#include "test_exit.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 2U> &Hessian
//                const double beq_data[]
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                h_struct_T &b_TrialState
//                struct_T &MeritFunction
//                const ::coder::internal::i_stickyStruct &FcnEvaluator
//                k_struct_T &FiniteDifferences
//                g_struct_T &memspace
//                i_struct_T &WorkingSet
//                d_struct_T &b_QRManager
//                e_struct_T &b_CholManager
//                f_struct_T &QPObjective
//                int runTimeOptions_MaxFunctionEvaluations
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void driver(array<double, 2U> &Hessian, const double beq_data[],
            const array<double, 1U> &lb, const array<double, 1U> &ub,
            h_struct_T &b_TrialState, struct_T &MeritFunction,
            const ::coder::internal::i_stickyStruct &FcnEvaluator,
            k_struct_T &FiniteDifferences, g_struct_T &memspace,
            i_struct_T &WorkingSet, d_struct_T &b_QRManager,
            e_struct_T &b_CholManager, f_struct_T &QPObjective,
            int runTimeOptions_MaxFunctionEvaluations)
{
  static const char qpoptions_SolverName[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  array<double, 1U> y;
  b_struct_T Flags;
  j_struct_T b_expl_temp;
  j_struct_T expl_temp;
  int b_nVar;
  int evalStatus;
  int i;
  int ineqStart;
  int ixlast;
  int mConstr;
  int mEq;
  int mFixed;
  int mLB;
  int mUB;
  int nVar;
  int n_tmp;
  int qpoptions_MaxIterations;
  nVar = WorkingSet.nVar;
  mFixed = WorkingSet.sizes[0];
  mEq = WorkingSet.sizes[1];
  mLB = WorkingSet.sizes[3];
  mUB = WorkingSet.sizes[4];
  mConstr =
      ((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[3]) +
      WorkingSet.sizes[4];
  ixlast = WorkingSet.nVar;
  ineqStart =
      (WorkingSet.sizes[3] + WorkingSet.sizes[4]) + (WorkingSet.sizes[0] << 1);
  if (ixlast >= ineqStart) {
    ineqStart = ixlast;
  }
  qpoptions_MaxIterations = 10 * ineqStart;
  b_TrialState.steplength = 1.0;
  Flags.gradOK = test_exit(MeritFunction, WorkingSet, b_TrialState, lb, ub,
                           runTimeOptions_MaxFunctionEvaluations, Flags.fevalOK,
                           Flags.done, Flags.stepAccepted,
                           Flags.failedLineSearch, Flags.stepType);
  b_TrialState.sqpFval_old = b_TrialState.sqpFval;
  b_nVar = b_TrialState.xstarsqp.size(0) - 1;
  for (ineqStart = 0; ineqStart <= b_nVar; ineqStart++) {
    b_TrialState.xstarsqp_old[ineqStart] = b_TrialState.xstarsqp[ineqStart];
    b_TrialState.grad_old[ineqStart] = b_TrialState.grad[ineqStart];
  }
  n_tmp = b_TrialState.mEq;
  y.set_size(b_TrialState.cEq_old.size[0]);
  ixlast = b_TrialState.cEq_old.size[0];
  for (i = 0; i < ixlast; i++) {
    y[i] = b_TrialState.cEq_old.data[i];
  }
  for (ineqStart = 0; ineqStart < n_tmp; ineqStart++) {
    y[ineqStart] = b_TrialState.cEq.data[ineqStart];
  }
  b_TrialState.cEq_old.size[0] = y.size(0);
  ixlast = y.size(0);
  for (i = 0; i < ixlast; i++) {
    b_TrialState.cEq_old.data[i] = y[i];
  }
  if (!Flags.done) {
    b_TrialState.sqpIterations = 1;
  }
  while (!Flags.done) {
    double c;
    while (!(Flags.stepAccepted || Flags.failedLineSearch)) {
      int iy;
      boolean_T hasLB;
      boolean_T hasUB;
      if (Flags.stepType != 3) {
        for (ixlast = 0; ixlast < mEq; ixlast++) {
          WorkingSet.beq.data[ixlast] = -b_TrialState.cEq.data[ixlast];
          WorkingSet.bwset[mFixed + ixlast] = WorkingSet.beq.data[ixlast];
        }
        hasLB = (lb.size(0) != 0);
        hasUB = (ub.size(0) != 0);
        if (hasLB) {
          for (ixlast = 0; ixlast < mLB; ixlast++) {
            WorkingSet.lb[WorkingSet.indexLB[ixlast] - 1] =
                -lb[WorkingSet.indexLB[ixlast] - 1] +
                b_TrialState.xstarsqp[WorkingSet.indexLB[ixlast] - 1];
          }
        }
        if (hasUB) {
          for (ixlast = 0; ixlast < mUB; ixlast++) {
            WorkingSet.ub[WorkingSet.indexUB[ixlast] - 1] =
                ub[WorkingSet.indexUB[ixlast] - 1] -
                b_TrialState.xstarsqp[WorkingSet.indexUB[ixlast] - 1];
          }
        }
        if (hasLB && hasUB) {
          for (ixlast = 0; ixlast < mFixed; ixlast++) {
            c = ub[WorkingSet.indexFixed[ixlast] - 1] -
                b_TrialState.xstarsqp[WorkingSet.indexFixed[ixlast] - 1];
            WorkingSet.ub[WorkingSet.indexFixed[ixlast] - 1] = c;
            WorkingSet.bwset[ixlast] = c;
          }
        }
        i = mFixed + mEq;
        if (WorkingSet.nActiveConstr > i) {
          ineqStart = i + 1;
          if (ineqStart < 1) {
            ineqStart = 1;
          }
          i = WorkingSet.nActiveConstr;
          for (ixlast = ineqStart; ixlast <= i; ixlast++) {
            switch (WorkingSet.Wid[ixlast - 1]) {
            case 4:
              WorkingSet.bwset[ixlast - 1] =
                  WorkingSet
                      .lb[WorkingSet
                              .indexLB[WorkingSet.Wlocalidx[ixlast - 1] - 1] -
                          1];
              break;
            case 5:
              WorkingSet.bwset[ixlast - 1] =
                  WorkingSet
                      .ub[WorkingSet
                              .indexUB[WorkingSet.Wlocalidx[ixlast - 1] - 1] -
                          1];
              break;
            default:
              // A check that is always false is detected at compile-time.
              // Eliminating code that follows.
              break;
            }
          }
        }
      }
      expl_temp.ObjectiveLimit = rtMinusInf;
      expl_temp.StepTolerance = 1.0E-6;
      expl_temp.MaxIterations = qpoptions_MaxIterations;
      for (i = 0; i < 7; i++) {
        expl_temp.SolverName[i] = qpoptions_SolverName[i];
      }
      b_expl_temp = expl_temp;
      Flags.stepAccepted =
          b_step(Flags.stepType, Hessian, lb, ub, b_TrialState, MeritFunction,
                 memspace, WorkingSet, b_QRManager, b_CholManager, QPObjective,
                 b_expl_temp);
      if (Flags.stepAccepted) {
        for (ixlast = 0; ixlast < nVar; ixlast++) {
          b_TrialState.xstarsqp[ixlast] =
              b_TrialState.xstarsqp[ixlast] + b_TrialState.delta_x[ixlast];
        }
        b_TrialState.sqpFval = utils::ObjNonlinEvaluator::evalObjAndConstr(
            FcnEvaluator, b_TrialState.xstarsqp, ixlast);
        Flags.fevalOK = (ixlast == 1);
        b_TrialState.FunctionEvaluations++;
        if (mEq > 0) {
          y.set_size(b_TrialState.cEq.size[0]);
          ixlast = b_TrialState.cEq.size[0];
          for (i = 0; i < ixlast; i++) {
            y[i] = b_TrialState.cEq.data[i];
          }
          for (ineqStart = 0; ineqStart < mEq; ineqStart++) {
            y[ineqStart] = beq_data[ineqStart];
          }
          ixlast = WorkingSet.ldA;
          if (nVar != 0) {
            for (iy = 0; iy < mEq; iy++) {
              y[iy] = -y[iy];
            }
            iy = 0;
            i = ixlast * (mEq - 1) + 1;
            for (int iac{1}; ixlast < 0 ? iac >= i : iac <= i; iac += ixlast) {
              c = 0.0;
              ineqStart = (iac + nVar) - 1;
              for (int ia{iac}; ia <= ineqStart; ia++) {
                c += WorkingSet.Aeq[ia - 1] * b_TrialState.xstarsqp[ia - iac];
              }
              y[iy] = y[iy] + c;
              iy++;
            }
          }
          b_TrialState.cEq.size[0] = y.size(0);
          ixlast = y.size(0);
          for (i = 0; i < ixlast; i++) {
            b_TrialState.cEq.data[i] = y[i];
          }
        }
        if (Flags.fevalOK) {
          c = 0.0;
          if (mEq >= 1) {
            i = static_cast<unsigned char>(mEq);
            for (ineqStart = 0; ineqStart < i; ineqStart++) {
              c += std::abs(b_TrialState.cEq.data[ineqStart]);
            }
          }
          MeritFunction.phiFullStep =
              b_TrialState.sqpFval + MeritFunction.penaltyParam * c;
        } else {
          MeritFunction.phiFullStep = rtInf;
        }
      }
      if ((Flags.stepType == 1) && Flags.stepAccepted && Flags.fevalOK &&
          (MeritFunction.phi < MeritFunction.phiFullStep) &&
          (b_TrialState.sqpFval < b_TrialState.sqpFval_old)) {
        Flags.stepType = 3;
        Flags.stepAccepted = false;
      } else {
        double alpha;
        int exitflagLnSrch;
        if ((Flags.stepType == 3) && Flags.stepAccepted) {
          hasLB = true;
        } else {
          hasLB = false;
        }
        hasUB = Flags.fevalOK;
        b_nVar = WorkingSet.nVar - 1;
        alpha = 1.0;
        exitflagLnSrch = 1;
        c = MeritFunction.phiFullStep;
        for (ineqStart = 0; ineqStart <= b_nVar; ineqStart++) {
          b_TrialState.searchDir[ineqStart] = b_TrialState.delta_x[ineqStart];
        }
        int exitg1;
        do {
          exitg1 = 0;
          if (b_TrialState.FunctionEvaluations <
              runTimeOptions_MaxFunctionEvaluations) {
            if (hasUB &&
                (c <= MeritFunction.phi +
                          alpha * 0.0001 * MeritFunction.phiPrimePlus)) {
              exitg1 = 1;
            } else {
              boolean_T exitg2;
              boolean_T tooSmallX;
              alpha *= 0.7;
              for (ixlast = 0; ixlast <= b_nVar; ixlast++) {
                b_TrialState.delta_x[ixlast] =
                    alpha * b_TrialState.xstar[ixlast];
              }
              if (hasLB) {
                c = alpha * alpha;
                if ((b_nVar + 1 >= 1) && (!(c == 0.0))) {
                  for (ineqStart = 0; ineqStart <= b_nVar; ineqStart++) {
                    b_TrialState.delta_x[ineqStart] =
                        b_TrialState.delta_x[ineqStart] +
                        c * b_TrialState.socDirection[ineqStart];
                  }
                }
              }
              tooSmallX = true;
              ixlast = 0;
              exitg2 = false;
              while ((!exitg2) && (ixlast <= b_nVar)) {
                if (1.0E-6 *
                        std::fmax(1.0,
                                  std::abs(b_TrialState.xstarsqp[ixlast])) <=
                    std::abs(b_TrialState.delta_x[ixlast])) {
                  tooSmallX = false;
                  exitg2 = true;
                } else {
                  ixlast++;
                }
              }
              if (tooSmallX) {
                exitflagLnSrch = -2;
                exitg1 = 1;
              } else {
                for (ixlast = 0; ixlast <= b_nVar; ixlast++) {
                  b_TrialState.xstarsqp[ixlast] =
                      b_TrialState.xstarsqp_old[ixlast] +
                      b_TrialState.delta_x[ixlast];
                }
                b_TrialState.sqpFval =
                    utils::ObjNonlinEvaluator::evalObjAndConstr(
                        FcnEvaluator, b_TrialState.xstarsqp, evalStatus);
                if (n_tmp > 0) {
                  y.set_size(b_TrialState.cEq.size[0]);
                  ixlast = b_TrialState.cEq.size[0];
                  for (i = 0; i < ixlast; i++) {
                    y[i] = b_TrialState.cEq.data[i];
                  }
                  for (ineqStart = 0; ineqStart < n_tmp; ineqStart++) {
                    y[ineqStart] = beq_data[ineqStart];
                  }
                  ixlast = WorkingSet.ldA;
                  if (b_nVar + 1 != 0) {
                    for (iy = 0; iy < n_tmp; iy++) {
                      y[iy] = -y[iy];
                    }
                    iy = 0;
                    i = ixlast * (n_tmp - 1) + 1;
                    for (int iac{1}; ixlast < 0 ? iac >= i : iac <= i;
                         iac += ixlast) {
                      c = 0.0;
                      ineqStart = iac + b_nVar;
                      for (int ia{iac}; ia <= ineqStart; ia++) {
                        c += WorkingSet.Aeq[ia - 1] *
                             b_TrialState.xstarsqp[ia - iac];
                      }
                      y[iy] = y[iy] + c;
                      iy++;
                    }
                  }
                  b_TrialState.cEq.size[0] = y.size(0);
                  ixlast = y.size(0);
                  for (i = 0; i < ixlast; i++) {
                    b_TrialState.cEq.data[i] = y[i];
                  }
                }
                b_TrialState.FunctionEvaluations++;
                hasUB = (evalStatus == 1);
                if (hasUB) {
                  c = 0.0;
                  if (n_tmp >= 1) {
                    i = static_cast<unsigned char>(n_tmp);
                    for (ineqStart = 0; ineqStart < i; ineqStart++) {
                      c += std::abs(b_TrialState.cEq.data[ineqStart]);
                    }
                  }
                  c = b_TrialState.sqpFval + MeritFunction.penaltyParam * c;
                } else {
                  c = rtInf;
                }
              }
            }
          } else {
            exitflagLnSrch = 0;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
        Flags.fevalOK = hasUB;
        b_TrialState.steplength = alpha;
        if (exitflagLnSrch > 0) {
          Flags.stepAccepted = true;
        } else {
          Flags.failedLineSearch = true;
        }
      }
    }
    if (Flags.stepAccepted && (!Flags.failedLineSearch)) {
      for (ixlast = 0; ixlast < nVar; ixlast++) {
        b_TrialState.xstarsqp[ixlast] =
            b_TrialState.xstarsqp_old[ixlast] + b_TrialState.delta_x[ixlast];
      }
      for (ixlast = 0; ixlast < mConstr; ixlast++) {
        c = b_TrialState.lambdasqp[ixlast];
        c += b_TrialState.steplength * (b_TrialState.lambda[ixlast] - c);
        b_TrialState.lambdasqp[ixlast] = c;
      }
      b_TrialState.sqpFval_old = b_TrialState.sqpFval;
      b_nVar = b_TrialState.xstarsqp.size(0) - 1;
      for (ineqStart = 0; ineqStart <= b_nVar; ineqStart++) {
        b_TrialState.xstarsqp_old[ineqStart] = b_TrialState.xstarsqp[ineqStart];
        b_TrialState.grad_old[ineqStart] = b_TrialState.grad[ineqStart];
      }
      y.set_size(b_TrialState.cEq_old.size[0]);
      ixlast = b_TrialState.cEq_old.size[0];
      for (i = 0; i < ixlast; i++) {
        y[i] = b_TrialState.cEq_old.data[i];
      }
      for (ineqStart = 0; ineqStart < n_tmp; ineqStart++) {
        y[ineqStart] = b_TrialState.cEq.data[ineqStart];
      }
      b_TrialState.cEq_old.size[0] = y.size(0);
      ixlast = y.size(0);
      for (i = 0; i < ixlast; i++) {
        b_TrialState.cEq_old.data[i] = y[i];
      }
      Flags.gradOK = utils::FiniteDifferences::computeFiniteDifferences(
          FiniteDifferences, b_TrialState.sqpFval, b_TrialState.xstarsqp,
          b_TrialState.grad, lb, ub);
      b_TrialState.FunctionEvaluations += FiniteDifferences.numEvals;
    } else {
      b_TrialState.sqpFval = b_TrialState.sqpFval_old;
      ixlast = b_TrialState.xstarsqp.size(0);
      for (ineqStart = 0; ineqStart < ixlast; ineqStart++) {
        b_TrialState.xstarsqp[ineqStart] = b_TrialState.xstarsqp_old[ineqStart];
      }
      y.set_size(b_TrialState.cEq.size[0]);
      ixlast = b_TrialState.cEq.size[0];
      for (i = 0; i < ixlast; i++) {
        y[i] = b_TrialState.cEq.data[i];
      }
      for (ineqStart = 0; ineqStart < n_tmp; ineqStart++) {
        y[ineqStart] = b_TrialState.cEq_old.data[ineqStart];
      }
      b_TrialState.cEq.size[0] = y.size(0);
      ixlast = y.size(0);
      for (i = 0; i < ixlast; i++) {
        b_TrialState.cEq.data[i] = y[i];
      }
    }
    b_test_exit(Flags, memspace, MeritFunction, WorkingSet, b_TrialState,
                b_QRManager, lb, ub, runTimeOptions_MaxFunctionEvaluations);
    if ((!Flags.done) && Flags.stepAccepted) {
      Flags.stepAccepted = false;
      Flags.stepType = 1;
      Flags.failedLineSearch = false;
      for (ixlast = 0; ixlast < nVar; ixlast++) {
        b_TrialState.delta_gradLag[ixlast] = b_TrialState.grad[ixlast];
      }
      if (nVar >= 1) {
        ixlast = nVar - 1;
        for (ineqStart = 0; ineqStart <= ixlast; ineqStart++) {
          b_TrialState.delta_gradLag[ineqStart] =
              b_TrialState.delta_gradLag[ineqStart] -
              b_TrialState.grad_old[ineqStart];
        }
      }
      BFGSUpdate(nVar, Hessian, b_TrialState.delta_x,
                 b_TrialState.delta_gradLag, memspace.workspace_double);
      b_TrialState.sqpIterations++;
    }
  }
}

} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for driver.cpp
//
// [EOF]
//
