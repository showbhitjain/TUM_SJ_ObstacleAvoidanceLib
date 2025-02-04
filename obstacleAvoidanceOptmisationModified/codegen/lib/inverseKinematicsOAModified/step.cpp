//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: step.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "step.h"
#include "addAeqConstr.h"
#include "driver1.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "makeBoundFeasible.h"
#include "relaxed.h"
#include "removeAllIneqConstr.h"
#include "rt_nonfinite.h"
#include "soc.h"
#include "sortLambdaQP.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int &STEP_TYPE
//                array<double, 2U> &Hessian
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
//                j_struct_T &b_TrialState
//                struct_T &b_MeritFunction
//                h_struct_T &memspace
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                f_struct_T &b_CholManager
//                g_struct_T &QPObjective
//                k_struct_T &qpoptions
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
boolean_T b_step(int &STEP_TYPE, array<double, 2U> &Hessian,
                 const array<double, 2U> &lb, const array<double, 2U> &ub,
                 j_struct_T &b_TrialState, struct_T &b_MeritFunction,
                 h_struct_T &memspace, d_struct_T &WorkingSet,
                 e_struct_T &b_QRManager, f_struct_T &b_CholManager,
                 g_struct_T &QPObjective, k_struct_T &qpoptions)
{
  static rtBoundsCheckInfo
      ab_emlrtBCI{
          -1,     // iFirst
          -1,     // iLast
          1,      // lineNo
          1,      // colNo
          "",     // aName
          "step", // fName
          "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
          "step.p", // pName
          0         // checkKind
      };
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,          // iFirst
      -1,          // iLast
      1,           // lineNo
      1,           // colNo
      "",          // aName
      "BFGSReset", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
      "BFGSReset.p", // pName
      0              // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "saturateDirection", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "saturateDirection.p", // pName
      0                      // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,       // iFirst
      -1,       // iLast
      1,        // lineNo
      1,        // colNo
      "",       // aName
      "normal", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "normal.p", // pName
      0           // checkKind
  };
  array<double, 1U> r;
  k_struct_T b_qpoptions;
  double constrViolationEq;
  double constrViolationIneq;
  int i;
  int iH0;
  int loop_ub;
  int n;
  int nVar;
  boolean_T checkBoundViolation;
  boolean_T stepSuccess;
  stepSuccess = true;
  checkBoundViolation = true;
  nVar = WorkingSet.nVar;
  if (STEP_TYPE != 3) {
    if (WorkingSet.nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int k{0}; k < nVar; k++) {
      b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
    }
  } else {
    if (WorkingSet.nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int k{0}; k < nVar; k++) {
      b_TrialState.searchDir[k] = b_TrialState.xstar[k];
    }
  }
  int exitg1;
  boolean_T guard1;
  do {
    exitg1 = 0;
    guard1 = false;
    switch (STEP_TYPE) {
    case 1: {
      r.set_size(b_TrialState.grad.size(0));
      loop_ub = b_TrialState.grad.size(0);
      for (i = 0; i < loop_ub; i++) {
        r[i] = b_TrialState.grad[i];
      }
      b_qpoptions = qpoptions;
      ::coder::optim::coder::qpactiveset::driver(
          Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
          b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
      if (b_TrialState.state > 0) {
        double constrViolation;
        double penaltyParamTrial;
        loop_ub = WorkingSet.sizes[2];
        penaltyParamTrial = b_MeritFunction.penaltyParam;
        constrViolationEq = 0.0;
        if (WorkingSet.sizes[1] >= 1) {
          if (WorkingSet.sizes[1] > 2147483646) {
            check_forloop_overflow_error();
          }
          i = static_cast<unsigned char>(WorkingSet.sizes[1]);
          for (int k{0}; k < i; k++) {
            constrViolationEq += std::abs(b_TrialState.cEq.data[k]);
          }
        }
        constrViolationIneq = 0.0;
        if (WorkingSet.sizes[2] > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int idx{0}; idx < loop_ub; idx++) {
          i = b_TrialState.cIneq.size(0);
          if ((idx + 1 < 1) || (idx + 1 > i)) {
            rtDynamicBoundsError(idx + 1, 1, i, b_emlrtBCI);
          }
          constrViolation = b_TrialState.cIneq[idx];
          if (constrViolation > 0.0) {
            i = b_TrialState.cIneq.size(0);
            if (idx + 1 > i) {
              rtDynamicBoundsError(idx + 1, 1, i, b_emlrtBCI);
            }
            constrViolationIneq += constrViolation;
          }
        }
        constrViolation = constrViolationEq + constrViolationIneq;
        constrViolationEq = b_MeritFunction.linearizedConstrViol;
        b_MeritFunction.linearizedConstrViol = 0.0;
        constrViolationIneq = constrViolation + constrViolationEq;
        if ((constrViolationIneq > 2.2204460492503131E-16) &&
            (b_TrialState.fstar > 0.0)) {
          if (b_TrialState.sqpFval == 0.0) {
            penaltyParamTrial = 1.0;
          } else {
            penaltyParamTrial = 1.5;
          }
          penaltyParamTrial =
              penaltyParamTrial * b_TrialState.fstar / constrViolationIneq;
        }
        if (penaltyParamTrial < b_MeritFunction.penaltyParam) {
          b_MeritFunction.phi =
              b_TrialState.sqpFval + penaltyParamTrial * constrViolation;
          if ((b_MeritFunction.initFval +
               penaltyParamTrial * (b_MeritFunction.initConstrViolationEq +
                                    b_MeritFunction.initConstrViolationIneq)) -
                  b_MeritFunction.phi >
              static_cast<double>(b_MeritFunction.nPenaltyDecreases) *
                  b_MeritFunction.threshold) {
            b_MeritFunction.nPenaltyDecreases++;
            if ((b_MeritFunction.nPenaltyDecreases << 1) >
                b_TrialState.sqpIterations) {
              b_MeritFunction.threshold *= 10.0;
            }
            b_MeritFunction.penaltyParam =
                std::fmax(penaltyParamTrial, 1.0E-10);
          } else {
            b_MeritFunction.phi =
                b_TrialState.sqpFval +
                b_MeritFunction.penaltyParam * constrViolation;
          }
        } else {
          b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
          b_MeritFunction.phi = b_TrialState.sqpFval +
                                b_MeritFunction.penaltyParam * constrViolation;
        }
        b_MeritFunction.phiPrimePlus = std::fmin(
            b_TrialState.fstar - b_MeritFunction.penaltyParam * constrViolation,
            0.0);
      }
      qpactiveset::parseoutput::sortLambdaQP(
          b_TrialState.lambda, WorkingSet.nActiveConstr, WorkingSet.sizes,
          WorkingSet.isActiveIdx, WorkingSet.Wid, WorkingSet.Wlocalidx,
          memspace.workspace_double);
      if (WorkingSet.indexEqRemoved.size[0] > 0) {
        boolean_T exitg2;
        exitg2 = false;
        while ((!exitg2) && (WorkingSet.mEqRemoved > 0)) {
          i = WorkingSet.indexEqRemoved.size[0];
          if ((WorkingSet.mEqRemoved < 1) || (WorkingSet.mEqRemoved > i)) {
            rtDynamicBoundsError(WorkingSet.mEqRemoved, 1, i, y_emlrtBCI);
          }
          i = WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1];
          if (i >= b_TrialState.iNonEq0) {
            loop_ub = WorkingSet.indexEqRemoved.size[0];
            if ((WorkingSet.mEqRemoved < 1) ||
                (WorkingSet.mEqRemoved > loop_ub)) {
              rtDynamicBoundsError(WorkingSet.mEqRemoved, 1, loop_ub,
                                   y_emlrtBCI);
            }
            qpactiveset::WorkingSet::addAeqConstr(WorkingSet, i);
            WorkingSet.mEqRemoved--;
          } else {
            exitg2 = true;
          }
        }
      }
      if ((b_TrialState.state <= 0) && (b_TrialState.state != -6)) {
        STEP_TYPE = 2;
      } else {
        if (nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int k{0}; k < nVar; k++) {
          b_TrialState.delta_x[k] = b_TrialState.xstar[k];
        }
        guard1 = true;
      }
    } break;
    case 2:
      qpactiveset::WorkingSet::removeAllIneqConstr(WorkingSet);
      step::makeBoundFeasible(b_TrialState.xstar, WorkingSet, lb, ub);
      step::b_relaxed(Hessian, b_TrialState.grad, b_TrialState, b_MeritFunction,
                      memspace, WorkingSet, b_QRManager, b_CholManager,
                      QPObjective, qpoptions);
      if (nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int k{0}; k < nVar; k++) {
        b_TrialState.delta_x[k] = b_TrialState.xstar[k];
      }
      guard1 = true;
      break;
    default:
      r.set_size(b_TrialState.grad.size(0));
      loop_ub = b_TrialState.grad.size(0);
      for (i = 0; i < loop_ub; i++) {
        r[i] = b_TrialState.grad[i];
      }
      stepSuccess =
          step::soc(Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
                    b_CholManager, QPObjective, qpoptions);
      checkBoundViolation = stepSuccess;
      if (stepSuccess && (b_TrialState.state != -6)) {
        if (nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int idx{0}; idx < nVar; idx++) {
          i = b_TrialState.xstar.size(0);
          if ((idx + 1 < 1) || (idx + 1 > i)) {
            rtDynamicBoundsError(idx + 1, 1, i, ab_emlrtBCI);
          }
          i = b_TrialState.socDirection.size(0);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, ab_emlrtBCI);
          }
          i = b_TrialState.delta_x.size(0);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, ab_emlrtBCI);
          }
          b_TrialState.delta_x[idx] =
              b_TrialState.xstar[idx] + b_TrialState.socDirection[idx];
        }
      }
      guard1 = true;
      break;
    }
    if (guard1) {
      if (b_TrialState.state != -6) {
        exitg1 = 1;
      } else {
        int b_nVar;
        if ((Hessian.size(0) == 0) || (Hessian.size(1) == 0)) {
          d_rtErrorWithMessageID("input", emlrtRTEI.fName, emlrtRTEI.lineNo);
        }
        b_nVar = Hessian.size(0) - 1;
        constrViolationEq = 0.0;
        constrViolationIneq = 1.0;
        if (Hessian.size(0) > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int idx{0}; idx <= b_nVar; idx++) {
          i = b_TrialState.grad.size(0);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
          }
          constrViolationEq =
              std::fmax(constrViolationEq, std::abs(b_TrialState.grad[idx]));
          i = b_TrialState.xstar.size(0);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
          }
          constrViolationIneq =
              std::fmax(constrViolationIneq, std::abs(b_TrialState.xstar[idx]));
        }
        constrViolationEq = std::fmax(2.2204460492503131E-16,
                                      constrViolationEq / constrViolationIneq);
        if (Hessian.size(0) > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int idx{0}; idx <= b_nVar; idx++) {
          loop_ub = idx + 1;
          iH0 = (b_nVar + 1) * idx;
          for (int k{0}; k <= loop_ub - 2; k++) {
            Hessian[iH0 + k] = 0.0;
          }
          i = Hessian.size(0);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
          }
          i = Hessian.size(1);
          if (idx + 1 > i) {
            rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
          }
          Hessian[idx + Hessian.size(0) * idx] = constrViolationEq;
          loop_ub = (iH0 + idx) + 1;
          n = (b_nVar - idx) - 1;
          for (int k{0}; k <= n; k++) {
            Hessian[loop_ub + k] = 0.0;
          }
        }
      }
    }
  } while (exitg1 == 0);
  if (checkBoundViolation) {
    n = WorkingSet.sizes[3];
    iH0 = WorkingSet.sizes[4];
    r.set_size(b_TrialState.delta_x.size(0));
    loop_ub = b_TrialState.delta_x.size(0);
    for (i = 0; i < loop_ub; i++) {
      r[i] = b_TrialState.delta_x[i];
    }
    if (lb.size(0) != 0) {
      if (WorkingSet.sizes[3] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < n; idx++) {
        i = WorkingSet.indexLB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
        }
        i = b_TrialState.xstarsqp.size(0);
        if ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i)) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, x_emlrtBCI);
        }
        if ((WorkingSet.indexLB[idx] < 1) ||
            (WorkingSet.indexLB[idx] > r.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, r.size(0),
                               x_emlrtBCI);
        }
        if ((WorkingSet.indexLB[idx] < 1) ||
            (WorkingSet.indexLB[idx] > lb.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, lb.size(0),
                               x_emlrtBCI);
        }
        constrViolationEq = r[WorkingSet.indexLB[idx] - 1];
        constrViolationIneq =
            (b_TrialState.xstarsqp[WorkingSet.indexLB[idx] - 1] +
             constrViolationEq) -
            lb[WorkingSet.indexLB[idx] - 1];
        if (constrViolationIneq < 0.0) {
          if ((WorkingSet.indexLB[idx] < 1) ||
              (WorkingSet.indexLB[idx] > r.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, r.size(0),
                                 x_emlrtBCI);
          }
          if ((WorkingSet.indexLB[idx] < 1) ||
              (WorkingSet.indexLB[idx] > r.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, r.size(0),
                                 x_emlrtBCI);
          }
          r[WorkingSet.indexLB[idx] - 1] =
              constrViolationEq - constrViolationIneq;
          i = b_TrialState.xstar.size(0);
          if ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i)) {
            rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, x_emlrtBCI);
          }
          i = b_TrialState.xstar.size(0);
          if ((WorkingSet.indexLB[idx] < 1) || (WorkingSet.indexLB[idx] > i)) {
            rtDynamicBoundsError(WorkingSet.indexLB[idx], 1, i, x_emlrtBCI);
          }
          b_TrialState.xstar[WorkingSet.indexLB[idx] - 1] =
              b_TrialState.xstar[WorkingSet.indexLB[idx] - 1] -
              constrViolationIneq;
        }
      }
    }
    if (ub.size(0) != 0) {
      if (WorkingSet.sizes[4] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx < iH0; idx++) {
        i = WorkingSet.indexUB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
        }
        if ((WorkingSet.indexUB[idx] < 1) ||
            (WorkingSet.indexUB[idx] > ub.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, ub.size(0),
                               x_emlrtBCI);
        }
        i = b_TrialState.xstarsqp.size(0);
        if ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i)) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, x_emlrtBCI);
        }
        if ((WorkingSet.indexUB[idx] < 1) ||
            (WorkingSet.indexUB[idx] > r.size(0))) {
          rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, r.size(0),
                               x_emlrtBCI);
        }
        constrViolationEq = r[WorkingSet.indexUB[idx] - 1];
        constrViolationIneq =
            (ub[WorkingSet.indexUB[idx] - 1] -
             b_TrialState.xstarsqp[WorkingSet.indexUB[idx] - 1]) -
            constrViolationEq;
        if (constrViolationIneq < 0.0) {
          if ((WorkingSet.indexUB[idx] < 1) ||
              (WorkingSet.indexUB[idx] > r.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, r.size(0),
                                 x_emlrtBCI);
          }
          if ((WorkingSet.indexUB[idx] < 1) ||
              (WorkingSet.indexUB[idx] > r.size(0))) {
            rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, r.size(0),
                                 x_emlrtBCI);
          }
          r[WorkingSet.indexUB[idx] - 1] =
              constrViolationEq + constrViolationIneq;
          i = b_TrialState.xstar.size(0);
          if ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i)) {
            rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, x_emlrtBCI);
          }
          i = b_TrialState.xstar.size(0);
          if ((WorkingSet.indexUB[idx] < 1) || (WorkingSet.indexUB[idx] > i)) {
            rtDynamicBoundsError(WorkingSet.indexUB[idx], 1, i, x_emlrtBCI);
          }
          b_TrialState.xstar[WorkingSet.indexUB[idx] - 1] =
              b_TrialState.xstar[WorkingSet.indexUB[idx] - 1] +
              constrViolationIneq;
        }
      }
    }
    b_TrialState.delta_x.set_size(r.size(0));
    loop_ub = r.size(0);
    for (i = 0; i < loop_ub; i++) {
      b_TrialState.delta_x[i] = r[i];
    }
  }
  return stepSuccess;
}

} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for step.cpp
//
// [EOF]
//
