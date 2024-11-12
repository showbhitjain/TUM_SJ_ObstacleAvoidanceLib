//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compressBounds.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef COMPRESSBOUNDS_H
#define COMPRESSBOUNDS_H

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
namespace initialize {
int compressBounds(int nVar, array<int, 1U> &indexLB, array<int, 1U> &indexUB,
                   array<int, 1U> &indexFixed, const array<double, 1U> &lb,
                   const array<double, 1U> &ub, int &mUB, int &mFixed);

}
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for compressBounds.h
//
// [EOF]
//
