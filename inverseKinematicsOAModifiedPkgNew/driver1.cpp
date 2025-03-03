//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver1.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "driver1.h"
#include "PresolveWorkingSet.h"
#include "computeFval.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "iterate.h"
#include "maxConstraintViolation.h"
#include "removeAllIneqConstr.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "coder_array.h"
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void e_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum);

// Function Definitions
//
// Arguments    : const char *r
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void e_rtErrorWithMessageID(const char *r, const char *aFcnName,
                                   int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "Expected ") << r) << " to be a square matrix.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const array<double, 2U> &H
//                const array<double, 1U> &f
//                j_struct_T &solution
//                h_struct_T &memspace
//                d_struct_T &workingset
//                e_struct_T &qrmanager
//                f_struct_T &cholmanager
//                g_struct_T &objective
//                k_struct_T &options
//                int runTimeOptions_MaxIterations
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
void driver(const array<double, 2U> &H, const array<double, 1U> &f,
            j_struct_T &solution, h_struct_T &memspace, d_struct_T &workingset,
            e_struct_T &qrmanager, f_struct_T &cholmanager,
            g_struct_T &objective, k_struct_T &options,
            int runTimeOptions_MaxIterations)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,            // iFirst
      -1,            // iLast
      1,             // lineNo
      1,             // colNo
      "",            // aName
      "snap_bounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "snap_bounds.p", // pName
      0                // checkKind
  };
  static rtBoundsCheckInfo x_emlrtBCI{
      -1,         // iFirst
      -1,         // iLast
      1,          // lineNo
      1,          // colNo
      "",         // aName
      "phaseone", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "phaseone.p", // pName
      0             // checkKind
  };
  static rtRunTimeErrorInfo e_emlrtRTEI{
      13,              // lineNo
      "validatesquare" // fName
  };
  int b;
  int i;
  int i1;
  int nVar_tmp_tmp;
  boolean_T guard1;
  if (H.size(0) != H.size(1)) {
    e_rtErrorWithMessageID("input", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
  if ((H.size(0) == 0) || (H.size(1) == 0)) {
    d_rtErrorWithMessageID("input", c_emlrtRTEI.fName, c_emlrtRTEI.lineNo);
  }
  solution.iterations = 0;
  nVar_tmp_tmp = workingset.nVar;
  guard1 = false;
  if (workingset.probType == 3) {
    b = workingset.sizes[0];
    if (workingset.sizes[0] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b; idx++) {
      i = workingset.indexFixed.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
      }
      i = workingset.ub.size(0);
      if ((workingset.indexFixed[idx] < 1) ||
          (workingset.indexFixed[idx] > i)) {
        rtDynamicBoundsError(workingset.indexFixed[idx], 1, i, w_emlrtBCI);
      }
      i = solution.xstar.size(0);
      if ((workingset.indexFixed[idx] < 1) ||
          (workingset.indexFixed[idx] > i)) {
        rtDynamicBoundsError(workingset.indexFixed[idx], 1, i, w_emlrtBCI);
      }
      solution.xstar[workingset.indexFixed[idx] - 1] =
          workingset.ub[workingset.indexFixed[idx] - 1];
    }
    b = workingset.sizes[3];
    if (workingset.sizes[3] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b; idx++) {
      i = workingset.isActiveConstr.size(0);
      i1 = workingset.isActiveIdx[3] + idx;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      if (workingset.isActiveConstr[i1 - 1]) {
        i = workingset.indexLB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = workingset.lb.size(0);
        if ((workingset.indexLB[idx] < 1) || (workingset.indexLB[idx] > i)) {
          rtDynamicBoundsError(workingset.indexLB[idx], 1, i, w_emlrtBCI);
        }
        i = solution.xstar.size(0);
        if ((workingset.indexLB[idx] < 1) || (workingset.indexLB[idx] > i)) {
          rtDynamicBoundsError(workingset.indexLB[idx], 1, i, w_emlrtBCI);
        }
        solution.xstar[workingset.indexLB[idx] - 1] =
            -workingset.lb[workingset.indexLB[idx] - 1];
      }
    }
    b = workingset.sizes[4];
    if (workingset.sizes[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b; idx++) {
      i = workingset.isActiveConstr.size(0);
      i1 = workingset.isActiveIdx[4] + idx;
      if ((i1 < 1) || (i1 > i)) {
        rtDynamicBoundsError(i1, 1, i, w_emlrtBCI);
      }
      if (workingset.isActiveConstr[i1 - 1]) {
        i = workingset.indexUB.size(0);
        if ((idx + 1 < 1) || (idx + 1 > i)) {
          rtDynamicBoundsError(idx + 1, 1, i, w_emlrtBCI);
        }
        i = workingset.ub.size(0);
        if ((workingset.indexUB[idx] < 1) || (workingset.indexUB[idx] > i)) {
          rtDynamicBoundsError(workingset.indexUB[idx], 1, i, w_emlrtBCI);
        }
        i = solution.xstar.size(0);
        if ((workingset.indexUB[idx] < 1) || (workingset.indexUB[idx] > i)) {
          rtDynamicBoundsError(workingset.indexUB[idx], 1, i, w_emlrtBCI);
        }
        solution.xstar[workingset.indexUB[idx] - 1] =
            workingset.ub[workingset.indexUB[idx] - 1];
      }
    }
    initialize::PresolveWorkingSet(solution, memspace, workingset, qrmanager);
    if (solution.state >= 0) {
      guard1 = true;
    }
  } else {
    solution.state = 82;
    guard1 = true;
  }
  if (guard1) {
    solution.iterations = 0;
    solution.maxConstr =
        WorkingSet::maxConstraintViolation(workingset, solution.xstar);
    if (solution.maxConstr > 1.0E-6) {
      int PROBTYPE_ORIG;
      int i2;
      int i3;
      int mEqFixed;
      int nVarP1_tmp;
      int startIdx;
      PROBTYPE_ORIG = workingset.probType;
      nVarP1_tmp = workingset.nVar + 1;
      i = solution.xstar.size(0);
      if ((nVarP1_tmp < 1) || (nVarP1_tmp > i)) {
        rtDynamicBoundsError(nVarP1_tmp, 1, i, x_emlrtBCI);
      }
      solution.xstar[nVarP1_tmp - 1] = solution.maxConstr + 1.0;
      if (workingset.probType == 3) {
        i = 1;
      } else {
        i = 4;
      }
      WorkingSet::setProblemType(workingset, i);
      WorkingSet::removeAllIneqConstr(workingset);
      objective.prev_objtype = objective.objtype;
      objective.prev_nvar = objective.nvar;
      objective.prev_hasLinear = objective.hasLinear;
      objective.objtype = 5;
      objective.nvar = nVarP1_tmp;
      objective.gammaScalar = 1.0;
      objective.hasLinear = true;
      solution.fstar = Objective::computeFval(
          objective, memspace.workspace_double, H, f, solution.xstar);
      solution.state = 5;
      iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
              objective, options.SolverName, 1.4901161193847657E-10, 1.0E-6,
              runTimeOptions_MaxIterations);
      startIdx = (workingset.sizes[0] + workingset.sizes[1]) + 1;
      i = workingset.isActiveConstr.size(0);
      i1 = workingset.isActiveIdx[3] + workingset.sizes[3];
      if ((i1 - 1 < 1) || (i1 - 1 > i)) {
        rtDynamicBoundsError(i1 - 1, 1, i, g_emlrtBCI);
      }
      if (workingset.isActiveConstr[i1 - 2]) {
        boolean_T exitg1;
        if ((startIdx <= workingset.nActiveConstr) &&
            (workingset.nActiveConstr > 2147483646)) {
          check_forloop_overflow_error();
        }
        exitg1 = false;
        while ((!exitg1) && (startIdx <= workingset.nActiveConstr)) {
          i = workingset.Wid.size(0);
          if ((startIdx < 1) || (startIdx > i)) {
            rtDynamicBoundsError(startIdx, 1, i, x_emlrtBCI);
          }
          i = workingset.Wid[startIdx - 1];
          if (i == 4) {
            i = workingset.Wlocalidx.size(0);
            if (startIdx > i) {
              rtDynamicBoundsError(startIdx, 1, i, x_emlrtBCI);
            }
            i = workingset.Wlocalidx[startIdx - 1];
            if (i == workingset.sizes[3]) {
              i1 = workingset.Wid.size(0);
              if (startIdx > i1) {
                rtDynamicBoundsError(startIdx, 1, i1, h_emlrtBCI);
              }
              i1 = workingset.Wlocalidx.size(0);
              if (startIdx > i1) {
                rtDynamicBoundsError(startIdx, 1, i1, h_emlrtBCI);
              }
              i1 = workingset.isActiveConstr.size(0);
              i = (workingset.isActiveIdx[3] + i) - 1;
              if ((i < 1) || (i > i1)) {
                rtDynamicBoundsError(i, 1, i1, h_emlrtBCI);
              }
              workingset.isActiveConstr[i - 1] = false;
              i = workingset.Wid.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i,
                                     j_emlrtBCI);
              }
              i = workingset.Wid.size(0);
              if (startIdx > i) {
                rtDynamicBoundsError(startIdx, 1, i, j_emlrtBCI);
              }
              i = workingset.nActiveConstr - 1;
              workingset.Wid[startIdx - 1] = workingset.Wid[i];
              i1 = workingset.Wlocalidx.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i1)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i1,
                                     j_emlrtBCI);
              }
              i1 = workingset.Wlocalidx.size(0);
              if (startIdx > i1) {
                rtDynamicBoundsError(startIdx, 1, i1, j_emlrtBCI);
              }
              workingset.Wlocalidx[startIdx - 1] = workingset.Wlocalidx[i];
              b = workingset.nVar;
              if (workingset.nVar > 2147483646) {
                check_forloop_overflow_error();
              }
              for (int idx{0}; idx < b; idx++) {
                i1 = workingset.ATwset.size(0);
                i2 =
                    (idx + workingset.ldA * (workingset.nActiveConstr - 1)) + 1;
                if ((i2 < 1) || (i2 > i1)) {
                  rtDynamicBoundsError(i2, 1, i1, j_emlrtBCI);
                }
                i1 = workingset.ATwset.size(0);
                i3 = (idx + workingset.ldA * (startIdx - 1)) + 1;
                if ((i3 < 1) || (i3 > i1)) {
                  rtDynamicBoundsError(i3, 1, i1, j_emlrtBCI);
                }
                workingset.ATwset[i3 - 1] = workingset.ATwset[i2 - 1];
              }
              i1 = workingset.bwset.size(0);
              if ((workingset.nActiveConstr < 1) ||
                  (workingset.nActiveConstr > i1)) {
                rtDynamicBoundsError(workingset.nActiveConstr, 1, i1,
                                     j_emlrtBCI);
              }
              i1 = workingset.bwset.size(0);
              if (startIdx > i1) {
                rtDynamicBoundsError(startIdx, 1, i1, j_emlrtBCI);
              }
              workingset.bwset[startIdx - 1] = workingset.bwset[i];
              workingset.nActiveConstr = i;
              workingset.nWConstr[3]--;
              exitg1 = true;
            } else {
              startIdx++;
            }
          } else {
            startIdx++;
          }
        }
      }
      startIdx = workingset.nActiveConstr;
      mEqFixed = workingset.sizes[0] + workingset.sizes[1];
      while ((startIdx > mEqFixed) && (startIdx > nVar_tmp_tmp)) {
        int TYPE_tmp;
        i = workingset.Wid.size(0);
        if ((startIdx < 1) || (startIdx > i)) {
          rtDynamicBoundsError(startIdx, 1, i, h_emlrtBCI);
        }
        TYPE_tmp = workingset.Wid[startIdx - 1];
        i = workingset.Wlocalidx.size(0);
        if (startIdx > i) {
          rtDynamicBoundsError(startIdx, 1, i, h_emlrtBCI);
        }
        if ((TYPE_tmp < 1) || (TYPE_tmp > 6)) {
          rtDynamicBoundsError(workingset.Wid[startIdx - 1], 1, 6, i_emlrtBCI);
        }
        i = workingset.isActiveConstr.size(0);
        i1 = (workingset.isActiveIdx[TYPE_tmp - 1] +
              workingset.Wlocalidx[startIdx - 1]) -
             1;
        if ((i1 < 1) || (i1 > i)) {
          rtDynamicBoundsError(i1, 1, i, h_emlrtBCI);
        }
        workingset.isActiveConstr[i1 - 1] = false;
        i = workingset.Wid.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i, j_emlrtBCI);
        }
        i = workingset.Wid.size(0);
        if (startIdx > i) {
          rtDynamicBoundsError(startIdx, 1, i, j_emlrtBCI);
        }
        i = workingset.nActiveConstr - 1;
        workingset.Wid[startIdx - 1] = workingset.Wid[i];
        i1 = workingset.Wlocalidx.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i1)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i1, j_emlrtBCI);
        }
        i1 = workingset.Wlocalidx.size(0);
        if (startIdx > i1) {
          rtDynamicBoundsError(startIdx, 1, i1, j_emlrtBCI);
        }
        workingset.Wlocalidx[startIdx - 1] = workingset.Wlocalidx[i];
        b = workingset.nVar;
        if (workingset.nVar > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int idx{0}; idx < b; idx++) {
          i1 = workingset.ATwset.size(0);
          i2 = (idx + workingset.ldA * i) + 1;
          if ((i2 < 1) || (i2 > i1)) {
            rtDynamicBoundsError(i2, 1, i1, j_emlrtBCI);
          }
          i1 = workingset.ATwset.size(0);
          i3 = (idx + workingset.ldA * (startIdx - 1)) + 1;
          if ((i3 < 1) || (i3 > i1)) {
            rtDynamicBoundsError(i3, 1, i1, j_emlrtBCI);
          }
          workingset.ATwset[i3 - 1] = workingset.ATwset[i2 - 1];
        }
        i1 = workingset.bwset.size(0);
        if ((workingset.nActiveConstr < 1) || (workingset.nActiveConstr > i1)) {
          rtDynamicBoundsError(workingset.nActiveConstr, 1, i1, j_emlrtBCI);
        }
        i1 = workingset.bwset.size(0);
        if (startIdx > i1) {
          rtDynamicBoundsError(startIdx, 1, i1, j_emlrtBCI);
        }
        workingset.bwset[startIdx - 1] = workingset.bwset[i];
        workingset.nActiveConstr = i;
        if (TYPE_tmp > 5) {
          rtDynamicBoundsError(6, 1, 5, k_emlrtBCI);
        }
        workingset.nWConstr[TYPE_tmp - 1]--;
        startIdx--;
      }
      i = solution.xstar.size(0);
      if (nVarP1_tmp > i) {
        rtDynamicBoundsError(nVarP1_tmp, 1, i, x_emlrtBCI);
      }
      solution.maxConstr = solution.xstar[nVarP1_tmp - 1];
      WorkingSet::setProblemType(workingset, PROBTYPE_ORIG);
      objective.objtype = objective.prev_objtype;
      objective.nvar = objective.prev_nvar;
      objective.hasLinear = objective.prev_hasLinear;
      options.ObjectiveLimit = rtMinusInf;
      options.StepTolerance = 1.0E-6;
      if (solution.state != 0) {
        solution.maxConstr =
            WorkingSet::maxConstraintViolation(workingset, solution.xstar);
        if (solution.maxConstr > 1.0E-6) {
          startIdx = workingset.mConstrMax;
          if (workingset.mConstrMax > 2147483646) {
            check_forloop_overflow_error();
          }
          for (mEqFixed = 0; mEqFixed < startIdx; mEqFixed++) {
            solution.lambda[mEqFixed] = 0.0;
          }
          solution.fstar = Objective::computeFval(
              objective, memspace.workspace_double, H, f, solution.xstar);
          solution.state = -2;
        } else {
          if (solution.maxConstr > 0.0) {
            double maxConstr_new;
            if (nVar_tmp_tmp > 2147483646) {
              check_forloop_overflow_error();
            }
            for (mEqFixed = 0; mEqFixed < nVar_tmp_tmp; mEqFixed++) {
              solution.searchDir[mEqFixed] = solution.xstar[mEqFixed];
            }
            initialize::PresolveWorkingSet(solution, memspace, workingset,
                                           qrmanager);
            maxConstr_new =
                WorkingSet::maxConstraintViolation(workingset, solution.xstar);
            if (maxConstr_new >= solution.maxConstr) {
              solution.maxConstr = maxConstr_new;
              for (mEqFixed = 0; mEqFixed < nVar_tmp_tmp; mEqFixed++) {
                solution.xstar[mEqFixed] = solution.searchDir[mEqFixed];
              }
            }
          }
          iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
                  objective, options.SolverName, options.StepTolerance,
                  options.ObjectiveLimit, runTimeOptions_MaxIterations);
        }
      }
    } else {
      iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
              objective, options.SolverName, options.StepTolerance,
              options.ObjectiveLimit, runTimeOptions_MaxIterations);
    }
  }
}

} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for driver1.cpp
//
// [EOF]
//
