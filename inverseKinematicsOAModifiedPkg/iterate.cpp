//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: iterate.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "iterate.h"
#include "addBoundToActiveSetMatrix_.h"
#include "computeFval_ReuseHx.h"
#include "computeGrad_StoreHx.h"
#include "computeQ_.h"
#include "compute_deltax.h"
#include "deleteColMoveEnd.h"
#include "eml_int_forloop_overflow_check.h"
#include "factorQR.h"
#include "feasibleX0ForWorkingSet.h"
#include "feasibleratiotest.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "maxConstraintViolation.h"
#include "moveConstraint_.h"
#include "rt_nonfinite.h"
#include "squareQ_appendCol.h"
#include "xgemv.h"
#include "xnrm2.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

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
  static rtBoundsCheckInfo ab_emlrtBCI{
      -1,                           // iFirst
      -1,                           // iLast
      1,                            // lineNo
      1,                            // colNo
      "",                           // aName
      "checkStoppingAndUpdateFval", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "+stopping/checkStoppingAndUpdateFval.p", // pName
      0                                         // checkKind
  };
  static rtBoundsCheckInfo bb_emlrtBCI{
      -1,        // iFirst
      -1,        // iLast
      1,         // lineNo
      1,         // colNo
      "",        // aName
      "iterate", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "iterate.p", // pName
      0            // checkKind
  };
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "isNonDegenerate", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+QRManager/"
      "isNonDegenerate.p", // pName
      0                    // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,               // iFirst
      -1,               // iLast
      1,                // lineNo
      1,                // colNo
      "",               // aName
      "compute_lambda", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "compute_lambda.p", // pName
      0                   // checkKind
  };
  static rtBoundsCheckInfo y_emlrtBCI{
      -1,                // iFirst
      -1,                // iLast
      1,                 // lineNo
      1,                 // colNo
      "",                // aName
      "find_neg_lambda", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "find_neg_lambda.p", // pName
      0                    // checkKind
  };
  static const char b[7]{'f', 'm', 'i', 'n', 'c', 'o', 'n'};
  int TYPE;
  int activeSetChangeID;
  int globalActiveConstrIdx;
  int i;
  int iAw0;
  int idx_local;
  int nVar;
  int ret;
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
  ret = workingset.mConstrMax;
  if (workingset.mConstrMax > 2147483646) {
    check_forloop_overflow_error();
  }
  for (iAw0 = 0; iAw0 < ret; iAw0++) {
    solution.lambda[iAw0] = 0.0;
  }
  int exitg1;
  do {
    exitg1 = 0;
    if (solution.state == -5) {
      double a;
      double normDelta;
      int i1;
      int idx;
      int idxMinLambda;
      int j;
      int nActiveConstr_tmp;
      boolean_T guard1;
      boolean_T guard2;
      guard1 = false;
      guard2 = false;
      if (subProblemChanged) {
        switch (activeSetChangeID) {
        case 1:
          QRManager::squareQ_appendCol(
              qrmanager, workingset.ATwset,
              workingset.ldA * (workingset.nActiveConstr - 1) + 1);
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
        ret = std::memcmp(&options_SolverName[0], &b[0], 7);
        compute_deltax(H, solution, memspace, qrmanager, cholmanager, objective,
                       ret == 0);
        if (solution.state != -5) {
          exitg1 = 1;
        } else {
          normDelta = internal::blas::xnrm2(nVar, solution.searchDir);
          if ((normDelta < options_StepTolerance) ||
              (workingset.nActiveConstr >= nVar)) {
            guard2 = true;
          } else {
            a = feasibleratiotest(
                solution.xstar, solution.searchDir, memspace.workspace_double,
                workingset.nVar, workingset.ldA, workingset.Aineq,
                workingset.bineq, workingset.lb, workingset.ub,
                workingset.indexLB, workingset.indexUB, workingset.sizes,
                workingset.isActiveIdx, workingset.isActiveConstr,
                workingset.nWConstr, TYPE == 5, updateFval, i, idx_local);
            if (updateFval) {
              switch (i) {
              case 3:
                workingset.nWConstr[2]++;
                i = workingset.isActiveConstr.size(0);
                i1 = (workingset.isActiveIdx[2] + idx_local) - 1;
                if ((i1 < 1) || (i1 > i)) {
                  rtDynamicBoundsError(i1, 1, i, q_emlrtBCI);
                }
                workingset.isActiveConstr[i1 - 1] = true;
                workingset.nActiveConstr++;
                i = workingset.Wid.size(0);
                if ((workingset.nActiveConstr < 1) ||
                    (workingset.nActiveConstr > i)) {
                  rtDynamicBoundsError(workingset.nActiveConstr, 1, i,
                                       q_emlrtBCI);
                }
                i = workingset.nActiveConstr - 1;
                workingset.Wid[i] = 3;
                i1 = workingset.Wlocalidx.size(0);
                if ((workingset.nActiveConstr < 1) ||
                    (workingset.nActiveConstr > i1)) {
                  rtDynamicBoundsError(workingset.nActiveConstr, 1, i1,
                                       q_emlrtBCI);
                }
                workingset.Wlocalidx[i] = idx_local;
                ret = workingset.ldA * (idx_local - 1) + 1;
                iAw0 = workingset.ldA * i + 1;
                i1 = workingset.nVar - 1;
                for (idx = 0; idx <= i1; idx++) {
                  idxMinLambda = workingset.Aineq.size(0);
                  nActiveConstr_tmp = ret + idx;
                  if ((nActiveConstr_tmp < 1) ||
                      (nActiveConstr_tmp > idxMinLambda)) {
                    rtDynamicBoundsError(nActiveConstr_tmp, 1, idxMinLambda,
                                         r_emlrtBCI);
                  }
                  idxMinLambda = workingset.ATwset.size(0);
                  j = iAw0 + idx;
                  if ((j < 1) || (j > idxMinLambda)) {
                    rtDynamicBoundsError(j, 1, idxMinLambda, r_emlrtBCI);
                  }
                  workingset.ATwset[j - 1] =
                      workingset.Aineq[nActiveConstr_tmp - 1];
                }
                i1 = workingset.bineq.size(0);
                if ((idx_local < 1) || (idx_local > i1)) {
                  rtDynamicBoundsError(idx_local, 1, i1, r_emlrtBCI);
                }
                i1 = workingset.bwset.size(0);
                if ((workingset.nActiveConstr < 1) ||
                    (workingset.nActiveConstr > i1)) {
                  rtDynamicBoundsError(workingset.nActiveConstr, 1, i1,
                                       r_emlrtBCI);
                }
                workingset.bwset[i] = workingset.bineq[idx_local - 1];
                break;
              case 4:
                WorkingSet::addBoundToActiveSetMatrix_(workingset, idx_local);
                break;
              default:
                WorkingSet::b_addBoundToActiveSetMatrix_(workingset, idx_local);
                break;
              }
              activeSetChangeID = 1;
            } else {
              if (objective.objtype == 5) {
                normDelta =
                    internal::blas::xnrm2(objective.nvar, solution.searchDir);
                if (normDelta > 100.0 * static_cast<double>(objective.nvar) *
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
            if ((nVar >= 1) && (!(a == 0.0))) {
              ret = nVar - 1;
              for (iAw0 = 0; iAw0 <= ret; iAw0++) {
                solution.xstar[iAw0] =
                    solution.xstar[iAw0] + a * solution.searchDir[iAw0];
              }
            }
            Objective::computeGrad_StoreHx(objective, H, f, solution.xstar);
            updateFval = true;
            guard1 = true;
          }
        }
      } else {
        if (nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (iAw0 = 0; iAw0 < nVar; iAw0++) {
          solution.searchDir[iAw0] = 0.0;
        }
        guard2 = true;
      }
      if (guard2) {
        nActiveConstr_tmp = qrmanager.ncols;
        if (qrmanager.ncols > 0) {
          boolean_T b_guard1;
          b_guard1 = false;
          if (objective.objtype != 4) {
            normDelta = 100.0 * static_cast<double>(qrmanager.mrows) *
                        2.2204460492503131E-16;
            if ((qrmanager.mrows > 0) && (qrmanager.ncols > 0)) {
              updateFval = true;
            } else {
              updateFval = false;
            }
            if (updateFval) {
              boolean_T b_guard2;
              boolean_T exitg2;
              idx = nActiveConstr_tmp;
              b_guard2 = false;
              if (qrmanager.mrows < qrmanager.ncols) {
                ret = qrmanager.mrows + qrmanager.ldq * (qrmanager.ncols - 1);
                exitg2 = false;
                while ((!exitg2) && (idx > qrmanager.mrows)) {
                  i = qrmanager.QR.size(0) * qrmanager.QR.size(1);
                  if ((ret < 1) || (ret > i)) {
                    rtDynamicBoundsError(ret, 1, i, w_emlrtBCI);
                  }
                  if (std::abs(qrmanager.QR[ret - 1]) >= normDelta) {
                    idx--;
                    ret -= qrmanager.ldq;
                  } else {
                    exitg2 = true;
                  }
                }
                updateFval = (idx == qrmanager.mrows);
                if (updateFval) {
                  b_guard2 = true;
                }
              } else {
                b_guard2 = true;
              }
              if (b_guard2) {
                ret = idx + qrmanager.ldq * (idx - 1);
                exitg2 = false;
                while ((!exitg2) && (idx >= 1)) {
                  i = qrmanager.QR.size(0) * qrmanager.QR.size(1);
                  if ((ret < 1) || (ret > i)) {
                    rtDynamicBoundsError(ret, 1, i, w_emlrtBCI);
                  }
                  if (std::abs(qrmanager.QR[ret - 1]) >= normDelta) {
                    idx--;
                    ret = (ret - qrmanager.ldq) - 1;
                  } else {
                    exitg2 = true;
                  }
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
            internal::blas::xgemv(qrmanager.mrows, qrmanager.ncols, qrmanager.Q,
                                  qrmanager.ldq, objective.grad,
                                  memspace.workspace_double);
            if (qrmanager.ncols != 0) {
              for (j = nActiveConstr_tmp; j >= 1; j--) {
                ret = (j + (j - 1) * qrmanager.ldq) - 1;
                memspace.workspace_double[j - 1] =
                    memspace.workspace_double[j - 1] / qrmanager.QR[ret];
                for (idxMinLambda = 0; idxMinLambda <= j - 2; idxMinLambda++) {
                  iAw0 = (j - idxMinLambda) - 2;
                  memspace.workspace_double[iAw0] =
                      memspace.workspace_double[iAw0] -
                      memspace.workspace_double[j - 1] *
                          qrmanager.QR[(ret - idxMinLambda) - 1];
                }
              }
            }
            if (qrmanager.ncols > 2147483646) {
              check_forloop_overflow_error();
            }
            i = memspace.workspace_double.size(0) *
                memspace.workspace_double.size(1);
            for (idx = 0; idx < nActiveConstr_tmp; idx++) {
              if ((idx + 1 < 1) || (idx + 1 > i)) {
                rtDynamicBoundsError(idx + 1, 1, i, x_emlrtBCI);
              }
              i1 = solution.lambda.size(0);
              if (idx + 1 > i1) {
                rtDynamicBoundsError(idx + 1, 1, i1, x_emlrtBCI);
              }
              solution.lambda[idx] = -memspace.workspace_double[idx];
            }
          }
        }
        if ((solution.state != -7) || (workingset.nActiveConstr > nVar)) {
          idxMinLambda = 0;
          normDelta = 0.0;
          ret = (workingset.nWConstr[0] + workingset.nWConstr[1]) + 1;
          iAw0 = workingset.nActiveConstr;
          if ((ret <= workingset.nActiveConstr) &&
              (workingset.nActiveConstr > 2147483646)) {
            check_forloop_overflow_error();
          }
          for (idx = ret; idx <= iAw0; idx++) {
            i = solution.lambda.size(0);
            if ((idx < 1) || (idx > i)) {
              rtDynamicBoundsError(idx, 1, i, y_emlrtBCI);
            }
            a = solution.lambda[idx - 1];
            if (a < normDelta) {
              i = solution.lambda.size(0);
              if (idx > i) {
                rtDynamicBoundsError(idx, 1, i, y_emlrtBCI);
              }
              normDelta = a;
              idxMinLambda = idx;
            }
          }
          if (idxMinLambda == 0) {
            solution.state = 1;
          } else {
            activeSetChangeID = -1;
            globalActiveConstrIdx = idxMinLambda;
            subProblemChanged = true;
            i = workingset.Wid.size(0);
            if (idxMinLambda > i) {
              rtDynamicBoundsError(idxMinLambda, 1, i, bb_emlrtBCI);
            }
            i = workingset.Wlocalidx.size(0);
            if (idxMinLambda > i) {
              rtDynamicBoundsError(idxMinLambda, 1, i, bb_emlrtBCI);
            }
            i = workingset.Wid.size(0);
            if (idxMinLambda > i) {
              rtDynamicBoundsError(idxMinLambda, 1, i, h_emlrtBCI);
            }
            ret = workingset.Wid[idxMinLambda - 1];
            i = workingset.Wlocalidx.size(0);
            if (idxMinLambda > i) {
              rtDynamicBoundsError(idxMinLambda, 1, i, h_emlrtBCI);
            }
            if ((ret < 1) || (ret > 6)) {
              rtDynamicBoundsError(workingset.Wid[idxMinLambda - 1], 1, 6,
                                   i_emlrtBCI);
            }
            i = workingset.isActiveConstr.size(0);
            i1 = (workingset.isActiveIdx[ret - 1] +
                  workingset.Wlocalidx[idxMinLambda - 1]) -
                 1;
            if ((i1 < 1) || (i1 > i)) {
              rtDynamicBoundsError(i1, 1, i, h_emlrtBCI);
            }
            workingset.isActiveConstr[i1 - 1] = false;
            WorkingSet::moveConstraint_(workingset, workingset.nActiveConstr,
                                        idxMinLambda);
            workingset.nActiveConstr--;
            if (ret > 5) {
              rtDynamicBoundsError(6, 1, 5, k_emlrtBCI);
            }
            workingset.nWConstr[ret - 1]--;
            i = solution.lambda.size(0);
            if (idxMinLambda > i) {
              rtDynamicBoundsError(idxMinLambda, 1, i, bb_emlrtBCI);
            }
            solution.lambda[idxMinLambda - 1] = 0.0;
          }
        } else {
          idxMinLambda = workingset.nActiveConstr;
          activeSetChangeID = 0;
          globalActiveConstrIdx = workingset.nActiveConstr;
          subProblemChanged = true;
          i = workingset.Wid.size(0);
          if ((workingset.nActiveConstr < 1) ||
              (workingset.nActiveConstr > i)) {
            rtDynamicBoundsError(workingset.nActiveConstr, 1, i, bb_emlrtBCI);
          }
          i = workingset.Wlocalidx.size(0);
          if ((workingset.nActiveConstr < 1) ||
              (workingset.nActiveConstr > i)) {
            rtDynamicBoundsError(workingset.nActiveConstr, 1, i, bb_emlrtBCI);
          }
          i = workingset.Wid.size(0);
          if ((workingset.nActiveConstr < 1) ||
              (workingset.nActiveConstr > i)) {
            rtDynamicBoundsError(workingset.nActiveConstr, 1, i, h_emlrtBCI);
          }
          ret = workingset.nActiveConstr - 1;
          iAw0 = workingset.Wid[ret];
          i = workingset.Wlocalidx.size(0);
          if ((workingset.nActiveConstr < 1) ||
              (workingset.nActiveConstr > i)) {
            rtDynamicBoundsError(workingset.nActiveConstr, 1, i, h_emlrtBCI);
          }
          if ((iAw0 < 1) || (iAw0 > 6)) {
            rtDynamicBoundsError(workingset.Wid[workingset.nActiveConstr - 1],
                                 1, 6, i_emlrtBCI);
          }
          i = workingset.isActiveConstr.size(0);
          i1 = (workingset.isActiveIdx[iAw0 - 1] + workingset.Wlocalidx[ret]) -
               1;
          if ((i1 < 1) || (i1 > i)) {
            rtDynamicBoundsError(i1, 1, i, h_emlrtBCI);
          }
          workingset.isActiveConstr[i1 - 1] = false;
          WorkingSet::moveConstraint_(workingset, workingset.nActiveConstr,
                                      workingset.nActiveConstr);
          workingset.nActiveConstr = ret;
          if (iAw0 > 5) {
            rtDynamicBoundsError(6, 1, 5, k_emlrtBCI);
          }
          workingset.nWConstr[iAw0 - 1]--;
          i = solution.lambda.size(0);
          if ((idxMinLambda < 1) || (idxMinLambda > i)) {
            rtDynamicBoundsError(idxMinLambda, 1, i, bb_emlrtBCI);
          }
          solution.lambda[idxMinLambda - 1] = 0.0;
        }
        updateFval = false;
        guard1 = true;
      }
      if (guard1) {
        solution.iterations++;
        ret = objective.nvar - 1;
        if ((solution.iterations >= runTimeOptions_MaxIterations) &&
            ((solution.state != 1) || (objective.objtype == 5))) {
          solution.state = 0;
        }
        if (solution.iterations - solution.iterations / 50 * 50 == 0) {
          solution.maxConstr =
              WorkingSet::maxConstraintViolation(workingset, solution.xstar);
          normDelta = solution.maxConstr;
          if (objective.objtype == 5) {
            i = solution.xstar.size(0);
            if ((objective.nvar < 1) || (objective.nvar > i)) {
              rtDynamicBoundsError(objective.nvar, 1, i, ab_emlrtBCI);
            }
            normDelta = solution.maxConstr - solution.xstar[ret];
          }
          if (normDelta > 1.0E-6) {
            boolean_T nonDegenerateWset;
            if (objective.nvar > 2147483646) {
              check_forloop_overflow_error();
            }
            for (iAw0 = 0; iAw0 <= ret; iAw0++) {
              solution.searchDir[iAw0] = solution.xstar[iAw0];
            }
            nonDegenerateWset = initialize::feasibleX0ForWorkingSet(
                memspace.workspace_double, solution.searchDir, workingset,
                qrmanager);
            if ((!nonDegenerateWset) && (solution.state != 0)) {
              solution.state = -2;
            }
            activeSetChangeID = 0;
            normDelta = WorkingSet::maxConstraintViolation(workingset,
                                                           solution.searchDir);
            if (normDelta < solution.maxConstr) {
              if (objective.nvar > 2147483646) {
                check_forloop_overflow_error();
              }
              for (idx = 0; idx <= ret; idx++) {
                i = solution.searchDir.size(0);
                if ((idx + 1 < 1) || (idx + 1 > i)) {
                  rtDynamicBoundsError(idx + 1, 1, i, ab_emlrtBCI);
                }
                i = solution.xstar.size(0);
                if (idx + 1 > i) {
                  rtDynamicBoundsError(idx + 1, 1, i, ab_emlrtBCI);
                }
                solution.xstar[idx] = solution.searchDir[idx];
              }
              solution.maxConstr = normDelta;
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
