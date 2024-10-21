//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: changeEndSegBreaks.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

// Include Files
#include "changeEndSegBreaks.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : array<double, 2U> &oldBreaks
//                const array<double, 2U> &evalTime
// Return Type  : void
//
namespace coder {
namespace robotics {
namespace core {
namespace internal {
void changeEndSegBreaks(array<double, 2U> &oldBreaks,
                        const array<double, 2U> &evalTime)
{
  array<boolean_T, 2U> x;
  double dt;
  double oldBreaks_tmp;
  int idx;
  int ii;
  int ii_data;
  int loop_ub;
  unsigned int tGreaterThanTfIdx_data;
  boolean_T exitg1;
  dt = 0.01;
  x.set_size(1, evalTime.size(1));
  oldBreaks_tmp = oldBreaks[oldBreaks.size(1) - 2];
  loop_ub = evalTime.size(1);
  for (idx = 0; idx < loop_ub; idx++) {
    x[idx] = (evalTime[idx] > oldBreaks_tmp);
  }
  loop_ub = (x.size(1) >= 1);
  idx = 0;
  ii = 0;
  exitg1 = false;
  while ((!exitg1) && (ii <= x.size(1) - 1)) {
    if (x[ii]) {
      idx = 1;
      ii_data = ii + 1;
      exitg1 = true;
    } else {
      ii++;
    }
  }
  if (loop_ub == 1) {
    if (idx == 0) {
      loop_ub = 0;
    }
  } else {
    loop_ub = (idx >= 1);
  }
  if (loop_ub - 1 >= 0) {
    tGreaterThanTfIdx_data = static_cast<unsigned int>(ii_data);
  }
  if (loop_ub != 0) {
    dt = std::fmin((evalTime[static_cast<int>(tGreaterThanTfIdx_data) - 1] -
                    oldBreaks_tmp) /
                       2.0,
                   0.01);
  }
  oldBreaks[oldBreaks.size(1) - 2] = oldBreaks_tmp + dt;
}

} // namespace internal
} // namespace core
} // namespace robotics
} // namespace coder

//
// File trailer for changeEndSegBreaks.cpp
//
// [EOF]
//
