//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: maxConstraintViolation.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "maxConstraintViolation.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                const array<double, 2U> &x
//                int ix0
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
double maxConstraintViolation(d_struct_T &obj, const array<double, 2U> &x,
                              int ix0)
{
  double v;
  int mFixed;
  int mLB;
  int mUB;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    int mIneq;
    int offsetEq2;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mIneq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.bineq[offsetEq2];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mIneq; idx++) {
        obj.maxConstrWorkspace[idx] =
            obj.maxConstrWorkspace[idx] - x[((ix0 + obj.nVarOrig) + idx) - 1];
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mEq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.beq.data[offsetEq2];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      mIneq = (obj.nVarOrig + obj.sizes[2]) - 1;
      offsetEq2 = mIneq + obj.sizes[1];
      for (int idx{0}; idx <= mEq; idx++) {
        double d;
        d = (obj.maxConstrWorkspace[idx] - x[(ix0 + mIneq) + idx]) +
            x[(ix0 + offsetEq2) + idx];
        obj.maxConstrWorkspace[idx] = d;
        v = std::fmax(v, std::abs(d));
      }
    }
  } else {
    int mEq;
    int mIneq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      for (int offsetEq2{0}; offsetEq2 <= mIneq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.bineq[offsetEq2];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mIneq; idx++) {
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (int offsetEq2{0}; offsetEq2 <= mEq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.beq.data[offsetEq2];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mEq; idx++) {
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  }
  if (obj.sizes[3] > 0) {
    for (int idx{0}; idx < mLB; idx++) {
      v = std::fmax(v, -x[(ix0 + obj.indexLB[idx]) - 2] -
                           obj.lb[obj.indexLB[idx] - 1]);
    }
  }
  if (obj.sizes[4] > 0) {
    for (int idx{0}; idx < mUB; idx++) {
      v = std::fmax(v, x[(ix0 + obj.indexUB[idx]) - 2] -
                           obj.ub[obj.indexUB[idx] - 1]);
    }
  }
  if (obj.sizes[0] > 0) {
    for (int idx{0}; idx < mFixed; idx++) {
      v = std::fmax(v, std::abs(x[(ix0 + obj.indexFixed[idx]) - 2] -
                                obj.ub[obj.indexFixed[idx] - 1]));
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
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    int offsetEq2;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mIneq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.bineq[offsetEq2];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mIneq; idx++) {
        obj.maxConstrWorkspace[idx] =
            obj.maxConstrWorkspace[idx] - x[obj.nVarOrig + idx];
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mEq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.beq.data[offsetEq2];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      mIneq = obj.nVarOrig + obj.sizes[2];
      offsetEq2 = mIneq + obj.sizes[1];
      for (int idx{0}; idx <= mEq; idx++) {
        double d;
        d = (obj.maxConstrWorkspace[idx] - x[mIneq + idx]) + x[offsetEq2 + idx];
        obj.maxConstrWorkspace[idx] = d;
        v = std::fmax(v, std::abs(d));
      }
    }
  } else {
    int mEq;
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      for (int offsetEq2{0}; offsetEq2 <= mIneq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.bineq[offsetEq2];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mIneq; idx++) {
        v = std::fmax(v, obj.maxConstrWorkspace[idx]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (int offsetEq2{0}; offsetEq2 <= mEq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.beq.data[offsetEq2];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      for (int idx{0}; idx <= mEq; idx++) {
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  }
  if (obj.sizes[3] > 0) {
    for (int idx{0}; idx < mLB; idx++) {
      mIneq = obj.indexLB[idx] - 1;
      v = std::fmax(v, -x[mIneq] - obj.lb[mIneq]);
    }
  }
  if (obj.sizes[4] > 0) {
    for (int idx{0}; idx < mUB; idx++) {
      mIneq = obj.indexUB[idx] - 1;
      v = std::fmax(v, x[mIneq] - obj.ub[mIneq]);
    }
  }
  if (obj.sizes[0] > 0) {
    for (int idx{0}; idx < mFixed; idx++) {
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
