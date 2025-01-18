//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factoryConstruct.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "factoryConstruct.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int nVarMax
//                int mConstrMax
//                int mIneq
//                int mEq
//                const array<double, 1U> &x0
//                j_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace fminconsqp {
namespace TrialState {
void factoryConstruct(int nVarMax, int mConstrMax, int mIneq, int mEq,
                      const array<double, 1U> &x0, j_struct_T &obj)
{
  obj.nVarMax = nVarMax;
  obj.mNonlinIneq = 0;
  obj.mNonlinEq = 0;
  obj.mIneq = mIneq;
  obj.mEq = mEq;
  obj.iNonIneq0 = mIneq + 1;
  obj.iNonEq0 = mEq + 1;
  obj.sqpFval = 0.0;
  obj.sqpFval_old = 0.0;
  obj.xstarsqp.set_size(x0.size(0));
  obj.xstarsqp_old.set_size(x0.size(0));
  obj.cIneq.set_size(mIneq);
  obj.cIneq_old.set_size(mIneq);
  obj.cEq.size[0] = mEq;
  obj.cEq_old.size[0] = mEq;
  obj.grad.set_size(nVarMax);
  obj.grad_old.set_size(nVarMax);
  obj.FunctionEvaluations = 0;
  obj.sqpIterations = 0;
  obj.sqpExitFlag = 0;
  obj.lambdasqp.set_size(mConstrMax);
  for (int i{0}; i < mConstrMax; i++) {
    obj.lambdasqp[i] = 0.0;
  }
  obj.lambdaStopTest.set_size(mConstrMax);
  obj.lambdaStopTestPrev.set_size(mConstrMax);
  obj.steplength = 1.0;
  obj.delta_x.set_size(nVarMax);
  for (int i{0}; i < nVarMax; i++) {
    obj.delta_x[i] = 0.0;
  }
  obj.socDirection.set_size(nVarMax);
  obj.workingset_old.set_size(mConstrMax);
  obj.gradLag.set_size(nVarMax);
  obj.delta_gradLag.set_size(nVarMax);
  obj.xstar.set_size(nVarMax);
  obj.fstar = 0.0;
  obj.firstorderopt = 0.0;
  obj.lambda.set_size(mConstrMax);
  for (int i{0}; i < mConstrMax; i++) {
    obj.lambda[i] = 0.0;
  }
  obj.state = 0;
  obj.maxConstr = 0.0;
  obj.iterations = 0;
  obj.searchDir.set_size(nVarMax);
}

} // namespace TrialState
} // namespace fminconsqp
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factoryConstruct.cpp
//
// [EOF]
//
