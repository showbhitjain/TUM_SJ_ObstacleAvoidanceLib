//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgemm.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "xgemm.h"
#include "eml_int_forloop_overflow_check.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cstring>

// Function Definitions
//
// Arguments    : int m
//                int n
//                int k
//                const array<double, 2U> &A
//                int lda
//                const array<double, 2U> &B
//                int ib0
//                int ldb
//                array<double, 2U> &C
//                int ldc
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void xgemm(int m, int n, int k, const array<double, 2U> &A, int lda,
           const array<double, 2U> &B, int ib0, int ldb, array<double, 2U> &C,
           int ldc)
{
  if ((m != 0) && (n != 0)) {
    int a;
    int b;
    int br;
    int lastColC;
    br = ib0;
    lastColC = ldc * (n - 1);
    if ((lastColC >= 0) && (lastColC > MAX_int32_T - ldc)) {
      check_forloop_overflow_error();
    }
    for (int cr{0}; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      a = cr + 1;
      b = cr + m;
      if ((cr + 1 <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ic{a}; ic <= b; ic++) {
        C[ic - 1] = 0.0;
      }
    }
    for (int cr{0}; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      int ar;
      ar = -1;
      b = (br + k) - 1;
      if ((br <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ib{br}; ib <= b; ib++) {
        int b_b;
        a = cr + 1;
        b_b = cr + m;
        if ((cr + 1 <= b_b) && (b_b > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int ic{a}; ic <= b_b; ic++) {
          C[ic - 1] = C[ic - 1] + B[ib - 1] * A[(ar + ic) - cr];
        }
        ar += lda;
      }
      br += ldb;
    }
  }
}

//
// Arguments    : int m
//                int n
//                int k
//                const array<double, 2U> &A
//                int ia0
//                int lda
//                const array<double, 2U> &B
//                int ldb
//                array<double, 2U> &C
//                int ldc
// Return Type  : void
//
void xgemm(int m, int n, int k, const array<double, 2U> &A, int ia0, int lda,
           const array<double, 2U> &B, int ldb, array<double, 2U> &C, int ldc)
{
  if ((m != 0) && (n != 0)) {
    int a;
    int b;
    int br;
    int lastColC;
    lastColC = ldc * (n - 1);
    if ((lastColC >= 0) && (lastColC > MAX_int32_T - ldc)) {
      check_forloop_overflow_error();
    }
    for (int cr{0}; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      a = cr + 1;
      b = cr + m;
      if ((cr + 1 <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ic{a}; ic <= b; ic++) {
        C[ic - 1] = 0.0;
      }
    }
    br = -1;
    for (int cr{0}; ldc < 0 ? cr >= lastColC : cr <= lastColC; cr += ldc) {
      int ar;
      ar = ia0;
      a = cr + 1;
      b = cr + m;
      if ((cr + 1 <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ic{a}; ic <= b; ic++) {
        double temp;
        temp = 0.0;
        if (k > 2147483646) {
          check_forloop_overflow_error();
        }
        for (int w{0}; w < k; w++) {
          temp += A[(w + ar) - 1] * B[(w + br) + 1];
        }
        C[ic - 1] = C[ic - 1] + temp;
        ar += lda;
      }
      br += ldb;
    }
  }
}

//
// Arguments    : int m
//                int k
//                const array<double, 2U> &A
//                int lda
//                const array<double, 2U> &B
//                int ldb
//                array<double, 2U> &C
//                int ldc
// Return Type  : void
//
void xgemm(int m, int k, const array<double, 2U> &A, int lda,
           const array<double, 2U> &B, int ldb, array<double, 2U> &C, int ldc)
{
  if (m != 0) {
    int a;
    int b;
    int br;
    if ((ldc >= 0) && (ldc > MAX_int32_T - ldc)) {
      check_forloop_overflow_error();
    }
    for (int cr{0}; ldc < 0 ? cr >= ldc : cr <= ldc; cr += ldc) {
      a = cr + 1;
      b = cr + m;
      if ((cr + 1 <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ic{a}; ic <= b; ic++) {
        C[ic - 1] = 0.0;
      }
    }
    br = 1;
    for (int cr{0}; ldc < 0 ? cr >= ldc : cr <= ldc; cr += ldc) {
      int ar;
      ar = -1;
      b = (br + k) - 1;
      if ((br <= b) && (b > 2147483646)) {
        check_forloop_overflow_error();
      }
      for (int ib{br}; ib <= b; ib++) {
        int b_b;
        a = cr + 1;
        b_b = cr + m;
        if ((cr + 1 <= b_b) && (b_b > 2147483646)) {
          check_forloop_overflow_error();
        }
        for (int ic{a}; ic <= b_b; ic++) {
          C[ic - 1] = C[ic - 1] + B[ib - 1] * A[(ar + ic) - cr];
        }
        ar += lda;
      }
      br += ldb;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xgemm.cpp
//
// [EOF]
//
