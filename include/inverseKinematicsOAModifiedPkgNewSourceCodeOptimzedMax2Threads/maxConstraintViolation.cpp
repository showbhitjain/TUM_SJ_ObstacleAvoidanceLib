//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: maxConstraintViolation.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "maxConstraintViolation.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "omp.h"
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
  double b_vPrime;
  double b_x;
  double c_vPrime;
  double d_vPrime;
  double e_vPrime;
  double v;
  double vPrime;
  int k;
  int mEq;
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  int offsetEq1;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (static_cast<int>(mIneq + 1 < 200)) {
        for (k = 0; k <= mIneq; k++) {
          obj.maxConstrWorkspace[k] = obj.bineq[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (k = 0; k <= mIneq; k++) {
          obj.maxConstrWorkspace[k] = obj.bineq[k];
        }
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] =
            obj.maxConstrWorkspace[offsetEq1] -
            x[((ix0 + obj.nVarOrig) + offsetEq1) - 1];
        v = std::fmax(v, obj.maxConstrWorkspace[offsetEq1]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            ix0, obj.maxConstrWorkspace);
      offsetEq1 = (obj.nVarOrig + obj.sizes[2]) - 1;
      mIneq = offsetEq1 + obj.sizes[1];
      if (static_cast<int>(mEq + 1 < 200)) {
        for (k = 0; k <= mEq; k++) {
          b_x = (obj.maxConstrWorkspace[k] - x[(ix0 + offsetEq1) + k]) +
                x[(ix0 + mIneq) + k];
          obj.maxConstrWorkspace[k] = b_x;
          v = std::fmax(v, std::abs(b_x));
        }
      } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(c_vPrime, b_x)
        {
          c_vPrime = rtMinusInf;
#pragma omp for nowait
          for (k = 0; k <= mEq; k++) {
            b_x = (obj.maxConstrWorkspace[k] - x[(ix0 + offsetEq1) + k]) +
                  x[(ix0 + mIneq) + k];
            obj.maxConstrWorkspace[k] = b_x;
            c_vPrime = std::fmax(c_vPrime, std::abs(b_x));
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, c_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
  } else {
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      offsetEq1 = (mIneq + 1 < 200);
      if (offsetEq1) {
        for (k = 0; k <= mIneq; k++) {
          obj.maxConstrWorkspace[k] = obj.bineq[k];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (k = 0; k <= mIneq; k++) {
          obj.maxConstrWorkspace[k] = obj.bineq[k];
        }
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      if (offsetEq1) {
        for (k = 0; k <= mIneq; k++) {
          v = std::fmax(v, obj.maxConstrWorkspace[k]);
        }
      } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(c_vPrime)
        {
          c_vPrime = rtMinusInf;
#pragma omp for nowait
          for (k = 0; k <= mIneq; k++) {
            c_vPrime = std::fmax(c_vPrime, obj.maxConstrWorkspace[k]);
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, c_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x, ix0,
                            obj.maxConstrWorkspace);
      if (static_cast<int>(mEq + 1 < 200)) {
        for (k = 0; k <= mEq; k++) {
          v = std::fmax(v, std::abs(obj.maxConstrWorkspace[k]));
        }
      } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(b_vPrime, b_x)
        {
          b_vPrime = rtMinusInf;
#pragma omp for nowait
          for (k = 0; k <= mEq; k++) {
            b_x = obj.maxConstrWorkspace[k];
            b_vPrime = std::fmax(b_vPrime, std::abs(b_x));
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, b_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
  }
  if (obj.sizes[3] > 0) {
    if (static_cast<int>(mLB < 200)) {
      for (k = 0; k < mLB; k++) {
        v = std::fmax(v, -x[(ix0 + obj.indexLB[k]) - 2] -
                             obj.lb[obj.indexLB[k] - 1]);
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(vPrime, b_x)
      {
        vPrime = rtMinusInf;
#pragma omp for nowait
        for (k = 0; k < mLB; k++) {
          b_x = -x[(ix0 + obj.indexLB[k]) - 2] - obj.lb[obj.indexLB[k] - 1];
          vPrime = std::fmax(vPrime, b_x);
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
  }
  if (obj.sizes[4] > 0) {
    if (static_cast<int>(mUB < 200)) {
      for (k = 0; k < mUB; k++) {
        v = std::fmax(v, x[(ix0 + obj.indexUB[k]) - 2] -
                             obj.ub[obj.indexUB[k] - 1]);
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(d_vPrime, b_x)
      {
        d_vPrime = rtMinusInf;
#pragma omp for nowait
        for (k = 0; k < mUB; k++) {
          b_x = x[(ix0 + obj.indexUB[k]) - 2] - obj.ub[obj.indexUB[k] - 1];
          d_vPrime = std::fmax(d_vPrime, b_x);
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, d_vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
  }
  if (obj.sizes[0] > 0) {
    if (static_cast<int>(mFixed < 200)) {
      for (k = 0; k < mFixed; k++) {
        v = std::fmax(v, std::abs(x[(ix0 + obj.indexFixed[k]) - 2] -
                                  obj.ub[obj.indexFixed[k] - 1]));
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(e_vPrime, b_x)
      {
        e_vPrime = rtMinusInf;
#pragma omp for nowait
        for (k = 0; k < mFixed; k++) {
          b_x =
              x[(ix0 + obj.indexFixed[k]) - 2] - obj.ub[obj.indexFixed[k] - 1];
          e_vPrime = std::fmax(e_vPrime, std::abs(b_x));
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, e_vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
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
  double b_vPrime;
  double b_x;
  double c_vPrime;
  double d_vPrime;
  double e_vPrime;
  double v;
  double vPrime;
  int idx;
  int idxLB;
  int mEq;
  int mFixed;
  int mIneq;
  int mLB;
  int mUB;
  int offsetEq1;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      if (static_cast<int>(mIneq + 1 < 200)) {
        for (idxLB = 0; idxLB <= mIneq; idxLB++) {
          obj.maxConstrWorkspace[idxLB] = obj.bineq[idxLB];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (idxLB = 0; idxLB <= mIneq; idxLB++) {
          obj.maxConstrWorkspace[idxLB] = obj.bineq[idxLB];
        }
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      for (offsetEq1 = 0; offsetEq1 <= mIneq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] =
            obj.maxConstrWorkspace[offsetEq1] - x[obj.nVarOrig + offsetEq1];
        v = std::fmax(v, obj.maxConstrWorkspace[offsetEq1]);
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      offsetEq1 = obj.nVarOrig + obj.sizes[2];
      mIneq = offsetEq1 + obj.sizes[1];
      if (static_cast<int>(mEq + 1 < 200)) {
        for (idx = 0; idx <= mEq; idx++) {
          b_x = (obj.maxConstrWorkspace[idx] - x[offsetEq1 + idx]) +
                x[mIneq + idx];
          obj.maxConstrWorkspace[idx] = b_x;
          v = std::fmax(v, std::abs(b_x));
        }
      } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(c_vPrime, b_x)
        {
          c_vPrime = rtMinusInf;
#pragma omp for nowait
          for (idx = 0; idx <= mEq; idx++) {
            b_x = (obj.maxConstrWorkspace[idx] - x[offsetEq1 + idx]) +
                  x[mIneq + idx];
            obj.maxConstrWorkspace[idx] = b_x;
            c_vPrime = std::fmax(c_vPrime, std::abs(b_x));
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, c_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
  } else {
    v = 0.0;
    mIneq = obj.sizes[2] - 1;
    mEq = obj.sizes[1] - 1;
    if (obj.Aineq.size(0) != 0) {
      offsetEq1 = (mIneq + 1 < 200);
      if (offsetEq1) {
        for (idxLB = 0; idxLB <= mIneq; idxLB++) {
          obj.maxConstrWorkspace[idxLB] = obj.bineq[idxLB];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (idxLB = 0; idxLB <= mIneq; idxLB++) {
          obj.maxConstrWorkspace[idxLB] = obj.bineq[idxLB];
        }
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[2], obj.Aineq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (offsetEq1) {
        for (idx = 0; idx <= mIneq; idx++) {
          v = std::fmax(v, obj.maxConstrWorkspace[idx]);
        }
      } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(c_vPrime)
        {
          c_vPrime = rtMinusInf;
#pragma omp for nowait
          for (idx = 0; idx <= mIneq; idx++) {
            c_vPrime = std::fmax(c_vPrime, obj.maxConstrWorkspace[idx]);
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, c_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq1 = 0; offsetEq1 <= mEq; offsetEq1++) {
        obj.maxConstrWorkspace[offsetEq1] = obj.beq.data[offsetEq1];
      }
      internal::blas::xgemv(obj.nVar, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      if (static_cast<int>(mEq + 1 < 200)) {
        for (idx = 0; idx <= mEq; idx++) {
          v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
        }
      } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(b_vPrime, b_x)
        {
          b_vPrime = rtMinusInf;
#pragma omp for nowait
          for (idx = 0; idx <= mEq; idx++) {
            b_x = obj.maxConstrWorkspace[idx];
            b_vPrime = std::fmax(b_vPrime, std::abs(b_x));
          }
          omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
          {

            v = std::fmax(v, b_vPrime);
          }
          omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        }
      }
    }
  }
  if (obj.sizes[3] > 0) {
    if (static_cast<int>(mLB < 200)) {
      for (idx = 0; idx < mLB; idx++) {
        idxLB = obj.indexLB[idx] - 1;
        v = std::fmax(v, -x[idxLB] - obj.lb[idxLB]);
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(vPrime, idxLB)
      {
        vPrime = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mLB; idx++) {
          idxLB = obj.indexLB[idx] - 1;
          vPrime = std::fmax(vPrime, -x[idxLB] - obj.lb[idxLB]);
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
  }
  if (obj.sizes[4] > 0) {
    if (static_cast<int>(mUB < 200)) {
      for (idx = 0; idx < mUB; idx++) {
        idxLB = obj.indexUB[idx] - 1;
        v = std::fmax(v, x[idxLB] - obj.ub[idxLB]);
      }
    } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(d_vPrime, idxLB)
      {
        d_vPrime = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mUB; idx++) {
          idxLB = obj.indexUB[idx] - 1;
          d_vPrime = std::fmax(d_vPrime, x[idxLB] - obj.ub[idxLB]);
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, d_vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
    }
  }
  if (obj.sizes[0] > 0) {
    if (static_cast<int>(mFixed < 200)) {
      for (idx = 0; idx < mFixed; idx++) {
        v = std::fmax(v, std::abs(x[obj.indexFixed[idx] - 1] -
                                  obj.ub[obj.indexFixed[idx] - 1]));
      }
    } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(e_vPrime)
      {
        e_vPrime = rtMinusInf;
#pragma omp for nowait
        for (idx = 0; idx < mFixed; idx++) {
          e_vPrime =
              std::fmax(e_vPrime, std::abs(x[obj.indexFixed[idx] - 1] -
                                           obj.ub[obj.indexFixed[idx] - 1]));
        }
        omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
        {

          v = std::fmax(v, e_vPrime);
        }
        omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
      }
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
