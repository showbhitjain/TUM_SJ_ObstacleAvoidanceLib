//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: assignResidualsToXSlack.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

#ifndef ASSIGNRESIDUALSTOXSLACK_H
#define ASSIGNRESIDUALSTOXSLACK_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct d_struct_T;

struct j_struct_T;

struct h_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace step {
namespace relaxed {
void assignResidualsToXSlack(int nVarOrig, d_struct_T &WorkingSet,
                             j_struct_T &b_TrialState, h_struct_T &memspace);

}
} // namespace step
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for assignResidualsToXSlack.h
//
// [EOF]
//
