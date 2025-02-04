//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleX0ForWorkingSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "feasibleX0ForWorkingSet.h"
#include "computeQ_.h"
#include "eml_int_forloop_overflow_check.h"
#include "factorQR.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "maxConstraintViolation.h"
#include "rt_nonfinite.h"
#include "xgemm.h"
#include "xzgeqp3.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 2U> &workspace
//                array<double, 1U> &xCurrent
//                d_struct_T &workingset
//                e_struct_T &qrmanager
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
boolean_T feasibleX0ForWorkingSet(array<double, 2U> &workspace,
                                  array<double, 1U> &xCurrent,
                                  d_struct_T &workingset, e_struct_T &qrmanager)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                        // iFirst
      -1,                        // iLast
      1,                         // lineNo
      1,                         // colNo
      "",                        // aName
      "feasibleX0ForWorkingSet", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+initialize/feasibleX0ForWorkingSet.p", // pName
      0                                        // checkKind
  };
  array<double, 2U> A;
  int mWConstr;
  int nVar;
  boolean_T nonDegenerateWset;
  mWConstr = workingset.nActiveConstr;
  nVar = workingset.nVar;
  nonDegenerateWset = true;
  if (mWConstr != 0) {
    double c;
    int b;
    int i;
    int iAcol;
    int iy;
    int jBcol;
    if (mWConstr > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iAcol = 0; iAcol < mWConstr; iAcol++) {
      i = workingset.bwset.size(0);
      if ((iAcol + 1 < 1) || (iAcol + 1 > i)) {
        rtDynamicBoundsError(iAcol + 1, 1, i, w_emlrtBCI);
      }
      i = workspace.size(0);
      if (iAcol + 1 > i) {
        rtDynamicBoundsError(iAcol + 1, 1, i, w_emlrtBCI);
      }
      c = workingset.bwset[iAcol];
      workspace[iAcol] = c;
      i = workspace.size(1);
      if (i < 2) {
        rtDynamicBoundsError(2, 1, i, w_emlrtBCI);
      }
      i = workingset.bwset.size(0);
      if (iAcol + 1 > i) {
        rtDynamicBoundsError(iAcol + 1, 1, i, w_emlrtBCI);
      }
      i = workspace.size(0);
      if (iAcol + 1 > i) {
        rtDynamicBoundsError(iAcol + 1, 1, i, w_emlrtBCI);
      }
      workspace[iAcol + workspace.size(0)] = c;
    }
    iAcol = workingset.ldA;
    if ((nVar != 0) && (mWConstr != 0)) {
      boolean_T overflow;
      iy = 0;
      jBcol = workingset.ldA * (mWConstr - 1) + 1;
      if ((workingset.ldA == 0) || ((workingset.ldA > 0) && (jBcol < 1)) ||
          ((workingset.ldA < 0) && (jBcol > 1))) {
        overflow = false;
      } else if (workingset.ldA > 0) {
        overflow = (jBcol > MAX_int32_T - workingset.ldA);
      } else {
        overflow = (jBcol < MIN_int32_T - workingset.ldA);
      }
      if (workingset.ldA == 0) {
        m_rtErrorWithMessageID(b_emlrtRTEI.fName, b_emlrtRTEI.lineNo);
      }
      if (overflow) {
        check_forloop_overflow_error();
      }
      for (int iac{1}; iAcol < 0 ? iac >= jBcol : iac <= jBcol; iac += iAcol) {
        c = 0.0;
        b = (iac + nVar) - 1;
        if ((iac <= b) && (b > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int ia{iac}; ia <= b; ia++) {
          c += workingset.ATwset[ia - 1] * xCurrent[ia - iac];
        }
        workspace[iy] = workspace[iy] - c;
        iy++;
      }
    }
    if (mWConstr >= nVar) {
      int ldq;
      int ldw;
      for (iAcol = 0; iAcol < nVar; iAcol++) {
        iy = qrmanager.ldq * iAcol + 1;
        for (int iac{0}; iac < mWConstr; iac++) {
          i = workingset.ATwset.size(0);
          b = (iAcol + workingset.ldA * iac) + 1;
          if ((b < 1) || (b > i)) {
            rtDynamicBoundsError(b, 1, i, w_emlrtBCI);
          }
          i = qrmanager.QR.size(0) * qrmanager.QR.size(1);
          jBcol = iac + iy;
          if ((jBcol < 1) || (jBcol > i)) {
            rtDynamicBoundsError(jBcol, 1, i, w_emlrtBCI);
          }
          qrmanager.QR[jBcol - 1] = workingset.ATwset[b - 1];
        }
      }
      if (mWConstr * nVar == 0) {
        qrmanager.mrows = mWConstr;
        qrmanager.ncols = nVar;
        qrmanager.minRowCol = 0;
      } else {
        qrmanager.usedPivoting = false;
        qrmanager.mrows = mWConstr;
        qrmanager.ncols = nVar;
        for (iAcol = 0; iAcol < nVar; iAcol++) {
          i = qrmanager.jpvt.size(0);
          if ((iAcol + 1 < 1) || (iAcol + 1 > i)) {
            rtDynamicBoundsError(iAcol + 1, 1, i, m_emlrtBCI);
          }
          qrmanager.jpvt[iAcol] = iAcol + 1;
        }
        if (mWConstr <= nVar) {
          i = mWConstr;
        } else {
          i = nVar;
        }
        qrmanager.minRowCol = i;
        A.set_size(qrmanager.QR.size(0), qrmanager.QR.size(1));
        iAcol = qrmanager.QR.size(0) * qrmanager.QR.size(1);
        for (b = 0; b < iAcol; b++) {
          A[b] = qrmanager.QR[b];
        }
        iAcol = qrmanager.QR.size(0);
        iy = qrmanager.QR.size(1);
        if (iAcol <= iy) {
          iy = iAcol;
        }
        qrmanager.tau.set_size(iy);
        for (b = 0; b < iy; b++) {
          qrmanager.tau[b] = 0.0;
        }
        if (i >= 1) {
          internal::reflapack::qrf(A, mWConstr, nVar, i, qrmanager.tau);
        }
        qrmanager.QR.set_size(A.size(0), A.size(1));
        iAcol = A.size(0) * A.size(1);
        for (i = 0; i < iAcol; i++) {
          qrmanager.QR[i] = A[i];
        }
      }
      QRManager::computeQ_(qrmanager, qrmanager.mrows);
      ldq = qrmanager.ldq;
      ldw = workspace.size(0);
      A.set_size(workspace.size(0), workspace.size(1));
      iAcol = workspace.size(0) * workspace.size(1);
      for (i = 0; i < iAcol; i++) {
        A[i] = workspace[i];
      }
      if (nVar != 0) {
        if (workspace.size(0) > MAX_int32_T - workspace.size(0)) {
          check_forloop_overflow_error();
        }
        for (int iac{0}; ldw < 0 ? iac >= ldw : iac <= ldw; iac += ldw) {
          iy = iac + 1;
          b = iac + nVar;
          if ((iac + 1 <= b) && (b > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (int ia{iy}; ia <= b; ia++) {
            workspace[ia - 1] = 0.0;
          }
        }
        jBcol = -1;
        if (workspace.size(0) > MAX_int32_T - workspace.size(0)) {
          check_forloop_overflow_error();
        }
        for (int iac{0}; ldw < 0 ? iac >= ldw : iac <= ldw; iac += ldw) {
          iAcol = -1;
          iy = iac + 1;
          b = iac + nVar;
          if ((iac + 1 <= b) && (b > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (int ia{iy}; ia <= b; ia++) {
            c = 0.0;
            for (int k{0}; k < mWConstr; k++) {
              c += qrmanager.Q[(k + iAcol) + 1] * A[(k + jBcol) + 1];
            }
            workspace[ia - 1] = workspace[ia - 1] + c;
            iAcol += ldq;
          }
          jBcol += ldw;
        }
      }
      for (int iac{0}; iac < 2; iac++) {
        jBcol = ldw * iac - 1;
        for (int k{nVar}; k >= 1; k--) {
          iy = ldq * (k - 1) - 1;
          i = k + jBcol;
          c = workspace[i];
          if (c != 0.0) {
            workspace[i] = c / qrmanager.QR[k + iy];
            for (int ia{0}; ia <= k - 2; ia++) {
              b = (ia + jBcol) + 1;
              workspace[b] =
                  workspace[b] - workspace[i] * qrmanager.QR[(ia + iy) + 1];
            }
          }
        }
      }
    } else {
      QRManager::factorQR(qrmanager, workingset.ATwset, nVar, mWConstr,
                          workingset.ldA);
      QRManager::computeQ_(qrmanager, qrmanager.minRowCol);
      for (int iac{0}; iac < 2; iac++) {
        jBcol = workspace.size(0) * iac;
        for (int ia{0}; ia < mWConstr; ia++) {
          iAcol = qrmanager.ldq * ia;
          iy = ia + jBcol;
          c = workspace[iy];
          if (ia > 2147483646) {
            check_forloop_overflow_error();
          }
          for (int k{0}; k < ia; k++) {
            c -= qrmanager.QR[k + iAcol] * workspace[k + jBcol];
          }
          workspace[iy] = c / qrmanager.QR[ia + iAcol];
        }
      }
      A.set_size(workspace.size(0), workspace.size(1));
      iAcol = workspace.size(0) * workspace.size(1);
      for (i = 0; i < iAcol; i++) {
        A[i] = workspace[i];
      }
      internal::blas::xgemm(nVar, mWConstr, qrmanager.Q, qrmanager.ldq, A,
                            workspace.size(0), workspace, workspace.size(0));
    }
    if (nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    iAcol = 0;
    int exitg1;
    do {
      exitg1 = 0;
      if (iAcol <= nVar - 1) {
        i = workspace.size(0);
        if ((iAcol + 1 < 1) || (iAcol + 1 > i)) {
          rtDynamicBoundsError(iAcol + 1, 1, i, w_emlrtBCI);
        }
        c = workspace[iAcol];
        if (std::isinf(c) || std::isnan(c)) {
          nonDegenerateWset = false;
          exitg1 = 1;
        } else {
          i = workspace.size(1);
          if (i < 2) {
            rtDynamicBoundsError(2, 1, i, w_emlrtBCI);
          }
          c = workspace[iAcol + workspace.size(0)];
          if (std::isinf(c) || std::isnan(c)) {
            nonDegenerateWset = false;
            exitg1 = 1;
          } else {
            iAcol++;
          }
        }
      } else {
        double constrViolation_basicX;
        if (nVar >= 1) {
          iAcol = nVar - 1;
          for (int k{0}; k <= iAcol; k++) {
            workspace[k] = workspace[k] + xCurrent[k];
          }
        }
        c = WorkingSet::maxConstraintViolation(workingset, workspace);
        constrViolation_basicX = WorkingSet::maxConstraintViolation(
            workingset, workspace, workspace.size(0) + 1);
        if ((c <= 2.2204460492503131E-16) || (c < constrViolation_basicX)) {
          for (int k{0}; k < nVar; k++) {
            xCurrent[k] = workspace[k];
          }
        } else {
          for (int k{0}; k < nVar; k++) {
            xCurrent[k] = workspace[workspace.size(0) + k];
          }
        }
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
  return nonDegenerateWset;
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for feasibleX0ForWorkingSet.cpp
//
// [EOF]
//
