//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "driver.h"
#include "BFGSUpdate.h"
#include "anonymous_function.h"
#include "computeFiniteDifferences.h"
#include "computeMeritFcn.h"
#include "inverseKinematicsMatlabObstacleAvoidance.h"
#include "inverseKinematicsOA_internal_types.h"
#include "inverseKinematicsOA_internal_types1.h"
#include "inverseKinematicsOA_types.h"
#include "rt_nonfinite.h"
#include "step.h"
#include "stickyStruct.h"
#include "test_exit.h"
#include "updateWorkingSetForNewQP.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 2U> &Hessian
//                const array<double, 2U> &bineq
//                const double beq_data[]
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                j_struct_T &b_TrialState
//                struct_T &b_MeritFunction
//                const ::coder::internal::i_stickyStruct &FcnEvaluator
//                l_struct_T &FiniteDifferences
//                h_struct_T &memspace
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                f_struct_T &b_CholManager
//                g_struct_T &QPObjective
//                const array<double, 1U> &fscales_lineq_constraint
//                const i_struct_T &runTimeOptions
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
void driver(array<double, 2U> &Hessian, const array<double, 2U> &bineq,
            const double beq_data[], const array<double, 2U> &lb,
            const array<double, 2U> &ub, j_struct_T &b_TrialState,
            struct_T &b_MeritFunction,
            const ::coder::internal::i_stickyStruct &FcnEvaluator,
            l_struct_T &FiniteDifferences, h_struct_T &memspace,
            d_struct_T &WorkingSet, e_struct_T &b_QRManager,
            f_struct_T &b_CholManager, g_struct_T &QPObjective,
            const array<double, 1U> &fscales_lineq_constraint,
            const i_struct_T &runTimeOptions)
{
  static const char qpoptions_SolverName[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  array<double, 1U> y;
  b_struct_T Flags;
  k_struct_T b_expl_temp;
  k_struct_T expl_temp;
  int b_nVar;
  int b_n_tmp;
  int ixlast;
  int mConstr;
  int mEq;
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  int nVar;
  int n_tmp;
  int qpoptions_MaxIterations;
  int u1;
  nVar = WorkingSet.nVar;
  mFixed = WorkingSet.sizes[0];
  mEq = WorkingSet.sizes[1];
  mIneq = WorkingSet.sizes[2];
  mLB = WorkingSet.sizes[3];
  mUB = WorkingSet.sizes[4];
  mConstr =
      (((WorkingSet.sizes[0] + WorkingSet.sizes[1]) + WorkingSet.sizes[2]) +
       WorkingSet.sizes[3]) +
      WorkingSet.sizes[4];
  ixlast = WorkingSet.nVar;
  u1 = ((WorkingSet.sizes[2] + WorkingSet.sizes[3]) + WorkingSet.sizes[4]) +
       (WorkingSet.sizes[0] << 1);
  if (ixlast >= u1) {
    u1 = ixlast;
  }
  qpoptions_MaxIterations = 10 * u1;
  b_TrialState.steplength = 1.0;
  Flags.gradOK = test_exit(b_MeritFunction, WorkingSet, b_TrialState, lb, ub,
                           runTimeOptions.MaxFunctionEvaluations, Flags.fevalOK,
                           Flags.done, Flags.stepAccepted,
                           Flags.failedLineSearch, Flags.stepType);
  b_TrialState.sqpFval_old = b_TrialState.sqpFval;
  b_nVar = b_TrialState.xstarsqp.size(0) - 1;
  for (u1 = 0; u1 <= b_nVar; u1++) {
    b_TrialState.xstarsqp_old[u1] = b_TrialState.xstarsqp[u1];
    b_TrialState.grad_old[u1] = b_TrialState.grad[u1];
  }
  n_tmp = b_TrialState.mIneq;
  for (u1 = 0; u1 < n_tmp; u1++) {
    b_TrialState.cIneq_old[u1] = b_TrialState.cIneq[u1];
  }
  b_n_tmp = b_TrialState.mEq;
  y.set_size(b_TrialState.cEq_old.size[0]);
  u1 = b_TrialState.cEq_old.size[0];
  for (int i{0}; i < u1; i++) {
    y[i] = b_TrialState.cEq_old.data[i];
  }
  for (u1 = 0; u1 < b_n_tmp; u1++) {
    y[u1] = b_TrialState.cEq.data[u1];
  }
  b_TrialState.cEq_old.size[0] = y.size(0);
  u1 = y.size(0);
  for (int i{0}; i < u1; i++) {
    b_TrialState.cEq_old.data[i] = y[i];
  }
  if (!Flags.done) {
    b_TrialState.sqpIterations = 1;
  }
  while (!Flags.done) {
    double phi_alpha;
    while (!(Flags.stepAccepted || Flags.failedLineSearch)) {
      boolean_T b;
      if (Flags.stepType != 3) {
        internal::updateWorkingSetForNewQP(
            b_TrialState.xstarsqp, WorkingSet, mIneq, b_TrialState.cIneq, mEq,
            b_TrialState.cEq.data, mLB, lb, mUB, ub, mFixed);
      }
      expl_temp.ObjectiveLimit = rtMinusInf;
      expl_temp.StepTolerance = 1.0E-6;
      expl_temp.MaxIterations = qpoptions_MaxIterations;
      for (int i{0}; i < 7; i++) {
        expl_temp.SolverName[i] = qpoptions_SolverName[i];
      }
      b_expl_temp = expl_temp;
      Flags.stepAccepted =
          b_step(Flags.stepType, Hessian, lb, ub, b_TrialState, b_MeritFunction,
                 memspace, WorkingSet, b_QRManager, b_CholManager, QPObjective,
                 b_expl_temp);
      if (Flags.stepAccepted) {
        for (u1 = 0; u1 < nVar; u1++) {
          b_TrialState.xstarsqp[u1] =
              b_TrialState.xstarsqp[u1] + b_TrialState.delta_x[u1];
        }
        phi_alpha = inverseKinematicsOA_anonFcn1(
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .jacobi,
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .xd_eff_vel,
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .starting_joint_vel,
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .joint_values,
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .jointVelocityWeightMatrix,
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .config,
            b_TrialState.xstarsqp);
        ixlast = 1;
        b = std::isnan(phi_alpha);
        if (std::isinf(phi_alpha) || b) {
          if (b) {
            ixlast = -3;
          } else if (phi_alpha < 0.0) {
            ixlast = -1;
          } else {
            ixlast = -2;
          }
        }
        if (ixlast == 1) {
          ixlast = 1;
        }
        b_TrialState.sqpFval = phi_alpha;
        Flags.fevalOK = (ixlast == 1);
        b_TrialState.FunctionEvaluations++;
        if (mIneq > 0) {
          for (u1 = 0; u1 < mIneq; u1++) {
            b_TrialState.cIneq[u1] = bineq[u1];
          }
          ::coder::internal::blas::xgemv(nVar, mIneq, WorkingSet.Aineq,
                                         WorkingSet.ldA, b_TrialState.xstarsqp,
                                         b_TrialState.cIneq);
        }
        if (mEq > 0) {
          y.set_size(b_TrialState.cEq.size[0]);
          u1 = b_TrialState.cEq.size[0];
          for (int i{0}; i < u1; i++) {
            y[i] = b_TrialState.cEq.data[i];
          }
          for (u1 = 0; u1 < mEq; u1++) {
            y[u1] = beq_data[u1];
          }
          ::coder::internal::blas::xgemv(nVar, mEq, WorkingSet.Aeq,
                                         WorkingSet.ldA, b_TrialState.xstarsqp,
                                         y);
          b_TrialState.cEq.size[0] = y.size(0);
          u1 = y.size(0);
          for (int i{0}; i < u1; i++) {
            b_TrialState.cEq.data[i] = y[i];
          }
        }
        b_MeritFunction.phiFullStep = MeritFunction::computeMeritFcn(
            b_MeritFunction.penaltyParam, phi_alpha, b_TrialState.cIneq, mIneq,
            b_TrialState.cEq.data, mEq, Flags.fevalOK);
      }
      if ((Flags.stepType == 1) && Flags.stepAccepted && Flags.fevalOK &&
          (b_MeritFunction.phi < b_MeritFunction.phiFullStep) &&
          (b_TrialState.sqpFval < b_TrialState.sqpFval_old)) {
        Flags.stepType = 3;
        Flags.stepAccepted = false;
      } else {
        double alpha;
        int exitflagLnSrch;
        boolean_T socTaken;
        if ((Flags.stepType == 3) && Flags.stepAccepted) {
          socTaken = true;
        } else {
          socTaken = false;
        }
        b = Flags.fevalOK;
        b_nVar = WorkingSet.nVar - 1;
        alpha = 1.0;
        exitflagLnSrch = 1;
        phi_alpha = b_MeritFunction.phiFullStep;
        for (u1 = 0; u1 <= b_nVar; u1++) {
          b_TrialState.searchDir[u1] = b_TrialState.delta_x[u1];
        }
        int exitg1;
        do {
          exitg1 = 0;
          if (b_TrialState.FunctionEvaluations <
              runTimeOptions.MaxFunctionEvaluations) {
            if (b && (phi_alpha <=
                      b_MeritFunction.phi +
                          alpha * 0.0001 * b_MeritFunction.phiPrimePlus)) {
              exitg1 = 1;
            } else {
              boolean_T exitg2;
              boolean_T tooSmallX;
              alpha *= 0.7;
              for (u1 = 0; u1 <= b_nVar; u1++) {
                b_TrialState.delta_x[u1] = alpha * b_TrialState.xstar[u1];
              }
              if (socTaken) {
                phi_alpha = alpha * alpha;
                if ((b_nVar + 1 >= 1) && (!(phi_alpha == 0.0))) {
                  for (u1 = 0; u1 <= b_nVar; u1++) {
                    b_TrialState.delta_x[u1] =
                        b_TrialState.delta_x[u1] +
                        phi_alpha * b_TrialState.socDirection[u1];
                  }
                }
              }
              tooSmallX = true;
              u1 = 0;
              exitg2 = false;
              while ((!exitg2) && (u1 <= b_nVar)) {
                if (1.0E-6 *
                        std::fmax(1.0, std::abs(b_TrialState.xstarsqp[u1])) <=
                    std::abs(b_TrialState.delta_x[u1])) {
                  tooSmallX = false;
                  exitg2 = true;
                } else {
                  u1++;
                }
              }
              if (tooSmallX) {
                exitflagLnSrch = -2;
                exitg1 = 1;
              } else {
                for (u1 = 0; u1 <= b_nVar; u1++) {
                  b_TrialState.xstarsqp[u1] =
                      b_TrialState.xstarsqp_old[u1] + b_TrialState.delta_x[u1];
                }
                phi_alpha = inverseKinematicsOA_anonFcn1(
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.jacobi,
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.xd_eff_vel,
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.starting_joint_vel,
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.joint_values,
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.jointVelocityWeightMatrix,
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.config,
                    b_TrialState.xstarsqp);
                ixlast = 1;
                b = std::isnan(phi_alpha);
                if (std::isinf(phi_alpha) || b) {
                  if (b) {
                    ixlast = -3;
                  } else if (phi_alpha < 0.0) {
                    ixlast = -1;
                  } else {
                    ixlast = -2;
                  }
                }
                if (ixlast == 1) {
                  ixlast = 1;
                }
                b_TrialState.sqpFval = phi_alpha;
                if (n_tmp > 0) {
                  for (u1 = 0; u1 < n_tmp; u1++) {
                    b_TrialState.cIneq[u1] = bineq[u1];
                  }
                  ::coder::internal::blas::xgemv(
                      b_nVar + 1, n_tmp, WorkingSet.Aineq, WorkingSet.ldA,
                      b_TrialState.xstarsqp, b_TrialState.cIneq);
                }
                if (b_n_tmp > 0) {
                  y.set_size(b_TrialState.cEq.size[0]);
                  u1 = b_TrialState.cEq.size[0];
                  for (int i{0}; i < u1; i++) {
                    y[i] = b_TrialState.cEq.data[i];
                  }
                  for (u1 = 0; u1 < b_n_tmp; u1++) {
                    y[u1] = beq_data[u1];
                  }
                  ::coder::internal::blas::xgemv(b_nVar + 1, b_n_tmp,
                                                 WorkingSet.Aeq, WorkingSet.ldA,
                                                 b_TrialState.xstarsqp, y);
                  b_TrialState.cEq.size[0] = y.size(0);
                  u1 = y.size(0);
                  for (int i{0}; i < u1; i++) {
                    b_TrialState.cEq.data[i] = y[i];
                  }
                }
                b_TrialState.FunctionEvaluations++;
                b = (ixlast == 1);
                phi_alpha = MeritFunction::computeMeritFcn(
                    b_MeritFunction.penaltyParam, phi_alpha, b_TrialState.cIneq,
                    n_tmp, b_TrialState.cEq.data, b_n_tmp, b);
              }
            }
          } else {
            exitflagLnSrch = 0;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
        Flags.fevalOK = b;
        b_TrialState.steplength = alpha;
        if (exitflagLnSrch > 0) {
          Flags.stepAccepted = true;
        } else {
          Flags.failedLineSearch = true;
        }
      }
    }
    if (Flags.stepAccepted && (!Flags.failedLineSearch)) {
      for (u1 = 0; u1 < nVar; u1++) {
        b_TrialState.xstarsqp[u1] =
            b_TrialState.xstarsqp_old[u1] + b_TrialState.delta_x[u1];
      }
      for (u1 = 0; u1 < mConstr; u1++) {
        phi_alpha = b_TrialState.lambdasqp[u1];
        phi_alpha +=
            b_TrialState.steplength * (b_TrialState.lambda[u1] - phi_alpha);
        b_TrialState.lambdasqp[u1] = phi_alpha;
      }
      b_TrialState.sqpFval_old = b_TrialState.sqpFval;
      b_nVar = b_TrialState.xstarsqp.size(0) - 1;
      for (u1 = 0; u1 <= b_nVar; u1++) {
        b_TrialState.xstarsqp_old[u1] = b_TrialState.xstarsqp[u1];
        b_TrialState.grad_old[u1] = b_TrialState.grad[u1];
      }
      for (u1 = 0; u1 < n_tmp; u1++) {
        b_TrialState.cIneq_old[u1] = b_TrialState.cIneq[u1];
      }
      y.set_size(b_TrialState.cEq_old.size[0]);
      u1 = b_TrialState.cEq_old.size[0];
      for (int i{0}; i < u1; i++) {
        y[i] = b_TrialState.cEq_old.data[i];
      }
      for (u1 = 0; u1 < b_n_tmp; u1++) {
        y[u1] = b_TrialState.cEq.data[u1];
      }
      b_TrialState.cEq_old.size[0] = y.size(0);
      u1 = y.size(0);
      for (int i{0}; i < u1; i++) {
        b_TrialState.cEq_old.data[i] = y[i];
      }
      Flags.gradOK = utils::FiniteDifferences::computeFiniteDifferences(
          FiniteDifferences, b_TrialState.sqpFval, b_TrialState.xstarsqp,
          b_TrialState.grad, lb, ub);
      b_TrialState.FunctionEvaluations += FiniteDifferences.numEvals;
    } else {
      b_TrialState.sqpFval = b_TrialState.sqpFval_old;
      ixlast = b_TrialState.xstarsqp.size(0);
      for (u1 = 0; u1 < ixlast; u1++) {
        b_TrialState.xstarsqp[u1] = b_TrialState.xstarsqp_old[u1];
      }
      for (u1 = 0; u1 < n_tmp; u1++) {
        b_TrialState.cIneq[u1] = b_TrialState.cIneq_old[u1];
      }
      y.set_size(b_TrialState.cEq.size[0]);
      u1 = b_TrialState.cEq.size[0];
      for (int i{0}; i < u1; i++) {
        y[i] = b_TrialState.cEq.data[i];
      }
      for (u1 = 0; u1 < b_n_tmp; u1++) {
        y[u1] = b_TrialState.cEq_old.data[u1];
      }
      b_TrialState.cEq.size[0] = y.size(0);
      u1 = y.size(0);
      for (int i{0}; i < u1; i++) {
        b_TrialState.cEq.data[i] = y[i];
      }
    }
    b_test_exit(Flags, memspace, b_MeritFunction, fscales_lineq_constraint,
                WorkingSet, b_TrialState, b_QRManager, lb, ub,
                runTimeOptions.MaxFunctionEvaluations);
    if ((!Flags.done) && Flags.stepAccepted) {
      Flags.stepAccepted = false;
      Flags.stepType = 1;
      Flags.failedLineSearch = false;
      for (u1 = 0; u1 < nVar; u1++) {
        b_TrialState.delta_gradLag[u1] = b_TrialState.grad[u1];
      }
      if (nVar >= 1) {
        ixlast = nVar - 1;
        for (u1 = 0; u1 <= ixlast; u1++) {
          b_TrialState.delta_gradLag[u1] =
              b_TrialState.delta_gradLag[u1] - b_TrialState.grad_old[u1];
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
