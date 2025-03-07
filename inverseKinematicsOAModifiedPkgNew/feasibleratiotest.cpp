//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleratiotest.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

// Include Files
#include "feasibleratiotest.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xnrm2.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &solution_xstar
//                const array<double, 1U> &solution_searchDir
//                array<double, 2U> &workspace
//                int workingset_nVar
//                int workingset_ldA
//                const array<double, 1U> &workingset_Aineq
//                const array<double, 1U> &workingset_bineq
//                const array<double, 1U> &workingset_lb
//                const array<double, 1U> &workingset_ub
//                const array<int, 1U> &workingset_indexLB
//                const array<int, 1U> &workingset_indexUB
//                const int workingset_sizes[5]
//                const int workingset_isActiveIdx[6]
//                const array<boolean_T, 1U> &workingset_isActiveConstr
//                const int workingset_nWConstr[5]
//                boolean_T isPhaseOne
//                boolean_T &newBlocking
//                int &constrType
//                int &constrIdx
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
double feasibleratiotest(const array<double, 1U> &solution_xstar,
                         const array<double, 1U> &solution_searchDir,
                         array<double, 2U> &workspace, int workingset_nVar,
                         int workingset_ldA,
                         const array<double, 1U> &workingset_Aineq,
                         const array<double, 1U> &workingset_bineq,
                         const array<double, 1U> &workingset_lb,
                         const array<double, 1U> &workingset_ub,
                         const array<int, 1U> &workingset_indexLB,
                         const array<int, 1U> &workingset_indexUB,
                         const int workingset_sizes[5],
                         const int workingset_isActiveIdx[6],
                         const array<boolean_T, 1U> &workingset_isActiveConstr,
                         const int workingset_nWConstr[5], boolean_T isPhaseOne,
                         boolean_T &newBlocking, int &constrType,
                         int &constrIdx)
{
  double alpha;
  double c;
  double denomTol;
  double phaseOneCorrectionP;
  double phaseOneCorrectionX;
  double ratio;
  int iyend;
  int k;
  int totalIneq;
  int totalUB;
  totalIneq = workingset_sizes[2] - 1;
  totalUB = workingset_sizes[4];
  alpha = 1.0E+30;
  newBlocking = false;
  constrType = 0;
  constrIdx = 0;
  denomTol = 2.2204460492503131E-13 *
             internal::blas::xnrm2(workingset_nVar, solution_searchDir);
  if (workingset_nWConstr[2] < workingset_sizes[2]) {
    int ldw;
    for (k = 0; k <= totalIneq; k++) {
      workspace[k] = workingset_bineq[k];
    }
    internal::blas::xgemv(workingset_nVar, workingset_sizes[2],
                          workingset_Aineq, workingset_ldA, solution_xstar,
                          workspace);
    ldw = workspace.size(0);
    k = workspace.size(0) + 1;
    if ((workingset_nVar != 0) && (workingset_sizes[2] != 0)) {
      int iy;
      iyend = workspace.size(0) + workingset_sizes[2];
      for (iy = k; iy <= iyend; iy++) {
        workspace[iy - 1] = 0.0;
      }
      iy = workspace.size(0);
      iyend = workingset_ldA * (workingset_sizes[2] - 1) + 1;
      for (int iac{1}; workingset_ldA < 0 ? iac >= iyend : iac <= iyend;
           iac += workingset_ldA) {
        c = 0.0;
        k = (iac + workingset_nVar) - 1;
        for (int ia{iac}; ia <= k; ia++) {
          c += workingset_Aineq[ia - 1] * solution_searchDir[ia - iac];
        }
        workspace[iy] = workspace[iy] + c;
        iy++;
      }
    }
    for (k = 0; k <= totalIneq; k++) {
      c = workspace[ldw + k];
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[2] + k) - 1])) {
        c = std::fmin(std::abs(workspace[k]), 1.0E-6 - workspace[k]) / c;
        if (c < alpha) {
          alpha = c;
          constrType = 3;
          constrIdx = k + 1;
          newBlocking = true;
        }
      }
    }
  }
  if (workingset_nWConstr[3] < workingset_sizes[3]) {
    phaseOneCorrectionX =
        static_cast<double>(isPhaseOne) * solution_xstar[workingset_nVar - 1];
    phaseOneCorrectionP = static_cast<double>(isPhaseOne) *
                          solution_searchDir[workingset_nVar - 1];
    iyend = workingset_sizes[3];
    for (k = 0; k <= iyend - 2; k++) {
      c = -solution_searchDir[workingset_indexLB[k] - 1] - phaseOneCorrectionP;
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[3] + k) - 1])) {
        ratio = (-solution_xstar[workingset_indexLB[k] - 1] -
                 workingset_lb[workingset_indexLB[k] - 1]) -
                phaseOneCorrectionX;
        c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / c;
        if (c < alpha) {
          alpha = c;
          constrType = 4;
          constrIdx = k + 1;
          newBlocking = true;
        }
      }
    }
    iyend = workingset_indexLB[workingset_sizes[3] - 1] - 1;
    c = -solution_searchDir[iyend];
    if ((c > denomTol) &&
        (!workingset_isActiveConstr
             [(workingset_isActiveIdx[3] + workingset_sizes[3]) - 2])) {
      ratio = -solution_xstar[iyend] - workingset_lb[iyend];
      c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / c;
      if (c < alpha) {
        alpha = c;
        constrType = 4;
        constrIdx = workingset_sizes[3];
        newBlocking = true;
      }
    }
  }
  if (workingset_nWConstr[4] < workingset_sizes[4]) {
    phaseOneCorrectionX =
        static_cast<double>(isPhaseOne) * solution_xstar[workingset_nVar - 1];
    phaseOneCorrectionP = static_cast<double>(isPhaseOne) *
                          solution_searchDir[workingset_nVar - 1];
    for (k = 0; k < totalUB; k++) {
      c = solution_searchDir[workingset_indexUB[k] - 1] - phaseOneCorrectionP;
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[4] + k) - 1])) {
        ratio = (solution_xstar[workingset_indexUB[k] - 1] -
                 workingset_ub[workingset_indexUB[k] - 1]) -
                phaseOneCorrectionX;
        c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / c;
        if (c < alpha) {
          alpha = c;
          constrType = 5;
          constrIdx = k + 1;
          newBlocking = true;
        }
      }
    }
  }
  if (!isPhaseOne) {
    if (newBlocking && (alpha > 1.0)) {
      newBlocking = false;
    }
    alpha = std::fmin(alpha, 1.0);
  }
  return alpha;
}

} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for feasibleratiotest.cpp
//
// [EOF]
//
