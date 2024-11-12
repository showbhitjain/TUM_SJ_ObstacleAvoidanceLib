//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: step.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "step.h"
#include "addAeqConstr.h"
#include "driver1.h"
#include "inverseKinematics_internal_types.h"
#include "relaxed.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "xnrm2.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int &STEP_TYPE
//                array<double, 2U> &Hessian
//                const array<double, 1U> &lb
//                const array<double, 1U> &ub
//                h_struct_T &b_TrialState
//                struct_T &MeritFunction
//                g_struct_T &memspace
//                i_struct_T &WorkingSet
//                d_struct_T &b_QRManager
//                e_struct_T &b_CholManager
//                f_struct_T &QPObjective
//                j_struct_T &qpoptions
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
boolean_T b_step(int &STEP_TYPE, array<double, 2U> &Hessian,
                 const array<double, 1U> &lb, const array<double, 1U> &ub,
                 h_struct_T &b_TrialState, struct_T &MeritFunction,
                 g_struct_T &memspace, i_struct_T &WorkingSet,
                 d_struct_T &b_QRManager, e_struct_T &b_CholManager,
                 f_struct_T &QPObjective, j_struct_T &qpoptions)
{
  array<double, 1U> r;
  j_struct_T b_qpoptions;
  double c;
  double oldDirIdx;
  int i;
  int iH0;
  int idxStartIneq;
  int iy;
  int nVar_tmp_tmp;
  boolean_T checkBoundViolation;
  boolean_T stepSuccess;
  stepSuccess = true;
  checkBoundViolation = true;
  nVar_tmp_tmp = WorkingSet.nVar - 1;
  if (STEP_TYPE != 3) {
    for (int k{0}; k <= nVar_tmp_tmp; k++) {
      b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
    }
  } else {
    for (int k{0}; k <= nVar_tmp_tmp; k++) {
      b_TrialState.searchDir[k] = b_TrialState.xstar[k];
    }
  }
  int exitg1;
  boolean_T guard1;
  do {
    int mConstrMax;
    int nVar;
    exitg1 = 0;
    guard1 = false;
    switch (STEP_TYPE) {
    case 1: {
      r.set_size(b_TrialState.grad.size(0));
      iH0 = b_TrialState.grad.size(0);
      for (i = 0; i < iH0; i++) {
        r[i] = b_TrialState.grad[i];
      }
      b_qpoptions = qpoptions;
      ::coder::optim::coder::qpactiveset::driver(
          Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
          b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
      if (b_TrialState.state > 0) {
        double constrViolationEq;
        oldDirIdx = MeritFunction.penaltyParam;
        constrViolationEq = 0.0;
        if (WorkingSet.sizes[1] >= 1) {
          i = static_cast<unsigned char>(WorkingSet.sizes[1]);
          for (int k{0}; k < i; k++) {
            constrViolationEq += std::abs(b_TrialState.cEq.data[k]);
          }
        }
        c = MeritFunction.linearizedConstrViol;
        MeritFunction.linearizedConstrViol = 0.0;
        c += constrViolationEq;
        if ((c > 2.2204460492503131E-16) && (b_TrialState.fstar > 0.0)) {
          if (b_TrialState.sqpFval == 0.0) {
            oldDirIdx = 1.0;
          } else {
            oldDirIdx = 1.5;
          }
          oldDirIdx = oldDirIdx * b_TrialState.fstar / c;
        }
        if (oldDirIdx < MeritFunction.penaltyParam) {
          MeritFunction.phi =
              b_TrialState.sqpFval + oldDirIdx * constrViolationEq;
          if ((MeritFunction.initFval +
               oldDirIdx * MeritFunction.initConstrViolationEq) -
                  MeritFunction.phi >
              static_cast<double>(MeritFunction.nPenaltyDecreases) *
                  MeritFunction.threshold) {
            MeritFunction.nPenaltyDecreases++;
            if ((MeritFunction.nPenaltyDecreases << 1) >
                b_TrialState.sqpIterations) {
              MeritFunction.threshold *= 10.0;
            }
            MeritFunction.penaltyParam = std::fmax(oldDirIdx, 1.0E-10);
          } else {
            MeritFunction.phi = b_TrialState.sqpFval +
                                MeritFunction.penaltyParam * constrViolationEq;
          }
        } else {
          MeritFunction.penaltyParam = std::fmax(oldDirIdx, 1.0E-10);
          MeritFunction.phi = b_TrialState.sqpFval +
                              MeritFunction.penaltyParam * constrViolationEq;
        }
        MeritFunction.phiPrimePlus = std::fmin(
            b_TrialState.fstar - MeritFunction.penaltyParam * constrViolationEq,
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
        for (int k{0}; k <= nVar_tmp_tmp; k++) {
          b_TrialState.delta_x[k] = b_TrialState.xstar[k];
        }
        guard1 = true;
      }
    } break;
    case 2:
      iH0 = WorkingSet.nWConstr[0] + WorkingSet.nWConstr[1];
      idxStartIneq = iH0 + 1;
      iy = WorkingSet.nActiveConstr;
      for (nVar = idxStartIneq; nVar <= iy; nVar++) {
        WorkingSet.isActiveConstr
            [(WorkingSet.isActiveIdx[WorkingSet.Wid[nVar - 1] - 1] +
              WorkingSet.Wlocalidx[nVar - 1]) -
             2] = false;
      }
      WorkingSet.nWConstr[2] = 0;
      WorkingSet.nWConstr[3] = 0;
      WorkingSet.nWConstr[4] = 0;
      WorkingSet.nActiveConstr = iH0;
      r.set_size(b_TrialState.xstar.size(0));
      iH0 = b_TrialState.xstar.size(0);
      for (i = 0; i < iH0; i++) {
        r[i] = b_TrialState.xstar[i];
      }
      idxStartIneq = WorkingSet.sizes[3] - 1;
      iy = WorkingSet.sizes[4] - 1;
      if (lb.size(0) != 0) {
        if (ub.size(0) == 0) {
          for (iH0 = 0; iH0 <= idxStartIneq; iH0++) {
            c = WorkingSet.lb[WorkingSet.indexLB[iH0] - 1];
            if (-r[WorkingSet.indexLB[iH0] - 1] > c) {
              r[WorkingSet.indexLB[iH0] - 1] = -c + std::abs(c);
            }
          }
        } else {
          for (iH0 = 0; iH0 <= idxStartIneq; iH0++) {
            c = WorkingSet.lb[WorkingSet.indexLB[iH0] - 1];
            if (-r[WorkingSet.indexLB[iH0] - 1] > c) {
              if (std::isinf(ub[WorkingSet.indexLB[iH0] - 1])) {
                r[WorkingSet.indexLB[iH0] - 1] = -c + std::abs(c);
              } else {
                r[WorkingSet.indexLB[iH0] - 1] =
                    (WorkingSet.ub[WorkingSet.indexLB[iH0] - 1] - c) / 2.0;
              }
            }
          }
        }
      }
      if (ub.size(0) != 0) {
        if (lb.size(0) == 0) {
          for (iH0 = 0; iH0 <= iy; iH0++) {
            c = WorkingSet.ub[WorkingSet.indexUB[iH0] - 1];
            if (r[WorkingSet.indexUB[iH0] - 1] > c) {
              r[WorkingSet.indexUB[iH0] - 1] = c - std::abs(c);
            }
          }
        } else {
          for (iH0 = 0; iH0 <= iy; iH0++) {
            c = WorkingSet.ub[WorkingSet.indexUB[iH0] - 1];
            if (r[WorkingSet.indexUB[iH0] - 1] > c) {
              if (std::isinf(lb[WorkingSet.indexUB[iH0] - 1])) {
                r[WorkingSet.indexUB[iH0] - 1] = c - std::abs(c);
              } else {
                r[WorkingSet.indexUB[iH0] - 1] =
                    (c - WorkingSet.lb[WorkingSet.indexUB[iH0] - 1]) / 2.0;
              }
            }
          }
        }
      }
      b_TrialState.xstar.set_size(r.size(0));
      iH0 = r.size(0);
      for (i = 0; i < iH0; i++) {
        b_TrialState.xstar[i] = r[i];
      }
      step::relaxed(Hessian, b_TrialState.grad, b_TrialState, MeritFunction,
                    memspace, WorkingSet, b_QRManager, b_CholManager,
                    QPObjective, qpoptions);
      for (int k{0}; k <= nVar_tmp_tmp; k++) {
        b_TrialState.delta_x[k] = b_TrialState.xstar[k];
      }
      guard1 = true;
      break;
    default: {
      int i1;
      i = WorkingSet.nVar - 1;
      mConstrMax = WorkingSet.mConstrMax - 1;
      for (int k{0}; k <= i; k++) {
        b_TrialState.xstarsqp[k] = b_TrialState.xstarsqp_old[k];
        b_TrialState.socDirection[k] = b_TrialState.xstar[k];
      }
      for (int k{0}; k <= mConstrMax; k++) {
        b_TrialState.lambdaStopTest[k] = b_TrialState.lambda[k];
      }
      if (WorkingSet.sizes[1] > 0) {
        i1 = static_cast<unsigned char>(WorkingSet.sizes[1]);
        for (iH0 = 0; iH0 < i1; iH0++) {
          WorkingSet.beq.data[iH0] = -b_TrialState.cEq.data[iH0];
        }
        idxStartIneq = WorkingSet.ldA;
        if (WorkingSet.nVar != 0) {
          iy = 0;
          iH0 = WorkingSet.ldA * (WorkingSet.sizes[1] - 1) + 1;
          for (int k{1}; idxStartIneq < 0 ? k >= iH0 : k <= iH0;
               k += idxStartIneq) {
            c = 0.0;
            nVar = (k + WorkingSet.nVar) - 1;
            for (int ia{k}; ia <= nVar; ia++) {
              c += WorkingSet.Aeq[ia - 1] * b_TrialState.searchDir[ia - k];
            }
            WorkingSet.beq.data[iy] += c;
            iy++;
          }
        }
        for (int k{0}; k < i1; k++) {
          WorkingSet.bwset[WorkingSet.sizes[0] + k] = WorkingSet.beq.data[k];
        }
      }
      for (int k{0}; k <= i; k++) {
        b_TrialState.xstar[k] = b_TrialState.xstarsqp[k];
      }
      r.set_size(b_TrialState.grad.size(0));
      iH0 = b_TrialState.grad.size(0);
      for (i1 = 0; i1 < iH0; i1++) {
        r[i1] = b_TrialState.grad[i1];
      }
      b_qpoptions = qpoptions;
      ::coder::optim::coder::qpactiveset::driver(
          Hessian, r, b_TrialState, memspace, WorkingSet, b_QRManager,
          b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
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
      for (iH0 = 0; iH0 <= i; iH0++) {
        c = b_TrialState.socDirection[iH0];
        oldDirIdx = c;
        c = b_TrialState.xstar[iH0] - c;
        b_TrialState.socDirection[iH0] = c;
        b_TrialState.xstar[iH0] = oldDirIdx;
      }
      stepSuccess =
          (::coder::internal::blas::xnrm2(i + 1, b_TrialState.socDirection) <=
           2.0 * ::coder::internal::blas::xnrm2(i + 1, b_TrialState.xstar));
      if (WorkingSet.sizes[1] > 0) {
        i = static_cast<unsigned char>(WorkingSet.sizes[1]);
        for (iH0 = 0; iH0 < i; iH0++) {
          WorkingSet.beq.data[iH0] = -b_TrialState.cEq.data[iH0];
          WorkingSet.bwset[WorkingSet.sizes[0] + iH0] =
              WorkingSet.beq.data[iH0];
        }
      }
      if (!stepSuccess) {
        for (int k{0}; k <= mConstrMax; k++) {
          b_TrialState.lambda[k] = b_TrialState.lambdaStopTest[k];
        }
      } else {
        qpactiveset::parseoutput::sortLambdaQP(
            b_TrialState.lambda, WorkingSet.nActiveConstr, WorkingSet.sizes,
            WorkingSet.isActiveIdx, WorkingSet.Wid, WorkingSet.Wlocalidx,
            memspace.workspace_double);
      }
      checkBoundViolation = stepSuccess;
      if (stepSuccess && (b_TrialState.state != -6)) {
        for (iH0 = 0; iH0 <= nVar_tmp_tmp; iH0++) {
          b_TrialState.delta_x[iH0] =
              b_TrialState.xstar[iH0] + b_TrialState.socDirection[iH0];
        }
      }
      guard1 = true;
    } break;
    }
    if (guard1) {
      if (b_TrialState.state != -6) {
        exitg1 = 1;
      } else {
        nVar = Hessian.size(0) - 1;
        c = 0.0;
        oldDirIdx = 1.0;
        for (iH0 = 0; iH0 <= nVar; iH0++) {
          c = std::fmax(c, std::abs(b_TrialState.grad[iH0]));
          oldDirIdx = std::fmax(oldDirIdx, std::abs(b_TrialState.xstar[iH0]));
        }
        c = std::fmax(2.2204460492503131E-16, c / oldDirIdx);
        for (mConstrMax = 0; mConstrMax <= nVar; mConstrMax++) {
          iH0 = (nVar + 1) * mConstrMax;
          for (int k{0}; k < mConstrMax; k++) {
            Hessian[iH0 + k] = 0.0;
          }
          Hessian[mConstrMax + Hessian.size(0) * mConstrMax] = c;
          idxStartIneq = iH0 + mConstrMax;
          iy = nVar - mConstrMax;
          for (int k{0}; k < iy; k++) {
            Hessian[(idxStartIneq + k) + 1] = 0.0;
          }
        }
      }
    }
  } while (exitg1 == 0);
  if (checkBoundViolation) {
    idxStartIneq = WorkingSet.sizes[3];
    iy = WorkingSet.sizes[4];
    r.set_size(b_TrialState.delta_x.size(0));
    iH0 = b_TrialState.delta_x.size(0);
    for (i = 0; i < iH0; i++) {
      r[i] = b_TrialState.delta_x[i];
    }
    if (lb.size(0) != 0) {
      for (iH0 = 0; iH0 < idxStartIneq; iH0++) {
        c = r[WorkingSet.indexLB[iH0] - 1];
        oldDirIdx = (b_TrialState.xstarsqp[WorkingSet.indexLB[iH0] - 1] + c) -
                    lb[WorkingSet.indexLB[iH0] - 1];
        if (oldDirIdx < 0.0) {
          r[WorkingSet.indexLB[iH0] - 1] = c - oldDirIdx;
          b_TrialState.xstar[WorkingSet.indexLB[iH0] - 1] =
              b_TrialState.xstar[WorkingSet.indexLB[iH0] - 1] - oldDirIdx;
        }
      }
    }
    if (ub.size(0) != 0) {
      for (iH0 = 0; iH0 < iy; iH0++) {
        c = r[WorkingSet.indexUB[iH0] - 1];
        oldDirIdx = (ub[WorkingSet.indexUB[iH0] - 1] -
                     b_TrialState.xstarsqp[WorkingSet.indexUB[iH0] - 1]) -
                    c;
        if (oldDirIdx < 0.0) {
          r[WorkingSet.indexUB[iH0] - 1] = c + oldDirIdx;
          b_TrialState.xstar[WorkingSet.indexUB[iH0] - 1] =
              b_TrialState.xstar[WorkingSet.indexUB[iH0] - 1] + oldDirIdx;
        }
      }
    }
    b_TrialState.delta_x.set_size(r.size(0));
    iH0 = r.size(0);
    for (i = 0; i < iH0; i++) {
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
