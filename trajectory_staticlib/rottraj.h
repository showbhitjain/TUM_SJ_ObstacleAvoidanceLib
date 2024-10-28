//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rottraj.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef ROTTRAJ_H
#define ROTTRAJ_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace trajectoryGeneration {
namespace coder {
class quaternion;

class b_quaternion;

} // namespace coder
} // namespace trajectoryGeneration

// Function Declarations
namespace trajectoryGeneration {
namespace coder {
void rottraj(const quaternion R0, const quaternion RF,
             const ::coder::array<double, 2U> &t,
             const ::coder::array<double, 2U> &varargin_2, b_quaternion &R,
             ::coder::array<double, 2U> &omega,
             ::coder::array<double, 2U> &alpha);

}
} // namespace trajectoryGeneration

#endif
//
// File trailer for rottraj.h
//
// [EOF]
//
