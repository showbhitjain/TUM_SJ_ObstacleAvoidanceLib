//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: countsort.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "countsort.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cstring>

// Function Definitions
//
// Arguments    : array<int, 1U> &x
//                int xLen
//                array<int, 1U> &workspace
//                int xMin
//                int xMax
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
void countsort(array<int, 1U> &x, int xLen, array<int, 1U> &workspace, int xMin,
               int xMax)
{
  if ((xLen > 1) && (xMax > xMin)) {
    int idxEnd;
    int idxStart;
    int maxOffset;
    idxStart = xMax - xMin;
    if (static_cast<int>(idxStart + 1 < 200)) {
      for (int idx{0}; idx <= idxStart; idx++) {
        workspace[idx] = 0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = 0; idx <= idxStart; idx++) {
        workspace[idx] = 0;
      }
    }
    maxOffset = idxStart - 1;
    for (idxEnd = 0; idxEnd < xLen; idxEnd++) {
      idxStart = x[idxEnd] - xMin;
      workspace[idxStart] = workspace[idxStart] + 1;
    }
    for (idxEnd = 2; idxEnd <= maxOffset + 2; idxEnd++) {
      workspace[idxEnd - 1] = workspace[idxEnd - 1] + workspace[idxEnd - 2];
    }
    idxStart = 1;
    idxEnd = workspace[0];
    for (int idxW{0}; idxW <= maxOffset; idxW++) {
      for (int idxFill{idxStart}; idxFill <= idxEnd; idxFill++) {
        x[idxFill - 1] = idxW + xMin;
      }
      idxStart = workspace[idxW] + 1;
      idxEnd = workspace[idxW + 1];
    }
    if (static_cast<int>((idxEnd - idxStart) + 1 < 200)) {
      for (int idx{idxStart}; idx <= idxEnd; idx++) {
        x[idx - 1] = xMax;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = idxStart; idx <= idxEnd; idx++) {
        x[idx - 1] = xMax;
      }
    }
  }
}

} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for countsort.cpp
//
// [EOF]
//
