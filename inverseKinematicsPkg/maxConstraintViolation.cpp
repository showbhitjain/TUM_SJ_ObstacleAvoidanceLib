//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: maxConstraintViolation.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "maxConstraintViolation.h"
#include "inverseKinematics_internal_types.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : i_struct_T &obj
//                const array<double, 1U> &x
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
double maxConstraintViolation(i_struct_T &obj, const array<double, 1U> &x)
{
  double v;
  int mFixed;
  int mLB;
  int mUB;
  int offsetEq2;
  mLB = obj.sizes[3];
  mUB = obj.sizes[4];
  mFixed = obj.sizes[0];
  if (obj.probType == 2) {
    int mEq;
    v = 0.0;
    mEq = obj.sizes[1] - 1;
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mEq; offsetEq2++) {
        obj.maxConstrWorkspace[offsetEq2] = obj.beq.data[offsetEq2];
      }
      internal::blas::xgemv(obj.nVarOrig, obj.sizes[1], obj.Aeq, obj.ldA, x,
                            obj.maxConstrWorkspace);
      offsetEq2 = obj.nVarOrig + obj.sizes[1];
      for (int idx{0}; idx <= mEq; idx++) {
        obj.maxConstrWorkspace[idx] =
            (obj.maxConstrWorkspace[idx] - x[obj.nVarOrig + idx]) +
            x[offsetEq2 + idx];
        v = std::fmax(v, std::abs(obj.maxConstrWorkspace[idx]));
      }
    }
  } else {
    int mEq;
    v = 0.0;
    mEq = obj.sizes[1] - 1;
    if (obj.Aeq.size(0) != 0) {
      for (offsetEq2 = 0; offsetEq2 <= mEq; offsetEq2++) {
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
      offsetEq2 = obj.indexLB[idx] - 1;
      v = std::fmax(v, -x[offsetEq2] - obj.lb[offsetEq2]);
    }
  }
  if (obj.sizes[4] > 0) {
    for (int idx{0}; idx < mUB; idx++) {
      offsetEq2 = obj.indexUB[idx] - 1;
      v = std::fmax(v, x[offsetEq2] - obj.ub[offsetEq2]);
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
