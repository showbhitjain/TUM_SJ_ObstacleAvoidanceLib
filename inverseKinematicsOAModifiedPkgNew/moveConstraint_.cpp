//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: moveConstraint_.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "moveConstraint_.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : d_struct_T &obj
//                int idx_global_start
//                int idx_global_dest
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
namespace WorkingSet {
void moveConstraint_(d_struct_T &obj, int idx_global_start, int idx_global_dest)
{
  int b;
  int i;
  i = obj.Wid.size(0);
  if ((idx_global_start < 1) || (idx_global_start > i)) {
    rtDynamicBoundsError(idx_global_start, 1, i, j_emlrtBCI);
  }
  i = obj.Wid.size(0);
  if ((idx_global_dest < 1) || (idx_global_dest > i)) {
    rtDynamicBoundsError(idx_global_dest, 1, i, j_emlrtBCI);
  }
  obj.Wid[idx_global_dest - 1] = obj.Wid[idx_global_start - 1];
  i = obj.Wlocalidx.size(0);
  if (idx_global_start > i) {
    rtDynamicBoundsError(idx_global_start, 1, i, j_emlrtBCI);
  }
  i = obj.Wlocalidx.size(0);
  if (idx_global_dest > i) {
    rtDynamicBoundsError(idx_global_dest, 1, i, j_emlrtBCI);
  }
  obj.Wlocalidx[idx_global_dest - 1] = obj.Wlocalidx[idx_global_start - 1];
  b = obj.nVar;
  if (obj.nVar > 2147483646) {
    check_forloop_overflow_error();
  }
  for (int idx{0}; idx < b; idx++) {
    int i1;
    int i2;
    i = obj.ATwset.size(0);
    i1 = (idx + obj.ldA * (idx_global_start - 1)) + 1;
    if ((i1 < 1) || (i1 > i)) {
      rtDynamicBoundsError(i1, 1, i, j_emlrtBCI);
    }
    i = obj.ATwset.size(0);
    i2 = (idx + obj.ldA * (idx_global_dest - 1)) + 1;
    if ((i2 < 1) || (i2 > i)) {
      rtDynamicBoundsError(i2, 1, i, j_emlrtBCI);
    }
    obj.ATwset[i2 - 1] = obj.ATwset[i1 - 1];
  }
  i = obj.bwset.size(0);
  if (idx_global_start > i) {
    rtDynamicBoundsError(idx_global_start, 1, i, j_emlrtBCI);
  }
  i = obj.bwset.size(0);
  if (idx_global_dest > i) {
    rtDynamicBoundsError(idx_global_dest, 1, i, j_emlrtBCI);
  }
  obj.bwset[idx_global_dest - 1] = obj.bwset[idx_global_start - 1];
}

} // namespace WorkingSet
} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for moveConstraint_.cpp
//
// [EOF]
//
