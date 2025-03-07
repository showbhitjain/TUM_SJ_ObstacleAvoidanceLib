//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: fmincon.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

#ifndef FMINCON_H
#define FMINCON_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace coder {
class anonymous_function;

}

// Function Declarations
namespace coder {
double fmincon(const anonymous_function &fun, array<double, 1U> &x0,
               const array<double, 2U> &Aineq, const array<double, 2U> &bineq,
               const array<double, 2U> &Aeq, const double beq_data[],
               const int beq_size[2], const array<double, 1U> &lb,
               const array<double, 1U> &ub, double *exitflag);

}

#endif
//
// File trailer for fmincon.h
//
// [EOF]
//
