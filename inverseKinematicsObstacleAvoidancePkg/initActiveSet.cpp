//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: initActiveSet.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "initActiveSet.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "setProblemType.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void initActiveSet(d_struct_T &obj)
{
  int i;
  int idxFillStart;
  int idx_global;
  int nWFixed;
  setProblemType(obj, 3);
  idxFillStart = obj.isActiveIdx[2];
  i = obj.mConstrMax;
  for (idx_global = idxFillStart; idx_global <= i; idx_global++) {
    obj.isActiveConstr[idx_global - 1] = false;
  }
  obj.nWConstr[0] = obj.sizes[0];
  obj.nWConstr[1] = obj.sizes[1];
  obj.nWConstr[2] = 0;
  obj.nWConstr[3] = 0;
  obj.nWConstr[4] = 0;
  obj.nActiveConstr = obj.nWConstr[0] + obj.nWConstr[1];
  nWFixed = obj.sizes[0];
  for (int idx_local{0}; idx_local < nWFixed; idx_local++) {
    obj.Wid[idx_local] = 1;
    obj.Wlocalidx[idx_local] = idx_local + 1;
    obj.isActiveConstr[idx_local] = true;
    idxFillStart = obj.ldA * idx_local;
    i = obj.indexFixed[idx_local];
    for (int b_i{0}; b_i <= i - 2; b_i++) {
      obj.ATwset[b_i + idxFillStart] = 0.0;
    }
    obj.ATwset[(obj.indexFixed[idx_local] + idxFillStart) - 1] = 1.0;
    i = obj.indexFixed[idx_local] + 1;
    idx_global = obj.nVar;
    for (int b_i{i}; b_i <= idx_global; b_i++) {
      obj.ATwset[(b_i + idxFillStart) - 1] = 0.0;
    }
    obj.bwset[idx_local] = obj.ub[obj.indexFixed[idx_local] - 1];
  }
  idxFillStart = obj.sizes[1];
  for (int idx_local{0}; idx_local < idxFillStart; idx_local++) {
    int iATw0;
    int iAeq0;
    idx_global = nWFixed + idx_local;
    obj.Wid[idx_global] = 2;
    obj.Wlocalidx[idx_global] = idx_local + 1;
    obj.isActiveConstr[idx_global] = true;
    iAeq0 = obj.ldA * idx_local;
    iATw0 = obj.ldA * idx_global;
    i = obj.nVar - 1;
    for (int b_i{0}; b_i <= i; b_i++) {
      obj.ATwset[iATw0 + b_i] = obj.Aeq[iAeq0 + b_i];
    }
    obj.bwset[idx_global] = obj.beq.data[idx_local];
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for initActiveSet.cpp
//
// [EOF]
//
