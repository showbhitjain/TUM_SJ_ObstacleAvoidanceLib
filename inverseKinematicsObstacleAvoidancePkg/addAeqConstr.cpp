//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: addAeqConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "addAeqConstr.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int idx_local
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void addAeqConstr(d_struct_T &obj, int idx_local)
{
  int totalEq;
  totalEq = obj.nWConstr[0] + obj.nWConstr[1];
  if ((obj.nActiveConstr == totalEq) && (idx_local > obj.nWConstr[1])) {
    int i;
    int i1;
    int iAeq0;
    int iAw0;
    obj.nWConstr[1]++;
    obj.isActiveConstr[(obj.isActiveIdx[1] + idx_local) - 2] = true;
    obj.nActiveConstr++;
    i = obj.nActiveConstr - 1;
    obj.Wid[i] = 2;
    obj.Wlocalidx[i] = idx_local;
    iAeq0 = obj.ldA * (idx_local - 1);
    iAw0 = obj.ldA * i;
    i1 = obj.nVar;
    for (int idx{0}; idx < i1; idx++) {
      obj.ATwset[iAw0 + idx] = obj.Aeq[iAeq0 + idx];
    }
    obj.bwset[i] = obj.beq.data[idx_local - 1];
  } else {
    int i;
    int i1;
    int iAeq0;
    int iAw0;
    obj.nActiveConstr++;
    i = obj.nActiveConstr - 1;
    obj.Wid[i] = obj.Wid[totalEq];
    obj.Wlocalidx[i] = obj.Wlocalidx[totalEq];
    i1 = obj.nVar;
    for (int idx{0}; idx < i1; idx++) {
      obj.ATwset[idx + obj.ldA * i] = obj.ATwset[idx + obj.ldA * totalEq];
    }
    obj.bwset[i] = obj.bwset[totalEq];
    obj.nWConstr[1]++;
    obj.isActiveConstr[(obj.isActiveIdx[1] + idx_local) - 2] = true;
    obj.Wid[totalEq] = 2;
    obj.Wlocalidx[totalEq] = idx_local;
    iAeq0 = obj.ldA * (idx_local - 1);
    iAw0 = obj.ldA * totalEq;
    for (int idx{0}; idx < i1; idx++) {
      obj.ATwset[iAw0 + idx] = obj.Aeq[iAeq0 + idx];
    }
    obj.bwset[totalEq] = obj.beq.data[idx_local - 1];
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for addAeqConstr.cpp
//
// [EOF]
//
