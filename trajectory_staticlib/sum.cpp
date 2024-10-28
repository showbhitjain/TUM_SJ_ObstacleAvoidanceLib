//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: sum.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

// Include Files
#include "sum.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : const ::coder::array<double, 2U> &x
//                double y[3]
// Return Type  : void
//
namespace trajectoryGeneration {
namespace coder {
void sum(const ::coder::array<double, 2U> &x, double y[3])
{
  int firstBlockLength;
  int lastBlockLength;
  int nblocks;
  int xoffset;
  if (x.size(1) <= 1024) {
    firstBlockLength = x.size(1);
    lastBlockLength = 0;
    nblocks = 1;
  } else {
    firstBlockLength = 1024;
    nblocks = static_cast<int>(static_cast<unsigned int>(x.size(1)) >> 10);
    lastBlockLength = x.size(1) - (nblocks << 10);
    if (lastBlockLength > 0) {
      nblocks++;
    } else {
      lastBlockLength = 1024;
    }
  }
  y[0] = x[0];
  y[1] = x[1];
  y[2] = x[2];
  for (int k{2}; k <= firstBlockLength; k++) {
    xoffset = (k - 1) * 3;
    y[0] += x[xoffset];
    y[1] += x[xoffset + 1];
    y[2] += x[xoffset + 2];
  }
  for (int ib{2}; ib <= nblocks; ib++) {
    double bsum_idx_0;
    double bsum_idx_1;
    double bsum_idx_2;
    int hi;
    firstBlockLength = (ib - 1) * 3072;
    bsum_idx_0 = x[firstBlockLength];
    bsum_idx_1 = x[firstBlockLength + 1];
    bsum_idx_2 = x[firstBlockLength + 2];
    if (ib == nblocks) {
      hi = lastBlockLength;
    } else {
      hi = 1024;
    }
    for (int k{2}; k <= hi; k++) {
      xoffset = firstBlockLength + (k - 1) * 3;
      bsum_idx_0 += x[xoffset];
      bsum_idx_1 += x[xoffset + 1];
      bsum_idx_2 += x[xoffset + 2];
    }
    y[0] += bsum_idx_0;
    y[1] += bsum_idx_1;
    y[2] += bsum_idx_2;
  }
}

} // namespace coder
} // namespace trajectoryGeneration

//
// File trailer for sum.cpp
//
// [EOF]
//
