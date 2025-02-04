//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: relaxed.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "relaxed.h"
#include "assignResidualsToXSlack.h"
#include "driver1.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "ixamax.h"
#include "modifyOverheadRegularized_.h"
#include "rt_nonfinite.h"
#include "sortLambdaQP.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &Hessian
//                const array<double, 1U> &grad
//                j_struct_T &b_TrialState
//                struct_T &b_MeritFunction
//                h_struct_T &memspace
//                d_struct_T &WorkingSet
//                e_struct_T &b_QRManager
//                f_struct_T &b_CholManager
//                g_struct_T &QPObjective
//                k_struct_T &qpoptions
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
void b_relaxed(const array<double, 2U> &Hessian, const array<double, 1U> &grad,
               j_struct_T &b_TrialState, struct_T &b_MeritFunction,
               h_struct_T &memspace, d_struct_T &WorkingSet,
               e_struct_T &b_QRManager, f_struct_T &b_CholManager,
               g_struct_T &QPObjective, k_struct_T &qpoptions)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                           // iFirst
      -1,                           // iLast
      1,                            // lineNo
      1,                            // colNo
      "",                           // aName
      "findActiveSlackLowerBounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "+relaxed/findActiveSlackLowerBounds.p", // pName
      0                                        // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,        // iFirst
      -1,        // iLast
      1,         // lineNo
      1,         // colNo
      "",        // aName
      "relaxed", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "relaxed.p", // pName
      0            // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                             // iFirst
      -1,                             // iLast
      1,                              // lineNo
      1,                              // colNo
      "",                             // aName
      "removeActiveSlackLowerBounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "+relaxed/removeActiveSlackLowerBounds.p", // pName
      0                                          // checkKind
  };
  double beta;
  double rho;
  int b_mEq;
  int i;
  int i1;
  int i2;
  int idx;
  int idx_max;
  int mEq;
  int mFiniteLB;
  int mIneq;
  int nActiveLBArtificial;
  int nArtificial;
  int nArtificial_tmp_tmp_tmp;
  int nVarOrig_tmp_tmp;
  boolean_T tf;
  nVarOrig_tmp_tmp = WorkingSet.nVar;
  mIneq = WorkingSet.sizes[2];
  mEq = WorkingSet.sizes[1];
  beta = 0.0;
  if (WorkingSet.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx = 0; idx < nVarOrig_tmp_tmp; idx++) {
    i = Hessian.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
    }
    i = Hessian.size(1);
    if (idx + 1 > i) {
      rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
    }
    beta += Hessian[idx + Hessian.size(0) * idx];
  }
  beta /= static_cast<double>(WorkingSet.nVar);
  if (b_TrialState.sqpIterations <= 1) {
    idx_max = ::coder::internal::blas::ixamax(QPObjective.nvar, grad);
    if ((idx_max < 1) || (idx_max > grad.size(0))) {
      rtDynamicBoundsError(idx_max, 1, grad.size(0), x_emlrtBCI);
    }
    rho = 100.0 * std::fmax(1.0, std::abs(grad[idx_max - 1]));
  } else {
    idx_max = ::coder::internal::blas::ixamax(WorkingSet.mConstr,
                                              b_TrialState.lambdasqp);
    i = b_TrialState.lambdasqp.size(0);
    if ((idx_max < 1) || (idx_max > i)) {
      rtDynamicBoundsError(idx_max, 1, i, x_emlrtBCI);
    }
    rho = std::abs(b_TrialState.lambdasqp[idx_max - 1]);
  }
  QPObjective.nvar = WorkingSet.nVar;
  QPObjective.beta = beta;
  QPObjective.rho = rho;
  QPObjective.hasLinear = true;
  QPObjective.objtype = 4;
  WorkingSet.nVar = WorkingSet.nVarMax - 1;
  WorkingSet.mConstr = WorkingSet.mConstrMax - 1;
  for (i = 0; i < 5; i++) {
    WorkingSet.sizes[i] = WorkingSet.sizesRegularized[i];
  }
  if (WorkingSet.probType != 4) {
    qpactiveset::WorkingSet::modifyOverheadRegularized_(WorkingSet);
  }
  for (i = 0; i < 6; i++) {
    WorkingSet.isActiveIdx[i] = WorkingSet.isActiveIdxRegularized[i];
  }
  k_struct_T b_qpoptions;
  WorkingSet.probType = 2;
  relaxed::assignResidualsToXSlack(nVarOrig_tmp_tmp, WorkingSet, b_TrialState,
                                   memspace);
  idx_max = qpoptions.MaxIterations;
  qpoptions.MaxIterations =
      (qpoptions.MaxIterations + WorkingSet.nVar) - nVarOrig_tmp_tmp;
  b_qpoptions = qpoptions;
  ::coder::optim::coder::qpactiveset::driver(
      Hessian, grad, b_TrialState, memspace, WorkingSet, b_QRManager,
      b_CholManager, QPObjective, b_qpoptions, qpoptions.MaxIterations);
  qpoptions.MaxIterations = idx_max;
  idx_max = WorkingSet.sizes[2];
  b_mEq = WorkingSet.sizes[1];
  nArtificial_tmp_tmp_tmp = WorkingSet.sizes[1] << 1;
  nArtificial = (nArtificial_tmp_tmp_tmp + WorkingSet.sizes[2]) - 1;
  mFiniteLB = WorkingSet.sizes[3] - 1;
  nActiveLBArtificial = 0;
  if (WorkingSet.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx = 0; idx < b_mEq; idx++) {
    boolean_T b_tf;
    i = WorkingSet.isActiveConstr.size(0);
    i1 = WorkingSet.isActiveIdx[3] + mFiniteLB;
    i2 = ((i1 - (b_mEq << 1)) + idx) + 1;
    if ((i2 < 1) || (i2 > i)) {
      rtDynamicBoundsError(i2, 1, i, g_emlrtBCI);
    }
    tf = WorkingSet.isActiveConstr[i2 - 1];
    i = WorkingSet.isActiveConstr.size(0);
    i1 = ((i1 - b_mEq) + idx) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, g_emlrtBCI);
    }
    b_tf = WorkingSet.isActiveConstr[i1 - 1];
    i = memspace.workspace_int.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    memspace.workspace_int[idx] = tf;
    i = memspace.workspace_int.size(0);
    i1 = (idx + b_mEq) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    memspace.workspace_int[i1 - 1] = b_tf;
    nActiveLBArtificial = (nActiveLBArtificial + tf) + b_tf;
  }
  if (WorkingSet.sizes[2] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx = 0; idx < idx_max; idx++) {
    i = WorkingSet.isActiveConstr.size(0);
    i1 = ((WorkingSet.isActiveIdx[3] + mFiniteLB) - nArtificial) + idx;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, g_emlrtBCI);
    }
    tf = WorkingSet.isActiveConstr[i1 - 1];
    i = memspace.workspace_int.size(0);
    i1 = (idx + nArtificial_tmp_tmp_tmp) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
    }
    memspace.workspace_int[i1 - 1] = tf;
    nActiveLBArtificial += tf;
  }
  if (b_TrialState.state != -6) {
    double penaltyParamTrial;
    double qpfvalLinearExcess;
    double qpfvalQuadExcess;
    b_mEq = (WorkingSet.nVarMax - nVarOrig_tmp_tmp) - 1;
    nArtificial = nVarOrig_tmp_tmp + 1;
    qpfvalLinearExcess = 0.0;
    if (b_mEq >= 1) {
      idx_max = nVarOrig_tmp_tmp + b_mEq;
      if ((nVarOrig_tmp_tmp + 1 <= idx_max) && (idx_max > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (mFiniteLB = nArtificial; mFiniteLB <= idx_max; mFiniteLB++) {
        qpfvalLinearExcess += std::abs(b_TrialState.xstar[mFiniteLB - 1]);
      }
    }
    qpfvalQuadExcess = 0.0;
    if (b_mEq >= 1) {
      if (b_mEq > 2147483646) {
        check_forloop_overflow_error();
      }
      for (mFiniteLB = 0; mFiniteLB < b_mEq; mFiniteLB++) {
        idx_max = nVarOrig_tmp_tmp + mFiniteLB;
        qpfvalQuadExcess +=
            b_TrialState.xstar[idx_max] * b_TrialState.xstar[idx_max];
      }
    }
    qpfvalQuadExcess = (b_TrialState.fstar - rho * qpfvalLinearExcess) -
                       beta / 2.0 * qpfvalQuadExcess;
    penaltyParamTrial = b_MeritFunction.penaltyParam;
    beta = 0.0;
    if (mEq >= 1) {
      if (mEq > 2147483646) {
        check_forloop_overflow_error();
      }
      i = static_cast<unsigned char>(mEq);
      for (mFiniteLB = 0; mFiniteLB < i; mFiniteLB++) {
        beta += std::abs(b_TrialState.cEq.data[mFiniteLB]);
      }
    }
    rho = 0.0;
    if (mIneq > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < mIneq; idx++) {
      i = b_TrialState.cIneq.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, b_emlrtBCI);
      }
      qpfvalLinearExcess = b_TrialState.cIneq[idx];
      if (qpfvalLinearExcess > 0.0) {
        i = b_TrialState.cIneq.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, b_emlrtBCI);
        }
        rho += qpfvalLinearExcess;
      }
    }
    qpfvalLinearExcess = beta + rho;
    beta = b_MeritFunction.linearizedConstrViol;
    rho = 0.0;
    if (b_mEq >= 1) {
      idx_max = nVarOrig_tmp_tmp + b_mEq;
      if ((nVarOrig_tmp_tmp + 1 <= idx_max) && (idx_max > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (mFiniteLB = nArtificial; mFiniteLB <= idx_max; mFiniteLB++) {
        rho += std::abs(b_TrialState.xstar[mFiniteLB - 1]);
      }
    }
    b_MeritFunction.linearizedConstrViol = rho;
    beta = (qpfvalLinearExcess + beta) - rho;
    if ((beta > 2.2204460492503131E-16) && (qpfvalQuadExcess > 0.0)) {
      if (b_TrialState.sqpFval == 0.0) {
        rho = 1.0;
      } else {
        rho = 1.5;
      }
      penaltyParamTrial = rho * qpfvalQuadExcess / beta;
    }
    if (penaltyParamTrial < b_MeritFunction.penaltyParam) {
      b_MeritFunction.phi =
          b_TrialState.sqpFval + penaltyParamTrial * qpfvalLinearExcess;
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
        b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
      } else {
        b_MeritFunction.phi = b_TrialState.sqpFval +
                              b_MeritFunction.penaltyParam * qpfvalLinearExcess;
      }
    } else {
      b_MeritFunction.penaltyParam = std::fmax(penaltyParamTrial, 1.0E-10);
      b_MeritFunction.phi = b_TrialState.sqpFval +
                            b_MeritFunction.penaltyParam * qpfvalLinearExcess;
    }
    b_MeritFunction.phiPrimePlus = std::fmin(
        qpfvalQuadExcess - b_MeritFunction.penaltyParam * qpfvalLinearExcess,
        0.0);
    idx_max = WorkingSet.isActiveIdx[1] - 1;
    for (idx = 0; idx < mEq; idx++) {
      i = memspace.workspace_int.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
      }
      if (memspace.workspace_int[idx] != 0) {
        i = memspace.workspace_int.size(0);
        i1 = (idx + mEq) + 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, x_emlrtBCI);
        }
        if (memspace.workspace_int[i1 - 1] != 0) {
          tf = true;
        } else {
          tf = false;
        }
      } else {
        tf = false;
      }
      i = b_TrialState.lambda.size(0);
      i1 = (idx_max + idx) + 1;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, x_emlrtBCI);
      }
      i = b_TrialState.lambda.size(0);
      if (i1 > i) {
        rtDynamicBoundsError(i1, 1, i, x_emlrtBCI);
      }
      b_TrialState.lambda[i1 - 1] =
          static_cast<double>(tf) * b_TrialState.lambda[i1 - 1];
    }
    idx_max = WorkingSet.isActiveIdx[2];
    b_mEq = WorkingSet.nActiveConstr;
    if ((WorkingSet.isActiveIdx[2] <= WorkingSet.nActiveConstr) &&
        (WorkingSet.nActiveConstr > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (idx = idx_max; idx <= b_mEq; idx++) {
      i = WorkingSet.Wlocalidx.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
      }
      i = WorkingSet.Wid.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
      }
      if (WorkingSet.Wid[idx - 1] == 3) {
        i = memspace.workspace_int.size(0);
        i1 = WorkingSet.Wlocalidx[idx - 1] + (mEq << 1);
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, x_emlrtBCI);
        }
        i = b_TrialState.lambda.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
        }
        i = b_TrialState.lambda.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, x_emlrtBCI);
        }
        b_TrialState.lambda[idx - 1] =
            static_cast<double>(memspace.workspace_int[i1 - 1]) *
            b_TrialState.lambda[idx - 1];
      }
    }
  }
  idx_max =
      (WorkingSet.sizes[3] - nArtificial_tmp_tmp_tmp) - WorkingSet.sizes[2];
  idx = WorkingSet.nActiveConstr;
  while ((idx > WorkingSet.sizes[0] + WorkingSet.sizes[1]) &&
         (nActiveLBArtificial > 0)) {
    i = WorkingSet.Wid.size(0);
    if ((idx < 1) || (idx > i)) {
      rtDynamicBoundsError(idx, 1, i, y_emlrtBCI);
    }
    i = WorkingSet.Wid[idx - 1];
    if (i == 4) {
      i = WorkingSet.Wlocalidx.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, y_emlrtBCI);
      }
      i = WorkingSet.Wlocalidx[idx - 1];
      if (i > idx_max) {
        i1 = b_TrialState.lambda.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i1)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i1, y_emlrtBCI);
        }
        nArtificial = WorkingSet.nActiveConstr - 1;
        beta = b_TrialState.lambda[nArtificial];
        i1 = b_TrialState.lambda.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i1)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i1, y_emlrtBCI);
        }
        b_TrialState.lambda[nArtificial] = 0.0;
        i1 = b_TrialState.lambda.size(0);
        if (idx > i1) {
          rtDynamicBoundsError(idx, 1, i1, y_emlrtBCI);
        }
        b_TrialState.lambda[idx - 1] = beta;
        i1 = WorkingSet.Wid.size(0);
        if (idx > i1) {
          rtDynamicBoundsError(idx, 1, i1, h_emlrtBCI);
        }
        i1 = WorkingSet.Wlocalidx.size(0);
        if (idx > i1) {
          rtDynamicBoundsError(idx, 1, i1, h_emlrtBCI);
        }
        i1 = WorkingSet.isActiveConstr.size(0);
        i = (WorkingSet.isActiveIdx[3] + i) - 1;
        if ((i < 1) || (i > i1)) {
          rtDynamicBoundsError(i, 1, i1, h_emlrtBCI);
        }
        WorkingSet.isActiveConstr[i - 1] = false;
        i = WorkingSet.Wid.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i, j_emlrtBCI);
        }
        i = WorkingSet.Wid.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, j_emlrtBCI);
        }
        WorkingSet.Wid[idx - 1] = WorkingSet.Wid[nArtificial];
        i = WorkingSet.Wlocalidx.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i, j_emlrtBCI);
        }
        i = WorkingSet.Wlocalidx.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, j_emlrtBCI);
        }
        WorkingSet.Wlocalidx[idx - 1] = WorkingSet.Wlocalidx[nArtificial];
        b_mEq = WorkingSet.nVar;
        if (WorkingSet.nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (mFiniteLB = 0; mFiniteLB < b_mEq; mFiniteLB++) {
          i = WorkingSet.ATwset.size(0);
          i1 = (mFiniteLB + WorkingSet.ldA * nArtificial) + 1;
          if ((i1 < 1) || (i1 > i)) {
            rtDynamicBoundsError(i1, 1, i, j_emlrtBCI);
          }
          i = WorkingSet.ATwset.size(0);
          i2 = (mFiniteLB + WorkingSet.ldA * (idx - 1)) + 1;
          if ((i2 < 1) || (i2 > i)) {
            rtDynamicBoundsError(i2, 1, i, j_emlrtBCI);
          }
          WorkingSet.ATwset[i2 - 1] = WorkingSet.ATwset[i1 - 1];
        }
        i = WorkingSet.bwset.size(0);
        if ((WorkingSet.nActiveConstr < 1) || (WorkingSet.nActiveConstr > i)) {
          rtDynamicBoundsError(WorkingSet.nActiveConstr, 1, i, j_emlrtBCI);
        }
        i = WorkingSet.bwset.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, j_emlrtBCI);
        }
        WorkingSet.bwset[idx - 1] = WorkingSet.bwset[nArtificial];
        WorkingSet.nActiveConstr = nArtificial;
        WorkingSet.nWConstr[3]--;
        nActiveLBArtificial--;
      }
    }
    idx--;
  }
  QPObjective.nvar = nVarOrig_tmp_tmp;
  QPObjective.hasLinear = true;
  QPObjective.objtype = 3;
  WorkingSet.nVar = WorkingSet.nVarOrig;
  WorkingSet.mConstr = WorkingSet.mConstrOrig;
  if (WorkingSet.nWConstr[4] > 0) {
    b_mEq = WorkingSet.sizesNormal[4];
    if (WorkingSet.sizesNormal[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < b_mEq; idx++) {
      i = WorkingSet.isActiveConstr.size(0);
      i1 = WorkingSet.isActiveIdx[4] + idx;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, d_emlrtBCI);
      }
      i = WorkingSet.isActiveConstr.size(0);
      i2 = WorkingSet.isActiveIdxNormal[4] + idx;
      if ((i2 < 1) || (i2 > i)) {
        rtDynamicBoundsError(i2, 1, i, d_emlrtBCI);
      }
      WorkingSet.isActiveConstr[i2 - 1] = WorkingSet.isActiveConstr[i1 - 1];
    }
  }
  for (i = 0; i < 5; i++) {
    WorkingSet.sizes[i] = WorkingSet.sizesNormal[i];
  }
  for (i = 0; i < 6; i++) {
    WorkingSet.isActiveIdx[i] = WorkingSet.isActiveIdxNormal[i];
  }
  WorkingSet.probType = 3;
  qpactiveset::parseoutput::sortLambdaQP(
      b_TrialState.lambda, WorkingSet.nActiveConstr, WorkingSet.sizes,
      WorkingSet.isActiveIdx, WorkingSet.Wid, WorkingSet.Wlocalidx,
      memspace.workspace_double);
}

} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for relaxed.cpp
//
// [EOF]
//
