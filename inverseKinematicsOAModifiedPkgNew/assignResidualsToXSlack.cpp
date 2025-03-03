//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: assignResidualsToXSlack.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "assignResidualsToXSlack.h"
#include "addBoundToActiveSetMatrix_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int nVarOrig
//                d_struct_T &WorkingSet
//                j_struct_T &b_TrialState
//                h_struct_T &memspace
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
namespace relaxed {
void assignResidualsToXSlack(int nVarOrig, d_struct_T &WorkingSet,
                             j_struct_T &b_TrialState, h_struct_T &memspace)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                        // iFirst
      -1,                        // iLast
      1,                         // lineNo
      1,                         // colNo
      "",                        // aName
      "assignResidualsToXSlack", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/+step/"
      "+relaxed/assignResidualsToXSlack.p", // pName
      0                                     // checkKind
  };
  double d;
  int i;
  int k;
  int mEq;
  int mIneq;
  int mLBOrig;
  mIneq = WorkingSet.sizes[2] - 1;
  mEq = WorkingSet.sizes[1] - 1;
  mLBOrig =
      (WorkingSet.sizes[3] - (WorkingSet.sizes[1] << 1)) - WorkingSet.sizes[2];
  if (WorkingSet.sizes[2] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (k = 0; k <= mIneq; k++) {
    memspace.workspace_double[k] = WorkingSet.bineq[k];
  }
  ::coder::internal::blas::xgemv(nVarOrig, WorkingSet.sizes[2],
                                 WorkingSet.Aineq, WorkingSet.ldA,
                                 b_TrialState.xstar, memspace.workspace_double);
  if (WorkingSet.sizes[2] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx <= mIneq; idx++) {
    k = memspace.workspace_double.size(0) * memspace.workspace_double.size(1);
    if ((idx + 1 < 1) || (idx + 1 > k)) {
      rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
    }
    if (idx + 1 > k) {
      rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
    }
    k = b_TrialState.xstar.size(0);
    i = (nVarOrig + idx) + 1;
    if ((i < 1) || (i > k)) {
      rtDynamicBoundsError(i, 1, k, w_emlrtBCI);
    }
    d = memspace.workspace_double[idx];
    b_TrialState.xstar[i - 1] = static_cast<double>(d > 0.0) * d;
  }
  if (WorkingSet.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (k = 0; k <= mEq; k++) {
    memspace.workspace_double[k] = WorkingSet.beq.data[k];
  }
  ::coder::internal::blas::xgemv(nVarOrig, WorkingSet.sizes[1], WorkingSet.Aeq,
                                 WorkingSet.ldA, b_TrialState.xstar,
                                 memspace.workspace_double);
  if (WorkingSet.sizes[1] > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx <= mEq; idx++) {
    int idx_negative;
    int idx_positive;
    idx_positive = (mIneq + idx) + 2;
    idx_negative = ((mIneq + mEq) + idx) + 3;
    k = memspace.workspace_double.size(0) * memspace.workspace_double.size(1);
    if ((idx + 1 < 1) || (idx + 1 > k)) {
      rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
    }
    d = memspace.workspace_double[idx];
    if (d <= 0.0) {
      int i1;
      i = b_TrialState.xstar.size(0);
      i1 = nVarOrig + idx_positive;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      b_TrialState.xstar[i1 - 1] = 0.0;
      if (idx + 1 > k) {
        rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
      }
      i = b_TrialState.xstar.size(0);
      i1 = nVarOrig + idx_negative;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      b_TrialState.xstar[i1 - 1] = -d;
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, mLBOrig + idx_positive);
      if (idx + 1 > k) {
        rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
      }
      if (d >= -1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, mLBOrig + idx_negative);
      }
    } else {
      int i1;
      if (idx + 1 > k) {
        rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
      }
      i = b_TrialState.xstar.size(0);
      i1 = nVarOrig + idx_positive;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      b_TrialState.xstar[i1 - 1] = d;
      i = b_TrialState.xstar.size(0);
      i1 = nVarOrig + idx_negative;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      b_TrialState.xstar[i1 - 1] = 0.0;
      qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
          WorkingSet, mLBOrig + idx_negative);
      if (idx + 1 > k) {
        rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
      }
      if (d <= 1.0E-6) {
        qpactiveset::WorkingSet::addBoundToActiveSetMatrix_(
            WorkingSet, mLBOrig + idx_positive);
      }
    }
  }
}

} // namespace relaxed
} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for assignResidualsToXSlack.cpp
//
// [EOF]
//
