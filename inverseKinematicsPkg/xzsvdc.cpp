//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzsvdc.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Nov-2024 04:04:05
//

// Include Files
#include "xzsvdc.h"
#include "rt_nonfinite.h"
#include "xaxpy.h"
#include "xnrm2.h"
#include "xrotg.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : array<double, 2U> &A
//                double S_data[]
// Return Type  : int
//
namespace coder {
namespace internal {
namespace reflapack {
int xzsvdc(array<double, 2U> &A, double S_data[])
{
  array<double, 2U> x;
  array<double, 1U> e;
  double s_data[7];
  double work_data[6];
  double nrm;
  double sm;
  double ztest;
  int S_size;
  int ii;
  int n;
  int ns;
  int p;
  int qs;
  n = A.size(0);
  p = A.size(1);
  if (A.size(0) + 1 <= A.size(1)) {
    ns = A.size(0);
  } else {
    ns = A.size(1) - 1;
  }
  qs = A.size(0);
  S_size = A.size(1);
  if (qs <= S_size) {
    S_size = qs;
  }
  if (ns >= 0) {
    std::memset(&s_data[0], 0,
                static_cast<unsigned int>(ns + 1) * sizeof(double));
  }
  e.set_size(A.size(1));
  ns = A.size(1);
  for (ii = 0; ii < ns; ii++) {
    e[ii] = 0.0;
  }
  ns = A.size(0);
  if (ns - 1 >= 0) {
    std::memset(&work_data[0], 0,
                static_cast<unsigned int>(ns) * sizeof(double));
  }
  if ((A.size(0) != 0) && (A.size(1) != 0)) {
    double rt;
    double snorm;
    int m;
    int nct;
    int nmq;
    int nrt;
    int qp1;
    int qq;
    if (A.size(1) >= 2) {
      qs = A.size(1) - 2;
    } else {
      qs = 0;
    }
    nrt = A.size(0);
    if (qs <= nrt) {
      nrt = qs;
    }
    qs = A.size(0) - 1;
    nct = A.size(1);
    if (qs <= nct) {
      nct = qs;
    }
    if (nct >= nrt) {
      ii = nct;
    } else {
      ii = nrt;
    }
    for (int q{0}; q < ii; q++) {
      boolean_T apply_transform;
      qp1 = q + 2;
      qq = (q + n * q) + 1;
      nmq = (n - q) - 1;
      apply_transform = false;
      if (q + 1 <= nct) {
        nrm = blas::xnrm2(nmq + 1, A, qq);
        if (nrm > 0.0) {
          apply_transform = true;
          if (A[qq - 1] < 0.0) {
            nrm = -nrm;
          }
          s_data[q] = nrm;
          if (std::abs(nrm) >= 1.0020841800044864E-292) {
            nrm = 1.0 / nrm;
            x.set_size(A.size(0), A.size(1));
            ns = A.size(0) * A.size(1);
            for (qs = 0; qs < ns; qs++) {
              x[qs] = A[qs];
            }
            qs = qq + nmq;
            for (int k{qq}; k <= qs; k++) {
              x[k - 1] = nrm * x[k - 1];
            }
            A.set_size(x.size(0), x.size(1));
            for (qs = 0; qs < ns; qs++) {
              A[qs] = x[qs];
            }
          } else {
            qs = qq + nmq;
            for (int k{qq}; k <= qs; k++) {
              A[k - 1] = A[k - 1] / s_data[q];
            }
          }
          A[qq - 1] = A[qq - 1] + 1.0;
          s_data[q] = -s_data[q];
        } else {
          s_data[q] = 0.0;
        }
      }
      for (qs = qp1; qs <= p; qs++) {
        ns = q + n * (qs - 1);
        if (apply_transform) {
          nrm = 0.0;
          if (nmq + 1 >= 1) {
            for (int k{0}; k <= nmq; k++) {
              nrm += A[(qq + k) - 1] * A[ns + k];
            }
          }
          nrm = -(nrm / A[q + A.size(0) * q]);
          blas::xaxpy(nmq + 1, nrm, qq, A, ns + 1);
        }
        e[qs - 1] = A[ns];
      }
      if (q + 1 <= nrt) {
        ns = p - q;
        nrm = blas::xnrm2(ns - 1, e, q + 2);
        if (nrm == 0.0) {
          e[q] = 0.0;
        } else {
          if (e[q + 1] < 0.0) {
            e[q] = -nrm;
          } else {
            e[q] = nrm;
          }
          nrm = e[q];
          if (std::abs(e[q]) >= 1.0020841800044864E-292) {
            nrm = 1.0 / e[q];
            qs = q + ns;
            for (int k{qp1}; k <= qs; k++) {
              e[k - 1] = nrm * e[k - 1];
            }
          } else {
            qs = q + ns;
            for (int k{qp1}; k <= qs; k++) {
              e[k - 1] = e[k - 1] / nrm;
            }
          }
          e[q + 1] = e[q + 1] + 1.0;
          e[q] = -e[q];
          if (q + 2 <= n) {
            if (qp1 <= n) {
              std::memset(&work_data[qp1 + -1], 0,
                          static_cast<unsigned int>((n - qp1) + 1) *
                              sizeof(double));
            }
            for (qs = qp1; qs <= p; qs++) {
              blas::xaxpy(nmq, e[qs - 1], A, (q + n * (qs - 1)) + 2, work_data,
                          q + 2);
            }
            for (qs = qp1; qs <= p; qs++) {
              blas::xaxpy(nmq, -e[qs - 1] / e[q + 1], work_data, q + 2, A,
                          (q + n * (qs - 1)) + 2);
            }
          }
        }
      }
    }
    m = n + 1;
    if (p <= m) {
      m = p;
    }
    if (nct < p) {
      s_data[nct] = A[nct + A.size(0) * nct];
    }
    if (n < m) {
      s_data[m - 1] = 0.0;
    }
    if (nrt + 1 < m) {
      e[nrt] = A[nrt + A.size(0) * (m - 1)];
    }
    e[m - 1] = 0.0;
    nrt = m;
    nmq = 0;
    snorm = 0.0;
    for (int q{0}; q < m; q++) {
      nrm = s_data[q];
      ztest = nrm;
      if (nrm != 0.0) {
        rt = std::abs(nrm);
        ztest = rt;
        s_data[q] = rt;
        if (q + 1 < m) {
          e[q] = e[q] / (nrm / rt);
        }
      }
      if ((q + 1 < m) && (e[q] != 0.0)) {
        rt = std::abs(e[q]);
        nrm = rt / e[q];
        e[q] = rt;
        s_data[q + 1] *= nrm;
      }
      snorm = std::fmax(snorm, std::fmax(std::abs(ztest), std::abs(e[q])));
    }
    while ((m > 0) && (nmq < 75)) {
      boolean_T exitg1;
      qq = m - 1;
      ii = m - 1;
      exitg1 = false;
      while (!(exitg1 || (ii == 0))) {
        nrm = std::abs(e[ii - 1]);
        if ((nrm <= 2.2204460492503131E-16 *
                        (std::abs(s_data[ii - 1]) + std::abs(s_data[ii]))) ||
            (nrm <= 1.0020841800044864E-292) ||
            ((nmq > 20) && (nrm <= 2.2204460492503131E-16 * snorm))) {
          e[ii - 1] = 0.0;
          exitg1 = true;
        } else {
          ii--;
        }
      }
      if (ii == m - 1) {
        ns = 4;
      } else {
        qs = m;
        ns = m;
        exitg1 = false;
        while ((!exitg1) && (ns >= ii)) {
          qs = ns;
          if (ns == ii) {
            exitg1 = true;
          } else {
            nrm = 0.0;
            if (ns < m) {
              nrm = std::abs(e[ns - 1]);
            }
            if (ns > ii + 1) {
              nrm += std::abs(e[ns - 2]);
            }
            ztest = std::abs(s_data[ns - 1]);
            if ((ztest <= 2.2204460492503131E-16 * nrm) ||
                (ztest <= 1.0020841800044864E-292)) {
              s_data[ns - 1] = 0.0;
              exitg1 = true;
            } else {
              ns--;
            }
          }
        }
        if (qs == ii) {
          ns = 3;
        } else if (qs == m) {
          ns = 1;
        } else {
          ns = 2;
          ii = qs;
        }
      }
      switch (ns) {
      case 1: {
        ztest = e[m - 2];
        e[m - 2] = 0.0;
        for (int k{qq}; k >= ii + 1; k--) {
          double sqds;
          sqds = blas::xrotg(&s_data[k - 1], ztest, sm);
          if (k > ii + 1) {
            rt = e[k - 2];
            ztest = -sm * rt;
            e[k - 2] = rt * sqds;
          }
        }
      } break;
      case 2: {
        ztest = e[ii - 1];
        e[ii - 1] = 0.0;
        for (int k{ii + 1}; k <= m; k++) {
          double sqds;
          sqds = blas::xrotg(&s_data[k - 1], ztest, sm);
          rt = e[k - 1];
          ztest = -sm * rt;
          e[k - 1] = rt * sqds;
        }
      } break;
      case 3: {
        double scale;
        double sqds;
        nrm = s_data[m - 1];
        ztest = s_data[m - 2];
        rt = e[m - 2];
        scale = std::fmax(
            std::fmax(std::fmax(std::fmax(std::abs(nrm), std::abs(ztest)),
                                std::abs(rt)),
                      std::abs(s_data[ii])),
            std::abs(e[ii]));
        sm = nrm / scale;
        nrm = ztest / scale;
        ztest = rt / scale;
        sqds = s_data[ii] / scale;
        rt = ((nrm + sm) * (nrm - sm) + ztest * ztest) / 2.0;
        nrm = sm * ztest;
        nrm *= nrm;
        if ((rt != 0.0) || (nrm != 0.0)) {
          ztest = std::sqrt(rt * rt + nrm);
          if (rt < 0.0) {
            ztest = -ztest;
          }
          ztest = nrm / (rt + ztest);
        } else {
          ztest = 0.0;
        }
        ztest += (sqds + sm) * (sqds - sm);
        nrm = sqds * (e[ii] / scale);
        for (int k{ii + 1}; k <= qq; k++) {
          sqds = blas::xrotg(&ztest, nrm, sm);
          if (k > ii + 1) {
            e[k - 2] = ztest;
          }
          nrm = e[k - 1];
          rt = s_data[k - 1];
          e[k - 1] = sqds * nrm - sm * rt;
          ztest = sm * s_data[k];
          s_data[k] *= sqds;
          s_data[k - 1] = sqds * rt + sm * nrm;
          sqds = blas::xrotg(&s_data[k - 1], ztest, sm);
          rt = e[k - 1];
          ztest = sqds * rt + sm * s_data[k];
          s_data[k] = -sm * rt + sqds * s_data[k];
          nrm = sm * e[k];
          e[k] = e[k] * sqds;
        }
        e[m - 2] = ztest;
        nmq++;
      } break;
      default:
        if (s_data[ii] < 0.0) {
          s_data[ii] = -s_data[ii];
        }
        qp1 = ii + 1;
        while ((ii + 1 < nrt) && (s_data[ii] < s_data[qp1])) {
          rt = s_data[ii];
          s_data[ii] = s_data[qp1];
          s_data[qp1] = rt;
          ii = qp1;
          qp1++;
        }
        nmq = 0;
        m--;
        break;
      }
    }
  }
  if (S_size - 1 >= 0) {
    std::copy(&s_data[0], &s_data[S_size], &S_data[0]);
  }
  return S_size;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

//
// File trailer for xzsvdc.cpp
//
// [EOF]
//
