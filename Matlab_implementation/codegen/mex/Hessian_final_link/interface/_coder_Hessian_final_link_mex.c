/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_Hessian_final_link_mex.c
 *
 * Code generation for function '_coder_Hessian_final_link_mex'
 *
 */

/* Include files */
#include "_coder_Hessian_final_link_mex.h"
#include "Hessian_final_link_data.h"
#include "Hessian_final_link_initialize.h"
#include "Hessian_final_link_terminate.h"
#include "_coder_Hessian_final_link_api.h"
#include "rt_nonfinite.h"

/* Function Definitions */
void Hessian_final_link_mexFunction(int32_T nlhs, mxArray *plhs[1],
                                    int32_T nrhs, const mxArray *prhs[1])
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 1, 4,
                        18, "Hessian_final_link");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 18,
                        "Hessian_final_link");
  }
  /* Call the function. */
  Hessian_final_link_api(prhs[0], &outputs);
  /* Copy over outputs to the caller. */
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&Hessian_final_link_atexit);
  /* Module initialization. */
  Hessian_final_link_initialize();
  /* Dispatch the entry-point. */
  Hessian_final_link_mexFunction(nlhs, plhs, nrhs, prhs);
  /* Module termination. */
  Hessian_final_link_terminate();
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL, "UTF-8", true);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_Hessian_final_link_mex.c) */
