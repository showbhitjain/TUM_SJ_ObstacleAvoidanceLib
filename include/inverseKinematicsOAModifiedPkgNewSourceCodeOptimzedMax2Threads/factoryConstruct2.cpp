//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: factoryConstruct2.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "factoryConstruct2.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int mIneqMax
//                int mEqMax
//                int nVar
//                int nVarMax
//                int mConstrMax
//                d_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void factoryConstruct(int mIneqMax, int mEqMax, int nVar, int nVarMax,
                      int mConstrMax, d_struct_T &obj)
{
  obj.mConstr = 0;
  obj.mConstrOrig = 0;
  obj.mConstrMax = mConstrMax;
  obj.nVar = nVar;
  obj.nVarOrig = nVar;
  obj.nVarMax = nVarMax;
  obj.ldA = nVarMax;
  obj.Aineq.set_size(mIneqMax * nVarMax);
  obj.bineq.set_size(mIneqMax);
  obj.Aeq.set_size(mEqMax * nVarMax);
  obj.beq.size[0] = mEqMax;
  obj.lb.set_size(nVarMax);
  obj.ub.set_size(nVarMax);
  obj.indexLB.set_size(nVarMax);
  obj.indexUB.set_size(nVarMax);
  obj.indexFixed.set_size(nVarMax);
  obj.mEqRemoved = 0;
  obj.indexEqRemoved.size[0] = mEqMax;
  obj.ATwset.set_size(nVarMax * mConstrMax);
  obj.bwset.set_size(mConstrMax);
  obj.nActiveConstr = 0;
  obj.maxConstrWorkspace.set_size(mConstrMax);
  obj.sizes[0] = 0;
  obj.sizesNormal[0] = 0;
  obj.sizesPhaseOne[0] = 0;
  obj.sizesRegularized[0] = 0;
  obj.sizesRegPhaseOne[0] = 0;
  obj.sizes[1] = 0;
  obj.sizesNormal[1] = 0;
  obj.sizesPhaseOne[1] = 0;
  obj.sizesRegularized[1] = 0;
  obj.sizesRegPhaseOne[1] = 0;
  obj.sizes[2] = 0;
  obj.sizesNormal[2] = 0;
  obj.sizesPhaseOne[2] = 0;
  obj.sizesRegularized[2] = 0;
  obj.sizesRegPhaseOne[2] = 0;
  obj.sizes[3] = 0;
  obj.sizesNormal[3] = 0;
  obj.sizesPhaseOne[3] = 0;
  obj.sizesRegularized[3] = 0;
  obj.sizesRegPhaseOne[3] = 0;
  obj.sizes[4] = 0;
  obj.sizesNormal[4] = 0;
  obj.sizesPhaseOne[4] = 0;
  obj.sizesRegularized[4] = 0;
  obj.sizesRegPhaseOne[4] = 0;
  obj.isActiveIdx[0] = 0;
  obj.isActiveIdxNormal[0] = 0;
  obj.isActiveIdxPhaseOne[0] = 0;
  obj.isActiveIdxRegularized[0] = 0;
  obj.isActiveIdxRegPhaseOne[0] = 0;
  obj.isActiveIdx[1] = 0;
  obj.isActiveIdxNormal[1] = 0;
  obj.isActiveIdxPhaseOne[1] = 0;
  obj.isActiveIdxRegularized[1] = 0;
  obj.isActiveIdxRegPhaseOne[1] = 0;
  obj.isActiveIdx[2] = 0;
  obj.isActiveIdxNormal[2] = 0;
  obj.isActiveIdxPhaseOne[2] = 0;
  obj.isActiveIdxRegularized[2] = 0;
  obj.isActiveIdxRegPhaseOne[2] = 0;
  obj.isActiveIdx[3] = 0;
  obj.isActiveIdxNormal[3] = 0;
  obj.isActiveIdxPhaseOne[3] = 0;
  obj.isActiveIdxRegularized[3] = 0;
  obj.isActiveIdxRegPhaseOne[3] = 0;
  obj.isActiveIdx[4] = 0;
  obj.isActiveIdxNormal[4] = 0;
  obj.isActiveIdxPhaseOne[4] = 0;
  obj.isActiveIdxRegularized[4] = 0;
  obj.isActiveIdxRegPhaseOne[4] = 0;
  obj.isActiveIdx[5] = 0;
  obj.isActiveIdxNormal[5] = 0;
  obj.isActiveIdxPhaseOne[5] = 0;
  obj.isActiveIdxRegularized[5] = 0;
  obj.isActiveIdxRegPhaseOne[5] = 0;
  obj.isActiveConstr.set_size(mConstrMax);
  obj.Wid.set_size(mConstrMax);
  obj.Wlocalidx.set_size(mConstrMax);
  obj.nWConstr[0] = 0;
  obj.nWConstr[1] = 0;
  obj.nWConstr[2] = 0;
  obj.nWConstr[3] = 0;
  obj.nWConstr[4] = 0;
  obj.probType = 3;
  obj.SLACK0 = 1.0E-5;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factoryConstruct2.cpp
//
// [EOF]
//
