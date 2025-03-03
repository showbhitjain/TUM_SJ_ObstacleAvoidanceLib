//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "driver.h"
#include "BFGSUpdate.h"
#include "computeFiniteDifferences.h"
#include "computeLinearResiduals.h"
#include "computeMeritFcn.h"
#include "eml_int_forloop_overflow_check.h"
#include "evalObjAndConstr.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "isDeltaXTooSmall.h"
#include "rt_nonfinite.h"
#include "saveState.h"
#include "step.h"
#include "stickyStruct.h"
#include "test_exit.h"
#include "updateWorkingSetForNewQP.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
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
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      1,        // lineNo
      1,        // colNo
      "",       // aName
      "driver", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "driver.p", // pName
      0           // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,           // iFirst
      -1,           // iLast
      1,            // lineNo
      1,            // colNo
      "",           // aName
      "linesearch", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "linesearch.p", // pName
      0               // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "computeDeltaLag", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "+stopping/computeDeltaLag.p", // pName
      0                              // checkKind
  };
  static const char qpoptions_SolverName[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  array<double, 1U> y;
  b_struct_T Flags;
  k_struct_T b_expl_temp;
  k_struct_T expl_temp;
  int b_mEq;
  int eqCol0_tmp;
  int ineqCol0_tmp;
  int ixlast;
  int mConstr;
  int mEq;
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  int nVar_tmp_tmp;
  int qpoptions_MaxIterations;
  nVar_tmp_tmp = WorkingSet.nVar;
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
  b_mEq = ((WorkingSet.sizes[2] + WorkingSet.sizes[3]) + WorkingSet.sizes[4]) +
          (WorkingSet.sizes[0] << 1);
  if (ixlast >= b_mEq) {
    b_mEq = ixlast;
  }
  qpoptions_MaxIterations = 10 * b_mEq;
  b_TrialState.steplength = 1.0;
  Flags.gradOK = test_exit(
      b_MeritFunction, fscales_lineq_constraint, WorkingSet, b_TrialState, lb,
      ub, runTimeOptions.MaxFunctionEvaluations, Flags.fevalOK, Flags.done,
      Flags.stepAccepted, Flags.failedLineSearch, Flags.stepType);
  ineqCol0_tmp = b_TrialState.iNonIneq0;
  eqCol0_tmp = b_TrialState.iNonEq0;
  if ((mIneq - ineqCol0_tmp) + 1 > 2147483646) {
    check_forloop_overflow_error();
  }
  if ((mEq - eqCol0_tmp) + 1 > 2147483646) {
    check_forloop_overflow_error();
  }
  TrialState::saveState(b_TrialState);
  if (!Flags.done) {
    b_TrialState.sqpIterations = 1;
  }
  while (!Flags.done) {
    double phi_alpha;
    int i;
    while (!(Flags.stepAccepted || Flags.failedLineSearch)) {
      if (Flags.stepType != 3) {
        internal::updateWorkingSetForNewQP(
            b_TrialState.xstarsqp, WorkingSet, mIneq, b_TrialState.cIneq, mEq,
            b_TrialState.cEq.data, b_TrialState.cEq.size[0], mLB, lb, mUB, ub,
            mFixed);
      }
      expl_temp.ObjectiveLimit = rtMinusInf;
      expl_temp.StepTolerance = 1.0E-6;
      expl_temp.MaxIterations = qpoptions_MaxIterations;
      for (i = 0; i < 7; i++) {
        expl_temp.SolverName[i] = qpoptions_SolverName[i];
      }
      b_expl_temp = expl_temp;
      Flags.stepAccepted =
          b_step(Flags.stepType, Hessian, lb, ub, b_TrialState, b_MeritFunction,
                 memspace, WorkingSet, b_QRManager, b_CholManager, QPObjective,
                 b_expl_temp);
      if (Flags.stepAccepted) {
        if (nVar_tmp_tmp > 2147483646) {
          check_forloop_overflow_error();
        }
        for (ixlast = 0; ixlast < nVar_tmp_tmp; ixlast++) {
          i = b_TrialState.xstarsqp.size(0);
          if ((ixlast + 1 < 1) || (ixlast + 1 > i)) {
            rtDynamicBoundsError(ixlast + 1, 1, i, w_emlrtBCI);
          }
          i = b_TrialState.delta_x.size(0);
          if (ixlast + 1 > i) {
            rtDynamicBoundsError(ixlast + 1, 1, i, w_emlrtBCI);
          }
          i = b_TrialState.xstarsqp.size(0);
          if (ixlast + 1 > i) {
            rtDynamicBoundsError(ixlast + 1, 1, i, w_emlrtBCI);
          }
          b_TrialState.xstarsqp[ixlast] =
              b_TrialState.xstarsqp[ixlast] + b_TrialState.delta_x[ixlast];
        }
        b_TrialState.sqpFval = utils::ObjNonlinEvaluator::evalObjAndConstr(
            FcnEvaluator, b_TrialState.xstarsqp, ixlast);
        Flags.fevalOK = (ixlast == 1);
        b_TrialState.FunctionEvaluations++;
        internal::computeLinearResiduals(
            b_TrialState.xstarsqp, nVar_tmp_tmp, b_TrialState.cIneq, mIneq,
            WorkingSet.Aineq, bineq, WorkingSet.ldA, b_TrialState.cEq.data,
            b_TrialState.cEq.size[0], mEq, WorkingSet.Aeq, beq_data,
            WorkingSet.ldA);
        b_MeritFunction.phiFullStep = MeritFunction::computeMeritFcn(
            b_MeritFunction.penaltyParam, b_TrialState.sqpFval,
            b_TrialState.cIneq, mIneq, b_TrialState.cEq.data, mEq,
            Flags.fevalOK);
      }
      if ((Flags.stepType == 1) && Flags.stepAccepted && Flags.fevalOK &&
          (b_MeritFunction.phi < b_MeritFunction.phiFullStep) &&
          (b_TrialState.sqpFval < b_TrialState.sqpFval_old)) {
        Flags.stepType = 3;
        Flags.stepAccepted = false;
      } else {
        double alpha;
        int b_mIneq;
        int exitflagLnSrch;
        boolean_T b;
        boolean_T socTaken;
        if ((Flags.stepType == 3) && Flags.stepAccepted) {
          socTaken = true;
        } else {
          socTaken = false;
        }
        b = Flags.fevalOK;
        i = WorkingSet.nVar;
        b_mEq = b_TrialState.mEq;
        b_mIneq = b_TrialState.mIneq;
        alpha = 1.0;
        exitflagLnSrch = 1;
        phi_alpha = b_MeritFunction.phiFullStep;
        if (WorkingSet.nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int k{0}; k < i; k++) {
          b_TrialState.searchDir[k] = b_TrialState.delta_x[k];
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
              boolean_T tooSmallX;
              alpha *= 0.7;
              if (i > 2147483646) {
                check_forloop_overflow_error();
              }
              for (int k{0}; k < i; k++) {
                ixlast = b_TrialState.xstar.size(0);
                if ((k + 1 < 1) || (k + 1 > ixlast)) {
                  rtDynamicBoundsError(k + 1, 1, ixlast, x_emlrtBCI);
                }
                ixlast = b_TrialState.delta_x.size(0);
                if (k + 1 > ixlast) {
                  rtDynamicBoundsError(k + 1, 1, ixlast, x_emlrtBCI);
                }
                b_TrialState.delta_x[k] = alpha * b_TrialState.xstar[k];
              }
              if (socTaken) {
                phi_alpha = alpha * alpha;
                if ((i >= 1) && (!(phi_alpha == 0.0))) {
                  ixlast = i - 1;
                  for (int k{0}; k <= ixlast; k++) {
                    b_TrialState.delta_x[k] =
                        b_TrialState.delta_x[k] +
                        phi_alpha * b_TrialState.socDirection[k];
                  }
                }
              }
              tooSmallX = stopping::isDeltaXTooSmall(b_TrialState.xstarsqp,
                                                     b_TrialState.delta_x, i);
              if (tooSmallX) {
                exitflagLnSrch = -2;
                exitg1 = 1;
              } else {
                for (int k{0}; k < i; k++) {
                  ixlast = b_TrialState.xstarsqp_old.size(0);
                  if ((k + 1 < 1) || (k + 1 > ixlast)) {
                    rtDynamicBoundsError(k + 1, 1, ixlast, x_emlrtBCI);
                  }
                  ixlast = b_TrialState.delta_x.size(0);
                  if (k + 1 > ixlast) {
                    rtDynamicBoundsError(k + 1, 1, ixlast, x_emlrtBCI);
                  }
                  ixlast = b_TrialState.xstarsqp.size(0);
                  if (k + 1 > ixlast) {
                    rtDynamicBoundsError(k + 1, 1, ixlast, x_emlrtBCI);
                  }
                  b_TrialState.xstarsqp[k] =
                      b_TrialState.xstarsqp_old[k] + b_TrialState.delta_x[k];
                }
                b_TrialState.sqpFval =
                    utils::ObjNonlinEvaluator::evalObjAndConstr(
                        FcnEvaluator, b_TrialState.xstarsqp, ixlast);
                internal::computeLinearResiduals(
                    b_TrialState.xstarsqp, i, b_TrialState.cIneq, b_mIneq,
                    WorkingSet.Aineq, bineq, WorkingSet.ldA,
                    b_TrialState.cEq.data, b_TrialState.cEq.size[0], b_mEq,
                    WorkingSet.Aeq, beq_data, WorkingSet.ldA);
                b_TrialState.FunctionEvaluations++;
                b = (ixlast == 1);
                phi_alpha = MeritFunction::computeMeritFcn(
                    b_MeritFunction.penaltyParam, b_TrialState.sqpFval,
                    b_TrialState.cIneq, b_mIneq, b_TrialState.cEq.data, b_mEq,
                    b);
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
      if (nVar_tmp_tmp > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < nVar_tmp_tmp; k++) {
        i = b_TrialState.xstarsqp_old.size(0);
        if ((k + 1 < 1) || (k + 1 > i)) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        i = b_TrialState.delta_x.size(0);
        if (k + 1 > i) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        i = b_TrialState.xstarsqp.size(0);
        if (k + 1 > i) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        b_TrialState.xstarsqp[k] =
            b_TrialState.xstarsqp_old[k] + b_TrialState.delta_x[k];
      }
      if (mConstr > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < mConstr; k++) {
        i = b_TrialState.lambdasqp.size(0);
        if ((k + 1 < 1) || (k + 1 > i)) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        i = b_TrialState.lambda.size(0);
        if (k + 1 > i) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        i = b_TrialState.lambdasqp.size(0);
        if (k + 1 > i) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        i = b_TrialState.lambdasqp.size(0);
        if (k + 1 > i) {
          rtDynamicBoundsError(k + 1, 1, i, w_emlrtBCI);
        }
        phi_alpha = b_TrialState.lambdasqp[k];
        b_TrialState.lambdasqp[k] =
            phi_alpha +
            b_TrialState.steplength * (b_TrialState.lambda[k] - phi_alpha);
      }
      TrialState::saveState(b_TrialState);
      Flags.gradOK = utils::FiniteDifferences::computeFiniteDifferences(
          FiniteDifferences, b_TrialState.sqpFval, b_TrialState.xstarsqp,
          b_TrialState.grad, lb, ub, runTimeOptions.FiniteDifferenceStepSize,
          runTimeOptions.TypicalX);
      b_TrialState.FunctionEvaluations += FiniteDifferences.numEvals;
    } else {
      b_TrialState.sqpFval = b_TrialState.sqpFval_old;
      b_mEq = b_TrialState.xstarsqp.size(0);
      if (b_TrialState.xstarsqp.size(0) > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < b_mEq; k++) {
        b_TrialState.xstarsqp[k] = b_TrialState.xstarsqp_old[k];
      }
      b_mEq = b_TrialState.mIneq;
      if (b_TrialState.mIneq > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < b_mEq; k++) {
        b_TrialState.cIneq[k] = b_TrialState.cIneq_old[k];
      }
      b_mEq = b_TrialState.mEq;
      y.set_size(b_TrialState.cEq.size[0]);
      ixlast = b_TrialState.cEq.size[0];
      for (i = 0; i < ixlast; i++) {
        y[i] = b_TrialState.cEq.data[i];
      }
      if (b_TrialState.mEq > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < b_mEq; k++) {
        y[k] = b_TrialState.cEq_old.data[k];
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
      if (nVar_tmp_tmp > 2147483646) {
        check_forloop_overflow_error();
      }
      for (ixlast = 0; ixlast < nVar_tmp_tmp; ixlast++) {
        i = b_TrialState.grad.size(0);
        if ((ixlast + 1 < 1) || (ixlast + 1 > i)) {
          rtDynamicBoundsError(ixlast + 1, 1, i, y_emlrtBCI);
        }
        i = b_TrialState.delta_gradLag.size(0);
        if (ixlast + 1 > i) {
          rtDynamicBoundsError(ixlast + 1, 1, i, y_emlrtBCI);
        }
        b_TrialState.delta_gradLag[ixlast] = b_TrialState.grad[ixlast];
      }
      if (nVar_tmp_tmp >= 1) {
        ixlast = nVar_tmp_tmp - 1;
        for (int k{0}; k <= ixlast; k++) {
          b_TrialState.delta_gradLag[k] =
              b_TrialState.delta_gradLag[k] - b_TrialState.grad_old[k];
        }
      }
      if ((mIneq - ineqCol0_tmp) + 1 > 2147483646) {
        check_forloop_overflow_error();
      }
      if ((mEq - eqCol0_tmp) + 1 > 2147483646) {
        check_forloop_overflow_error();
      }
      BFGSUpdate(nVar_tmp_tmp, Hessian, b_TrialState.delta_x,
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
