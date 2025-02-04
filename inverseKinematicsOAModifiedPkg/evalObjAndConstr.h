//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: evalObjAndConstr.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

#ifndef EVALOBJANDCONSTR_H
#define EVALOBJANDCONSTR_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace coder {
namespace internal {
class i_stickyStruct;

}
} // namespace coder

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace ObjNonlinEvaluator {
double evalObjAndConstr(const ::coder::internal::i_stickyStruct &obj,
                        const array<double, 1U> &x, int &status);

}
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for evalObjAndConstr.h
//
// [EOF]
//
