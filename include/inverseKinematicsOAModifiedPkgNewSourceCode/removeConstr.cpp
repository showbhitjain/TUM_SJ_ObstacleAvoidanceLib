//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeConstr.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 06-Mar-2025 15:16:52
//

// Include Files
#include "removeConstr.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int idx_global
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void removeConstr(d_struct_T &obj, int idx_global)
{
  int TYPE_tmp;
  int i;
  int i1;
  TYPE_tmp = obj.Wid[idx_global - 1] - 1;
  obj.isActiveConstr[(obj.isActiveIdx[TYPE_tmp] +
                      obj.Wlocalidx[idx_global - 1]) -
                     2] = false;
  i = obj.nActiveConstr - 1;
  obj.Wid[idx_global - 1] = obj.Wid[i];
  obj.Wlocalidx[idx_global - 1] = obj.Wlocalidx[i];
  i1 = obj.nVar;
  for (int idx{0}; idx < i1; idx++) {
    obj.ATwset[idx + obj.ldA * (idx_global - 1)] =
        obj.ATwset[idx + obj.ldA * i];
  }
  obj.bwset[idx_global - 1] = obj.bwset[i];
  obj.nActiveConstr = i;
  obj.nWConstr[TYPE_tmp]--;
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for removeConstr.cpp
//
// [EOF]
//
