//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: iterate.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
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
#include "inverseKinematics_internal_types.h"
#include "maxConstraintViolation.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "xrotg.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &H
//                const array<double, 1U> &f
//                h_struct_T &solution
//                g_struct_T &memspace
//                i_struct_T &workingset
//                d_struct_T &qrmanager
//                e_struct_T &cholmanager
//                f_struct_T &objective
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
             h_struct_T &solution, g_struct_T &memspace, i_struct_T &workingset,
             d_struct_T &qrmanager, e_struct_T &cholmanager,
             f_struct_T &objective, const char options_SolverName[7],
             double options_StepTolerance, double options_ObjectiveLimit,
             int runTimeOptions_MaxIterations)
{
  static const char b[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  array<double, 2U> *y;
  double s;
  int TYPE;
  int activeSetChangeID;
  int globalActiveConstrIdx;
  int i;
  int iyend;
  int k;
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
  for (k = 0; k < n; k++) {
    solution.lambda[k] = 0.0;
  }
  int exitg1;
  do {
    exitg1 = 0;
    if (solution.state == -5) {
      double c;
      double d;
      double temp;
      int b_iy;
      int i1;
      int idx;
      int ix0;
      int iy;
      boolean_T guard1;
      boolean_T guard2;
      guard1 = false;
      guard2 = false;
      if (subProblemChanged) {
        switch (activeSetChangeID) {
        case 1:
          ix0 = workingset.ldA * (workingset.nActiveConstr - 1);
          iyend = qrmanager.mrows;
          iy = qrmanager.ncols + 1;
          if (iyend <= iy) {
            iy = iyend;
          }
          qrmanager.minRowCol = iy;
          iy = qrmanager.ldq * qrmanager.ncols;
          k = qrmanager.ldq;
          if (qrmanager.mrows != 0) {
            iyend = iy + qrmanager.mrows;
            for (b_iy = iy + 1; b_iy <= iyend; b_iy++) {
              qrmanager.QR[b_iy - 1] = 0.0;
            }
            i = qrmanager.ldq * (qrmanager.mrows - 1) + 1;
            for (idx = 1; k < 0 ? idx >= i : idx <= i; idx += k) {
              c = 0.0;
              i1 = (idx + qrmanager.mrows) - 1;
              for (int ia{idx}; ia <= i1; ia++) {
                c += qrmanager.Q[ia - 1] * workingset.ATwset[(ix0 + ia) - idx];
              }
              qrmanager.QR[iy] = qrmanager.QR[iy] + c;
              iy++;
            }
          }
          qrmanager.ncols++;
          i = qrmanager.ncols - 1;
          qrmanager.jpvt[i] = qrmanager.ncols;
          for (idx = qrmanager.mrows - 2; idx + 2 > qrmanager.ncols; idx--) {
            i1 = idx + qrmanager.ldq * i;
            d = qrmanager.QR[i1 + 1];
            c = internal::blas::xrotg(&qrmanager.QR[i1], d, s);
            qrmanager.QR[i1 + 1] = d;
            iyend = qrmanager.ldq * idx;
            n = qrmanager.mrows;
            if (qrmanager.mrows >= 1) {
              b_iy = qrmanager.ldq + iyend;
              for (k = 0; k < n; k++) {
                iy = b_iy + k;
                ix0 = iyend + k;
                temp = c * qrmanager.Q[ix0] + s * qrmanager.Q[iy];
                qrmanager.Q[iy] = c * qrmanager.Q[iy] - s * qrmanager.Q[ix0];
                qrmanager.Q[ix0] = temp;
              }
            }
          }
          break;
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
              solution.xstar, solution.searchDir, workingset.nVar,
              workingset.Aineq.size[0], workingset.lb, workingset.ub,
              workingset.indexLB, workingset.indexUB, workingset.sizes,
              workingset.isActiveIdx, workingset.isActiveConstr,
              workingset.nWConstr, TYPE == 5, updateFval, i, iyend);
          if (updateFval) {
            switch (i) {
            case 3:
              workingset.nWConstr[2]++;
              workingset
                  .isActiveConstr[(workingset.isActiveIdx[2] + iyend) - 2] =
                  true;
              workingset.nActiveConstr++;
              workingset.Wid[workingset.nActiveConstr - 1] = 3;
              workingset.Wlocalidx[workingset.nActiveConstr - 1] = iyend;
              // A check that is always false is detected at compile-time.
              // Eliminating code that follows.
              break;
            case 4:
              WorkingSet::addBoundToActiveSetMatrix_(workingset, 4, iyend);
              break;
            default:
              WorkingSet::addBoundToActiveSetMatrix_(workingset, 5, iyend);
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
            for (k = 0; k <= iyend; k++) {
              solution.xstar[k] =
                  solution.xstar[k] + temp * solution.searchDir[k];
            }
          }
          Objective::computeGrad_StoreHx(objective, H, f, solution.xstar);
          updateFval = true;
          guard1 = true;
        }
      } else {
        for (k = 0; k < nVar; k++) {
          solution.searchDir[k] = 0.0;
        }
        guard2 = true;
      }
      if (guard2) {
        int nActiveConstr;
        nActiveConstr = qrmanager.ncols;
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
            iyend = qrmanager.mrows;
            iy = qrmanager.ncols;
            k = qrmanager.ldq;
            y = &memspace.workspace_double;
            if ((iyend != 0) && (iy != 0)) {
              for (b_iy = 0; b_iy < iy; b_iy++) {
                (*y)[b_iy] = 0.0;
              }
              b_iy = 0;
              i = k * (iy - 1) + 1;
              for (idx = 1; k < 0 ? idx >= i : idx <= i; idx += k) {
                c = 0.0;
                i1 = (idx + iyend) - 1;
                for (int ia{idx}; ia <= i1; ia++) {
                  c += qrmanager.Q[ia - 1] * objective.grad[ia - idx];
                }
                (*y)[b_iy] = (*y)[b_iy] + c;
                b_iy++;
              }
            }
            if (qrmanager.ncols != 0) {
              for (ix0 = n; ix0 >= 1; ix0--) {
                iy = (ix0 + (ix0 - 1) * qrmanager.ldq) - 1;
                memspace.workspace_double[ix0 - 1] =
                    memspace.workspace_double[ix0 - 1] / qrmanager.QR[iy];
                for (k = 0; k <= ix0 - 2; k++) {
                  iyend = (ix0 - k) - 2;
                  memspace.workspace_double[iyend] =
                      memspace.workspace_double[iyend] -
                      memspace.workspace_double[ix0 - 1] *
                          qrmanager.QR[(iy - k) - 1];
                }
              }
            }
            for (idx = 0; idx < nActiveConstr; idx++) {
              solution.lambda[idx] = -memspace.workspace_double[idx];
            }
          }
        }
        if ((solution.state != -7) || (workingset.nActiveConstr > nVar)) {
          iyend = 0;
          temp = 0.0;
          i = (workingset.nWConstr[0] + workingset.nWConstr[1]) + 1;
          i1 = workingset.nActiveConstr;
          for (idx = i; idx <= i1; idx++) {
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
          if (temp > 1.0E-6) {
            boolean_T nonDegenerateWset;
            for (k = 0; k <= iyend; k++) {
              solution.searchDir[k] = solution.xstar[k];
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
