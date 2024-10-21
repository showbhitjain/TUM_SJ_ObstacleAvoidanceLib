/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Jacobi_final_link1_terminate.c
 *
 * Code generation for function 'Jacobi_final_link1_terminate'
 *
 */

/* Include files */
#include "Jacobi_final_link1_terminate.h"
#include "Jacobi_final_link1_data.h"
#include "_coder_Jacobi_final_link1_mex.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void Jacobi_final_link1_atexit(void)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  emlrtExitTimeCleanup(&emlrtContextGlobal);
}

void Jacobi_final_link1_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (Jacobi_final_link1_terminate.c) */
