//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: driver1.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

#ifndef DRIVER1_H
#define DRIVER1_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct h_struct_T;

struct g_struct_T;

struct i_struct_T;

struct d_struct_T;

struct e_struct_T;

struct f_struct_T;

struct j_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
void driver(const array<double, 2U> &H, const array<double, 1U> &f,
            h_struct_T &solution, g_struct_T &memspace, i_struct_T &workingset,
            d_struct_T &qrmanager, e_struct_T &cholmanager,
            f_struct_T &objective, j_struct_T &options,
            int runTimeOptions_MaxIterations);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for driver1.h
//
// [EOF]
//
