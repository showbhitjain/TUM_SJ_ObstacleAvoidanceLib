//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeMeritFcn.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

#ifndef COMPUTEMERITFCN_H
#define COMPUTEMERITFCN_H

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
namespace MeritFunction {
double computeMeritFcn(double obj_penaltyParam, double fval,
                       const array<double, 1U> &Cineq_workspace, int mIneq,
                       const double Ceq_workspace_data[], int mEq,
                       boolean_T evalWellDefined);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeMeritFcn.h
//
// [EOF]
//
