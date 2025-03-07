//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compute_deltax.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 05-Mar-2025 16:53:20
//

#ifndef COMPUTE_DELTAX_H
#define COMPUTE_DELTAX_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct j_struct_T;

struct h_struct_T;

struct e_struct_T;

struct f_struct_T;

struct g_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
void compute_deltax(const array<double, 2U> &H, j_struct_T &solution,
                    h_struct_T &memspace, const e_struct_T &qrmanager,
                    f_struct_T &cholmanager, const g_struct_T &objective,
                    boolean_T alwaysPositiveDef);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for compute_deltax.h
//
// [EOF]
//
