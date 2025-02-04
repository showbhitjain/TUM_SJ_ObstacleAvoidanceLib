//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: computeLinearResiduals.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef COMPUTELINEARRESIDUALS_H
#define COMPUTELINEARRESIDUALS_H

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
namespace internal {
void computeLinearResiduals(const array<double, 1U> &x, int nVar,
                            array<double, 1U> &workspaceIneq, int mLinIneq,
                            const array<double, 1U> &AineqT,
                            const array<double, 2U> &bineq, int ldAi,
                            double workspaceEq_data[], int &workspaceEq_size,
                            int mLinEq, const array<double, 1U> &AeqT,
                            const double beq_data[], int ldAe);

}
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeLinearResiduals.h
//
// [EOF]
//
