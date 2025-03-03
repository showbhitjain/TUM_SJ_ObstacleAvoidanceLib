//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: maxConstraintViolation.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "maxConstraintViolation.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo n_emlrtBCI{
    -1,                                          // iFirst
    -1,                                          // iLast
    1,                                           // lineNo
    1,                                           // colNo
    "",                                          // aName
    "maxConstraintViolation_AMats_regularized_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/maxConstraintViolation_AMats_regularized_.p", // pName
    0                                                          // checkKind
};

static rtBoundsCheckInfo o_emlrtBCI{
    -1,                                             // iFirst
    -1,                                             // iLast
    1,                                              // lineNo
    1,                                              // colNo
    "",                                             // aName
    "maxConstraintViolation_AMats_nonregularized_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/maxConstraintViolation_AMats_nonregularized_.p", // pName
    0                                                             // checkKind
};

static rtBoundsCheckInfo p_emlrtBCI{
    -1,                       // iFirst
    -1,                       // iLast
    1,                        // lineNo
    1,                        // colNo
    "",                       // aName
    "maxConstraintViolation", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/maxConstraintViolation.p", // pName
    0                                       // checkKind
};

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                const array<double, 2U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
double maxConstraintViolation(d_struct_T &obj, const array<double, 2U> &x)
{
  double v;
  int i;
  int i1;
  int i2;
  int mFixed;
  int mLB;
  int mUB;
  int offsetEq1;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = x.size(0) * x.size(1);
        i1 = (obj.nVarOrig + idx) + 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, n_emlrtBCI);
        }
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] = obj.maxConstrWorkspace[idx] - x[i1 - 1];
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      offsetEq1 = obj.nVarOrig + obj.sizes[2];
      mIneq = offsetEq1 + obj.sizes[1];
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = x.size(0) * x.size(1);
        i1 = (offsetEq1 + idx) + 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, n_emlrtBCI);
        }
        i2 = (mIneq + idx) + 1;
        if ((i2 < 1) || (i2 > i)) {
          rtDynamicBoundsError(i2, 1, i, n_emlrtBCI);
        }
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] =
            (obj.maxConstrWorkspace[idx] - x[i1 - 1]) + x[i2 - 1];
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  } else {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  }
  if (obj.sizes[3] > 0) {
    if (obj.sizes[3] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = x.size(0) * x.size(1);
    i1 = obj.indexLB.size(0);
    i2 = obj.lb.size(0);
    for (int idx{0}; idx < mLB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i1)) {
        rtDynamicBoundsError(idx + 1, 1, i1, p_emlrtBCI);
      }
      offsetEq1 = obj.indexLB[idx] - 1;
      if ((obj.indexLB[idx] < 1) || (obj.indexLB[idx] > i)) {
        rtDynamicBoundsError(obj.indexLB[idx], 1, i, p_emlrtBCI);
      }
      if ((obj.indexLB[idx] < 1) || (obj.indexLB[idx] > i2)) {
        rtDynamicBoundsError(obj.indexLB[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(v, -x[offsetEq1] - obj.lb[offsetEq1]);
    }
  }
  if (obj.sizes[4] > 0) {
    if (obj.sizes[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = x.size(0) * x.size(1);
    i1 = obj.indexUB.size(0);
    i2 = obj.ub.size(0);
    for (int idx{0}; idx < mUB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i1)) {
        rtDynamicBoundsError(idx + 1, 1, i1, p_emlrtBCI);
      }
      offsetEq1 = obj.indexUB[idx] - 1;
      if ((obj.indexUB[idx] < 1) || (obj.indexUB[idx] > i)) {
        rtDynamicBoundsError(obj.indexUB[idx], 1, i, p_emlrtBCI);
      }
      if ((obj.indexUB[idx] < 1) || (obj.indexUB[idx] > i2)) {
        rtDynamicBoundsError(obj.indexUB[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(v, x[offsetEq1] - obj.ub[offsetEq1]);
    }
  }
  if (obj.sizes[0] > 0) {
    if (obj.sizes[0] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = x.size(0) * x.size(1);
    i1 = obj.indexFixed.size(0);
    i2 = obj.ub.size(0);
    for (int idx{0}; idx < mFixed; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i1)) {
        rtDynamicBoundsError(idx + 1, 1, i1, p_emlrtBCI);
      }
      if ((obj.indexFixed[idx] < 1) || (obj.indexFixed[idx] > i)) {
        rtDynamicBoundsError(obj.indexFixed[idx], 1, i, p_emlrtBCI);
      }
      if ((obj.indexFixed[idx] < 1) || (obj.indexFixed[idx] > i2)) {
        rtDynamicBoundsError(obj.indexFixed[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(v, std::abs(x[obj.indexFixed[idx] - 1] -
                                obj.ub[obj.indexFixed[idx] - 1]));
    }
  }
  return v;
}

//
// Arguments    : d_struct_T &obj
//                const array<double, 2U> &x
//                int ix0
// Return Type  : double
//
double maxConstraintViolation(d_struct_T &obj, const array<double, 2U> &x,
                              int ix0)
{
  double v;
  int i;
  int i1;
  int i2;
  int mFixed;
  int mLB;
  int mUB;
  int offsetEq1;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = x.size(0) * x.size(1);
        i1 = (ix0 + obj.nVarOrig) + idx;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, n_emlrtBCI);
        }
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] = obj.maxConstrWorkspace[idx] - x[i1 - 1];
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      offsetEq1 = (obj.nVarOrig + obj.sizes[2]) - 1;
      mIneq = offsetEq1 + obj.sizes[1];
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = x.size(0) * x.size(1);
        i1 = ((ix0 + offsetEq1) + idx) + 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, n_emlrtBCI);
        }
        i2 = ((ix0 + mIneq) + idx) + 1;
        if ((i2 < 1) || (i2 > i)) {
          rtDynamicBoundsError(i2, 1, i, n_emlrtBCI);
        }
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] =
            (obj.maxConstrWorkspace[idx] - x[i1 - 1]) + x[i2 - 1];
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  } else {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  }
  if (obj.sizes[3] > 0) {
    if (obj.sizes[3] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexLB.size(0);
    i1 = x.size(0) * x.size(1);
    i2 = obj.lb.size(0);
    for (int idx{0}; idx < mLB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      offsetEq1 = (ix0 + obj.indexLB[idx]) - 1;
      if ((offsetEq1 < 1) || (offsetEq1 > i1)) {
        rtDynamicBoundsError(offsetEq1, 1, i1, p_emlrtBCI);
      }
      if ((obj.indexLB[idx] < 1) || (obj.indexLB[idx] > i2)) {
        rtDynamicBoundsError(obj.indexLB[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(v, -x[offsetEq1 - 1] - obj.lb[obj.indexLB[idx] - 1]);
    }
  }
  if (obj.sizes[4] > 0) {
    if (obj.sizes[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexUB.size(0);
    i1 = x.size(0) * x.size(1);
    i2 = obj.ub.size(0);
    for (int idx{0}; idx < mUB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      offsetEq1 = (ix0 + obj.indexUB[idx]) - 1;
      if ((offsetEq1 < 1) || (offsetEq1 > i1)) {
        rtDynamicBoundsError(offsetEq1, 1, i1, p_emlrtBCI);
      }
      if ((obj.indexUB[idx] < 1) || (obj.indexUB[idx] > i2)) {
        rtDynamicBoundsError(obj.indexUB[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(v, x[offsetEq1 - 1] - obj.ub[obj.indexUB[idx] - 1]);
    }
  }
  if (obj.sizes[0] > 0) {
    if (obj.sizes[0] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexFixed.size(0);
    i1 = x.size(0) * x.size(1);
    i2 = obj.ub.size(0);
    for (int idx{0}; idx < mFixed; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      offsetEq1 = (ix0 + obj.indexFixed[idx]) - 1;
      if ((offsetEq1 < 1) || (offsetEq1 > i1)) {
        rtDynamicBoundsError(offsetEq1, 1, i1, p_emlrtBCI);
      }
      if ((obj.indexFixed[idx] < 1) || (obj.indexFixed[idx] > i2)) {
        rtDynamicBoundsError(obj.indexFixed[idx], 1, i2, p_emlrtBCI);
      }
      v = std::fmax(
          v, std::abs(x[offsetEq1 - 1] - obj.ub[obj.indexFixed[idx] - 1]));
    }
  }
  return v;
}

//
// Arguments    : d_struct_T &obj
//                const array<double, 1U> &x
// Return Type  : double
//
double maxConstraintViolation(d_struct_T &obj, const array<double, 1U> &x)
{
  double v;
  int i;
  int i1;
  int mFixed;
  int mLB;
  int mUB;
  int offsetEq1;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = (obj.nVarOrig + idx) + 1;
        if ((i < 1) || (i > x.size(0))) {
          rtDynamicBoundsError(i, 1, x.size(0), n_emlrtBCI);
        }
        i1 = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i1) {
          rtDynamicBoundsError(idx + 1, 1, i1, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] = obj.maxConstrWorkspace[idx] - x[i - 1];
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      offsetEq1 = obj.nVarOrig + obj.sizes[2];
      mIneq = offsetEq1 + obj.sizes[1];
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        int i2;
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        i = (offsetEq1 + idx) + 1;
        if ((i < 1) || (i > x.size(0))) {
          rtDynamicBoundsError(i, 1, x.size(0), n_emlrtBCI);
        }
        i1 = (mIneq + idx) + 1;
        if ((i1 < 1) || (i1 > x.size(0))) {
          rtDynamicBoundsError(i1, 1, x.size(0), n_emlrtBCI);
        }
        i2 = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i2) {
          rtDynamicBoundsError(idx + 1, 1, i2, n_emlrtBCI);
        }
        obj.maxConstrWorkspace[idx] =
            (obj.maxConstrWorkspace[idx] - x[i - 1]) + x[i1 - 1];
        i = obj.maxConstrWorkspace.size(0);
        if (idx + 1 > i) {
          rtDynamicBoundsError(idx + 1, 1, i, n_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  } else {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.bineq[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[2] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mIneq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (obj.sizes[1] > 2147483646) {
        check_forloop_overflow_error();
      }
      for (int idx{0}; idx <= mEq; idx++) {
        i = obj.maxConstrWorkspace.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, o_emlrtBCI);
        }
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  }
  if (obj.sizes[3] > 0) {
    if (obj.sizes[3] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexLB.size(0);
    i1 = obj.lb.size(0);
    for (int idx{0}; idx < mLB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      offsetEq1 = obj.indexLB[idx] - 1;
      if ((obj.indexLB[idx] < 1) || (obj.indexLB[idx] > x.size(0))) {
        rtDynamicBoundsError(obj.indexLB[idx], 1, x.size(0), p_emlrtBCI);
      }
      if ((obj.indexLB[idx] < 1) || (obj.indexLB[idx] > i1)) {
        rtDynamicBoundsError(obj.indexLB[idx], 1, i1, p_emlrtBCI);
      }
      v = std::fmax(v, -x[offsetEq1] - obj.lb[offsetEq1]);
    }
  }
  if (obj.sizes[4] > 0) {
    if (obj.sizes[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexUB.size(0);
    i1 = obj.ub.size(0);
    for (int idx{0}; idx < mUB; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      offsetEq1 = obj.indexUB[idx] - 1;
      if ((obj.indexUB[idx] < 1) || (obj.indexUB[idx] > x.size(0))) {
        rtDynamicBoundsError(obj.indexUB[idx], 1, x.size(0), p_emlrtBCI);
      }
      if ((obj.indexUB[idx] < 1) || (obj.indexUB[idx] > i1)) {
        rtDynamicBoundsError(obj.indexUB[idx], 1, i1, p_emlrtBCI);
      }
      v = std::fmax(v, x[offsetEq1] - obj.ub[offsetEq1]);
    }
  }
  if (obj.sizes[0] > 0) {
    if (obj.sizes[0] > 2147483646) {
      check_forloop_overflow_error();
    }
    i = obj.indexFixed.size(0);
    i1 = obj.ub.size(0);
    for (int idx{0}; idx < mFixed; idx++) {
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, p_emlrtBCI);
      }
      if ((obj.indexFixed[idx] < 1) || (obj.indexFixed[idx] > x.size(0))) {
        rtDynamicBoundsError(obj.indexFixed[idx], 1, x.size(0), p_emlrtBCI);
      }
      if ((obj.indexFixed[idx] < 1) || (obj.indexFixed[idx] > i1)) {
        rtDynamicBoundsError(obj.indexFixed[idx], 1, i1, p_emlrtBCI);
      }
      v = std::fmax(v, std::abs(x[obj.indexFixed[idx] - 1] -
                                obj.ub[obj.indexFixed[idx] - 1]));
    }
  }
  return v;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for maxConstraintViolation.cpp
//
// [EOF]
//
