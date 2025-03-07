//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PresolveWorkingSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "PresolveWorkingSet.h"
#include "ComputeNumDependentEq_.h"
#include "countsort.h"
#include "feasibleX0ForWorkingSet.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "maxConstraintViolation.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "xgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "globalParameters.h"
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
  int TYPE;
  int i;
  int i1;
  int idxEndIneq;
  int k;
  int mTotalWorkingEq_tmp_tmp;
  int nDepInd;
  int nVar;
  solution.state = 82;
  nVar = workingset.nVar - 1;
  mTotalWorkingEq_tmp_tmp = workingset.nWConstr[0] + workingset.nWConstr[1];
  nDepInd = 0;
  if (mTotalWorkingEq_tmp_tmp > 0) {
    for (idxEndIneq = 0; idxEndIneq < mTotalWorkingEq_tmp_tmp; idxEndIneq++) {
      for (int idx_col{0}; idx_col <= nVar; idx_col++) {
        qrmanager.QR[idxEndIneq + qrmanager.ldq * idx_col] =
            workingset.ATwset[idx_col + workingset.ldA * idxEndIneq];
      }
    }
    nDepInd = ComputeNumDependentEq_(qrmanager, workingset.bwset,
                                     mTotalWorkingEq_tmp_tmp, workingset.nVar);
    if (nDepInd > 0) {
      for (int idx_col{0}; idx_col < mTotalWorkingEq_tmp_tmp; idx_col++) {
        idxEndIneq = qrmanager.ldq * idx_col;
        TYPE = workingset.ldA * idx_col;
        for (k = 0; k <= nVar; k++) {
          qrmanager.QR[idxEndIneq + k] = workingset.ATwset[TYPE + k];
        }
      }
      TYPE = workingset.nWConstr[0];
      for (k = 0; k < TYPE; k++) {
        qrmanager.jpvt[k] = 1;
      }
      i = workingset.nWConstr[0] + 1;
      for (k = i; k <= mTotalWorkingEq_tmp_tmp; k++) {
        qrmanager.jpvt[k - 1] = 0;
      }
      if (workingset.nVar * mTotalWorkingEq_tmp_tmp == 0) {
        qrmanager.mrows = workingset.nVar;
        qrmanager.ncols = mTotalWorkingEq_tmp_tmp;
        qrmanager.minRowCol = 0;
      } else {
        qrmanager.usedPivoting = true;
        qrmanager.mrows = workingset.nVar;
        qrmanager.ncols = mTotalWorkingEq_tmp_tmp;
        idxEndIneq = workingset.nVar;
        if (idxEndIneq > mTotalWorkingEq_tmp_tmp) {
          idxEndIneq = mTotalWorkingEq_tmp_tmp;
        }
        qrmanager.minRowCol = idxEndIneq;
        internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                 mTotalWorkingEq_tmp_tmp, qrmanager.jpvt,
                                 qrmanager.tau);
      }
      for (k = 0; k < nDepInd; k++) {
        memspace.workspace_int[k] =
            qrmanager.jpvt[(mTotalWorkingEq_tmp_tmp - nDepInd) + k];
      }
      utils::countsort(memspace.workspace_int, nDepInd, memspace.workspace_sort,
                       1, mTotalWorkingEq_tmp_tmp);
      for (k = nDepInd; k >= 1; k--) {
        i = workingset.nWConstr[0] + workingset.nWConstr[1];
        if (i != 0) {
          i1 = memspace.workspace_int[k - 1];
          if (i1 <= i) {
            if ((workingset.nActiveConstr == i) || (i1 == i)) {
              workingset.mEqRemoved++;
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[i1 - 1];
              WorkingSet::removeConstr(workingset, i1);
            } else {
              workingset.mEqRemoved++;
              TYPE = workingset.Wid[i1 - 1] - 1;
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[i1 - 1];
              workingset.isActiveConstr
                  [(workingset.isActiveIdx[workingset.Wid[i1 - 1] - 1] +
                    workingset.Wlocalidx[i1 - 1]) -
                   2] = false;
              workingset.Wid[i1 - 1] = workingset.Wid[i - 1];
              workingset.Wlocalidx[i1 - 1] = workingset.Wlocalidx[i - 1];
              idxEndIneq = workingset.nVar;
              for (mTotalWorkingEq_tmp_tmp = 0;
                   mTotalWorkingEq_tmp_tmp < idxEndIneq;
                   mTotalWorkingEq_tmp_tmp++) {
                workingset.ATwset[mTotalWorkingEq_tmp_tmp +
                                  workingset.ldA * (i1 - 1)] =
                    workingset.ATwset[mTotalWorkingEq_tmp_tmp +
                                      workingset.ldA * (i - 1)];
              }
              workingset.bwset[i1 - 1] = workingset.bwset[i - 1];
              workingset.Wid[i - 1] =
                  workingset.Wid[workingset.nActiveConstr - 1];
              workingset.Wlocalidx[i - 1] =
                  workingset.Wlocalidx[workingset.nActiveConstr - 1];
              i1 = workingset.nVar;
              for (mTotalWorkingEq_tmp_tmp = 0; mTotalWorkingEq_tmp_tmp < i1;
                   mTotalWorkingEq_tmp_tmp++) {
                workingset.ATwset[mTotalWorkingEq_tmp_tmp +
                                  workingset.ldA * (i - 1)] =
                    workingset.ATwset[mTotalWorkingEq_tmp_tmp +
                                      workingset.ldA *
                                          (workingset.nActiveConstr - 1)];
              }
              workingset.bwset[i - 1] =
                  workingset.bwset[workingset.nActiveConstr - 1];
              workingset.nActiveConstr--;
              workingset.nWConstr[TYPE]--;
            }
          }
        }
      }
    }
  }
  if ((nDepInd != -1) && (workingset.nActiveConstr <= qrmanager.ldq)) {
    double tol;
    boolean_T guard1;
    boolean_T okWorkingSet;
    TYPE = workingset.nActiveConstr;
    i = workingset.nWConstr[0] + workingset.nWConstr[1];
    nVar = workingset.nVar;
    if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
            workingset.nWConstr[4] >
        0) {
      tol =
          100.0 * static_cast<double>(workingset.nVar) * 2.2204460492503131E-16;
      for (k = 0; k < i; k++) {
        qrmanager.jpvt[k] = 1;
      }
      i1 = i + 1;
      for (k = i1; k <= TYPE; k++) {
        qrmanager.jpvt[k - 1] = 0;
      }
      i1 = workingset.nActiveConstr;
      for (int idx_col{0}; idx_col < i1; idx_col++) {
        idxEndIneq = qrmanager.ldq * idx_col;
        TYPE = workingset.ldA * idx_col;
        for (k = 0; k < nVar; k++) {
          qrmanager.QR[idxEndIneq + k] = workingset.ATwset[TYPE + k];
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
        idxEndIneq = workingset.nVar;
        TYPE = workingset.nActiveConstr;
        if (idxEndIneq <= TYPE) {
          TYPE = idxEndIneq;
        }
        qrmanager.minRowCol = TYPE;
        internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                 workingset.nActiveConstr, qrmanager.jpvt,
                                 qrmanager.tau);
      }
      idxEndIneq = 0;
      for (k = workingset.nActiveConstr - 1; k + 1 > nVar; k--) {
        idxEndIneq++;
        memspace.workspace_int[idxEndIneq - 1] = qrmanager.jpvt[k];
      }
      if (k + 1 <= workingset.nVar) {
        TYPE = k + qrmanager.ldq * k;
        while ((k + 1 > i) && (std::abs(qrmanager.QR[TYPE]) < tol)) {
          idxEndIneq++;
          memspace.workspace_int[idxEndIneq - 1] = qrmanager.jpvt[k];
          k--;
          TYPE = (TYPE - qrmanager.ldq) - 1;
        }
      }
      utils::countsort(memspace.workspace_int, idxEndIneq,
                       memspace.workspace_sort, i + 1,
                       workingset.nActiveConstr);
      for (k = idxEndIneq; k >= 1; k--) {
        WorkingSet::removeConstr(workingset, memspace.workspace_int[k - 1]);
      }
    }
    okWorkingSet = feasibleX0ForWorkingSet(
        memspace.workspace_double, solution.xstar, workingset, qrmanager);
    guard1 = false;
    if (!okWorkingSet) {
      TYPE = workingset.nActiveConstr;
      i = workingset.nWConstr[0] + workingset.nWConstr[1];
      nVar = workingset.nVar;
      if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
              workingset.nWConstr[4] >
          0) {
        tol = 1000.0 * static_cast<double>(workingset.nVar) *
              2.2204460492503131E-16;
        for (k = 0; k < i; k++) {
          qrmanager.jpvt[k] = 1;
        }
        i1 = i + 1;
        for (k = i1; k <= TYPE; k++) {
          qrmanager.jpvt[k - 1] = 0;
        }
        i1 = workingset.nActiveConstr;
        for (int idx_col{0}; idx_col < i1; idx_col++) {
          idxEndIneq = qrmanager.ldq * idx_col;
          TYPE = workingset.ldA * idx_col;
          for (k = 0; k < nVar; k++) {
            qrmanager.QR[idxEndIneq + k] = workingset.ATwset[TYPE + k];
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
          idxEndIneq = workingset.nVar;
          TYPE = workingset.nActiveConstr;
          if (idxEndIneq <= TYPE) {
            TYPE = idxEndIneq;
          }
          qrmanager.minRowCol = TYPE;
          internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                   workingset.nActiveConstr, qrmanager.jpvt,
                                   qrmanager.tau);
        }
        idxEndIneq = 0;
        for (k = workingset.nActiveConstr - 1; k + 1 > nVar; k--) {
          idxEndIneq++;
          memspace.workspace_int[idxEndIneq - 1] = qrmanager.jpvt[k];
        }
        if (k + 1 <= workingset.nVar) {
          TYPE = k + qrmanager.ldq * k;
          while ((k + 1 > i) && (std::abs(qrmanager.QR[TYPE]) < tol)) {
            idxEndIneq++;
            memspace.workspace_int[idxEndIneq - 1] = qrmanager.jpvt[k];
            k--;
            TYPE = (TYPE - qrmanager.ldq) - 1;
          }
        }
        utils::countsort(memspace.workspace_int, idxEndIneq,
                         memspace.workspace_sort, i + 1,
                         workingset.nActiveConstr);
        for (k = idxEndIneq; k >= 1; k--) {
          WorkingSet::removeConstr(workingset, memspace.workspace_int[k - 1]);
        }
      }
      okWorkingSet = feasibleX0ForWorkingSet(
          memspace.workspace_double, solution.xstar, workingset, qrmanager);
      if (!okWorkingSet) {
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
      if (tol > OAGlobals::constraintTolerance) {
        solution.state = -2;
      }
    }
  } else {
    solution.state = -3;
    TYPE = (workingset.nWConstr[0] + workingset.nWConstr[1]) + 1;
    idxEndIneq = workingset.nActiveConstr;
    for (mTotalWorkingEq_tmp_tmp = TYPE; mTotalWorkingEq_tmp_tmp <= idxEndIneq;
         mTotalWorkingEq_tmp_tmp++) {
      workingset.isActiveConstr
          [(workingset
                .isActiveIdx[workingset.Wid[mTotalWorkingEq_tmp_tmp - 1] - 1] +
            workingset.Wlocalidx[mTotalWorkingEq_tmp_tmp - 1]) -
           2] = false;
    }
    workingset.nWConstr[2] = 0;
    workingset.nWConstr[3] = 0;
    workingset.nWConstr[4] = 0;
    workingset.nActiveConstr = workingset.nWConstr[0] + workingset.nWConstr[1];
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
