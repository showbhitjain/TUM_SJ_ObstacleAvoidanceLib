//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleX0ForWorkingSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "feasibleX0ForWorkingSet.h"
#include "computeQ_.h"
#include "factorQR.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "maxConstraintViolation.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xzgeqp3.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include "omp.h"
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
  array<double, 2U> B;
  double b_constrViolation_minnormXPrime;
  double b_vPrime;
  double c;
  double c_constrViolation_minnormXPrime;
  double constrViolation_minnormXPrime;
  double v;
  double vPrime;
  double x;
  int ar;
  int exitg1;
  int i;
  int i1;
  int iAcol;
  int idx;
  int idxLB;
  int iy;
  int k;
  int ldq;
  int ldw;
  int mFixed;
  int mLB;
  int mUB;
  int mWConstr;
  int minmn;
  int nVar;
  boolean_T nonDegenerateWset;
  mWConstr = workingset.nActiveConstr;
  nVar = workingset.nVar;
  nonDegenerateWset = true;
  if (mWConstr != 0) {
    if (static_cast<int>(mWConstr < 200)) {
      for (idx = 0; idx < mWConstr; idx++) {
        x = workingset.bwset[idx];
        workspace[idx] = x;
        workspace[idx + workspace.size(0)] = x;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(x)

      for (idx = 0; idx < mWConstr; idx++) {
        x = workingset.bwset[idx];
        workspace[idx] = x;
        workspace[idx + workspace.size(0)] = x;
      }
    }
    iAcol = workingset.ldA;
    if ((nVar != 0) && (mWConstr != 0)) {
      iy = 0;
      i = workingset.ldA * (mWConstr - 1) + 1;
      for (minmn = 1; iAcol < 0 ? minmn >= i : minmn <= i; minmn += iAcol) {
        c = 0.0;
        i1 = (minmn + nVar) - 1;
        for (ar = minmn; ar <= i1; ar++) {
          c += workingset.ATwset[ar - 1] * xCurrent[ar - minmn];
        }
        workspace[iy] = workspace[iy] - c;
        iy++;
      }
    }
    if (mWConstr >= nVar) {
      for (minmn = 0; minmn < nVar; minmn++) {
        iAcol = qrmanager.ldq * minmn;
        for (iy = 0; iy < mWConstr; iy++) {
          qrmanager.QR[iy + iAcol] =
              workingset.ATwset[minmn + workingset.ldA * iy];
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
        if (static_cast<int>(nVar < 200)) {
          for (idx = 0; idx < nVar; idx++) {
            qrmanager.jpvt[idx] = idx + 1;
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (idx = 0; idx < nVar; idx++) {
            qrmanager.jpvt[idx] = idx + 1;
          }
        }
        if (mWConstr <= nVar) {
          minmn = mWConstr;
        } else {
          minmn = nVar;
        }
        qrmanager.minRowCol = minmn;
        B.set_size(qrmanager.QR.size(0), qrmanager.QR.size(1));
        iAcol = qrmanager.QR.size(0) * qrmanager.QR.size(1);
        if (static_cast<int>(iAcol < 200)) {
          for (idxLB = 0; idxLB < iAcol; idxLB++) {
            B[idxLB] = qrmanager.QR[idxLB];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (idxLB = 0; idxLB < iAcol; idxLB++) {
            B[idxLB] = qrmanager.QR[idxLB];
          }
        }
        iAcol = qrmanager.QR.size(0);
        iy = qrmanager.QR.size(1);
        if (iAcol <= iy) {
          iy = iAcol;
        }
        qrmanager.tau.set_size(iy);
        if (static_cast<int>(iy < 200)) {
          for (idxLB = 0; idxLB < iy; idxLB++) {
            qrmanager.tau[idxLB] = 0.0;
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (idxLB = 0; idxLB < iy; idxLB++) {
            qrmanager.tau[idxLB] = 0.0;
          }
        }
        if (minmn >= 1) {
          internal::reflapack::qrf(B, mWConstr, nVar, minmn, qrmanager.tau);
        }
        qrmanager.QR.set_size(B.size(0), B.size(1));
        iAcol = B.size(0) * B.size(1);
        if (static_cast<int>(iAcol < 200)) {
          for (idxLB = 0; idxLB < iAcol; idxLB++) {
            qrmanager.QR[idxLB] = B[idxLB];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (idxLB = 0; idxLB < iAcol; idxLB++) {
            qrmanager.QR[idxLB] = B[idxLB];
          }
        }
      }
      QRManager::computeQ_(qrmanager, qrmanager.mrows);
      ldq = qrmanager.ldq;
      ldw = workspace.size(0);
      B.set_size(workspace.size(0), workspace.size(1));
      iAcol = workspace.size(0) * workspace.size(1);
      if (static_cast<int>(iAcol < 200)) {
        for (idxLB = 0; idxLB < iAcol; idxLB++) {
          B[idxLB] = workspace[idxLB];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (idxLB = 0; idxLB < iAcol; idxLB++) {
          B[idxLB] = workspace[idxLB];
        }
      }
      if (nVar != 0) {
        for (mUB = 0; ldw < 0 ? mUB >= ldw : mUB <= ldw; mUB += ldw) {
          i = mUB + 1;
          i1 = mUB + nVar;
          for (mFixed = i; mFixed <= i1; mFixed++) {
            workspace[mFixed - 1] = 0.0;
          }
        }
        iAcol = -1;
        for (mUB = 0; ldw < 0 ? mUB >= ldw : mUB <= ldw; mUB += ldw) {
          ar = -1;
          i = mUB + 1;
          i1 = mUB + nVar;
          for (mFixed = i; mFixed <= i1; mFixed++) {
            c = 0.0;
            for (minmn = 0; minmn < mWConstr; minmn++) {
              c += qrmanager.Q[(minmn + ar) + 1] * B[(minmn + iAcol) + 1];
            }
            workspace[mFixed - 1] = workspace[mFixed - 1] + c;
            ar += ldq;
          }
          iAcol += ldw;
        }
      }
      for (ar = 0; ar < 2; ar++) {
        iy = ldw * ar - 1;
        for (k = nVar; k >= 1; k--) {
          minmn = ldq * (k - 1) - 1;
          i = k + iy;
          c = workspace[i];
          if (c != 0.0) {
            workspace[i] = c / qrmanager.QR[k + minmn];
            for (mLB = 0; mLB <= k - 2; mLB++) {
              i1 = (mLB + iy) + 1;
              workspace[i1] = workspace[i1] -
                              workspace[i] * qrmanager.QR[(mLB + minmn) + 1];
            }
          }
        }
      }
    } else {
      QRManager::factorQR(qrmanager, workingset.ATwset, nVar, mWConstr,
                          workingset.ldA);
      QRManager::computeQ_(qrmanager, qrmanager.minRowCol);
      ldq = qrmanager.ldq;
      ldw = workspace.size(0);
      for (ar = 0; ar < 2; ar++) {
        iy = ldw * ar;
        for (mLB = 0; mLB < mWConstr; mLB++) {
          iAcol = ldq * mLB;
          minmn = mLB + iy;
          c = workspace[minmn];
          for (k = 0; k < mLB; k++) {
            c -= qrmanager.QR[k + iAcol] * workspace[k + iy];
          }
          workspace[minmn] = c / qrmanager.QR[mLB + iAcol];
        }
      }
      B.set_size(workspace.size(0), workspace.size(1));
      iAcol = workspace.size(0) * workspace.size(1);
      if (static_cast<int>(iAcol < 200)) {
        for (idxLB = 0; idxLB < iAcol; idxLB++) {
          B[idxLB] = workspace[idxLB];
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (idxLB = 0; idxLB < iAcol; idxLB++) {
          B[idxLB] = workspace[idxLB];
        }
      }
      if (nVar != 0) {
        for (mUB = 0; ldw < 0 ? mUB >= ldw : mUB <= ldw; mUB += ldw) {
          i = mUB + 1;
          i1 = mUB + nVar;
          for (mFixed = i; mFixed <= i1; mFixed++) {
            workspace[mFixed - 1] = 0.0;
          }
        }
        iAcol = 0;
        for (mUB = 0; ldw < 0 ? mUB >= ldw : mUB <= ldw; mUB += ldw) {
          ar = -1;
          i = iAcol + 1;
          i1 = iAcol + mWConstr;
          for (mLB = i; mLB <= i1; mLB++) {
            minmn = mUB + 1;
            iy = mUB + nVar;
            for (mFixed = minmn; mFixed <= iy; mFixed++) {
              workspace[mFixed - 1] =
                  workspace[mFixed - 1] +
                  B[mLB - 1] * qrmanager.Q[(ar + mFixed) - mUB];
            }
            ar += ldq;
          }
          iAcol += ldw;
        }
      }
    }
    iy = 0;
    do {
      exitg1 = 0;
      if (iy <= nVar - 1) {
        c = workspace[iy];
        if (std::isinf(c) || std::isnan(c)) {
          nonDegenerateWset = false;
          exitg1 = 1;
        } else {
          c = workspace[iy + workspace.size(0)];
          if (std::isinf(c) || std::isnan(c)) {
            nonDegenerateWset = false;
            exitg1 = 1;
          } else {
            iy++;
          }
        }
      } else {
        if (nVar >= 1) {
          iAcol = nVar - 1;
          if (static_cast<int>(nVar < 200)) {
            for (idxLB = 0; idxLB <= iAcol; idxLB++) {
              workspace[idxLB] = workspace[idxLB] + xCurrent[idxLB];
            }
          } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

            for (idxLB = 0; idxLB <= iAcol; idxLB++) {
              workspace[idxLB] = workspace[idxLB] + xCurrent[idxLB];
            }
          }
        }
        mLB = workingset.sizes[3];
        mUB = workingset.sizes[4];
        mFixed = workingset.sizes[0];
        if (workingset.probType == 2) {
          v = 0.0;
          minmn = workingset.sizes[2] - 1;
          ar = workingset.sizes[1] - 1;
          if (workingset.Aineq.size(0) != 0) {
            if (static_cast<int>(minmn + 1 < 200)) {
              for (idxLB = 0; idxLB <= minmn; idxLB++) {
                workingset.maxConstrWorkspace[idxLB] = workingset.bineq[idxLB];
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (idxLB = 0; idxLB <= minmn; idxLB++) {
                workingset.maxConstrWorkspace[idxLB] = workingset.bineq[idxLB];
              }
            }
            internal::blas::xgemv(workingset.nVarOrig, workingset.sizes[2],
                                  workingset.Aineq, workingset.ldA, workspace,
                                  workingset.maxConstrWorkspace);
            for (iy = 0; iy <= minmn; iy++) {
              workingset.maxConstrWorkspace[iy] =
                  workingset.maxConstrWorkspace[iy] -
                  workspace[workingset.nVarOrig + iy];
              v = std::fmax(v, workingset.maxConstrWorkspace[iy]);
            }
          }
          if (workingset.Aeq.size(0) != 0) {
            for (k = 0; k <= ar; k++) {
              workingset.maxConstrWorkspace[k] = workingset.beq.data[k];
            }
            internal::blas::xgemv(workingset.nVarOrig, workingset.sizes[1],
                                  workingset.Aeq, workingset.ldA, workspace,
                                  workingset.maxConstrWorkspace);
            iAcol = workingset.nVarOrig + workingset.sizes[2];
            minmn = iAcol + workingset.sizes[1];
            if (static_cast<int>(ar + 1 < 200)) {
              for (idx = 0; idx <= ar; idx++) {
                x = (workingset.maxConstrWorkspace[idx] -
                     workspace[iAcol + idx]) +
                    workspace[minmn + idx];
                workingset.maxConstrWorkspace[idx] = x;
                v = std::fmax(v, std::abs(x));
              }
            } else {
#pragma omp parallel num_threads(4 > omp_get_max_threads()                     \
                                     ? omp_get_max_threads()                   \
                                     : 4) private(b_vPrime, x)
              {
                b_vPrime = rtMinusInf;
#pragma omp for nowait
                for (idx = 0; idx <= ar; idx++) {
                  x = (workingset.maxConstrWorkspace[idx] -
                       workspace[iAcol + idx]) +
                      workspace[minmn + idx];
                  workingset.maxConstrWorkspace[idx] = x;
                  b_vPrime = std::fmax(b_vPrime, std::abs(x));
                }
                omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
                {

                  v = std::fmax(v, b_vPrime);
                }
                omp_unset_nest_lock(
                    &inverseKinematicsOAModified_nestLockGlobal);
              }
            }
          }
        } else {
          v = 0.0;
          minmn = workingset.sizes[2] - 1;
          ar = workingset.sizes[1] - 1;
          if (workingset.Aineq.size(0) != 0) {
            i = (minmn + 1 < 200);
            if (i) {
              for (idxLB = 0; idxLB <= minmn; idxLB++) {
                workingset.maxConstrWorkspace[idxLB] = workingset.bineq[idxLB];
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (idxLB = 0; idxLB <= minmn; idxLB++) {
                workingset.maxConstrWorkspace[idxLB] = workingset.bineq[idxLB];
              }
            }
            internal::blas::xgemv(workingset.nVar, workingset.sizes[2],
                                  workingset.Aineq, workingset.ldA, workspace,
                                  workingset.maxConstrWorkspace);
            if (i) {
              for (idx = 0; idx <= minmn; idx++) {
                v = std::fmax(v, workingset.maxConstrWorkspace[idx]);
              }
            } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(b_vPrime)
              {
                b_vPrime = rtMinusInf;
#pragma omp for nowait
                for (idx = 0; idx <= minmn; idx++) {
                  b_vPrime =
                      std::fmax(b_vPrime, workingset.maxConstrWorkspace[idx]);
                }
                omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
                {

                  v = std::fmax(v, b_vPrime);
                }
                omp_unset_nest_lock(
                    &inverseKinematicsOAModified_nestLockGlobal);
              }
            }
          }
          if (workingset.Aeq.size(0) != 0) {
            for (k = 0; k <= ar; k++) {
              workingset.maxConstrWorkspace[k] = workingset.beq.data[k];
            }
            internal::blas::xgemv(workingset.nVar, workingset.sizes[1],
                                  workingset.Aeq, workingset.ldA, workspace,
                                  workingset.maxConstrWorkspace);
            if (static_cast<int>(ar + 1 < 200)) {
              for (idx = 0; idx <= ar; idx++) {
                v = std::fmax(v, std::abs(workingset.maxConstrWorkspace[idx]));
              }
            } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(vPrime, x)
              {
                vPrime = rtMinusInf;
#pragma omp for nowait
                for (idx = 0; idx <= ar; idx++) {
                  x = workingset.maxConstrWorkspace[idx];
                  vPrime = std::fmax(vPrime, std::abs(x));
                }
                omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
                {

                  v = std::fmax(v, vPrime);
                }
                omp_unset_nest_lock(
                    &inverseKinematicsOAModified_nestLockGlobal);
              }
            }
          }
        }
        if (workingset.sizes[3] > 0) {
          if (static_cast<int>(mLB < 200)) {
            for (idx = 0; idx < mLB; idx++) {
              idxLB = workingset.indexLB[idx] - 1;
              v = std::fmax(v, -workspace[idxLB] - workingset.lb[idxLB]);
            }
          } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads()                                                  \
        ? omp_get_max_threads()                                                \
        : 2) private(constrViolation_minnormXPrime, idxLB)
            {
              constrViolation_minnormXPrime = rtMinusInf;
#pragma omp for nowait
              for (idx = 0; idx < mLB; idx++) {
                idxLB = workingset.indexLB[idx] - 1;
                constrViolation_minnormXPrime =
                    std::fmax(constrViolation_minnormXPrime,
                              -workspace[idxLB] - workingset.lb[idxLB]);
              }
              omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
              {

                v = std::fmax(v, constrViolation_minnormXPrime);
              }
              omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
            }
          }
        }
        if (workingset.sizes[4] > 0) {
          if (static_cast<int>(mUB < 200)) {
            for (idx = 0; idx < mUB; idx++) {
              idxLB = workingset.indexUB[idx] - 1;
              v = std::fmax(v, workspace[idxLB] - workingset.ub[idxLB]);
            }
          } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads()                                                  \
        ? omp_get_max_threads()                                                \
        : 2) private(b_constrViolation_minnormXPrime, idxLB)
            {
              b_constrViolation_minnormXPrime = rtMinusInf;
#pragma omp for nowait
              for (idx = 0; idx < mUB; idx++) {
                idxLB = workingset.indexUB[idx] - 1;
                b_constrViolation_minnormXPrime =
                    std::fmax(b_constrViolation_minnormXPrime,
                              workspace[idxLB] - workingset.ub[idxLB]);
              }
              omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
              {

                v = std::fmax(v, b_constrViolation_minnormXPrime);
              }
              omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
            }
          }
        }
        if (workingset.sizes[0] > 0) {
          if (static_cast<int>(mFixed < 200)) {
            for (idx = 0; idx < mFixed; idx++) {
              v = std::fmax(
                  v, std::abs(workspace[workingset.indexFixed[idx] - 1] -
                              workingset.ub[workingset.indexFixed[idx] - 1]));
            }
          } else {
#pragma omp parallel num_threads(                                              \
    4 > omp_get_max_threads() ? omp_get_max_threads()                          \
                              : 4) private(c_constrViolation_minnormXPrime)
            {
              c_constrViolation_minnormXPrime = rtMinusInf;
#pragma omp for nowait
              for (idx = 0; idx < mFixed; idx++) {
                c_constrViolation_minnormXPrime = std::fmax(
                    c_constrViolation_minnormXPrime,
                    std::abs(workspace[workingset.indexFixed[idx] - 1] -
                             workingset.ub[workingset.indexFixed[idx] - 1]));
              }
              omp_set_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
              {

                v = std::fmax(v, c_constrViolation_minnormXPrime);
              }
              omp_unset_nest_lock(&inverseKinematicsOAModified_nestLockGlobal);
            }
          }
        }
        c = WorkingSet::maxConstraintViolation(workingset, workspace,
                                               workspace.size(0) + 1);
        if ((v <= 2.2204460492503131E-16) || (v < c)) {
          if (static_cast<int>(nVar < 200)) {
            for (idxLB = 0; idxLB < nVar; idxLB++) {
              xCurrent[idxLB] = workspace[idxLB];
            }
          } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

            for (idxLB = 0; idxLB < nVar; idxLB++) {
              xCurrent[idxLB] = workspace[idxLB];
            }
          }
        } else if (static_cast<int>(nVar < 200)) {
          for (idxLB = 0; idxLB < nVar; idxLB++) {
            xCurrent[idxLB] = workspace[workspace.size(0) + idxLB];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (idxLB = 0; idxLB < nVar; idxLB++) {
            xCurrent[idxLB] = workspace[workspace.size(0) + idxLB];
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
