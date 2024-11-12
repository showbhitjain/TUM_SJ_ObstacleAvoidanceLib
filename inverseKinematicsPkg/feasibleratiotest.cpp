//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleratiotest.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "feasibleratiotest.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 1U> &solution_xstar
//                const array<double, 1U> &solution_searchDir
//                int workingset_nVar
//                int workingset_Aineq_size
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
                         int workingset_nVar, int workingset_Aineq_size,
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
  array<double, 1U> workingset_Aineq_data;
  double alpha;
  double denomTol;
  double phaseOneCorrectionP;
  double phaseOneCorrectionX;
  double pk_corrected;
  double ratio;
  int totalUB;
  totalUB = workingset_sizes[4];
  alpha = 1.0E+30;
  newBlocking = false;
  constrType = 0;
  constrIdx = 0;
  denomTol = 2.2204460492503131E-13 *
             internal::blas::xnrm2(workingset_nVar, solution_searchDir);
  if (workingset_nWConstr[2] < 0) {
    workingset_Aineq_data.set(nullptr, workingset_Aineq_size);
  }
  if (workingset_nWConstr[3] < workingset_sizes[3]) {
    int i;
    phaseOneCorrectionX =
        static_cast<double>(isPhaseOne) * solution_xstar[workingset_nVar - 1];
    phaseOneCorrectionP = static_cast<double>(isPhaseOne) *
                          solution_searchDir[workingset_nVar - 1];
    i = workingset_sizes[3];
    for (int idx{0}; idx <= i - 2; idx++) {
      pk_corrected = -solution_searchDir[workingset_indexLB[idx] - 1] -
                     phaseOneCorrectionP;
      if ((pk_corrected > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[3] + idx) - 1])) {
        ratio = (-solution_xstar[workingset_indexLB[idx] - 1] -
                 workingset_lb[workingset_indexLB[idx] - 1]) -
                phaseOneCorrectionX;
        pk_corrected =
            std::fmin(std::abs(ratio), 1.0E-6 - ratio) / pk_corrected;
        if (pk_corrected < alpha) {
          alpha = pk_corrected;
          constrType = 4;
          constrIdx = idx + 1;
          newBlocking = true;
        }
      }
    }
    i = workingset_indexLB[workingset_sizes[3] - 1] - 1;
    pk_corrected = -solution_searchDir[i];
    if ((pk_corrected > denomTol) &&
        (!workingset_isActiveConstr
             [(workingset_isActiveIdx[3] + workingset_sizes[3]) - 2])) {
      ratio = -solution_xstar[i] - workingset_lb[i];
      pk_corrected = std::fmin(std::abs(ratio), 1.0E-6 - ratio) / pk_corrected;
      if (pk_corrected < alpha) {
        alpha = pk_corrected;
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
    for (int idx{0}; idx < totalUB; idx++) {
      pk_corrected =
          solution_searchDir[workingset_indexUB[idx] - 1] - phaseOneCorrectionP;
      if ((pk_corrected > denomTol) &&
          (!workingset_isActiveConstr[(workingset_isActiveIdx[4] + idx) - 1])) {
        ratio = (solution_xstar[workingset_indexUB[idx] - 1] -
                 workingset_ub[workingset_indexUB[idx] - 1]) -
                phaseOneCorrectionX;
        pk_corrected =
            std::fmin(std::abs(ratio), 1.0E-6 - ratio) / pk_corrected;
        if (pk_corrected < alpha) {
          alpha = pk_corrected;
          constrType = 5;
          constrIdx = idx + 1;
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
