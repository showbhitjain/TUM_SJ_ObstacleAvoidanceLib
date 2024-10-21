//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ppval.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 21-Oct-2024 03:52:20
//

// Include Files
#include "ppval.h"
#include "bsearch.h"
#include "cartesian_trajectory_internal_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const array<double, 2U> &pp_breaks
//                const array<double, 3U> &pp_coefs
//                const array<double, 2U> &x
//                array<double, 2U> &v
// Return Type  : void
//
namespace coder {
void b_ppval(const array<double, 2U> &pp_breaks,
             const array<double, 3U> &pp_coefs, const array<double, 2U> &x,
             array<double, 2U> &v)
{
  int coefStride;
  int nx;
  coefStride = 3 * (pp_breaks.size(1) - 1);
  v.set_size(3, x.size(1));
  nx = x.size(1);
  for (int ix{0}; ix < nx; ix++) {
    double xloc;
    int iv0;
    iv0 = ix * 3;
    xloc = x[ix];
    if (std::isnan(xloc)) {
      v[iv0] = x[ix];
      v[iv0 + 1] = x[ix];
      v[iv0 + 2] = x[ix];
    } else {
      int icp;
      int ip;
      ip = internal::b_bsearch(pp_breaks, xloc) - 1;
      icp = ip * 3;
      xloc -= pp_breaks[ip];
      v[iv0] = pp_coefs[icp];
      v[iv0 + 1] = pp_coefs[icp + 1];
      v[iv0 + 2] = pp_coefs[icp + 2];
      for (int ic{0}; ic < 5; ic++) {
        ip = icp + (ic + 1) * coefStride;
        v[iv0] = xloc * v[iv0] + pp_coefs[ip];
        v[iv0 + 1] = xloc * v[iv0 + 1] + pp_coefs[ip + 1];
        v[iv0 + 2] = xloc * v[iv0 + 2] + pp_coefs[ip + 2];
      }
    }
  }
}

//
// Arguments    : const struct_T &pp
//                const array<double, 2U> &x
//                array<double, 2U> &v
// Return Type  : void
//
void ppval(const struct_T &pp, const array<double, 2U> &x, array<double, 2U> &v)
{
  double b_xloc;
  int b_ip;
  int coefStride;
  int elementsPerPage;
  int nx;
  elementsPerPage = pp.coefs.size(0);
  coefStride = pp.coefs.size(0) * (pp.breaks.size(1) - 1);
  v.set_size(pp.coefs.size(0), x.size(1));
  nx = x.size(1);
  if (pp.coefs.size(0) == 1) {
    int ip;
    ip = x.size(1) - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(b_xloc,    \
                                                                    b_ip)

    for (int b_ix = 0; b_ix <= ip; b_ix++) {
      b_xloc = x[b_ix];
      if (std::isnan(b_xloc)) {
        b_xloc = rtNaN;
      } else {
        b_ip = internal::b_bsearch(pp.breaks, b_xloc) - 1;
        b_xloc -= pp.breaks[b_ip];
        b_xloc =
            b_xloc * (b_xloc * pp.coefs[b_ip] + pp.coefs[b_ip + coefStride]) +
            pp.coefs[b_ip + (coefStride << 1)];
      }
      v[b_ix] = b_xloc;
    }
  } else {
    for (int ix{0}; ix < nx; ix++) {
      double xloc;
      int iv0;
      iv0 = ix * elementsPerPage;
      xloc = x[ix];
      if (std::isnan(xloc)) {
        int ip;
        ip = static_cast<unsigned char>(elementsPerPage);
        for (int j{0}; j < ip; j++) {
          v[iv0 + j] = x[ix];
        }
      } else {
        int icp;
        int ip;
        ip = internal::b_bsearch(pp.breaks, xloc) - 1;
        icp = ip * elementsPerPage;
        xloc -= pp.breaks[ip];
        ip = static_cast<unsigned char>(elementsPerPage);
        for (int j{0}; j < ip; j++) {
          v[iv0 + j] = pp.coefs[icp + j];
        }
        for (int ic{0}; ic < 2; ic++) {
          int ic0;
          ic0 = icp + (ic + 1) * coefStride;
          for (int j{0}; j < ip; j++) {
            int i;
            i = iv0 + j;
            v[i] = xloc * v[i] + pp.coefs[ic0 + j];
          }
        }
      }
    }
  }
}

//
// Arguments    : const array<double, 2U> &pp_breaks
//                const array<double, 3U> &pp_coefs
//                const array<double, 2U> &x
//                array<double, 2U> &v
// Return Type  : void
//
void ppval(const array<double, 2U> &pp_breaks,
           const array<double, 3U> &pp_coefs, const array<double, 2U> &x,
           array<double, 2U> &v)
{
  int coefStride;
  int nx;
  coefStride = 3 * (pp_breaks.size(1) - 1);
  v.set_size(3, x.size(1));
  nx = x.size(1);
  for (int ix{0}; ix < nx; ix++) {
    double xloc;
    int iv0;
    iv0 = ix * 3;
    xloc = x[ix];
    if (std::isnan(xloc)) {
      v[iv0] = x[ix];
      v[iv0 + 1] = x[ix];
      v[iv0 + 2] = x[ix];
    } else {
      int icp;
      int ip;
      ip = internal::b_bsearch(pp_breaks, xloc) - 1;
      icp = ip * 3;
      xloc -= pp_breaks[ip];
      v[iv0] = pp_coefs[icp];
      v[iv0 + 1] = pp_coefs[icp + 1];
      v[iv0 + 2] = pp_coefs[icp + 2];
      for (int ic{0}; ic < 3; ic++) {
        ip = icp + (ic + 1) * coefStride;
        v[iv0] = xloc * v[iv0] + pp_coefs[ip];
        v[iv0 + 1] = xloc * v[iv0 + 1] + pp_coefs[ip + 1];
        v[iv0 + 2] = xloc * v[iv0 + 2] + pp_coefs[ip + 2];
      }
    }
  }
}

} // namespace coder

//
// File trailer for ppval.cpp
//
// [EOF]
//
