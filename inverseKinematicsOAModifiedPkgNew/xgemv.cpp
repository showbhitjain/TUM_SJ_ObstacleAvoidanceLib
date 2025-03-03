//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemv.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "xgemv.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 1U> &y
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void b_xgemv(int m, int n, const array<double, 1U> &A, int lda,
             const array<double, 1U> &x, array<double, 1U> &y)
{
  if (m != 0) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 2U> &A
//                int ia0
//                int lda
//                const array<double, 2U> &x
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &x, array<double, 1U> &y)
{
  if (m != 0) {
    int b;
    int ix;
    if (m > 2147483646) {
      check_forloop_overflow_error();
    }
    for (ix = 0; ix < m; ix++) {
      y[ix] = 0.0;
    }
    ix = 0;
    b = ia0 + lda * (n - 1);
    if ((ia0 <= b) && (b > MAX_int32_T - lda)) {
      check_forloop_overflow_error();
    }
    for (int iac{ia0}; lda < 0 ? iac >= b : iac <= b; iac += lda) {
      int b_b;
      b_b = (iac + m) - 1;
      if ((iac <= b_b) && (b_b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b_b; ia++) {
        int i;
        i = ia - iac;
        y[i] = y[i] + A[ia - 1] * x[ix];
      }
      ix++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 2U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 2U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 2U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 2U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int ix;
    boolean_T overflow;
    if (m > 2147483646) {
      check_forloop_overflow_error();
    }
    for (ix = 0; ix < m; ix++) {
      y[ix] = 0.0;
    }
    ix = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      int b;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        int i;
        i = ia - iac;
        y[i] = y[i] + A[ia - 1] * x[ix];
      }
      ix++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 2U> &x
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 2U> &x
//                int ix0
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[((ix0 + ia) - iac) - 1];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = -y[iy];
    }
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                int ix0
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, int ix0, array<double, 1U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int ix;
    boolean_T overflow;
    ix = ix0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      int b;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        int i;
        i = ia - iac;
        y[i] = y[i] + A[ia - 1] * x[ix - 1];
      }
      ix++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 2U> &A
//                int ia0
//                int lda
//                const array<double, 2U> &x
//                int ix0
//                array<double, 1U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &x, int ix0, array<double, 1U> &y)
{
  if (n != 0) {
    int b;
    int iy;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = 0.0;
    }
    iy = 0;
    b = ia0 + lda * (n - 1);
    if ((ia0 <= b) && (b > MAX_int32_T - lda)) {
      check_forloop_overflow_error();
    }
    for (int iac{ia0}; lda < 0 ? iac >= b : iac <= b; iac += lda) {
      double c;
      int b_b;
      c = 0.0;
      b_b = (iac + m) - 1;
      if ((iac <= b_b) && (b_b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b_b; ia++) {
        c += A[ia - 1] * x[((ix0 + ia) - iac) - 1];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 2U> &A
//                int lda
//                const array<double, 1U> &x
//                array<double, 2U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 2U> &A, int lda,
           const array<double, 1U> &x, array<double, 2U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int iy;
    boolean_T overflow;
    if (n > 2147483646) {
      check_forloop_overflow_error();
    }
    for (iy = 0; iy < n; iy++) {
      y[iy] = 0.0;
    }
    iy = 0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      double c;
      int b;
      c = 0.0;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] = y[iy] + c;
      iy++;
    }
  }
}

//
// Arguments    : int m
//                int n
//                const array<double, 1U> &A
//                int lda
//                const array<double, 1U> &x
//                int ix0
//                array<double, 2U> &y
// Return Type  : void
//
void xgemv(int m, int n, const array<double, 1U> &A, int lda,
           const array<double, 1U> &x, int ix0, array<double, 2U> &y)
{
  if ((m != 0) && (n != 0)) {
    int b_tmp;
    int ix;
    boolean_T overflow;
    ix = ix0;
    b_tmp = lda * (n - 1) + 1;
    if ((lda == 0) || ((lda > 0) && (b_tmp < 1)) ||
        ((lda < 0) && (b_tmp > 1))) {
      overflow = false;
    } else if (lda > 0) {
      overflow = (b_tmp > MAX_int32_T - lda);
    } else {
      overflow = (b_tmp < MIN_int32_T - lda);
    }
    if (lda == 0) {
      m_rtErrorWithMessageID(d_emlrtRTEI.fName, d_emlrtRTEI.lineNo);
    }
    if (overflow) {
      check_forloop_overflow_error();
    }
    for (int iac{1}; lda < 0 ? iac >= b_tmp : iac <= b_tmp; iac += lda) {
      int b;
      b = (iac + m) - 1;
      if ((iac <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ia{iac}; ia <= b; ia++) {
        int i;
        i = ia - iac;
        y[i] = y[i] + A[ia - 1] * x[ix - 1];
      }
      ix++;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xgemv.cpp
//
// [EOF]
//
