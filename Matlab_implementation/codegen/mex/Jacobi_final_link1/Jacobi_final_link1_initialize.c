/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Jacobi_final_link1_initialize.c
 *
 * Code generation for function 'Jacobi_final_link1_initialize'
 *
 */

/* Include files */
#include "Jacobi_final_link1_initialize.h"
#include "Jacobi_final_link1_data.h"
#include "_coder_Jacobi_final_link1_mex.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void Jacobi_final_link1_once(void);

/* Function Definitions */
static void Jacobi_final_link1_once(void)
{
  mex_InitInfAndNan();
}

void Jacobi_final_link1_initialize(void)
{
  static const volatile char_T *emlrtBreakCheckR2012bFlagVar = NULL;
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtBreakCheckR2012bFlagVar = emlrtGetBreakCheckFlagAddressR2022b(&st);
  emlrtClearAllocCountR2012b(&st, false, 0U, NULL);
  emlrtEnterRtStackR2012b(&st);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    Jacobi_final_link1_once();
  }
}

/* End of code generation (Jacobi_final_link1_initialize.c) */
