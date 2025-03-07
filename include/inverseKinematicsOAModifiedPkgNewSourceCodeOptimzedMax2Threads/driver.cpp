//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "driver.h"
#include "BFGSUpdate.h"
#include "OptimizationBasedIKWithOAModified.h"
#include "anonymous_function.h"
#include "computeFiniteDifferences.h"
#include "computeMeritFcn.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_internal_types1.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "step.h"
#include "stickyStruct.h"
#include "test_exit.h"
#include "updateWorkingSetForNewQP.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
// Function Definitions
//
// Arguments    : array<double, 2U> &Hessian
//                const array<double, 2U> &bineq
//                const double beq_data[]
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
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
            const double beq_data[], const array<double, 1U> &lb,
            const array<double, 1U> &ub, j_struct_T &b_TrialState,
            struct_T &b_MeritFunction,
            const ::coder::internal::i_stickyStruct &FcnEvaluator,
            l_struct_T &FiniteDifferences, h_struct_T &memspace,
            d_struct_T &WorkingSet, e_struct_T &b_QRManager,
            f_struct_T &b_CholManager, g_struct_T &QPObjective,
            const array<double, 1U> &fscales_lineq_constraint,
            const i_struct_T &runTimeOptions)
{
  array<double, 1U> y;
  b_struct_T Flags;
  k_struct_T b_expl_temp;
  k_struct_T expl_temp;
  int b_nVar;
  int i;
  int ixlast;
  int mConstr;
  int mEq;
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  int nVar;
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
  if (static_cast<int>(b_nVar + 1 < 200)) {
    for (int k{0}; k <= b_nVar; k++) {
      b_TrialState.xstarsqp_old[k] = b_TrialState.xstarsqp[k];
      b_TrialState.grad_old[k] = b_TrialState.grad[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k <= b_nVar; k++) {
      b_TrialState.xstarsqp_old[k] = b_TrialState.xstarsqp[k];
      b_TrialState.grad_old[k] = b_TrialState.grad[k];
    }
  }
  u1 = b_TrialState.mIneq;
  if (static_cast<int>(u1 < 200)) {
    for (int k{0}; k < u1; k++) {
      b_TrialState.cIneq_old[k] = b_TrialState.cIneq[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < u1; k++) {
      b_TrialState.cIneq_old[k] = b_TrialState.cIneq[k];
    }
  }
  u1 = b_TrialState.mEq;
  y.set_size(b_TrialState.cEq_old.size[0]);
  ixlast = b_TrialState.cEq_old.size[0];
  for (i = 0; i < ixlast; i++) {
    y[i] = b_TrialState.cEq_old.data[i];
  }
  if (static_cast<int>(u1 < 200)) {
    for (int k{0}; k < u1; k++) {
      y[k] = b_TrialState.cEq.data[k];
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < u1; k++) {
      y[k] = b_TrialState.cEq.data[k];
    }
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
      expl_temp.SolverName[0] = 'f';
      expl_temp.SolverName[1] = 'm';
      expl_temp.SolverName[2] = 'i';
      expl_temp.SolverName[3] = 'n';
      expl_temp.SolverName[4] = 'c';
      expl_temp.SolverName[5] = 'o';
      expl_temp.SolverName[6] = 'n';
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
        phi_alpha = inverseKinematicsOAModified_anonFcn1(
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
            FcnEvaluator.next.next.next.next.next.next.next.next.value.workspace
                .slackIsUsed,
            b_TrialState.xstarsqp);
        u1 = 1;
        b = std::isnan(phi_alpha);
        if (std::isinf(phi_alpha) || b) {
          if (b) {
            u1 = -3;
          } else if (phi_alpha < 0.0) {
            u1 = -1;
          } else {
            u1 = -2;
          }
        }
        if (u1 == 1) {
          u1 = 1;
        }
        b_TrialState.sqpFval = phi_alpha;
        Flags.fevalOK = (u1 == 1);
        b_TrialState.FunctionEvaluations++;
        if (mIneq > 0) {
          for (int b_k{0}; b_k < mIneq; b_k++) {
            b_TrialState.cIneq[b_k] = bineq[b_k];
          }
          ::coder::internal::blas::xgemv(nVar, mIneq, WorkingSet.Aineq,
                                         WorkingSet.ldA, b_TrialState.xstarsqp,
                                         b_TrialState.cIneq);
        }
        if (mEq > 0) {
          y.set_size(b_TrialState.cEq.size[0]);
          ixlast = b_TrialState.cEq.size[0];
          for (i = 0; i < ixlast; i++) {
            y[i] = b_TrialState.cEq.data[i];
          }
          for (int b_k{0}; b_k < mEq; b_k++) {
            y[b_k] = beq_data[b_k];
          }
          ::coder::internal::blas::xgemv(nVar, mEq, WorkingSet.Aeq,
                                         WorkingSet.ldA, b_TrialState.xstarsqp,
                                         y);
          b_TrialState.cEq.size[0] = y.size(0);
          ixlast = y.size(0);
          for (i = 0; i < ixlast; i++) {
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
        int i1;
        boolean_T socTaken;
        if ((Flags.stepType == 3) && Flags.stepAccepted) {
          socTaken = true;
        } else {
          socTaken = false;
        }
        b = Flags.fevalOK;
        b_nVar = WorkingSet.nVar - 1;
        i = b_TrialState.mEq;
        i1 = b_TrialState.mIneq;
        alpha = 1.0;
        exitflagLnSrch = 1;
        phi_alpha = b_MeritFunction.phiFullStep;
        for (int b_k{0}; b_k <= b_nVar; b_k++) {
          b_TrialState.searchDir[b_k] = b_TrialState.delta_x[b_k];
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
                  for (int b_k{0}; b_k <= b_nVar; b_k++) {
                    b_TrialState.delta_x[b_k] =
                        b_TrialState.delta_x[b_k] +
                        phi_alpha * b_TrialState.socDirection[b_k];
                  }
                }
              }
              tooSmallX = true;
              u1 = 0;
              exitg2 = false;
              while ((!exitg2) && (u1 <= b_nVar)) {
                if (OAGlobals::stepTolerance *
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
                phi_alpha = inverseKinematicsOAModified_anonFcn1(
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
                    FcnEvaluator.next.next.next.next.next.next.next.next.value
                        .workspace.slackIsUsed,
                    b_TrialState.xstarsqp);
                u1 = 1;
                b = std::isnan(phi_alpha);
                if (std::isinf(phi_alpha) || b) {
                  if (b) {
                    u1 = -3;
                  } else if (phi_alpha < 0.0) {
                    u1 = -1;
                  } else {
                    u1 = -2;
                  }
                }
                if (u1 == 1) {
                  u1 = 1;
                }
                b_TrialState.sqpFval = phi_alpha;
                if (i1 > 0) {
                  for (int b_k{0}; b_k < i1; b_k++) {
                    b_TrialState.cIneq[b_k] = bineq[b_k];
                  }
                  ::coder::internal::blas::xgemv(
                      b_nVar + 1, i1, WorkingSet.Aineq, WorkingSet.ldA,
                      b_TrialState.xstarsqp, b_TrialState.cIneq);
                }
                if (i > 0) {
                  y.set_size(b_TrialState.cEq.size[0]);
                  ixlast = b_TrialState.cEq.size[0];
                  for (int b_k{0}; b_k < ixlast; b_k++) {
                    y[b_k] = b_TrialState.cEq.data[b_k];
                  }
                  for (int b_k{0}; b_k < i; b_k++) {
                    y[b_k] = beq_data[b_k];
                  }
                  ::coder::internal::blas::xgemv(b_nVar + 1, i, WorkingSet.Aeq,
                                                 WorkingSet.ldA,
                                                 b_TrialState.xstarsqp, y);
                  b_TrialState.cEq.size[0] = y.size(0);
                  ixlast = y.size(0);
                  for (int b_k{0}; b_k < ixlast; b_k++) {
                    b_TrialState.cEq.data[b_k] = y[b_k];
                  }
                }
                b_TrialState.FunctionEvaluations++;
                b = (u1 == 1);
                phi_alpha = MeritFunction::computeMeritFcn(
                    b_MeritFunction.penaltyParam, phi_alpha, b_TrialState.cIneq,
                    i1, b_TrialState.cEq.data, i, b);
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
      for (int b_k{0}; b_k <= b_nVar; b_k++) {
        b_TrialState.xstarsqp_old[b_k] = b_TrialState.xstarsqp[b_k];
        b_TrialState.grad_old[b_k] = b_TrialState.grad[b_k];
      }
      u1 = b_TrialState.mIneq;
      for (int b_k{0}; b_k < u1; b_k++) {
        b_TrialState.cIneq_old[b_k] = b_TrialState.cIneq[b_k];
      }
      u1 = b_TrialState.mEq;
      y.set_size(b_TrialState.cEq_old.size[0]);
      ixlast = b_TrialState.cEq_old.size[0];
      for (i = 0; i < ixlast; i++) {
        y[i] = b_TrialState.cEq_old.data[i];
      }
      for (int b_k{0}; b_k < u1; b_k++) {
        y[b_k] = b_TrialState.cEq.data[b_k];
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
      u1 = b_TrialState.xstarsqp.size(0);
      for (int b_k{0}; b_k < u1; b_k++) {
        b_TrialState.xstarsqp[b_k] = b_TrialState.xstarsqp_old[b_k];
      }
      u1 = b_TrialState.mIneq;
      for (int b_k{0}; b_k < u1; b_k++) {
        b_TrialState.cIneq[b_k] = b_TrialState.cIneq_old[b_k];
      }
      u1 = b_TrialState.mEq;
      y.set_size(b_TrialState.cEq.size[0]);
      ixlast = b_TrialState.cEq.size[0];
      for (i = 0; i < ixlast; i++) {
        y[i] = b_TrialState.cEq.data[i];
      }
      for (int b_k{0}; b_k < u1; b_k++) {
        y[b_k] = b_TrialState.cEq_old.data[b_k];
      }
      b_TrialState.cEq.size[0] = y.size(0);
      ixlast = y.size(0);
      for (i = 0; i < ixlast; i++) {
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
        for (int b_k{0}; b_k <= ixlast; b_k++) {
          b_TrialState.delta_gradLag[b_k] =
              b_TrialState.delta_gradLag[b_k] - b_TrialState.grad_old[b_k];
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
