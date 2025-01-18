//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: addBoundToActiveSetMatrix_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "addBoundToActiveSetMatrix_.h"
#include "inverseKinematicsOA_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int TYPE
//                int idx_local
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void addBoundToActiveSetMatrix_(d_struct_T &obj, int TYPE, int idx_local)
{
  int colOffset;
  int i;
  int idx_bnd_local;
  obj.nWConstr[TYPE - 1]++;
  obj.isActiveConstr[(obj.isActiveIdx[TYPE - 1] + idx_local) - 2] = true;
  obj.nActiveConstr++;
  i = obj.nActiveConstr - 1;
  obj.Wid[i] = TYPE;
  obj.Wlocalidx[i] = idx_local;
  colOffset = obj.ldA * i - 1;
  if (TYPE == 5) {
    idx_bnd_local = obj.indexUB[idx_local - 1];
    obj.bwset[i] = obj.ub[idx_bnd_local - 1];
  } else {
    idx_bnd_local = obj.indexLB[idx_local - 1];
    obj.bwset[i] = obj.lb[idx_bnd_local - 1];
  }
  for (int idx{0}; idx <= idx_bnd_local - 2; idx++) {
    obj.ATwset[(idx + colOffset) + 1] = 0.0;
  }
  obj.ATwset[idx_bnd_local + colOffset] =
      2.0 * static_cast<double>(TYPE == 5) - 1.0;
  i = idx_bnd_local + 1;
  idx_bnd_local = obj.nVar;
  for (int idx{i}; idx <= idx_bnd_local; idx++) {
    obj.ATwset[idx + colOffset] = 0.0;
  }
  switch (obj.probType) {
  case 3:
  case 2:
    break;
  default:
    obj.ATwset[obj.nVar + colOffset] = -1.0;
    break;
  }
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for addBoundToActiveSetMatrix_.cpp
//
// [EOF]
//
