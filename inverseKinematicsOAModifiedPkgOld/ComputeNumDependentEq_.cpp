//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ComputeNumDependentEq_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "ComputeNumDependentEq_.h"
#include "computeQ_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : e_struct_T &qrmanager
//                const array<double, 1U> &beqf
//                int mConstr
//                int nVar
// Return Type  : int
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
int ComputeNumDependentEq_(e_struct_T &qrmanager, const array<double, 1U> &beqf,
                           int mConstr, int nVar)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                       // iFirst
      -1,                       // iLast
      1,                        // lineNo
      1,                        // colNo
      "",                       // aName
      "ComputeNumDependentEq_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+initialize/ComputeNumDependentEq_.p", // pName
      0                                       // checkKind
  };
  double tol;
  int idx;
  int k;
  int numDependent;
  int totalRank;
  boolean_T exitg1;
  numDependent = mConstr - nVar;
  if (numDependent <= 0) {
    numDependent = 0;
  }
  if (nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (idx = 0; idx < nVar; idx++) {
    k = qrmanager.jpvt.size(0);
    if ((idx + 1 < 1) || (idx + 1 > k)) {
      rtDynamicBoundsError(idx + 1, 1, k, w_emlrtBCI);
    }
    qrmanager.jpvt[idx] = 0;
  }
  if (mConstr * nVar == 0) {
    qrmanager.mrows = mConstr;
    qrmanager.ncols = nVar;
    qrmanager.minRowCol = 0;
  } else {
    qrmanager.usedPivoting = true;
    qrmanager.mrows = mConstr;
    qrmanager.ncols = nVar;
    if (mConstr <= nVar) {
      totalRank = mConstr;
    } else {
      totalRank = nVar;
    }
    qrmanager.minRowCol = totalRank;
    internal::lapack::xgeqp3(qrmanager.QR, mConstr, nVar, qrmanager.jpvt,
                             qrmanager.tau);
  }
  tol = 100.0 * static_cast<double>(nVar) * 2.2204460492503131E-16;
  if (nVar <= mConstr) {
    totalRank = nVar;
  } else {
    totalRank = mConstr;
  }
  totalRank += qrmanager.ldq * (totalRank - 1);
  exitg1 = false;
  while ((!exitg1) && (totalRank > 0)) {
    k = qrmanager.QR.size(0) * qrmanager.QR.size(1);
    if (totalRank > k) {
      rtDynamicBoundsError(totalRank, 1, k, w_emlrtBCI);
    }
    if (std::abs(qrmanager.QR[totalRank - 1]) < tol) {
      totalRank = (totalRank - qrmanager.ldq) - 1;
      numDependent++;
    } else {
      exitg1 = true;
    }
  }
  if (numDependent > 0) {
    QRManager::computeQ_(qrmanager, qrmanager.mrows);
    if (numDependent > 2147483646) {
      check_forloop_overflow_error();
    }
    idx = 0;
    exitg1 = false;
    while ((!exitg1) && (idx <= numDependent - 1)) {
      double qtb;
      qtb = 0.0;
      if (mConstr >= 1) {
        totalRank = qrmanager.ldq * ((mConstr - idx) - 1);
        if (mConstr > 2147483646) {
          check_forloop_overflow_error();
        }
        for (k = 0; k < mConstr; k++) {
          qtb += qrmanager.Q[totalRank + k] * beqf[k];
        }
      }
      if (std::abs(qtb) >= tol) {
        numDependent = -1;
        exitg1 = true;
      } else {
        idx++;
      }
    }
  }
  return numDependent;
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for ComputeNumDependentEq_.cpp
//
// [EOF]
//
