//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleratiotest.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "feasibleratiotest.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
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
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,                  // iFirst
      -1,                  // iLast
      1,                   // lineNo
      1,                   // colNo
      "",                  // aName
      "feasibleratiotest", // fName
      "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
      "feasibleratiotest.p", // pName
      0                      // checkKind
  };
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
  boolean_T overflow;
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
    if (workingset_sizes[2] > 2147483646) {
      check_forloop_overflow_error();
    }
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
      if ((workspace.size(0) + 1 <= iyend) && (iyend > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (iy = k; iy <= iyend; iy++) {
        workspace[iy - 1] = 0.0;
      }
      iy = workspace.size(0);
      k = workingset_ldA * (workingset_sizes[2] - 1) + 1;
      if ((workingset_ldA == 0) || ((workingset_ldA > 0) && (k < 1)) ||
          ((workingset_ldA < 0) && (k > 1))) {
        overflow = false;
      } else if (workingset_ldA > 0) {
        overflow = (k > MAX_int32_T - workingset_ldA);
      } else {
        overflow = (k < MIN_int32_T - workingset_ldA);
      }
      if (workingset_ldA == 0) {
        m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
      }
      if (overflow) {
        check_forloop_overflow_error();
      }
      for (int iac{1}; workingset_ldA < 0 ? iac >= k : iac <= k;
           iac += workingset_ldA) {
        c = 0.0;
        iyend = (iac + workingset_nVar) - 1;
        if ((iac <= iyend) && (iyend > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int ia{iac}; ia <= iyend; ia++) {
          c += workingset_Aineq[ia - 1] * solution_searchDir[ia - iac];
        }
        workspace[iy] = workspace[iy] + c;
        iy++;
      }
    }
    for (int iac{0}; iac <= totalIneq; iac++) {
      k = workspace.size(0) * workspace.size(1);
      iyend = (ldw + iac) + 1;
      if ((iyend < 1) || (iyend > k)) {
        rtDynamicBoundsError(iyend, 1, k, w_emlrtBCI);
      }
      phaseOneCorrectionX = workspace[iyend - 1];
      if (phaseOneCorrectionX > denomTol) {
        iyend = (workingset_isActiveIdx[2] + iac) + 1;
        if ((iyend - 1 < 1) ||
            (iyend - 1 > workingset_isActiveConstr.size(0))) {
          rtDynamicBoundsError(iyend - 1, 1, workingset_isActiveConstr.size(0),
                               g_emlrtBCI);
        }
        if (!workingset_isActiveConstr[iyend - 2]) {
          if ((iac + 1 < 1) || (iac + 1 > k)) {
            rtDynamicBoundsError(iac + 1, 1, k, w_emlrtBCI);
          }
          if (iac + 1 > k) {
            rtDynamicBoundsError(iac + 1, 1, k, w_emlrtBCI);
          }
          c = workspace[iac];
          c = std::fmin(std::abs(c), 1.0E-6 - c) / phaseOneCorrectionX;
          if (c < alpha) {
            alpha = c;
            constrType = 3;
            constrIdx = iac + 1;
            newBlocking = true;
          }
        }
      }
    }
  }
  if (workingset_nWConstr[3] < workingset_sizes[3]) {
    if ((workingset_nVar < 1) || (workingset_nVar > solution_xstar.size(0))) {
      rtDynamicBoundsError(workingset_nVar, 1, solution_xstar.size(0),
                           w_emlrtBCI);
    }
    phaseOneCorrectionX =
        static_cast<double>(isPhaseOne) * solution_xstar[workingset_nVar - 1];
    if (workingset_nVar > solution_searchDir.size(0)) {
      rtDynamicBoundsError(workingset_nVar, 1, solution_searchDir.size(0),
                           w_emlrtBCI);
    }
    phaseOneCorrectionP = static_cast<double>(isPhaseOne) *
                          solution_searchDir[workingset_nVar - 1];
    k = workingset_sizes[3];
    for (int iac{0}; iac <= k - 2; iac++) {
      if ((iac + 1 < 1) || (iac + 1 > workingset_indexLB.size(0))) {
        rtDynamicBoundsError(iac + 1, 1, workingset_indexLB.size(0),
                             w_emlrtBCI);
      }
      if ((workingset_indexLB[iac] < 1) ||
          (workingset_indexLB[iac] > solution_searchDir.size(0))) {
        rtDynamicBoundsError(workingset_indexLB[iac], 1,
                             solution_searchDir.size(0), w_emlrtBCI);
      }
      c = -solution_searchDir[workingset_indexLB[iac] - 1] -
          phaseOneCorrectionP;
      if (c > denomTol) {
        iyend = workingset_isActiveIdx[3] + iac;
        if ((iyend < 1) || (iyend > workingset_isActiveConstr.size(0))) {
          rtDynamicBoundsError(iyend, 1, workingset_isActiveConstr.size(0),
                               g_emlrtBCI);
        }
        if (!workingset_isActiveConstr[iyend - 1]) {
          if ((workingset_indexLB[iac] < 1) ||
              (workingset_indexLB[iac] > solution_xstar.size(0))) {
            rtDynamicBoundsError(workingset_indexLB[iac], 1,
                                 solution_xstar.size(0), w_emlrtBCI);
          }
          if ((workingset_indexLB[iac] < 1) ||
              (workingset_indexLB[iac] > workingset_lb.size(0))) {
            rtDynamicBoundsError(workingset_indexLB[iac], 1,
                                 workingset_lb.size(0), w_emlrtBCI);
          }
          ratio = (-solution_xstar[workingset_indexLB[iac] - 1] -
                   workingset_lb[workingset_indexLB[iac] - 1]) -
                  phaseOneCorrectionX;
          c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / c;
          if (c < alpha) {
            alpha = c;
            constrType = 4;
            constrIdx = iac + 1;
            newBlocking = true;
          }
        }
      }
    }
    if ((workingset_sizes[3] < 1) ||
        (workingset_sizes[3] > workingset_indexLB.size(0))) {
      rtDynamicBoundsError(workingset_sizes[3], 1, workingset_indexLB.size(0),
                           w_emlrtBCI);
    }
    k = workingset_indexLB[workingset_sizes[3] - 1];
    overflow = ((k < 1) || (k > solution_searchDir.size(0)));
    if (overflow) {
      rtDynamicBoundsError(k, 1, solution_searchDir.size(0), w_emlrtBCI);
    }
    phaseOneCorrectionX = -solution_searchDir[k - 1];
    if (phaseOneCorrectionX > denomTol) {
      iyend = workingset_isActiveIdx[3] + workingset_sizes[3];
      if ((iyend - 1 < 1) || (iyend - 1 > workingset_isActiveConstr.size(0))) {
        rtDynamicBoundsError(iyend - 1, 1, workingset_isActiveConstr.size(0),
                             g_emlrtBCI);
      }
      if (!workingset_isActiveConstr[iyend - 2]) {
        if ((k < 1) || (k > solution_xstar.size(0))) {
          rtDynamicBoundsError(k, 1, solution_xstar.size(0), w_emlrtBCI);
        }
        if (k > workingset_lb.size(0)) {
          rtDynamicBoundsError(k, 1, workingset_lb.size(0), w_emlrtBCI);
        }
        ratio = -solution_xstar[k - 1] - workingset_lb[k - 1];
        c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / phaseOneCorrectionX;
        if (c < alpha) {
          alpha = c;
          constrType = 4;
          constrIdx = workingset_sizes[3];
          newBlocking = true;
        }
      }
    }
  }
  if (workingset_nWConstr[4] < workingset_sizes[4]) {
    if ((workingset_nVar < 1) || (workingset_nVar > solution_xstar.size(0))) {
      rtDynamicBoundsError(workingset_nVar, 1, solution_xstar.size(0),
                           w_emlrtBCI);
    }
    phaseOneCorrectionX =
        static_cast<double>(isPhaseOne) * solution_xstar[workingset_nVar - 1];
    if (workingset_nVar > solution_searchDir.size(0)) {
      rtDynamicBoundsError(workingset_nVar, 1, solution_searchDir.size(0),
                           w_emlrtBCI);
    }
    phaseOneCorrectionP = static_cast<double>(isPhaseOne) *
                          solution_searchDir[workingset_nVar - 1];
    if (workingset_sizes[4] > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int iac{0}; iac < totalUB; iac++) {
      if ((iac + 1 < 1) || (iac + 1 > workingset_indexUB.size(0))) {
        rtDynamicBoundsError(iac + 1, 1, workingset_indexUB.size(0),
                             w_emlrtBCI);
      }
      if ((workingset_indexUB[iac] < 1) ||
          (workingset_indexUB[iac] > solution_searchDir.size(0))) {
        rtDynamicBoundsError(workingset_indexUB[iac], 1,
                             solution_searchDir.size(0), w_emlrtBCI);
      }
      c = solution_searchDir[workingset_indexUB[iac] - 1] - phaseOneCorrectionP;
      if (c > denomTol) {
        k = workingset_isActiveIdx[4] + iac;
        if ((k < 1) || (k > workingset_isActiveConstr.size(0))) {
          rtDynamicBoundsError(k, 1, workingset_isActiveConstr.size(0),
                               g_emlrtBCI);
        }
        if (!workingset_isActiveConstr[k - 1]) {
          if ((workingset_indexUB[iac] < 1) ||
              (workingset_indexUB[iac] > solution_xstar.size(0))) {
            rtDynamicBoundsError(workingset_indexUB[iac], 1,
                                 solution_xstar.size(0), w_emlrtBCI);
          }
          if ((workingset_indexUB[iac] < 1) ||
              (workingset_indexUB[iac] > workingset_ub.size(0))) {
            rtDynamicBoundsError(workingset_indexUB[iac], 1,
                                 workingset_ub.size(0), w_emlrtBCI);
          }
          ratio = (solution_xstar[workingset_indexUB[iac] - 1] -
                   workingset_ub[workingset_indexUB[iac] - 1]) -
                  phaseOneCorrectionX;
          c = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / c;
          if (c < alpha) {
            alpha = c;
            constrType = 5;
            constrIdx = iac + 1;
            newBlocking = true;
          }
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
