//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: utilsMatlab_initialize.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 14-Nov-2024 02:49:51
//

// Include Files
#include "utilsMatlab_initialize.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"
#include "utilsMatlab_data.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void utilsMatlab_initialize()
{
  eml_rand_mt19937ar_stateful_init();
  isInitialized_utilsMatlab = true;
}

//
// File trailer for utilsMatlab_initialize.cpp
//
// [EOF]
//
