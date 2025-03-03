//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: feasibleratiotest.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef FEASIBLERATIOTEST_H
#define FEASIBLERATIOTEST_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
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
                         int &constrIdx);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for feasibleratiotest.h
//
// [EOF]
//
