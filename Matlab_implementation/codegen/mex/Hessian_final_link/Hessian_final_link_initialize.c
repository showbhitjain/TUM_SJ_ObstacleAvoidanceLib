/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Hessian_final_link_initialize.c
 *
 * Code generation for function 'Hessian_final_link_initialize'
 *
 */

/* Include files */
#include "Hessian_final_link_initialize.h"
#include "Hessian_final_link_data.h"
#include "_coder_Hessian_final_link_mex.h"
#include "rt_nonfinite.h"

/* Function Declarations */
static void Hessian_final_link_once(void);

/* Function Definitions */
static void Hessian_final_link_once(void)
{
  mex_InitInfAndNan();
}

void Hessian_final_link_initialize(void)
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
    Hessian_final_link_once();
  }
}

/* End of code generation (Hessian_final_link_initialize.c) */
