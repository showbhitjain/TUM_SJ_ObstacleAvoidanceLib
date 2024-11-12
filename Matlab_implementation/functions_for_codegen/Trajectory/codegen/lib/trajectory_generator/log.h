//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: log.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Oct-2024 03:49:55
//

#ifndef LOG_H
#define LOG_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace TrajectoryGeneration {
namespace coder {
class quaternion;

}
} // namespace TrajectoryGeneration

// Function Declarations
namespace TrajectoryGeneration {
void binary_expand_op(double &in1, const coder::quaternion *in3,
                      const int in4_size[2], const double in5_data[],
                      const int in5_size[2]);

void binary_expand_op_1(double &in1, const coder::quaternion *in3,
                        const int in4_size[2], const double in5_data[],
                        const int in5_size[2]);

void binary_expand_op_2(double &in1, const coder::quaternion *in3,
                        const int in4_size[2], const double in5_data[],
                        const int in5_size[2]);

} // namespace TrajectoryGeneration

#endif
//
// File trailer for log.h
//
// [EOF]
//
