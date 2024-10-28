//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ppval.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

// Include Files
#include "ppval.h"
#include "bsearch.h"
#include "rt_nonfinite.h"
#include "trajectory_generator_internal_types.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const ::coder::array<double, 2U> &pp_breaks
//                const ::coder::array<double, 3U> &pp_coefs
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
namespace trajectoryGeneration {
namespace coder {
void b_ppval(const ::coder::array<double, 2U> &pp_breaks,
             const ::coder::array<double, 3U> &pp_coefs,
             const ::coder::array<double, 2U> &x, ::coder::array<double, 2U> &v)
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
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
void ppval(const struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v)
{
  double xloc;
  int high_i;
  int low_i;
  int low_ip1;
  int mid_i;
  int ub_loop;
  v.set_size(1, x.size(1));
  ub_loop = x.size(1) - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    low_i, xloc, low_ip1, high_i, mid_i)

  for (int ix = 0; ix <= ub_loop; ix++) {
    if (std::isnan(x[ix])) {
      xloc = rtNaN;
    } else {
      low_i = 0;
      low_ip1 = 2;
      high_i = 6;
      while (high_i > low_ip1) {
        mid_i = ((low_i + high_i) + 1) >> 1;
        if (x[ix] >= pp.breaks[mid_i - 1]) {
          low_i = mid_i - 1;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
      xloc = x[ix] - pp.breaks[low_i];
      xloc = xloc * (xloc * pp.coefs[low_i] + pp.coefs[low_i + 5]) +
             pp.coefs[low_i + 10];
    }
    v[ix] = xloc;
  }
}

//
// Arguments    : const b_struct_T &pp
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
void ppval(const b_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v)
{
  double xloc;
  int high_i;
  int low_i;
  int low_ip1;
  int mid_i;
  int ub_loop;
  v.set_size(1, x.size(1));
  ub_loop = x.size(1) - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    low_i, xloc, low_ip1, high_i, mid_i)

  for (int ix = 0; ix <= ub_loop; ix++) {
    if (std::isnan(x[ix])) {
      xloc = rtNaN;
    } else {
      low_i = 0;
      low_ip1 = 2;
      high_i = 4;
      while (high_i > low_ip1) {
        mid_i = ((low_i + high_i) + 1) >> 1;
        if (x[ix] >= pp.breaks[mid_i - 1]) {
          low_i = mid_i - 1;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
      xloc = x[ix] - pp.breaks[low_i];
      xloc = xloc * (xloc * (xloc * pp.coefs[low_i] + pp.coefs[low_i + 3]) +
                     pp.coefs[low_i + 6]) +
             pp.coefs[low_i + 9];
    }
    v[ix] = xloc;
  }
}

//
// Arguments    : const c_struct_T &pp
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
void ppval(const c_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v)
{
  double b_v;
  double xloc;
  int high_i;
  int low_i;
  int low_ip1;
  int mid_i;
  int ub_loop;
  v.set_size(1, x.size(1));
  ub_loop = x.size(1) - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(           \
    low_i, b_v, low_ip1, high_i, xloc, mid_i)

  for (int ix = 0; ix <= ub_loop; ix++) {
    if (std::isnan(x[ix])) {
      b_v = rtNaN;
    } else {
      low_i = 0;
      low_ip1 = 2;
      high_i = 4;
      while (high_i > low_ip1) {
        mid_i = ((low_i + high_i) + 1) >> 1;
        if (x[ix] >= pp.breaks[mid_i - 1]) {
          low_i = mid_i - 1;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }
      xloc = x[ix] - pp.breaks[low_i];
      b_v = pp.coefs[low_i];
      for (low_ip1 = 0; low_ip1 < 5; low_ip1++) {
        b_v = xloc * b_v + pp.coefs[low_i + (low_ip1 + 1) * 3];
      }
    }
    v[ix] = b_v;
  }
}

//
// Arguments    : const d_struct_T &pp
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
void ppval(const d_struct_T &pp, const ::coder::array<double, 2U> &x,
           ::coder::array<double, 2U> &v)
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
// Arguments    : const ::coder::array<double, 2U> &pp_breaks
//                const ::coder::array<double, 3U> &pp_coefs
//                const ::coder::array<double, 2U> &x
//                ::coder::array<double, 2U> &v
// Return Type  : void
//
void ppval(const ::coder::array<double, 2U> &pp_breaks,
           const ::coder::array<double, 3U> &pp_coefs,
           const ::coder::array<double, 2U> &x, ::coder::array<double, 2U> &v)
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
} // namespace trajectoryGeneration

//
// File trailer for ppval.cpp
//
// [EOF]
//
