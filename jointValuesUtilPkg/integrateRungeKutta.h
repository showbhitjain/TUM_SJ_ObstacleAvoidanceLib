//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: integrateRungeKutta.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

#ifndef INTEGRATERUNGEKUTTA_H
#define INTEGRATERUNGEKUTTA_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void integrateRungeKutta(
    const coder::array<double, 1U> &desired_joint_velocity_current,
    const double tspan[2], const coder::array<double, 1U> &start_joint_Velocity,
    coder::array<double, 1U> &desired_joint_vector);

#endif
//
// File trailer for integrateRungeKutta.h
//
// [EOF]
//
