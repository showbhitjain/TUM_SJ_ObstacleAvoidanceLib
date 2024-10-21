/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Hessian_final_link_terminate.c
 *
 * Code generation for function 'Hessian_final_link_terminate'
 *
 */

/* Include files */
#include "Hessian_final_link_terminate.h"
#include "Hessian_final_link_data.h"
#include "_coder_Hessian_final_link_mex.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void Hessian_final_link_atexit(void)
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

void Hessian_final_link_terminate(void)
{
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (Hessian_final_link_terminate.c) */
