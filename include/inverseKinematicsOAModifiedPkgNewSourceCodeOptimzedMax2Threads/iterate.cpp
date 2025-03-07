//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: iterate.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "iterate.h"
#include "addBoundToActiveSetMatrix_.h"
#include "computeFval_ReuseHx.h"
#include "computeGrad_StoreHx.h"
#include "computeQ_.h"
#include "compute_deltax.h"
#include "deleteColMoveEnd.h"
#include "factorQR.h"
#include "feasibleX0ForWorkingSet.h"
#include "feasibleratiotest.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "maxConstraintViolation.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xnrm2.h"
#include "xrotg.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
// Function Definitions
//
// Arguments    : const array<double, 2U> &H
//                const array<double, 1U> &f
//                j_struct_T &solution
//                h_struct_T &memspace
//                d_struct_T &workingset
//                e_struct_T &qrmanager
//                f_struct_T &cholmanager
//                g_struct_T &objective
//                const char options_SolverName[7]
//                double options_StepTolerance
//                double options_ObjectiveLimit
//                int runTimeOptions_MaxIterations
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
void iterate(const array<double, 2U> &H, const array<double, 1U> &f,
             j_struct_T &solution, h_struct_T &memspace, d_struct_T &workingset,
             e_struct_T &qrmanager, f_struct_T &cholmanager,
             g_struct_T &objective, const char options_SolverName[7],
             double options_StepTolerance, double options_ObjectiveLimit,
             int runTimeOptions_MaxIterations)
{
  static const char b[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  double d;
  double s;
  int TYPE;
  int activeSetChangeID;
  int globalActiveConstrIdx;
  int i;
  int iy;
  int n;
  int nVar;
  boolean_T subProblemChanged;
  boolean_T updateFval;
  subProblemChanged = true;
  updateFval = true;
  activeSetChangeID = 0;
  TYPE = objective.objtype;
  nVar = workingset.nVar;
  globalActiveConstrIdx = 0;
  Objective::computeGrad_StoreHx(objective, H, f, solution.xstar);
  solution.fstar = Objective::computeFval_ReuseHx(
      objective, memspace.workspace_double, f, solution.xstar);
  if (solution.iterations < runTimeOptions_MaxIterations) {
    solution.state = -5;
  } else {
    solution.state = 0;
  }
  n = workingset.mConstrMax;
  if (static_cast<int>(n < 200)) {
    for (int k{0}; k < n; k++) {
      solution.lambda[k] = 0.0;
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int k = 0; k < n; k++) {
      solution.lambda[k] = 0.0;
    }
  }
  int exitg1;
  do {
    exitg1 = 0;
    if (solution.state == -5) {
      double temp;
      int Qk0;
      int b_iy;
      int b_k;
      int idx;
      int iyend;
      boolean_T guard1;
      boolean_T guard2;
      guard1 = false;
      guard2 = false;
      if (subProblemChanged) {
        switch (activeSetChangeID) {
        case 1: {
          double c;
          b_k = workingset.ldA * (workingset.nActiveConstr - 1);
          iyend = qrmanager.mrows;
          Qk0 = qrmanager.ncols + 1;
          if (iyend <= Qk0) {
            Qk0 = iyend;
          }
          qrmanager.minRowCol = Qk0;
          iy = qrmanager.ldq * qrmanager.ncols;
          Qk0 = qrmanager.ldq;
          if (qrmanager.mrows != 0) {
            iyend = iy + qrmanager.mrows;
            for (b_iy = iy + 1; b_iy <= iyend; b_iy++) {
              qrmanager.QR[b_iy - 1] = 0.0;
            }
            i = qrmanager.ldq * (qrmanager.mrows - 1) + 1;
            for (iyend = 1; Qk0 < 0 ? iyend >= i : iyend <= i; iyend += Qk0) {
              c = 0.0;
              n = (iyend + qrmanager.mrows) - 1;
              for (b_iy = iyend; b_iy <= n; b_iy++) {
                c += qrmanager.Q[b_iy - 1] *
                     workingset.ATwset[(b_k + b_iy) - iyend];
              }
              qrmanager.QR[iy] = qrmanager.QR[iy] + c;
              iy++;
            }
          }
          qrmanager.ncols++;
          i = qrmanager.ncols - 1;
          qrmanager.jpvt[i] = qrmanager.ncols;
          for (idx = qrmanager.mrows - 2; idx + 2 > qrmanager.ncols; idx--) {
            n = idx + qrmanager.ldq * i;
            d = qrmanager.QR[n + 1];
            c = internal::blas::xrotg(&qrmanager.QR[n], d, s);
            qrmanager.QR[n + 1] = d;
            Qk0 = qrmanager.ldq * idx;
            n = qrmanager.mrows;
            if (qrmanager.mrows >= 1) {
              b_iy = qrmanager.ldq + Qk0;
              for (b_k = 0; b_k < n; b_k++) {
                iyend = b_iy + b_k;
                iy = Qk0 + b_k;
                temp = c * qrmanager.Q[iy] + s * qrmanager.Q[iyend];
                qrmanager.Q[iyend] =
                    c * qrmanager.Q[iyend] - s * qrmanager.Q[iy];
                qrmanager.Q[iy] = temp;
              }
            }
          }
        } break;
        case -1:
          QRManager::deleteColMoveEnd(qrmanager, globalActiveConstrIdx);
          break;
        default:
          QRManager::factorQR(qrmanager, workingset.ATwset, nVar,
                              workingset.nActiveConstr, workingset.ldA);
          QRManager::computeQ_(qrmanager, qrmanager.mrows);
          break;
        }
        iyend = std::memcmp(&options_SolverName[0], &b[0], 7);
        compute_deltax(H, solution, memspace, qrmanager, cholmanager, objective,
                       iyend == 0);
        if (solution.state != -5) {
          exitg1 = 1;
        } else if ((internal::blas::xnrm2(nVar, solution.searchDir) <
                    options_StepTolerance) ||
                   (workingset.nActiveConstr >= nVar)) {
          guard2 = true;
        } else {
          temp = feasibleratiotest(
              solution.xstar, solution.searchDir, memspace.workspace_double,
              workingset.nVar, workingset.ldA, workingset.Aineq,
              workingset.bineq, workingset.lb, workingset.ub,
              workingset.indexLB, workingset.indexUB, workingset.sizes,
              workingset.isActiveIdx, workingset.isActiveConstr,
              workingset.nWConstr, TYPE == 5, updateFval, i, iy);
          if (updateFval) {
            switch (i) {
            case 3:
              workingset.nWConstr[2]++;
              workingset.isActiveConstr[(workingset.isActiveIdx[2] + iy) - 2] =
                  true;
              workingset.nActiveConstr++;
              workingset.Wid[workingset.nActiveConstr - 1] = 3;
              workingset.Wlocalidx[workingset.nActiveConstr - 1] = iy;
              iyend = workingset.ldA * (iy - 1);
              Qk0 = workingset.ldA * (workingset.nActiveConstr - 1);
              i = workingset.nVar - 1;
              for (idx = 0; idx <= i; idx++) {
                workingset.ATwset[Qk0 + idx] = workingset.Aineq[iyend + idx];
              }
              workingset.bwset[workingset.nActiveConstr - 1] =
                  workingset.bineq[iy - 1];
              break;
            case 4:
              WorkingSet::addBoundToActiveSetMatrix_(workingset, 4, iy);
              break;
            default:
              WorkingSet::addBoundToActiveSetMatrix_(workingset, 5, iy);
              break;
            }
            activeSetChangeID = 1;
          } else {
            if (objective.objtype == 5) {
              if (internal::blas::xnrm2(objective.nvar, solution.searchDir) >
                  100.0 * static_cast<double>(objective.nvar) *
                      1.4901161193847656E-8) {
                solution.state = 3;
              } else {
                solution.state = 4;
              }
            }
            subProblemChanged = false;
            if (workingset.nActiveConstr == 0) {
              solution.state = 1;
            }
          }
          if ((nVar >= 1) && (!(temp == 0.0))) {
            iyend = nVar - 1;
            for (b_k = 0; b_k <= iyend; b_k++) {
              solution.xstar[b_k] =
                  solution.xstar[b_k] + temp * solution.searchDir[b_k];
            }
          }
          Objective::computeGrad_StoreHx(objective, H, f, solution.xstar);
          updateFval = true;
          guard1 = true;
        }
      } else {
        for (b_k = 0; b_k < nVar; b_k++) {
          solution.searchDir[b_k] = 0.0;
        }
        guard2 = true;
      }
      if (guard2) {
        iy = qrmanager.ncols;
        if (qrmanager.ncols > 0) {
          boolean_T b_guard1;
          b_guard1 = false;
          if (objective.objtype != 4) {
            temp = 100.0 * static_cast<double>(qrmanager.mrows) *
                   2.2204460492503131E-16;
            if ((qrmanager.mrows > 0) && (qrmanager.ncols > 0)) {
              updateFval = true;
            } else {
              updateFval = false;
            }
            if (updateFval) {
              boolean_T b_guard2;
              idx = qrmanager.ncols;
              b_guard2 = false;
              if (qrmanager.mrows < qrmanager.ncols) {
                iyend = qrmanager.mrows + qrmanager.ldq * (qrmanager.ncols - 1);
                while ((idx > qrmanager.mrows) &&
                       (std::abs(qrmanager.QR[iyend - 1]) >= temp)) {
                  idx--;
                  iyend -= qrmanager.ldq;
                }
                updateFval = (idx == qrmanager.mrows);
                if (updateFval) {
                  b_guard2 = true;
                }
              } else {
                b_guard2 = true;
              }
              if (b_guard2) {
                iyend = idx + qrmanager.ldq * (idx - 1);
                while ((idx >= 1) &&
                       (std::abs(qrmanager.QR[iyend - 1]) >= temp)) {
                  idx--;
                  iyend = (iyend - qrmanager.ldq) - 1;
                }
                updateFval = (idx == 0);
              }
            }
            if (!updateFval) {
              solution.state = -7;
            } else {
              b_guard1 = true;
            }
          } else {
            b_guard1 = true;
          }
          if (b_guard1) {
            n = qrmanager.ncols;
            internal::blas::xgemv(qrmanager.mrows, qrmanager.ncols, qrmanager.Q,
                                  qrmanager.ldq, objective.grad,
                                  memspace.workspace_double);
            if (qrmanager.ncols != 0) {
              for (b_k = n; b_k >= 1; b_k--) {
                Qk0 = (b_k + (b_k - 1) * qrmanager.ldq) - 1;
                memspace.workspace_double[b_k - 1] =
                    memspace.workspace_double[b_k - 1] / qrmanager.QR[Qk0];
                for (b_iy = 0; b_iy <= b_k - 2; b_iy++) {
                  iyend = (b_k - b_iy) - 2;
                  memspace.workspace_double[iyend] =
                      memspace.workspace_double[iyend] -
                      memspace.workspace_double[b_k - 1] *
                          qrmanager.QR[(Qk0 - b_iy) - 1];
                }
              }
            }
            for (idx = 0; idx < iy; idx++) {
              solution.lambda[idx] = -memspace.workspace_double[idx];
            }
          }
        }
        if ((solution.state != -7) || (workingset.nActiveConstr > nVar)) {
          iyend = 0;
          temp = 0.0;
          i = (workingset.nWConstr[0] + workingset.nWConstr[1]) + 1;
          n = workingset.nActiveConstr;
          for (idx = i; idx <= n; idx++) {
            d = solution.lambda[idx - 1];
            if (d < temp) {
              temp = d;
              iyend = idx;
            }
          }
          if (iyend == 0) {
            solution.state = 1;
          } else {
            activeSetChangeID = -1;
            globalActiveConstrIdx = iyend;
            subProblemChanged = true;
            WorkingSet::removeConstr(workingset, iyend);
            solution.lambda[iyend - 1] = 0.0;
          }
        } else {
          iyend = workingset.nActiveConstr;
          activeSetChangeID = 0;
          globalActiveConstrIdx = workingset.nActiveConstr;
          subProblemChanged = true;
          WorkingSet::removeConstr(workingset, workingset.nActiveConstr);
          solution.lambda[iyend - 1] = 0.0;
        }
        updateFval = false;
        guard1 = true;
      }
      if (guard1) {
        solution.iterations++;
        iyend = objective.nvar - 1;
        if ((solution.iterations >= runTimeOptions_MaxIterations) &&
            ((solution.state != 1) || (objective.objtype == 5))) {
          solution.state = 0;
        }
        if (solution.iterations - solution.iterations / 50 * 50 == 0) {
          solution.maxConstr =
              WorkingSet::maxConstraintViolation(workingset, solution.xstar);
          temp = solution.maxConstr;
          if (objective.objtype == 5) {
            temp = solution.maxConstr - solution.xstar[iyend];
          }
          if (temp > OAGlobals::constraintTolerance) {
            boolean_T nonDegenerateWset;
            for (b_k = 0; b_k <= iyend; b_k++) {
              solution.searchDir[b_k] = solution.xstar[b_k];
            }
            nonDegenerateWset = initialize::feasibleX0ForWorkingSet(
                memspace.workspace_double, solution.searchDir, workingset,
                qrmanager);
            if ((!nonDegenerateWset) && (solution.state != 0)) {
              solution.state = -2;
            }
            activeSetChangeID = 0;
            temp = WorkingSet::maxConstraintViolation(workingset,
                                                      solution.searchDir);
            if (temp < solution.maxConstr) {
              for (idx = 0; idx <= iyend; idx++) {
                solution.xstar[idx] = solution.searchDir[idx];
              }
              solution.maxConstr = temp;
            }
          }
        }
        if (updateFval && (options_ObjectiveLimit > rtMinusInf)) {
          solution.fstar = Objective::computeFval_ReuseHx(
              objective, memspace.workspace_double, f, solution.xstar);
          if ((solution.fstar < options_ObjectiveLimit) &&
              ((solution.state != 0) || (objective.objtype != 5))) {
            solution.state = 2;
          }
        }
      }
    } else {
      if (!updateFval) {
        solution.fstar = Objective::computeFval_ReuseHx(
            objective, memspace.workspace_double, f, solution.xstar);
      }
      exitg1 = 1;
    }
  } while (exitg1 == 0);
}

} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for iterate.cpp
//
// [EOF]
//
