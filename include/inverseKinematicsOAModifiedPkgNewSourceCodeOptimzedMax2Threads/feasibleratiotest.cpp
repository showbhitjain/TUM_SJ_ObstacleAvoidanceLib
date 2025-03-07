//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleratiotest.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "feasibleratiotest.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "xnrm2.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>
#include "globalParameters.h"
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
  int i;
  int iyend;
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
    int iy;
    int ldw;
    if (static_cast<int>(workingset_sizes[2] < 200)) {
      for (int k{0}; k <= totalIneq; k++) {
        workspace[k] = workingset_bineq[k];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int k = 0; k <= totalIneq; k++) {
        workspace[k] = workingset_bineq[k];
      }
    }
    internal::blas::xgemv(workingset_nVar, workingset_sizes[2],
                          workingset_Aineq, workingset_ldA, solution_xstar,
                          workspace);
    ldw = workspace.size(0);
    iy = workspace.size(0) + 1;
    if ((workingset_nVar != 0) && (workingset_sizes[2] != 0)) {
      iyend = workspace.size(0) + workingset_sizes[2];
      if (static_cast<int>((iyend - iy) + 1 < 200)) {
        for (int k{iy}; k <= iyend; k++) {
          workspace[k - 1] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int k = iy; k <= iyend; k++) {
          workspace[k - 1] = 0.0;
        }
      }
      iy = workspace.size(0);
      i = workingset_ldA * (workingset_sizes[2] - 1) + 1;
      for (int iac{1}; workingset_ldA < 0 ? iac >= i : iac <= i;
           iac += workingset_ldA) {
        c = 0.0;
        iyend = (iac + workingset_nVar) - 1;
        for (int ia{iac}; ia <= iyend; ia++) {
          c += workingset_Aineq[ia - 1] * solution_searchDir[ia - iac];
        }
        workspace[iy] = workspace[iy] + c;
        iy++;
      }
    }
    for (iyend = 0; iyend <= totalIneq; iyend++) {
      c = workspace[ldw + iyend];
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[2] + iyend) -
                                      1])) {
        c = std::fmin(std::abs(workspace[iyend]),
                      OAGlobals::constraintTolerance - workspace[iyend]) /
            c;
        if (c < alpha) {
          alpha = c;
          constrType = 3;
          constrIdx = iyend + 1;
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
    i = workingset_sizes[3];
    for (iyend = 0; iyend <= i - 2; iyend++) {
      c = -solution_searchDir[workingset_indexLB[iyend] - 1] -
          phaseOneCorrectionP;
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[3] + iyend) -
                                      1])) {
        ratio = (-solution_xstar[workingset_indexLB[iyend] - 1] -
                 workingset_lb[workingset_indexLB[iyend] - 1]) -
                phaseOneCorrectionX;
        c = std::fmin(std::abs(ratio), OAGlobals::constraintTolerance - ratio) / c;
        if (c < alpha) {
          alpha = c;
          constrType = 4;
          constrIdx = iyend + 1;
          newBlocking = true;
        }
      }
    }
    i = workingset_indexLB[workingset_sizes[3] - 1] - 1;
    c = -solution_searchDir[i];
    if ((c > denomTol) &&
        (!workingset_isActiveConstr
             [(workingset_isActiveIdx[3] + workingset_sizes[3]) - 2])) {
      ratio = -solution_xstar[i] - workingset_lb[i];
      c = std::fmin(std::abs(ratio), OAGlobals::constraintTolerance - ratio) / c;
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
    for (iyend = 0; iyend < totalUB; iyend++) {
      c = solution_searchDir[workingset_indexUB[iyend] - 1] -
          phaseOneCorrectionP;
      if ((c > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[4] + iyend) -
                                      1])) {
        ratio = (solution_xstar[workingset_indexUB[iyend] - 1] -
                 workingset_ub[workingset_indexUB[iyend] - 1]) -
                phaseOneCorrectionX;
        c = std::fmin(std::abs(ratio), OAGlobals::constraintTolerance - ratio) / c;
        if (c < alpha) {
          alpha = c;
          constrType = 5;
          constrIdx = iyend + 1;
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
