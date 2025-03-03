//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: countsort.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "countsort.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Variable Definitions
static rtBoundsCheckInfo
    c_emlrtBCI{
        -1,          // iFirst
        -1,          // iLast
        1,           // lineNo
        1,           // colNo
        "",          // aName
        "countsort", // fName
        "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+utils/"
        "countsort.p", // pName
        0              // checkKind
    };

// Function Definitions
//
// Arguments    : array<int, 1U> &x
//                int xLen
//                array<int, 1U> &workspace
//                int xMax
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
void countsort(array<int, 1U> &x, int xLen, array<int, 1U> &workspace, int xMax)
{
  if ((xLen > 1) && (xMax > 1)) {
    int i;
    int i1;
    int i2;
    int idxEnd;
    int idxStart;
    if (xMax > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < xMax; idx++) {
      i = workspace.size(0);
      if (idx + 1 > i) {
        rtDynamicBoundsError(idx + 1, 1, i, c_emlrtBCI);
      }
      workspace[idx] = 0;
    }
    if (xLen > 2147483646) {
      check_forloop_overflow_error();
    }
    i = x.size(0);
    i1 = x.size(0);
    for (int idx{0}; idx < xLen; idx++) {
      i2 = workspace.size(0);
      if (idx + 1 > i) {
        rtDynamicBoundsError(idx + 1, 1, i, c_emlrtBCI);
      }
      idxStart = x[idx];
      if ((idxStart < 1) || (idxStart > i2)) {
        rtDynamicBoundsError(idxStart, 1, i2, c_emlrtBCI);
      }
      if (idx + 1 > i1) {
        rtDynamicBoundsError(idx + 1, 1, i1, c_emlrtBCI);
      }
      workspace[idxStart - 1] = workspace[idxStart - 1] + 1;
    }
    for (int idx{2}; idx <= xMax; idx++) {
      i = workspace.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      i = workspace.size(0);
      if (idx - 1 > i) {
        rtDynamicBoundsError(idx - 1, 1, i, c_emlrtBCI);
      }
      i = workspace.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      workspace[idx - 1] = workspace[idx - 1] + workspace[idx - 2];
    }
    idxStart = 1;
    idxEnd = workspace[0];
    i = workspace.size(0);
    i1 = workspace.size(0);
    for (int idx{0}; idx <= xMax - 2; idx++) {
      if ((idxStart <= idxEnd) && (idxEnd > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idxFill{idxStart}; idxFill <= idxEnd; idxFill++) {
        i2 = x.size(0);
        if ((idxFill < 1) || (idxFill > i2)) {
          rtDynamicBoundsError(idxFill, 1, i2, c_emlrtBCI);
        }
        x[idxFill - 1] = idx + 1;
      }
      if (idx + 1 > i) {
        rtDynamicBoundsError(idx + 1, 1, i, c_emlrtBCI);
      }
      idxStart = workspace[idx] + 1;
      if (idx + 2 > i1) {
        rtDynamicBoundsError(idx + 2, 1, i1, c_emlrtBCI);
      }
      idxEnd = workspace[idx + 1];
    }
    if ((idxStart <= idxEnd) && (idxEnd > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx{idxStart}; idx <= idxEnd; idx++) {
      i = x.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      x[idx - 1] = xMax;
    }
  }
}

//
// Arguments    : array<int, 1U> &x
//                int xLen
//                array<int, 1U> &workspace
//                int xMin
//                int xMax
// Return Type  : void
//
void countsort(array<int, 1U> &x, int xLen, array<int, 1U> &workspace, int xMin,
               int xMax)
{
  if ((xLen > 1) && (xMax > xMin)) {
    int b_tmp;
    int i;
    int idxEnd;
    int idxStart;
    int idxW;
    b_tmp = (xMax - xMin) + 1;
    if (b_tmp > 2147483646) {
      check_forloop_overflow_error();
    }
    for (int idx{0}; idx < b_tmp; idx++) {
      i = workspace.size(0);
      if ((idx + 1 < 1) || (idx + 1 > i)) {
        rtDynamicBoundsError(idx + 1, 1, i, c_emlrtBCI);
      }
      workspace[idx] = 0;
    }
    if (xLen > 2147483646) {
      check_forloop_overflow_error();
    }
    i = x.size(0);
    idxStart = x.size(0);
    for (int idx{0}; idx < xLen; idx++) {
      idxEnd = workspace.size(0);
      if (idx + 1 > i) {
        rtDynamicBoundsError(idx + 1, 1, i, c_emlrtBCI);
      }
      idxW = (x[idx] - xMin) + 1;
      if ((idxW < 1) || (idxW > idxEnd)) {
        rtDynamicBoundsError(idxW, 1, idxEnd, c_emlrtBCI);
      }
      if (idx + 1 > idxStart) {
        rtDynamicBoundsError(idx + 1, 1, idxStart, c_emlrtBCI);
      }
      workspace[idxW - 1] = workspace[idxW - 1] + 1;
    }
    for (int idx{2}; idx <= b_tmp; idx++) {
      i = workspace.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      i = workspace.size(0);
      if (idx - 1 > i) {
        rtDynamicBoundsError(idx - 1, 1, i, c_emlrtBCI);
      }
      i = workspace.size(0);
      if (idx > i) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      workspace[idx - 1] = workspace[idx - 1] + workspace[idx - 2];
    }
    idxStart = 1;
    idxEnd = workspace[0];
    if (b_tmp - 1 > 2147483646) {
      check_forloop_overflow_error();
    }
    for (idxW = 0; idxW <= b_tmp - 2; idxW++) {
      if ((idxStart <= idxEnd) && (idxEnd > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int idx{idxStart}; idx <= idxEnd; idx++) {
        i = x.size(0);
        if ((idx < 1) || (idx > i)) {
          rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
        }
        x[idx - 1] = idxW + xMin;
      }
      i = workspace.size(0);
      if ((idxW + 1 < 1) || (idxW + 1 > i)) {
        rtDynamicBoundsError(idxW + 1, 1, i, c_emlrtBCI);
      }
      idxStart = workspace[idxW] + 1;
      i = workspace.size(0);
      if ((idxW + 2 < 1) || (idxW + 2 > i)) {
        rtDynamicBoundsError(idxW + 2, 1, i, c_emlrtBCI);
      }
      idxEnd = workspace[idxW + 1];
    }
    if ((idxStart <= idxEnd) && (idxEnd > 2147483646)) {
      check_forloop_overflow_error();
    }
    for (int idx{idxStart}; idx <= idxEnd; idx++) {
      i = x.size(0);
      if ((idx < 1) || (idx > i)) {
        rtDynamicBoundsError(idx, 1, i, c_emlrtBCI);
      }
      x[idx - 1] = xMax;
    }
  }
}

} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for countsort.cpp
//
// [EOF]
//
