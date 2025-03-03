//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: IndexOfDependentEq_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "IndexOfDependentEq_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : array<int, 1U> &depIdx
//                int mFixed
//                int nDep
//                e_struct_T &qrmanager
//                int mRows
//                int nCols
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
void IndexOfDependentEq_(array<int, 1U> &depIdx, int mFixed, int nDep,
                         e_struct_T &qrmanager, int mRows, int nCols)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                    // iFirst
      -1,                    // iLast
      1,                     // lineNo
      1,                     // colNo
      "",                    // aName
      "IndexOfDependentEq_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+initialize/IndexOfDependentEq_.p", // pName
      0                                    // checkKind
  };
  int a;
  int i;
  if (mFixed > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < mFixed; idx++) {
    i = qrmanager.jpvt.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i)) {
      rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
    }
    qrmanager.jpvt[idx] = 1;
  }
  a = mFixed + 1;
  if ((mFixed + 1 <= nCols) && (nCols > 2147483646)) {
    check_forloop_overflow_error();
  }
  for (int idx{a}; idx <= nCols; idx++) {
    i = qrmanager.jpvt.size(0);
    if ((idx < 1) || (idx > i)) {
      rtDynamicBoundsError(idx, 1, i, w_emlrtBCI);
    }
    qrmanager.jpvt[idx - 1] = 0;
  }
  if (mRows * nCols == 0) {
    qrmanager.mrows = mRows;
    qrmanager.ncols = nCols;
    qrmanager.minRowCol = 0;
  } else {
    qrmanager.usedPivoting = true;
    qrmanager.mrows = mRows;
    qrmanager.ncols = nCols;
    if (mRows <= nCols) {
      a = mRows;
    } else {
      a = nCols;
    }
    qrmanager.minRowCol = a;
    internal::lapack::xgeqp3(qrmanager.QR, mRows, nCols, qrmanager.jpvt,
                             qrmanager.tau);
  }
  if (nDep > 2147483646) {
    check_forloop_overflow_error();
  }
  i = qrmanager.jpvt.size(0);
  for (int idx{0}; idx < nDep; idx++) {
    int i1;
    a = ((nCols - nDep) + idx) + 1;
    if ((a < 1) || (a > i)) {
      rtDynamicBoundsError(a, 1, i, w_emlrtBCI);
    }
    i1 = depIdx.size(0);
    if ((idx + 1 < 1) || (idx + 1 > i1)) {
      rtDynamicBoundsError(idx + 1, 1, i1, w_emlrtBCI);
    }
    depIdx[idx] = qrmanager.jpvt[a - 1];
  }
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for IndexOfDependentEq_.cpp
//
// [EOF]
//
