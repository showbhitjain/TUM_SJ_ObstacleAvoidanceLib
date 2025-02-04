//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PresolveWorkingSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "PresolveWorkingSet.h"
#include "ComputeNumDependentEq_.h"
#include "IndexOfDependentEq_.h"
#include "RemoveDependentIneq_.h"
#include "countsort.h"
#include "eml_int_forloop_overflow_check.h"
#include "feasibleX0ForWorkingSet.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "maxConstraintViolation.h"
#include "removeAllIneqConstr.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : j_struct_T &solution
//                h_struct_T &memspace
//                d_struct_T &workingset
//                e_struct_T &qrmanager
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace initialize {
void PresolveWorkingSet(j_struct_T &solution, h_struct_T &memspace,
                        d_struct_T &workingset, e_struct_T &qrmanager)
{
  static rtBoundsCheckInfo ab_emlrtBCI{
      -1,               // iFirst
      -1,               // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "removeEqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/removeEqConstr.p", // pName
      0                               // checkKind
  };
  static rtBoundsCheckInfo w_emlrtBCI{
      1,                // iFirst
      5,                // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "removeEqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/removeEqConstr.p", // pName
      3                               // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      1,                // iFirst
      6,                // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "removeEqConstr", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+WorkingSet/removeEqConstr.p", // pName
      0                               // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                   // iFirst
      -1,                   // iLast
      1,                    // lineNo
      1,                    // colNo
      "",                   // aName
      "RemoveDependentEq_", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+initialize/RemoveDependentEq_.p", // pName
      0                                   // checkKind
  };
  int TYPE;
  int i;
  int i1;
  int i2;
  int idx;
  int idx_col;
  int idx_row;
  int nActiveConstr;
  int nDepInd;
  int nDepIneq;
  int nVar_tmp;
  boolean_T overflow;
  solution.state = 82;
  nVar_tmp = workingset.nVar;
  nActiveConstr = workingset.nWConstr[0] + workingset.nWConstr[1];
  nDepInd = 0;
  if (nActiveConstr > 0) {
    if (nActiveConstr > 2147483646) {
      check_forloop_overflow_error();
    }
    overflow = (nVar_tmp > 2147483646);
    for (idx_row = 0; idx_row < nActiveConstr; idx_row++) {
      if (overflow) {
        check_forloop_overflow_error();
      }
      for (idx_col = 0; idx_col < nVar_tmp; idx_col++) {
        i = workingset.ATwset.size(0);
        i1 = (idx_col + workingset.ldA * idx_row) + 1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, y_emlrtBCI);
        }
        i = qrmanager.QR.size(0) * qrmanager.QR.size(1);
        i2 = (idx_row + qrmanager.ldq * idx_col) + 1;
        if ((i2 < 1) || (i2 > i)) {
          rtDynamicBoundsError(i2, 1, i, y_emlrtBCI);
        }
        qrmanager.QR[i2 - 1] = workingset.ATwset[i1 - 1];
      }
    }
    nDepInd = ComputeNumDependentEq_(qrmanager, workingset.bwset, nActiveConstr,
                                     workingset.nVar);
    if (nDepInd > 0) {
      overflow = (nVar_tmp > 2147483646);
      for (idx_col = 0; idx_col < nActiveConstr; idx_col++) {
        idx_row = qrmanager.ldq * idx_col;
        nDepIneq = workingset.ldA * idx_col;
        if (overflow) {
          check_forloop_overflow_error();
        }
        for (TYPE = 0; TYPE < nVar_tmp; TYPE++) {
          qrmanager.QR[idx_row + TYPE] = workingset.ATwset[nDepIneq + TYPE];
        }
      }
      IndexOfDependentEq_(memspace.workspace_int, workingset.nWConstr[0],
                          nDepInd, qrmanager, workingset.nVar, nActiveConstr);
      utils::countsort(memspace.workspace_int, nDepInd, memspace.workspace_sort,
                       nActiveConstr);
      i = memspace.workspace_int.size(0);
      for (idx = nDepInd; idx >= 1; idx--) {
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, y_emlrtBCI);
        }
        i1 = workingset.nWConstr[0] + workingset.nWConstr[1];
        if (i1 != 0) {
          i2 = memspace.workspace_int[idx - 1];
          if (i2 <= i1) {
            if ((workingset.nActiveConstr == i1) || (i2 == i1)) {
              workingset.mEqRemoved++;
              i1 = workingset.Wlocalidx.size(0);
              if ((i2 < 1) || (i2 > i1)) {
                rtDynamicBoundsError(i2, 1, i1, ab_emlrtBCI);
              }
              i1 = workingset.indexEqRemoved.size[0];
              if ((workingset.mEqRemoved < 1) || (workingset.mEqRemoved > i1)) {
                rtDynamicBoundsError(workingset.mEqRemoved, 1, i1, ab_emlrtBCI);
              }
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[i2 - 1];
              i1 = workingset.Wid.size(0);
              if (i2 > i1) {
                rtDynamicBoundsError(i2, 1, i1, h_emlrtBCI);
              }
              TYPE = workingset.Wid[i2 - 1];
              i1 = workingset.Wlocalidx.size(0);
              if (i2 > i1) {
                rtDynamicBoundsError(i2, 1, i1, h_emlrtBCI);
              }
              i1 = workingset.Wid[i2 - 1];
              if ((i1 < 1) || (i1 > 6)) {
                rtDynamicBoundsError(i1, 1, 6, i_emlrtBCI);
              }
              i1 = workingset.isActiveConstr.size(0);
              idx_row = (workingset.isActiveIdx[workingset.Wid[i2 - 1] - 1] +
                         workingset.Wlocalidx[i2 - 1]) -
                        1;
              if ((idx_row < 1) || (idx_row > i1)) {
                rtDynamicBoundsError(idx_row, 1, i1, h_emlrtBCI);
              }
              workingset.isActiveConstr[idx_row - 1] = false;
              i1 = workingset.Wid.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i1)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i1,
                                     j_emlrtBCI);
              }
              i1 = workingset.Wid.size(0);
              if (i2 > i1) {
                rtDynamicBoundsError(i2, 1, i1, j_emlrtBCI);
              }
              i1 = workingset.nActiveConstr - 1;
              workingset.Wid[i2 - 1] = workingset.Wid[i1];
              idx_row = workingset.Wlocalidx.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > idx_row)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, idx_row,
                                     j_emlrtBCI);
              }
              idx_row = workingset.Wlocalidx.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, j_emlrtBCI);
              }
              workingset.Wlocalidx[i2 - 1] = workingset.Wlocalidx[i1];
              if (workingset.nVar > 2147483646) {
                check_forloop_overflow_error();
              }
              for (int b_idx{0}; b_idx < nVar_tmp; b_idx++) {
                idx_row = workingset.ATwset.size(0);
                idx_col = (b_idx + workingset.ldA * i1) + 1;
                if ((idx_col < 1) || (idx_col > idx_row)) {
                  rtDynamicBoundsError(idx_col, 1, idx_row, j_emlrtBCI);
                }
                idx_row = workingset.ATwset.size(0);
                nActiveConstr = (b_idx + workingset.ldA * (i2 - 1)) + 1;
                if ((nActiveConstr < 1) || (nActiveConstr > idx_row)) {
                  rtDynamicBoundsError(nActiveConstr, 1, idx_row, j_emlrtBCI);
                }
                workingset.ATwset[nActiveConstr - 1] =
                    workingset.ATwset[idx_col - 1];
              }
              idx_row = workingset.bwset.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > idx_row)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, idx_row,
                                     j_emlrtBCI);
              }
              idx_row = workingset.bwset.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, j_emlrtBCI);
              }
              workingset.bwset[i2 - 1] = workingset.bwset[i1];
              workingset.nActiveConstr = i1;
              if ((TYPE < 1) || (TYPE > 5)) {
                rtDynamicBoundsError(TYPE, 1, 5, k_emlrtBCI);
              }
              workingset.nWConstr[TYPE - 1]--;
            } else {
              workingset.mEqRemoved++;
              idx_row = workingset.Wid.size(0);
              if ((i2 < 1) || (i2 > idx_row)) {
                rtDynamicBoundsError(i2, 1, idx_row, ab_emlrtBCI);
              }
              TYPE = workingset.Wid[i2 - 1];
              idx_row = workingset.Wlocalidx.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, ab_emlrtBCI);
              }
              idx_row = workingset.indexEqRemoved.size[0];
              if ((workingset.mEqRemoved < 1) ||
                  (workingset.mEqRemoved > idx_row)) {
                rtDynamicBoundsError(workingset.mEqRemoved, 1, idx_row,
                                     ab_emlrtBCI);
              }
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[i2 - 1];
              idx_row = workingset.Wid[i2 - 1];
              if ((idx_row < 1) || (idx_row > 6)) {
                rtDynamicBoundsError(idx_row, 1, 6, x_emlrtBCI);
              }
              idx_row = workingset.isActiveConstr.size(0);
              idx_col = (workingset.isActiveIdx[workingset.Wid[i2 - 1] - 1] +
                         workingset.Wlocalidx[i2 - 1]) -
                        1;
              if ((idx_col < 1) || (idx_col > idx_row)) {
                rtDynamicBoundsError(idx_col, 1, idx_row, ab_emlrtBCI);
              }
              workingset.isActiveConstr[idx_col - 1] = false;
              idx_row = workingset.Wid.size(0);
              if ((i1 < 1) || (i1 > idx_row)) {
                rtDynamicBoundsError(i1, 1, idx_row, j_emlrtBCI);
              }
              idx_row = workingset.Wid.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, j_emlrtBCI);
              }
              workingset.Wid[i2 - 1] = workingset.Wid[i1 - 1];
              idx_row = workingset.Wlocalidx.size(0);
              if (i1 > idx_row) {
                rtDynamicBoundsError(i1, 1, idx_row, j_emlrtBCI);
              }
              idx_row = workingset.Wlocalidx.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, j_emlrtBCI);
              }
              workingset.Wlocalidx[i2 - 1] = workingset.Wlocalidx[i1 - 1];
              if (workingset.nVar > 2147483646) {
                check_forloop_overflow_error();
              }
              for (int b_idx{0}; b_idx < nVar_tmp; b_idx++) {
                idx_row = workingset.ATwset.size(0);
                idx_col = (b_idx + workingset.ldA * (i1 - 1)) + 1;
                if ((idx_col < 1) || (idx_col > idx_row)) {
                  rtDynamicBoundsError(idx_col, 1, idx_row, j_emlrtBCI);
                }
                idx_row = workingset.ATwset.size(0);
                nActiveConstr = (b_idx + workingset.ldA * (i2 - 1)) + 1;
                if ((nActiveConstr < 1) || (nActiveConstr > idx_row)) {
                  rtDynamicBoundsError(nActiveConstr, 1, idx_row, j_emlrtBCI);
                }
                workingset.ATwset[nActiveConstr - 1] =
                    workingset.ATwset[idx_col - 1];
              }
              idx_row = workingset.bwset.size(0);
              if (i1 > idx_row) {
                rtDynamicBoundsError(i1, 1, idx_row, j_emlrtBCI);
              }
              idx_row = workingset.bwset.size(0);
              if (i2 > idx_row) {
                rtDynamicBoundsError(i2, 1, idx_row, j_emlrtBCI);
              }
              workingset.bwset[i2 - 1] = workingset.bwset[i1 - 1];
              i2 = workingset.Wid.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i2)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i2,
                                     j_emlrtBCI);
              }
              i2 = workingset.Wid.size(0);
              if (i1 > i2) {
                rtDynamicBoundsError(i1, 1, i2, j_emlrtBCI);
              }
              workingset.Wid[i1 - 1] =
                  workingset.Wid[workingset.nActiveConstr - 1];
              i2 = workingset.Wlocalidx.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i2)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i2,
                                     j_emlrtBCI);
              }
              i2 = workingset.Wlocalidx.size(0);
              if (i1 > i2) {
                rtDynamicBoundsError(i1, 1, i2, j_emlrtBCI);
              }
              workingset.Wlocalidx[i1 - 1] =
                  workingset.Wlocalidx[workingset.nActiveConstr - 1];
              if (workingset.nVar > 2147483646) {
                check_forloop_overflow_error();
              }
              for (int b_idx{0}; b_idx < nVar_tmp; b_idx++) {
                i2 = workingset.ATwset.size(0);
                idx_row =
                    (b_idx + workingset.ldA * (workingset.nActiveConstr - 1)) +
                    1;
                if ((idx_row < 1) || (idx_row > i2)) {
                  rtDynamicBoundsError(idx_row, 1, i2, j_emlrtBCI);
                }
                i2 = workingset.ATwset.size(0);
                idx_col = (b_idx + workingset.ldA * (i1 - 1)) + 1;
                if ((idx_col < 1) || (idx_col > i2)) {
                  rtDynamicBoundsError(idx_col, 1, i2, j_emlrtBCI);
                }
                workingset.ATwset[idx_col - 1] = workingset.ATwset[idx_row - 1];
              }
              i2 = workingset.bwset.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i2)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i2,
                                     j_emlrtBCI);
              }
              i2 = workingset.bwset.size(0);
              if (i1 > i2) {
                rtDynamicBoundsError(i1, 1, i2, j_emlrtBCI);
              }
              workingset.bwset[i1 - 1] =
                  workingset.bwset[workingset.nActiveConstr - 1];
              workingset.nActiveConstr--;
              if ((TYPE < 1) || (TYPE > 5)) {
                rtDynamicBoundsError(TYPE, 1, 5, w_emlrtBCI);
              }
              workingset.nWConstr[TYPE - 1]--;
            }
          }
        }
      }
    }
  }
  if ((nDepInd != -1) && (workingset.nActiveConstr <= qrmanager.ldq)) {
    double tol;
    boolean_T guard1;
    nActiveConstr = workingset.nActiveConstr;
    i = workingset.nWConstr[0] + workingset.nWConstr[1];
    if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
            workingset.nWConstr[4] >
        0) {
      tol =
          100.0 * static_cast<double>(workingset.nVar) * 2.2204460492503131E-16;
      if (i > 2147483646) {
        check_forloop_overflow_error();
      }
      for (idx = 0; idx < i; idx++) {
        i1 = qrmanager.jpvt.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i1)) {
          rtDynamicBoundsError(idx + 1, 1, i1, l_emlrtBCI);
        }
        qrmanager.jpvt[idx] = 1;
      }
      idx_row = i + 1;
      if ((i + 1 <= workingset.nActiveConstr) &&
          (workingset.nActiveConstr > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (idx = idx_row; idx <= nActiveConstr; idx++) {
        i1 = qrmanager.jpvt.size(0);
        if ((idx < 1) || (idx > i1)) {
          rtDynamicBoundsError(idx, 1, i1, l_emlrtBCI);
        }
        qrmanager.jpvt[idx - 1] = 0;
      }
      nActiveConstr = workingset.nActiveConstr;
      if (workingset.nActiveConstr > 2147483646) {
        check_forloop_overflow_error();
      }
      for (idx_col = 0; idx_col < nActiveConstr; idx_col++) {
        idx_row = qrmanager.ldq * idx_col;
        nDepIneq = workingset.ldA * idx_col;
        if (nVar_tmp > 2147483646) {
          check_forloop_overflow_error();
        }
        for (TYPE = 0; TYPE < nVar_tmp; TYPE++) {
          qrmanager.QR[idx_row + TYPE] = workingset.ATwset[nDepIneq + TYPE];
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
        nActiveConstr = workingset.nVar;
        idx_row = workingset.nActiveConstr;
        if (nActiveConstr <= idx_row) {
          idx_row = nActiveConstr;
        }
        qrmanager.minRowCol = idx_row;
        internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                 workingset.nActiveConstr, qrmanager.jpvt,
                                 qrmanager.tau);
      }
      nDepIneq = 0;
      for (idx = workingset.nActiveConstr; idx > nVar_tmp; idx--) {
        nDepIneq++;
        i1 = qrmanager.jpvt.size(0);
        if ((idx < 1) || (idx > i1)) {
          rtDynamicBoundsError(idx, 1, i1, l_emlrtBCI);
        }
        i1 = memspace.workspace_int.size(0);
        if ((nDepIneq < 1) || (nDepIneq > i1)) {
          rtDynamicBoundsError(nDepIneq, 1, i1, l_emlrtBCI);
        }
        memspace.workspace_int[nDepIneq - 1] = qrmanager.jpvt[idx - 1];
      }
      if (idx <= workingset.nVar) {
        boolean_T exitg1;
        nActiveConstr = idx + qrmanager.ldq * (idx - 1);
        exitg1 = false;
        while ((!exitg1) && (idx > i)) {
          i1 = qrmanager.QR.size(0) * qrmanager.QR.size(1);
          if ((nActiveConstr < 1) || (nActiveConstr > i1)) {
            rtDynamicBoundsError(nActiveConstr, 1, i1, l_emlrtBCI);
          }
          if (std::abs(qrmanager.QR[nActiveConstr - 1]) < tol) {
            nDepIneq++;
            i1 = qrmanager.jpvt.size(0);
            if ((idx < 1) || (idx > i1)) {
              rtDynamicBoundsError(idx, 1, i1, l_emlrtBCI);
            }
            i1 = memspace.workspace_int.size(0);
            if ((nDepIneq < 1) || (nDepIneq > i1)) {
              rtDynamicBoundsError(nDepIneq, 1, i1, l_emlrtBCI);
            }
            memspace.workspace_int[nDepIneq - 1] = qrmanager.jpvt[idx - 1];
            idx--;
            nActiveConstr = (nActiveConstr - qrmanager.ldq) - 1;
          } else {
            exitg1 = true;
          }
        }
      }
      utils::countsort(memspace.workspace_int, nDepIneq,
                       memspace.workspace_sort, i + 1,
                       workingset.nActiveConstr);
      for (idx = nDepIneq; idx >= 1; idx--) {
        i = memspace.workspace_int.size(0);
        if (idx > i) {
          rtDynamicBoundsError(idx, 1, i, l_emlrtBCI);
        }
        i = workingset.Wid.size(0);
        i1 = memspace.workspace_int[idx - 1];
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, h_emlrtBCI);
        }
        TYPE = workingset.Wid[i1 - 1];
        i = workingset.Wlocalidx.size(0);
        if (i1 > i) {
          rtDynamicBoundsError(i1, 1, i, h_emlrtBCI);
        }
        i = workingset.Wid[i1 - 1];
        if ((i < 1) || (i > 6)) {
          rtDynamicBoundsError(i, 1, 6, i_emlrtBCI);
        }
        i = workingset.isActiveConstr.size(0);
        i2 = (workingset.isActiveIdx[workingset.Wid[i1 - 1] - 1] +
              workingset.Wlocalidx[i1 - 1]) -
             1;
        if ((i2 < 1) || (i2 > i)) {
          rtDynamicBoundsError(i2, 1, i, h_emlrtBCI);
        }
        workingset.isActiveConstr[i2 - 1] = false;
        i = workingset.Wid.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i, j_emlrtBCI);
        }
        i = workingset.Wid.size(0);
        if (i1 > i) {
          rtDynamicBoundsError(i1, 1, i, j_emlrtBCI);
        }
        i = workingset.nActiveConstr - 1;
        workingset.Wid[i1 - 1] = workingset.Wid[i];
        i2 = workingset.Wlocalidx.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i2)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i2, j_emlrtBCI);
        }
        i2 = workingset.Wlocalidx.size(0);
        if (i1 > i2) {
          rtDynamicBoundsError(i1, 1, i2, j_emlrtBCI);
        }
        workingset.Wlocalidx[i1 - 1] = workingset.Wlocalidx[i];
        if (workingset.nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int b_idx{0}; b_idx < nVar_tmp; b_idx++) {
          i2 = workingset.ATwset.size(0);
          idx_row =
              (b_idx + workingset.ldA * (workingset.nActiveConstr - 1)) + 1;
          if ((idx_row < 1) || (idx_row > i2)) {
            rtDynamicBoundsError(idx_row, 1, i2, j_emlrtBCI);
          }
          i2 = workingset.ATwset.size(0);
          idx_col = (b_idx + workingset.ldA * (i1 - 1)) + 1;
          if ((idx_col < 1) || (idx_col > i2)) {
            rtDynamicBoundsError(idx_col, 1, i2, j_emlrtBCI);
          }
          workingset.ATwset[idx_col - 1] = workingset.ATwset[idx_row - 1];
        }
        i2 = workingset.bwset.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i2)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i2, j_emlrtBCI);
        }
        i2 = workingset.bwset.size(0);
        if (i1 > i2) {
          rtDynamicBoundsError(i1, 1, i2, j_emlrtBCI);
        }
        workingset.bwset[i1 - 1] = workingset.bwset[i];
        workingset.nActiveConstr = i;
        if ((TYPE < 1) || (TYPE > 5)) {
          rtDynamicBoundsError(TYPE, 1, 5, k_emlrtBCI);
        }
        workingset.nWConstr[TYPE - 1]--;
      }
    }
    overflow = feasibleX0ForWorkingSet(memspace.workspace_double,
                                       solution.xstar, workingset, qrmanager);
    guard1 = false;
    if (!overflow) {
      RemoveDependentIneq_(workingset, qrmanager, memspace);
      overflow = feasibleX0ForWorkingSet(memspace.workspace_double,
                                         solution.xstar, workingset, qrmanager);
      if (!overflow) {
        solution.state = -7;
      } else {
        guard1 = true;
      }
    } else {
      guard1 = true;
    }
    if (guard1 &&
        (workingset.nWConstr[0] + workingset.nWConstr[1] == workingset.nVar)) {
      tol = WorkingSet::maxConstraintViolation(workingset, solution.xstar);
      if (tol > 1.0E-6) {
        solution.state = -2;
      }
    }
  } else {
    solution.state = -3;
    WorkingSet::removeAllIneqConstr(workingset);
  }
}

} // namespace initialize
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for PresolveWorkingSet.cpp
//
// [EOF]
//
