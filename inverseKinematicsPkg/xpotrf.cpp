//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xpotrf.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "xpotrf.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : int n
//                array<double, 2U> &A
//                int lda
// Return Type  : int
//
namespace coder {
namespace internal {
namespace lapack {
int xpotrf(int n, array<double, 2U> &A, int lda)
{
  int info;
  int j;
  boolean_T exitg1;
  info = 0;
  j = 0;
  exitg1 = false;
  while ((!exitg1) && (j <= n - 1)) {
    double c;
    double ssq;
    int idxA1j;
    int idxAjj;
    int k;
    idxA1j = j * lda;
    idxAjj = idxA1j + j;
    ssq = 0.0;
    if (j >= 1) {
      for (k = 0; k < j; k++) {
        c = A[idxA1j + k];
        ssq += c * c;
      }
    }
    ssq = A[idxAjj] - ssq;
    if (ssq > 0.0) {
      ssq = std::sqrt(ssq);
      A[idxAjj] = ssq;
      if (j + 1 < n) {
        int i;
        int ia0;
        int idxAjjp1;
        int nmj;
        nmj = (n - j) - 2;
        ia0 = (idxA1j + lda) + 1;
        idxAjjp1 = idxAjj + lda;
        if ((j != 0) && (nmj + 1 != 0)) {
          idxAjj = idxAjjp1;
          i = ia0 + lda * nmj;
          for (int iac{ia0}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
            c = 0.0;
            k = (iac + j) - 1;
            for (int ia{iac}; ia <= k; ia++) {
              c += A[ia - 1] * A[(idxA1j + ia) - iac];
            }
            A[idxAjj] = A[idxAjj] - c;
            idxAjj += lda;
          }
        }
        ssq = 1.0 / ssq;
        i = (idxAjjp1 + lda * nmj) + 1;
        for (k = idxAjjp1 + 1; lda < 0 ? k >= i : k <= i; k += lda) {
          A[k - 1] = ssq * A[k - 1];
        }
      }
      j++;
    } else {
      A[idxAjj] = ssq;
      info = j + 1;
      exitg1 = true;
    }
  }
  return info;
}

} // namespace lapack
} // namespace internal
} // namespace coder

//
// File trailer for xpotrf.cpp
//
// [EOF]
//
