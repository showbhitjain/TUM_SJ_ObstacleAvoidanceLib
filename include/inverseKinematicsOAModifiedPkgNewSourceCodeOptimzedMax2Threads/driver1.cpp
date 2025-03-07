//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver1.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "driver1.h"
#include "PresolveWorkingSet.h"
#include "computeFval.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "iterate.h"
#include "maxConstraintViolation.h"
#include "removeConstr.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "coder_array.h"
#include "omp.h"
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
  int idxStartIneq;
  int mConstr;
  int nVar;
  boolean_T guard1;
  solution.iterations = 0;
  nVar = workingset.nVar - 1;
  guard1 = false;
  if (workingset.probType == 3) {
    mConstr = workingset.sizes[0];
    for (idxStartIneq = 0; idxStartIneq < mConstr; idxStartIneq++) {
      solution.xstar[workingset.indexFixed[idxStartIneq] - 1] =
          workingset.ub[workingset.indexFixed[idxStartIneq] - 1];
    }
    mConstr = workingset.sizes[3];
    for (idxStartIneq = 0; idxStartIneq < mConstr; idxStartIneq++) {
      if (workingset
              .isActiveConstr[(workingset.isActiveIdx[3] + idxStartIneq) - 1]) {
        solution.xstar[workingset.indexLB[idxStartIneq] - 1] =
            -workingset.lb[workingset.indexLB[idxStartIneq] - 1];
      }
    }
    mConstr = workingset.sizes[4];
    for (idxStartIneq = 0; idxStartIneq < mConstr; idxStartIneq++) {
      if (workingset
              .isActiveConstr[(workingset.isActiveIdx[4] + idxStartIneq) - 1]) {
        solution.xstar[workingset.indexUB[idxStartIneq] - 1] =
            workingset.ub[workingset.indexUB[idxStartIneq] - 1];
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
    if (solution.maxConstr > OAGlobals::constraintTolerance) {
      int PROBTYPE_ORIG;
      int b_nVar;
      int idxEndIneq_tmp_tmp;
      int nVarP1;
      PROBTYPE_ORIG = workingset.probType;
      b_nVar = workingset.nVar;
      nVarP1 = workingset.nVar;
      solution.xstar[workingset.nVar] = solution.maxConstr + 1.0;
      if (workingset.probType == 3) {
        idxEndIneq_tmp_tmp = 1;
      } else {
        idxEndIneq_tmp_tmp = 4;
      }
      WorkingSet::setProblemType(workingset, idxEndIneq_tmp_tmp);
      mConstr = workingset.nWConstr[0] + workingset.nWConstr[1];
      idxStartIneq = mConstr + 1;
      idxEndIneq_tmp_tmp = workingset.nActiveConstr;
      for (int idx_global{idxStartIneq}; idx_global <= idxEndIneq_tmp_tmp;
           idx_global++) {
        workingset.isActiveConstr
            [(workingset.isActiveIdx[workingset.Wid[idx_global - 1] - 1] +
              workingset.Wlocalidx[idx_global - 1]) -
             2] = false;
      }
      workingset.nWConstr[2] = 0;
      workingset.nWConstr[3] = 0;
      workingset.nWConstr[4] = 0;
      workingset.nActiveConstr = mConstr;
      objective.prev_objtype = objective.objtype;
      objective.prev_nvar = objective.nvar;
      objective.prev_hasLinear = objective.hasLinear;
      objective.objtype = 5;
      objective.nvar = nVarP1 + 1;
      objective.gammaScalar = 1.0;
      objective.hasLinear = true;
      solution.fstar = Objective::computeFval(
          objective, memspace.workspace_double, H, f, solution.xstar);
      solution.state = 5;
      iterate(H, f, solution, memspace, workingset, qrmanager, cholmanager,
              objective, options.SolverName, 1.4901161193847657E-10, OAGlobals::constraintTolerance,
              runTimeOptions_MaxIterations);
      if (workingset.isActiveConstr
              [(workingset.isActiveIdx[3] + workingset.sizes[3]) - 2]) {
        boolean_T exitg1;
        idxStartIneq = workingset.sizes[0] + workingset.sizes[1];
        exitg1 = false;
        while ((!exitg1) && (idxStartIneq + 1 <= workingset.nActiveConstr)) {
          if ((workingset.Wid[idxStartIneq] == 4) &&
              (workingset.Wlocalidx[idxStartIneq] == workingset.sizes[3])) {
            WorkingSet::removeConstr(workingset, idxStartIneq + 1);
            exitg1 = true;
          } else {
            idxStartIneq++;
          }
        }
      }
      mConstr = workingset.nActiveConstr;
      idxStartIneq = workingset.sizes[0] + workingset.sizes[1];
      while ((mConstr > idxStartIneq) && (mConstr > b_nVar)) {
        WorkingSet::removeConstr(workingset, mConstr);
        mConstr--;
      }
      solution.maxConstr = solution.xstar[nVarP1];
      WorkingSet::setProblemType(workingset, PROBTYPE_ORIG);
      objective.objtype = objective.prev_objtype;
      objective.nvar = objective.prev_nvar;
      objective.hasLinear = objective.prev_hasLinear;
      options.ObjectiveLimit = rtMinusInf;
      options.StepTolerance = 1.0E-6;
      if (solution.state != 0) {
        solution.maxConstr =
            WorkingSet::maxConstraintViolation(workingset, solution.xstar);
        if (solution.maxConstr > OAGlobals::constraintTolerance) {
          mConstr = workingset.mConstrMax;
          if (static_cast<int>(mConstr < 200)) {
            for (int k{0}; k < mConstr; k++) {
              solution.lambda[k] = 0.0;
            }
          } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

            for (int k = 0; k < mConstr; k++) {
              solution.lambda[k] = 0.0;
            }
          }
          solution.fstar = Objective::computeFval(
              objective, memspace.workspace_double, H, f, solution.xstar);
          solution.state = -2;
        } else {
          if (solution.maxConstr > 0.0) {
            double maxConstr_new;
            mConstr = (nVar + 1 < 200);
            if (mConstr) {
              for (int k{0}; k <= nVar; k++) {
                solution.searchDir[k] = solution.xstar[k];
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (int k = 0; k <= nVar; k++) {
                solution.searchDir[k] = solution.xstar[k];
              }
            }
            initialize::PresolveWorkingSet(solution, memspace, workingset,
                                           qrmanager);
            maxConstr_new =
                WorkingSet::maxConstraintViolation(workingset, solution.xstar);
            if (maxConstr_new >= solution.maxConstr) {
              solution.maxConstr = maxConstr_new;
              if (mConstr) {
                for (int k{0}; k <= nVar; k++) {
                  solution.xstar[k] = solution.searchDir[k];
                }
              } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

                for (int k = 0; k <= nVar; k++) {
                  solution.xstar[k] = solution.searchDir[k];
                }
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
