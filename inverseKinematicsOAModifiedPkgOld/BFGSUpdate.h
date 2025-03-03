//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BFGSUpdate.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef BFGSUPDATE_H
#define BFGSUPDATE_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
boolean_T BFGSUpdate(int nvar, array<double, 2U> &Bk,
                     const array<double, 1U> &sk, array<double, 1U> &yk,
                     array<double, 2U> &workspace);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for BFGSUpdate.h
//
// [EOF]
//
