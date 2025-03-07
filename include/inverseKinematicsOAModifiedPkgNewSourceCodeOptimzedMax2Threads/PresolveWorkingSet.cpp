//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: PresolveWorkingSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
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
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
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
  int idxStartIneq;
  int idx_col;
  int mTotalWorkingEq;
  int nDepInd;
  int nVar;
  solution.state = 82;
  nVar = workingset.nVar - 1;
  mTotalWorkingEq = workingset.nWConstr[0] + workingset.nWConstr[1];
  nDepInd = 0;
  if (mTotalWorkingEq > 0) {
    for (idxStartIneq = 0; idxStartIneq < mTotalWorkingEq; idxStartIneq++) {
      for (idx_col = 0; idx_col <= nVar; idx_col++) {
        qrmanager.QR[idxStartIneq + qrmanager.ldq * idx_col] =
            workingset.ATwset[idx_col + workingset.ldA * idxStartIneq];
      }
    }
    nDepInd = ComputeNumDependentEq_(qrmanager, workingset.bwset,
                                     mTotalWorkingEq, workingset.nVar);
    if (nDepInd > 0) {
      for (idx_col = 0; idx_col < mTotalWorkingEq; idx_col++) {
        idxStartIneq = qrmanager.ldq * idx_col;
        TYPE = workingset.ldA * idx_col;
        for (int k{0}; k <= nVar; k++) {
          qrmanager.QR[idxStartIneq + k] = workingset.ATwset[TYPE + k];
        }
      }
      idxStartIneq = workingset.nWConstr[0];
      if (static_cast<int>(idxStartIneq < 200)) {
        for (int idx{0}; idx < idxStartIneq; idx++) {
          qrmanager.jpvt[idx] = 1;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = 0; idx < idxStartIneq; idx++) {
          qrmanager.jpvt[idx] = 1;
        }
      }
      i = workingset.nWConstr[0] + 1;
      if (static_cast<int>((mTotalWorkingEq - i) + 1 < 200)) {
        for (int idx{i}; idx <= mTotalWorkingEq; idx++) {
          qrmanager.jpvt[idx - 1] = 0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = i; idx <= mTotalWorkingEq; idx++) {
          qrmanager.jpvt[idx - 1] = 0;
        }
      }
      if (workingset.nVar * mTotalWorkingEq == 0) {
        qrmanager.mrows = workingset.nVar;
        qrmanager.ncols = mTotalWorkingEq;
        qrmanager.minRowCol = 0;
      } else {
        qrmanager.usedPivoting = true;
        qrmanager.mrows = workingset.nVar;
        qrmanager.ncols = mTotalWorkingEq;
        TYPE = workingset.nVar;
        if (TYPE > mTotalWorkingEq) {
          TYPE = mTotalWorkingEq;
        }
        qrmanager.minRowCol = TYPE;
        internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar, mTotalWorkingEq,
                                 qrmanager.jpvt, qrmanager.tau);
      }
      if (static_cast<int>(nDepInd < 200)) {
        for (int idx{0}; idx < nDepInd; idx++) {
          memspace.workspace_int[idx] =
              qrmanager.jpvt[(mTotalWorkingEq - nDepInd) + idx];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = 0; idx < nDepInd; idx++) {
          memspace.workspace_int[idx] =
              qrmanager.jpvt[(mTotalWorkingEq - nDepInd) + idx];
        }
      }
      utils::countsort(memspace.workspace_int, nDepInd, memspace.workspace_sort,
                       1, mTotalWorkingEq);
      for (idx_col = nDepInd; idx_col >= 1; idx_col--) {
        i = workingset.nWConstr[0] + workingset.nWConstr[1];
        if (i != 0) {
          idxStartIneq = memspace.workspace_int[idx_col - 1];
          if (idxStartIneq <= i) {
            if ((workingset.nActiveConstr == i) || (idxStartIneq == i)) {
              workingset.mEqRemoved++;
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[idxStartIneq - 1];
              WorkingSet::removeConstr(workingset, idxStartIneq);
            } else {
              workingset.mEqRemoved++;
              TYPE = workingset.Wid[idxStartIneq - 1] - 1;
              workingset.indexEqRemoved.data[workingset.mEqRemoved - 1] =
                  workingset.Wlocalidx[idxStartIneq - 1];
              workingset
                  .isActiveConstr[(workingset.isActiveIdx
                                       [workingset.Wid[idxStartIneq - 1] - 1] +
                                   workingset.Wlocalidx[idxStartIneq - 1]) -
                                  2] = false;
              workingset.Wid[idxStartIneq - 1] = workingset.Wid[i - 1];
              workingset.Wlocalidx[idxStartIneq - 1] =
                  workingset.Wlocalidx[i - 1];
              mTotalWorkingEq = workingset.nVar;
              for (int k{0}; k < mTotalWorkingEq; k++) {
                workingset.ATwset[k + workingset.ldA * (idxStartIneq - 1)] =
                    workingset.ATwset[k + workingset.ldA * (i - 1)];
              }
              workingset.bwset[idxStartIneq - 1] = workingset.bwset[i - 1];
              workingset.Wid[i - 1] =
                  workingset.Wid[workingset.nActiveConstr - 1];
              workingset.Wlocalidx[i - 1] =
                  workingset.Wlocalidx[workingset.nActiveConstr - 1];
              idxStartIneq = workingset.nVar;
              for (int k{0}; k < idxStartIneq; k++) {
                workingset.ATwset[k + workingset.ldA * (i - 1)] =
                    workingset.ATwset[k + workingset.ldA *
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
    idxStartIneq = workingset.nActiveConstr;
    mTotalWorkingEq = workingset.nWConstr[0] + workingset.nWConstr[1];
    nVar = workingset.nVar;
    if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
            workingset.nWConstr[4] >
        0) {
      tol =
          100.0 * static_cast<double>(workingset.nVar) * 2.2204460492503131E-16;
      if (static_cast<int>(mTotalWorkingEq < 200)) {
        for (int idx{0}; idx < mTotalWorkingEq; idx++) {
          qrmanager.jpvt[idx] = 1;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = 0; idx < mTotalWorkingEq; idx++) {
          qrmanager.jpvt[idx] = 1;
        }
      }
      i = mTotalWorkingEq + 1;
      if (static_cast<int>(idxStartIneq - mTotalWorkingEq < 200)) {
        for (int idx{i}; idx <= idxStartIneq; idx++) {
          qrmanager.jpvt[idx - 1] = 0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int idx = i; idx <= idxStartIneq; idx++) {
          qrmanager.jpvt[idx - 1] = 0;
        }
      }
      i = workingset.nActiveConstr;
      for (idx_col = 0; idx_col < i; idx_col++) {
        idxStartIneq = qrmanager.ldq * idx_col;
        TYPE = workingset.ldA * idx_col;
        for (int k{0}; k < nVar; k++) {
          qrmanager.QR[idxStartIneq + k] = workingset.ATwset[TYPE + k];
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
        TYPE = workingset.nVar;
        idxStartIneq = workingset.nActiveConstr;
        if (TYPE <= idxStartIneq) {
          idxStartIneq = TYPE;
        }
        qrmanager.minRowCol = idxStartIneq;
        internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                 workingset.nActiveConstr, qrmanager.jpvt,
                                 qrmanager.tau);
      }
      TYPE = 0;
      for (idx_col = workingset.nActiveConstr - 1; idx_col + 1 > nVar;
           idx_col--) {
        TYPE++;
        memspace.workspace_int[TYPE - 1] = qrmanager.jpvt[idx_col];
      }
      if (idx_col + 1 <= workingset.nVar) {
        idxStartIneq = idx_col + qrmanager.ldq * idx_col;
        while ((idx_col + 1 > mTotalWorkingEq) &&
               (std::abs(qrmanager.QR[idxStartIneq]) < tol)) {
          TYPE++;
          memspace.workspace_int[TYPE - 1] = qrmanager.jpvt[idx_col];
          idx_col--;
          idxStartIneq = (idxStartIneq - qrmanager.ldq) - 1;
        }
      }
      utils::countsort(memspace.workspace_int, TYPE, memspace.workspace_sort,
                       mTotalWorkingEq + 1, workingset.nActiveConstr);
      for (idx_col = TYPE; idx_col >= 1; idx_col--) {
        WorkingSet::removeConstr(workingset,
                                 memspace.workspace_int[idx_col - 1]);
      }
    }
    okWorkingSet = feasibleX0ForWorkingSet(
        memspace.workspace_double, solution.xstar, workingset, qrmanager);
    guard1 = false;
    if (!okWorkingSet) {
      idxStartIneq = workingset.nActiveConstr;
      mTotalWorkingEq = workingset.nWConstr[0] + workingset.nWConstr[1];
      nVar = workingset.nVar;
      if ((workingset.nWConstr[2] + workingset.nWConstr[3]) +
              workingset.nWConstr[4] >
          0) {
        tol = 1000.0 * static_cast<double>(workingset.nVar) *
              2.2204460492503131E-16;
        if (static_cast<int>(mTotalWorkingEq < 200)) {
          for (int idx{0}; idx < mTotalWorkingEq; idx++) {
            qrmanager.jpvt[idx] = 1;
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int idx = 0; idx < mTotalWorkingEq; idx++) {
            qrmanager.jpvt[idx] = 1;
          }
        }
        i = mTotalWorkingEq + 1;
        if (static_cast<int>(idxStartIneq - mTotalWorkingEq < 200)) {
          for (int idx{i}; idx <= idxStartIneq; idx++) {
            qrmanager.jpvt[idx - 1] = 0;
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int idx = i; idx <= idxStartIneq; idx++) {
            qrmanager.jpvt[idx - 1] = 0;
          }
        }
        i = workingset.nActiveConstr;
        for (idx_col = 0; idx_col < i; idx_col++) {
          idxStartIneq = qrmanager.ldq * idx_col;
          TYPE = workingset.ldA * idx_col;
          for (int k{0}; k < nVar; k++) {
            qrmanager.QR[idxStartIneq + k] = workingset.ATwset[TYPE + k];
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
          TYPE = workingset.nVar;
          idxStartIneq = workingset.nActiveConstr;
          if (TYPE <= idxStartIneq) {
            idxStartIneq = TYPE;
          }
          qrmanager.minRowCol = idxStartIneq;
          internal::lapack::xgeqp3(qrmanager.QR, workingset.nVar,
                                   workingset.nActiveConstr, qrmanager.jpvt,
                                   qrmanager.tau);
        }
        TYPE = 0;
        for (idx_col = workingset.nActiveConstr - 1; idx_col + 1 > nVar;
             idx_col--) {
          TYPE++;
          memspace.workspace_int[TYPE - 1] = qrmanager.jpvt[idx_col];
        }
        if (idx_col + 1 <= workingset.nVar) {
          idxStartIneq = idx_col + qrmanager.ldq * idx_col;
          while ((idx_col + 1 > mTotalWorkingEq) &&
                 (std::abs(qrmanager.QR[idxStartIneq]) < tol)) {
            TYPE++;
            memspace.workspace_int[TYPE - 1] = qrmanager.jpvt[idx_col];
            idx_col--;
            idxStartIneq = (idxStartIneq - qrmanager.ldq) - 1;
          }
        }
        utils::countsort(memspace.workspace_int, TYPE, memspace.workspace_sort,
                         mTotalWorkingEq + 1, workingset.nActiveConstr);
        for (idx_col = TYPE; idx_col >= 1; idx_col--) {
          WorkingSet::removeConstr(workingset,
                                   memspace.workspace_int[idx_col - 1]);
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
    idxStartIneq = (workingset.nWConstr[0] + workingset.nWConstr[1]) + 1;
    TYPE = workingset.nActiveConstr;
    for (mTotalWorkingEq = idxStartIneq; mTotalWorkingEq <= TYPE;
         mTotalWorkingEq++) {
      workingset.isActiveConstr
          [(workingset.isActiveIdx[workingset.Wid[mTotalWorkingEq - 1] - 1] +
            workingset.Wlocalidx[mTotalWorkingEq - 1]) -
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
