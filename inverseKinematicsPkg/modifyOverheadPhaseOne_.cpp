//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: modifyOverheadPhaseOne_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "modifyOverheadPhaseOne_.h"
#include "inverseKinematics_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : i_struct_T &obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void modifyOverheadPhaseOne_(i_struct_T &obj)
{
  int i;
  int idxEq;
  i = obj.sizes[0];
  for (int idx{0}; idx < i; idx++) {
    obj.ATwset[(obj.nVar + obj.ldA * idx) - 1] = 0.0;
  }
  i = obj.sizes[1];
  for (int idx{0}; idx < i; idx++) {
    idxEq = (obj.nVar + obj.ldA * idx) - 1;
    obj.Aeq[idxEq] = 0.0;
    obj.ATwset[idxEq + obj.ldA * (obj.isActiveIdx[1] - 1)] = 0.0;
  }
  obj.indexLB[obj.sizes[3] - 1] = obj.nVar;
  obj.lb[obj.nVar - 1] = 1.0E-5;
  idxEq = obj.isActiveIdx[2];
  i = obj.nActiveConstr;
  for (int idx{idxEq}; idx <= i; idx++) {
    obj.ATwset[(obj.nVar + obj.ldA * (idx - 1)) - 1] = -1.0;
  }
  if (obj.nWConstr[4] > 0) {
    i = obj.sizesNormal[4];
    for (int idx{0}; idx <= i; idx++) {
      obj.isActiveConstr[(obj.isActiveIdx[4] + idx) - 1] = false;
    }
  }
  obj.isActiveConstr[obj.isActiveIdx[4] - 2] = false;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for modifyOverheadPhaseOne_.cpp
//
// [EOF]
//
