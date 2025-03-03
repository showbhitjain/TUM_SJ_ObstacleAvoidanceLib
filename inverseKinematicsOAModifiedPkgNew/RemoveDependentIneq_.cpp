//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: RemoveDependentIneq_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "RemoveDependentIneq_.h"
#include "countsort.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
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
// Arguments    : d_struct_T &workingset
//                e_struct_T &qrmanager
//                h_struct_T &memspace
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
void RemoveDependentIneq_(d_struct_T &workingset, e_struct_T &qrmanager,
                          h_struct_T &memspace)
{
  int nActiveConstr_tmp;
  int nFixedConstr;
  int nVar_tmp;
  nActiveConstr_tmp = workingset.nActiveConstr;
  nFixedConstr = workingset.nWConstr[0] + workingset.nWConstr[1];
  nVar_tmp = workingset.nVar;
  if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
          workingset.nWConstr[4] >
      0) {
    double tol;
    int a;
    int i;
    int idx;
    int idxDiag;
    int idx_col;
    int k;
    int nDepIneq;
    tol =
        1000.0 * static_cast<double>(workingset.nVar) * 2.2204460492503131E-16;
    if (nFixedConstr > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx = 0; idx < nFixedConstr; idx++) {
      i = qrmanager.jpvt.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, l_emlrtBCI);
      }
      qrmanager.jpvt[idx] = 1;
    }
    a = nFixedConstr + 1;
    if ((nFixedConstr + 1 <= workingset.nActiveConstr) &&
        (workingset.nActiveConstr > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (idx = a; idx <= nActiveConstr_tmp; idx++) {
      i = qrmanager.jpvt.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, l_emlrtBCI);
      }
      qrmanager.jpvt[idx - 1] = 0;
    }
    if (workingset.nActiveConstr > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idx_col = 0; idx_col < nActiveConstr_tmp; idx_col++) {
      a = qrmanager.ldq * idx_col;
      idxDiag = workingset.ldA * idx_col;
      if (nVar_tmp > 2147483646) {
        check_forloop_overflow_error();
      }
      for (k = 0; k < nVar_tmp; k++) {
        qrmanager.QR[a + k] = workingset.ATwset[idxDiag + k];
      }
    }
    if (workingset.nVar * workingset.nActiveConstr == 0) {
      qrmanager.mrows = workingset.nVar;
      qrmanager.ncols = workingset.nActiveConstr;
      qrmanager.minRowCol = 0;
    } else {
      qrmanager.usedPivoting = true;
      qrmanager.mrows = workingset.nVar;
      qrmanager.ncols = workingset.nActiveConstr;
      a = workingset.nVar;
      idxDiag = workingset.nActiveConstr;
      if (a <= idxDiag) {
        idxDiag = a;
      }
      qrmanager.minRowCol = idxDiag;
      internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                               workingset.nActiveConstr, qrmanager.jpvt,
                               qrmanager.tau);
    }
    nDepIneq = 0;
    for (idx = workingset.nActiveConstr; idx > nVar_tmp; idx--) {
      nDepIneq++;
      i = qrmanager.jpvt.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, l_emlrtBCI);
      }
      i = memspace.workspace_int.size(0);
      if ((nDepIneq < 1) || (nDepIneq > i)) {
        rtDynamicBoundsError(nDepIneq, 1, i, l_emlrtBCI);
      }
      memspace.workspace_int[nDepIneq - 1] = qrmanager.jpvt[idx - 1];
    }
    if (idx <= workingset.nVar) {
      boolean_T exitg1;
      idxDiag = idx + qrmanager.ldq * (idx - 1);
      exitg1 = false;
      while ((!exitg1) && (idx > nFixedConstr)) {
        i = qrmanager.QR.size(0) * qrmanager.QR.size(1);
        if ((idxDiag < 1) || (idxDiag > i)) {
          rtDynamicBoundsError(idxDiag, 1, i, l_emlrtBCI);
        }
        if (std::abs(qrmanager.QR[idxDiag - 1]) < tol) {
          nDepIneq++;
          i = qrmanager.jpvt.size(0);
          if ((idx < 1) || (idx > i)) {
            rtDynamicBoundsError(idx, 1, i, l_emlrtBCI);
          }
          i = memspace.workspace_int.size(0);
          if ((nDepIneq < 1) || (nDepIneq > i)) {
            rtDynamicBoundsError(nDepIneq, 1, i, l_emlrtBCI);
          }
          memspace.workspace_int[nDepIneq - 1] = qrmanager.jpvt[idx - 1];
          idx--;
          idxDiag = (idxDiag - qrmanager.ldq) - 1;
        } else {
          exitg1 = true;
        }
      }
    }
    utils::countsort(memspace.workspace_int, nDepIneq, memspace.workspace_sort,
                     nFixedConstr + 1, workingset.nActiveConstr);
    for (idx = nDepIneq; idx >= 1; idx--) {
      i = memspace.workspace_int.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, l_emlrtBCI);
      }
      i = workingset.Wid.size(0);
      a = memspace.workspace_int[idx - 1];
      if ((a < 1) || (a > i)) {
        rtDynamicBoundsError(a, 1, i, h_emlrtBCI);
      }
      idxDiag = workingset.Wid[a - 1];
      i = workingset.Wlocalidx.size(0);
      if (a > i) {
        rtDynamicBoundsError(a, 1, i, h_emlrtBCI);
      }
      i = workingset.Wid[a - 1];
      if ((i < 1) || (i > 6)) {
        rtDynamicBoundsError(i, 1, 6, i_emlrtBCI);
      }
      i = workingset.isActiveConstr.size(0);
      nActiveConstr_tmp = (workingset.isActiveIdx[workingset.Wid[a - 1] - 1] +
                           workingset.Wlocalidx[a - 1]) -
                          1;
      if ((nActiveConstr_tmp < 1) || (nActiveConstr_tmp > i)) {
        rtDynamicBoundsError(nActiveConstr_tmp, 1, i, h_emlrtBCI);
      }
      workingset.isActiveConstr[nActiveConstr_tmp - 1] = false;
      i = workingset.Wid.size(0);
      if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i)) {
        rtDynamicBoundsError(workingset.nActiveConstr, 1, i, j_emlrtBCI);
      }
      i = workingset.Wid.size(0);
      if (a > i) {
        rtDynamicBoundsError(a, 1, i, j_emlrtBCI);
      }
      i = workingset.nActiveConstr - 1;
      workingset.Wid[a - 1] = workingset.Wid[i];
      nActiveConstr_tmp = workingset.Wlocalidx.size(0);
      if ((workingset.nActiveConstr < 1) ||
          (workingset.nActiveConstr > nActiveConstr_tmp)) {
        rtDynamicBoundsError(workingset.nActiveConstr, 1, nActiveConstr_tmp,
                             j_emlrtBCI);
      }
      nActiveConstr_tmp = workingset.Wlocalidx.size(0);
      if (a > nActiveConstr_tmp) {
        rtDynamicBoundsError(a, 1, nActiveConstr_tmp, j_emlrtBCI);
      }
      workingset.Wlocalidx[a - 1] = workingset.Wlocalidx[i];
      if (workingset.nVar > 2147483646) {
        check_forloop_overflow_error();
      }
      for (nFixedConstr = 0; nFixedConstr < nVar_tmp; nFixedConstr++) {
        nActiveConstr_tmp = workingset.ATwset.size(0);
        idx_col = (nFixedConstr + workingset.ldA * i) + 1;
        if ((idx_col < 1) || (idx_col > nActiveConstr_tmp)) {
          rtDynamicBoundsError(idx_col, 1, nActiveConstr_tmp, j_emlrtBCI);
        }
        nActiveConstr_tmp = workingset.ATwset.size(0);
        k = (nFixedConstr + workingset.ldA * (a - 1)) + 1;
        if ((k < 1) || (k > nActiveConstr_tmp)) {
          rtDynamicBoundsError(k, 1, nActiveConstr_tmp, j_emlrtBCI);
        }
        workingset.ATwset[k - 1] = workingset.ATwset[idx_col - 1];
      }
      nActiveConstr_tmp = workingset.bwset.size(0);
      if ((workingset.nActiveConstr < 1) ||
          (workingset.nActiveConstr > nActiveConstr_tmp)) {
        rtDynamicBoundsError(workingset.nActiveConstr, 1, nActiveConstr_tmp,
                             j_emlrtBCI);
      }
      nActiveConstr_tmp = workingset.bwset.size(0);
      if (a > nActiveConstr_tmp) {
        rtDynamicBoundsError(a, 1, nActiveConstr_tmp, j_emlrtBCI);
      }
      workingset.bwset[a - 1] = workingset.bwset[i];
      workingset.nActiveConstr = i;
      if ((idxDiag < 1) || (idxDiag > 5)) {
        rtDynamicBoundsError(idxDiag, 1, 5, k_emlrtBCI);
      }
      workingset.nWConstr[idxDiag - 1]--;
    }
  }
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for RemoveDependentIneq_.cpp
//
// [EOF]
//
