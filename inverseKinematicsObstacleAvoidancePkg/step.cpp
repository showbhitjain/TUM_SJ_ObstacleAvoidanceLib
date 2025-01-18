//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: step.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "step.h"
#include "addAeqConstr.h"
#include "driver1.h"
#include "inverseKinematicsOA_internal_types.h"
#include "relaxed.h"
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
  array<double, 1U> r;
  k_struct_T b_qpoptions;
  double constrViolationEq;
  double constrViolationIneq;
  int b_nVar;
  int iH0;
  int idxEndIneq;
  int idxStartIneq;
  int nVar;
  boolean_T checkBoundViolation;
  boolean_T stepSuccess;
  stepSuccess = true;
  checkBoundViolation = true;
  nVar = WorkingSet.nVar - 1;
  if (STEP_TYPE != 3) {
    for (int k{0}; k <= nVar; k++) {
      b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
    }
  } else {
    for (int k{0}; k <= nVar; k++) {
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
      iH0 = b_TrialState.grad.size(0);
      for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
        r[b_nVar] = b_TrialState.grad[b_nVar];
      }
      b_qpoptions = qpoptions;
      ::coder::optim::coder::qpactiveset::driver(
          Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
          b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
      if (b_TrialState.state > 0) {
        double constrViolation;
        double penaltyParamTrial;
        iH0 = WorkingSet.sizes[2];
        penaltyParamTrial = b_MeritFunction.penaltyParam;
        constrViolationEq = 0.0;
        if (WorkingSet.sizes[1] >= 1) {
          b_nVar = static_cast<unsigned char>(WorkingSet.sizes[1]);
          for (int k{0}; k < b_nVar; k++) {
            constrViolationEq += std::abs(b_TrialState.cEq.data[k]);
          }
        }
        constrViolationIneq = 0.0;
        for (int idx{0}; idx < iH0; idx++) {
          constrViolation = b_TrialState.cIneq[idx];
          if (constrViolation > 0.0) {
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
        while ((WorkingSet.mEqRemoved > 0) &&
               (WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1] >=
                b_TrialState.iNonEq0)) {
          qpactiveset::WorkingSet::addAeqConstr(
              WorkingSet,
              WorkingSet.indexEqRemoved.data[WorkingSet.mEqRemoved - 1]);
          WorkingSet.mEqRemoved--;
        }
      }
      if ((b_TrialState.state <= 0) && (b_TrialState.state != -6)) {
        STEP_TYPE = 2;
      } else {
        for (int k{0}; k <= nVar; k++) {
          b_TrialState.delta_x[k] = b_TrialState.xstar[k];
        }
        guard1 = true;
      }
    } break;
    case 2: {
      double constrViolation;
      iH0 = WorkingSet.nWConstr[0] + WorkingSet.nWConstr[1];
      idxStartIneq = iH0 + 1;
      idxEndIneq = WorkingSet.nActiveConstr;
      for (b_nVar = idxStartIneq; b_nVar <= idxEndIneq; b_nVar++) {
        WorkingSet.isActiveConstr
            [(WorkingSet.isActiveIdx[WorkingSet.Wid[b_nVar - 1] - 1] +
              WorkingSet.Wlocalidx[b_nVar - 1]) -
             2] = false;
      }
      WorkingSet.nWConstr[2] = 0;
      WorkingSet.nWConstr[3] = 0;
      WorkingSet.nWConstr[4] = 0;
      WorkingSet.nActiveConstr = iH0;
      r.set_size(b_TrialState.xstar.size(0));
      iH0 = b_TrialState.xstar.size(0);
      for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
        r[b_nVar] = b_TrialState.xstar[b_nVar];
      }
      idxStartIneq = WorkingSet.sizes[3] - 1;
      idxEndIneq = WorkingSet.sizes[4] - 1;
      if ((lb.size(0) != 0) && (lb.size(1) != 0)) {
        if ((ub.size(0) == 0) || (ub.size(1) == 0)) {
          for (int idx{0}; idx <= idxStartIneq; idx++) {
            constrViolation = WorkingSet.lb[WorkingSet.indexLB[idx] - 1];
            if (-r[WorkingSet.indexLB[idx] - 1] > constrViolation) {
              r[WorkingSet.indexLB[idx] - 1] =
                  -constrViolation + std::abs(constrViolation);
            }
          }
        } else {
          for (int idx{0}; idx <= idxStartIneq; idx++) {
            constrViolation = WorkingSet.lb[WorkingSet.indexLB[idx] - 1];
            if (-r[WorkingSet.indexLB[idx] - 1] > constrViolation) {
              if (std::isinf(ub[WorkingSet.indexLB[idx] - 1])) {
                r[WorkingSet.indexLB[idx] - 1] =
                    -constrViolation + std::abs(constrViolation);
              } else {
                r[WorkingSet.indexLB[idx] - 1] =
                    (WorkingSet.ub[WorkingSet.indexLB[idx] - 1] -
                     constrViolation) /
                    2.0;
              }
            }
          }
        }
      }
      if ((ub.size(0) != 0) && (ub.size(1) != 0)) {
        if ((lb.size(0) == 0) || (lb.size(1) == 0)) {
          for (int idx{0}; idx <= idxEndIneq; idx++) {
            constrViolation = WorkingSet.ub[WorkingSet.indexUB[idx] - 1];
            if (r[WorkingSet.indexUB[idx] - 1] > constrViolation) {
              r[WorkingSet.indexUB[idx] - 1] =
                  constrViolation - std::abs(constrViolation);
            }
          }
        } else {
          for (int idx{0}; idx <= idxEndIneq; idx++) {
            constrViolation = WorkingSet.ub[WorkingSet.indexUB[idx] - 1];
            if (r[WorkingSet.indexUB[idx] - 1] > constrViolation) {
              if (std::isinf(lb[WorkingSet.indexUB[idx] - 1])) {
                r[WorkingSet.indexUB[idx] - 1] =
                    constrViolation - std::abs(constrViolation);
              } else {
                r[WorkingSet.indexUB[idx] - 1] =
                    (constrViolation -
                     WorkingSet.lb[WorkingSet.indexUB[idx] - 1]) /
                    2.0;
              }
            }
          }
        }
      }
      b_TrialState.xstar.set_size(r.size(0));
      iH0 = r.size(0);
      for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
        b_TrialState.xstar[b_nVar] = r[b_nVar];
      }
      step::relaxed(Hessian, b_TrialState.grad, b_TrialState, b_MeritFunction,
                    memspace, WorkingSet, b_QRManager, b_CholManager,
                    QPObjective, qpoptions);
      for (int k{0}; k <= nVar; k++) {
        b_TrialState.delta_x[k] = b_TrialState.xstar[k];
      }
      guard1 = true;
    } break;
    default:
      r.set_size(b_TrialState.grad.size(0));
      iH0 = b_TrialState.grad.size(0);
      for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
        r[b_nVar] = b_TrialState.grad[b_nVar];
      }
      stepSuccess =
          step::soc(Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
                    b_CholManager, QPObjective, qpoptions);
      checkBoundViolation = stepSuccess;
      if (stepSuccess && (b_TrialState.state != -6)) {
        for (int idx{0}; idx <= nVar; idx++) {
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
        b_nVar = Hessian.size(0) - 1;
        constrViolationEq = 0.0;
        constrViolationIneq = 1.0;
        for (int idx{0}; idx <= b_nVar; idx++) {
          constrViolationEq =
              std::fmax(constrViolationEq, std::abs(b_TrialState.grad[idx]));
          constrViolationIneq =
              std::fmax(constrViolationIneq, std::abs(b_TrialState.xstar[idx]));
        }
        constrViolationEq = std::fmax(2.2204460492503131E-16,
                                      constrViolationEq / constrViolationIneq);
        for (int idx{0}; idx <= b_nVar; idx++) {
          iH0 = (b_nVar + 1) * idx;
          for (int k{0}; k < idx; k++) {
            Hessian[iH0 + k] = 0.0;
          }
          Hessian[idx + Hessian.size(0) * idx] = constrViolationEq;
          idxStartIneq = iH0 + idx;
          idxEndIneq = b_nVar - idx;
          for (int k{0}; k < idxEndIneq; k++) {
            Hessian[(idxStartIneq + k) + 1] = 0.0;
          }
        }
      }
    }
  } while (exitg1 == 0);
  if (checkBoundViolation) {
    idxStartIneq = WorkingSet.sizes[3];
    idxEndIneq = WorkingSet.sizes[4];
    r.set_size(b_TrialState.delta_x.size(0));
    iH0 = b_TrialState.delta_x.size(0);
    for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
      r[b_nVar] = b_TrialState.delta_x[b_nVar];
    }
    if ((lb.size(0) != 0) && (lb.size(1) != 0)) {
      for (int idx{0}; idx < idxStartIneq; idx++) {
        constrViolationEq = r[WorkingSet.indexLB[idx] - 1];
        constrViolationIneq =
            (b_TrialState.xstarsqp[WorkingSet.indexLB[idx] - 1] +
             constrViolationEq) -
            lb[WorkingSet.indexLB[idx] - 1];
        if (constrViolationIneq < 0.0) {
          r[WorkingSet.indexLB[idx] - 1] =
              constrViolationEq - constrViolationIneq;
          b_TrialState.xstar[WorkingSet.indexLB[idx] - 1] =
              b_TrialState.xstar[WorkingSet.indexLB[idx] - 1] -
              constrViolationIneq;
        }
      }
    }
    if ((ub.size(0) != 0) && (ub.size(1) != 0)) {
      for (int idx{0}; idx < idxEndIneq; idx++) {
        constrViolationEq = r[WorkingSet.indexUB[idx] - 1];
        constrViolationIneq =
            (ub[WorkingSet.indexUB[idx] - 1] -
             b_TrialState.xstarsqp[WorkingSet.indexUB[idx] - 1]) -
            constrViolationEq;
        if (constrViolationIneq < 0.0) {
          r[WorkingSet.indexUB[idx] - 1] =
              constrViolationEq + constrViolationIneq;
          b_TrialState.xstar[WorkingSet.indexUB[idx] - 1] =
              b_TrialState.xstar[WorkingSet.indexUB[idx] - 1] +
              constrViolationIneq;
        }
      }
    }
    b_TrialState.delta_x.set_size(r.size(0));
    iH0 = r.size(0);
    for (b_nVar = 0; b_nVar < iH0; b_nVar++) {
      b_TrialState.delta_x[b_nVar] = r[b_nVar];
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
