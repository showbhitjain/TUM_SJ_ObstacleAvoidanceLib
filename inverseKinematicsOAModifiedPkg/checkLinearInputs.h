//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: checkLinearInputs.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef CHECKLINEARINPUTS_H
#define CHECKLINEARINPUTS_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace validate {
double checkLinearInputs(int nVar, const array<double, 2U> &Aineq,
                         const array<double, 2U> &bineq,
                         const array<double, 2U> &Aeq, const double beq_data[],
                         const int beq_size[2], const array<double, 2U> &lb,
                         const array<double, 2U> &ub);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for checkLinearInputs.h
//
// [EOF]
//
