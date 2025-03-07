//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgeqp3.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "xgeqp3.h"
#include "rt_nonfinite.h"
#include "xnrm2.h"
#include "xzgeqp3.h"
#include "xzlarf.h"
#include "xzlarfg.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 2U> &A
//                int m
//                int n
//                array<int, 1U> &jpvt
//                array<double, 1U> &tau
// Return Type  : void
//
namespace coder {
namespace internal {
namespace lapack {
void xgeqp3(array<double, 2U> &A, int m, int n, array<int, 1U> &jpvt,
            array<double, 1U> &tau)
{
  array<double, 1U> vn1;
  array<double, 1U> vn2;
  array<double, 1U> work;
  double d;
  double temp;
  int ij;
  int ma;
  int minmana;
  int minmn;
  ma = A.size(0);
  ij = A.size(0);
  minmana = A.size(1);
  if (ij <= minmana) {
    minmana = ij;
  }
  if (m <= n) {
    minmn = m;
  } else {
    minmn = n;
  }
  tau.set_size(minmana);
  if (static_cast<int>(minmana < 200)) {
    for (int j{0}; j < minmana; j++) {
      tau[j] = 0.0;
    }
  } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

    for (int j = 0; j < minmana; j++) {
      tau[j] = 0.0;
    }
  }
  if (minmn < 1) {
    if (static_cast<int>(n < 200)) {
      for (int j{0}; j < n; j++) {
        jpvt[j] = j + 1;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int j = 0; j < n; j++) {
        jpvt[j] = j + 1;
      }
    }
  } else {
    int i;
    int ii;
    int ix;
    int nfxd;
    int temp_tmp;
    nfxd = 0;
    for (ii = 0; ii < n; ii++) {
      if (jpvt[ii] != 0) {
        nfxd++;
        if (ii + 1 != nfxd) {
          ix = ii * ma;
          minmana = (nfxd - 1) * ma;
          for (int k{0}; k < m; k++) {
            temp_tmp = ix + k;
            temp = A[temp_tmp];
            i = minmana + k;
            A[temp_tmp] = A[i];
            A[i] = temp;
          }
          jpvt[ii] = jpvt[nfxd - 1];
          jpvt[nfxd - 1] = ii + 1;
        } else {
          jpvt[ii] = ii + 1;
        }
      } else {
        jpvt[ii] = ii + 1;
      }
    }
    if (nfxd > minmn) {
      nfxd = minmn;
    }
    reflapack::qrf(A, m, n, nfxd, tau);
    if (nfxd < minmn) {
      int i1;
      ma = A.size(0);
      work.set_size(A.size(1));
      ij = A.size(1);
      vn1.set_size(A.size(1));
      vn2.set_size(A.size(1));
      if (static_cast<int>(ij < 200)) {
        for (int j{0}; j < ij; j++) {
          work[j] = 0.0;
          vn1[j] = 0.0;
          vn2[j] = 0.0;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

        for (int j = 0; j < ij; j++) {
          work[j] = 0.0;
          vn1[j] = 0.0;
          vn2[j] = 0.0;
        }
      }
      i = nfxd + 1;
      i1 = nfxd + 1;
      ij = nfxd + 1;
      if (static_cast<int>(n - nfxd < 200)) {
        for (int j{i}; j <= n; j++) {
          d = blas::xnrm2(m - nfxd, A, (nfxd + (j - 1) * ma) + 1);
          vn1[j - 1] = d;
          vn2[j - 1] = d;
        }
      } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4) private(d)

        for (int j = ij; j <= n; j++) {
          d = blas::xnrm2(m - nfxd, A, (nfxd + (j - 1) * ma) + 1);
          vn1[j - 1] = d;
          vn2[j - 1] = d;
        }
      }
      for (int b_i{i1}; b_i <= minmn; b_i++) {
        double d1;
        double s;
        int ip1;
        int mmi;
        int nmi;
        ip1 = b_i + 1;
        ij = (b_i - 1) * ma;
        ii = (ij + b_i) - 1;
        nmi = (n - b_i) + 1;
        mmi = m - b_i;
        if (nmi < 1) {
          minmana = -2;
        } else {
          minmana = -1;
          if (nmi > 1) {
            temp = std::abs(vn1[b_i - 1]);
            for (int k{2}; k <= nmi; k++) {
              s = std::abs(vn1[(b_i + k) - 2]);
              if (s > temp) {
                minmana = k - 2;
                temp = s;
              }
            }
          }
        }
        nfxd = b_i + minmana;
        if (nfxd + 1 != b_i) {
          ix = nfxd * ma;
          for (int k{0}; k < m; k++) {
            temp_tmp = ix + k;
            temp = A[temp_tmp];
            i = ij + k;
            A[temp_tmp] = A[i];
            A[i] = temp;
          }
          minmana = jpvt[nfxd];
          jpvt[nfxd] = jpvt[b_i - 1];
          jpvt[b_i - 1] = minmana;
          vn1[nfxd] = vn1[b_i - 1];
          vn2[nfxd] = vn2[b_i - 1];
        }
        if (b_i < m) {
          temp = A[ii];
          d1 = reflapack::xzlarfg(mmi + 1, temp, A, ii + 2);
          tau[b_i - 1] = d1;
          A[ii] = temp;
        } else {
          d1 = 0.0;
          tau[b_i - 1] = 0.0;
        }
        if (b_i < n) {
          temp = A[ii];
          A[ii] = 1.0;
          reflapack::xzlarf(mmi + 1, nmi - 1, ii + 1, d1, A, (ii + ma) + 1, ma,
                            work);
          A[ii] = temp;
        }
        for (ii = ip1; ii <= n; ii++) {
          ij = b_i + (ii - 1) * ma;
          d1 = vn1[ii - 1];
          if (d1 != 0.0) {
            temp = std::abs(A[ij - 1]) / d1;
            temp = 1.0 - temp * temp;
            if (temp < 0.0) {
              temp = 0.0;
            }
            s = d1 / vn2[ii - 1];
            s = temp * (s * s);
            if (s <= 1.4901161193847656E-8) {
              if (b_i < m) {
                d1 = blas::xnrm2(mmi, A, ij + 1);
                vn1[ii - 1] = d1;
                vn2[ii - 1] = d1;
              } else {
                vn1[ii - 1] = 0.0;
                vn2[ii - 1] = 0.0;
              }
            } else {
              vn1[ii - 1] = d1 * std::sqrt(temp);
            }
          }
        }
      }
    }
  }
}

} // namespace lapack
} // namespace internal
} // namespace coder

//
// File trailer for xgeqp3.cpp
//
// [EOF]
//
