//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ppval.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef PPVAL_H
#define PPVAL_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace trajectoryGeneration {
struct struct_T;

struct b_struct_T;

struct c_struct_T;

struct d_struct_T;

} // namespace trajectoryGeneration

// Function Declarations
namespace trajectoryGeneration {
namespace coder {
void b_ppval(const ::coder::array<double, 2U> &pp_breaks,
             const ::coder::array<double, 3U> &pp_coefs,
             const ::coder::array<double, 2U> &x,
             ::coder::array<double, 2U> &v);

void ppval(const struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v);

void ppval(const b_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v);

void ppval(const c_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v);

void ppval(const d_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v);

void ppval(const ::coder::array<double, 2U> &pp_breaks,
           const ::coder::array<double, 3U> &pp_coefs,
           const ::coder::array<double, 2U> &x, ::coder::array<double, 2U> &v);

} // namespace coder
} // namespace trajectoryGeneration

#endif
//
// File trailer for ppval.h
//
// [EOF]
//
