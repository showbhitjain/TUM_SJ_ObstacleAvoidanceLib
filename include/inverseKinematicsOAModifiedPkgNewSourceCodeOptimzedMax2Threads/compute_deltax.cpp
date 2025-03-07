//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compute_deltax.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 07-Mar-2025 21:34:16
//

// Include Files
#include "compute_deltax.h"
#include "factor.h"
#include "fullColLDL2_.h"
#include "inverseKinematicsOAModified_internal_types.h"
#include "partialColLDL3_.h"
#include "rt_nonfinite.h"
#include "solve.h"
#include "xgemm.h"
#include "xpotrf.h"
#include "coder_array.h"
#include "omp.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : const array<double, 2U> &H
//                j_struct_T &solution
//                h_struct_T &memspace
//                const e_struct_T &qrmanager
//                f_struct_T &cholmanager
//                const g_struct_T &objective
//                boolean_T alwaysPositiveDef
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace qpactiveset {
void compute_deltax(const array<double, 2U> &H, j_struct_T &solution,
                    h_struct_T &memspace, const e_struct_T &qrmanager,
                    f_struct_T &cholmanager, const g_struct_T &objective,
                    boolean_T alwaysPositiveDef)
{
  int mNull;
  int nVar;
  nVar = qrmanager.mrows - 1;
  mNull = qrmanager.mrows - qrmanager.ncols;
  if (mNull <= 0) {
    if (static_cast<int>(nVar + 1 < 200)) {
      for (int idx{0}; idx <= nVar; idx++) {
        solution.searchDir[idx] = 0.0;
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = 0; idx <= nVar; idx++) {
        solution.searchDir[idx] = 0.0;
      }
    }
  } else {
    int i;
    i = (nVar + 1 < 200);
    if (i) {
      for (int idx{0}; idx <= nVar; idx++) {
        solution.searchDir[idx] = -objective.grad[idx];
      }
    } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

      for (int idx = 0; idx <= nVar; idx++) {
        solution.searchDir[idx] = -objective.grad[idx];
      }
    }
    if (qrmanager.ncols <= 0) {
      switch (objective.objtype) {
      case 5:
        break;
      case 3: {
        int LDimSizeP1;
        int nVars;
        if (alwaysPositiveDef) {
          cholmanager.ndims = qrmanager.mrows;
          if ((H.size(0) != 0) && (H.size(1) != 0)) {
            for (int b_idx{0}; b_idx <= nVar; b_idx++) {
              LDimSizeP1 = (nVar + 1) * b_idx;
              nVars = cholmanager.ldm * b_idx;
              for (int k{0}; k <= nVar; k++) {
                cholmanager.FMat[nVars + k] = H[LDimSizeP1 + k];
              }
            }
          }
          cholmanager.info = internal::lapack::xpotrf(
              qrmanager.mrows, cholmanager.FMat, cholmanager.ldm);
        } else {
          DynamicRegCholManager::factor(cholmanager, H, qrmanager.mrows,
                                        qrmanager.mrows);
        }
        if (cholmanager.info != 0) {
          solution.state = -6;
        } else if (alwaysPositiveDef) {
          CholManager::solve(cholmanager, solution.searchDir);
        } else {
          int A_maxDiag_idx;
          nVars = cholmanager.ndims - 2;
          if ((solution.searchDir.size(0) != 0) && (cholmanager.ndims != 0)) {
            for (int k{0}; k <= nVars + 1; k++) {
              A_maxDiag_idx = k + k * cholmanager.ldm;
              i = nVars - k;
              for (int ia{0}; ia <= i; ia++) {
                LDimSizeP1 = (k + ia) + 1;
                solution.searchDir[LDimSizeP1] =
                    solution.searchDir[LDimSizeP1] -
                    solution.searchDir[k] *
                        cholmanager.FMat[(A_maxDiag_idx + ia) + 1];
              }
            }
          }
          i = cholmanager.ndims;
          if (static_cast<int>(i < 200)) {
            for (int idx{0}; idx < i; idx++) {
              solution.searchDir[idx] =
                  solution.searchDir[idx] /
                  cholmanager.FMat[idx + cholmanager.ldm * idx];
            }
          } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

            for (int idx = 0; idx < i; idx++) {
              solution.searchDir[idx] =
                  solution.searchDir[idx] /
                  cholmanager.FMat[idx + cholmanager.ldm * idx];
            }
          }
          nVars = cholmanager.ndims;
          if ((solution.searchDir.size(0) != 0) && (cholmanager.ndims != 0)) {
            for (int k{nVars}; k >= 1; k--) {
              double smax;
              A_maxDiag_idx = (k - 1) * cholmanager.ldm;
              smax = solution.searchDir[k - 1];
              i = k + 1;
              for (int ia{nVars}; ia >= i; ia--) {
                smax -= cholmanager.FMat[(A_maxDiag_idx + ia) - 1] *
                        solution.searchDir[ia - 1];
              }
              solution.searchDir[k - 1] = smax;
            }
          }
        }
      } break;
      default: {
        if (alwaysPositiveDef) {
          int A_maxDiag_idx;
          int LDimSizeP1;
          A_maxDiag_idx = objective.nvar;
          cholmanager.ndims = objective.nvar;
          if ((H.size(0) != 0) && (H.size(1) != 0)) {
            for (int b_idx{0}; b_idx < A_maxDiag_idx; b_idx++) {
              int nVars;
              LDimSizeP1 = A_maxDiag_idx * b_idx;
              nVars = cholmanager.ldm * b_idx;
              for (int k{0}; k < A_maxDiag_idx; k++) {
                cholmanager.FMat[nVars + k] = H[LDimSizeP1 + k];
              }
            }
          }
          cholmanager.info = internal::lapack::xpotrf(
              objective.nvar, cholmanager.FMat, cholmanager.ldm);
          if (cholmanager.info != 0) {
            solution.state = -6;
          } else {
            double smax;
            CholManager::solve(cholmanager, solution.searchDir);
            smax = 1.0 / objective.beta;
            LDimSizeP1 = objective.nvar + 1;
            i = qrmanager.mrows;
            if (static_cast<int>((i - LDimSizeP1) + 1 < 200)) {
              for (int idx{LDimSizeP1}; idx <= i; idx++) {
                solution.searchDir[idx - 1] =
                    smax * solution.searchDir[idx - 1];
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (int idx = LDimSizeP1; idx <= i; idx++) {
                solution.searchDir[idx - 1] =
                    smax * solution.searchDir[idx - 1];
              }
            }
          }
        }
      } break;
      }
    } else {
      int nullStartIdx;
      nullStartIdx = qrmanager.ldq * qrmanager.ncols + 1;
      if (objective.objtype == 5) {
        int A_maxDiag_idx;
        if (static_cast<int>(mNull < 200)) {
          for (int idx{0}; idx < mNull; idx++) {
            memspace.workspace_double[idx] =
                -qrmanager.Q[nVar + qrmanager.ldq * (qrmanager.ncols + idx)];
          }
        } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

          for (int idx = 0; idx < mNull; idx++) {
            memspace.workspace_double[idx] =
                -qrmanager.Q[nVar + qrmanager.ldq * (qrmanager.ncols + idx)];
          }
        }
        A_maxDiag_idx = qrmanager.ldq;
        if (qrmanager.mrows != 0) {
          int LDimSizeP1;
          if (i) {
            for (int idx{0}; idx <= nVar; idx++) {
              solution.searchDir[idx] = 0.0;
            }
          } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

            for (int idx = 0; idx <= nVar; idx++) {
              solution.searchDir[idx] = 0.0;
            }
          }
          LDimSizeP1 = 0;
          i = nullStartIdx + qrmanager.ldq * (mNull - 1);
          for (int k{nullStartIdx}; A_maxDiag_idx < 0 ? k >= i : k <= i;
               k += A_maxDiag_idx) {
            int i1;
            i1 = k + nVar;
            for (int ia{k}; ia <= i1; ia++) {
              int nVars;
              nVars = ia - k;
              solution.searchDir[nVars] =
                  solution.searchDir[nVars] +
                  qrmanager.Q[ia - 1] * memspace.workspace_double[LDimSizeP1];
            }
            LDimSizeP1++;
          }
        }
      } else {
        double smax;
        int A_maxDiag_idx;
        int LDimSizeP1;
        int b_idx;
        int i1;
        int k;
        int nVars;
        if (objective.objtype == 3) {
          internal::blas::xgemm(qrmanager.mrows, mNull, qrmanager.mrows, H,
                                qrmanager.mrows, qrmanager.Q, nullStartIdx,
                                qrmanager.ldq, memspace.workspace_double,
                                memspace.workspace_double.size(0));
          internal::blas::xgemm(mNull, mNull, qrmanager.mrows, qrmanager.Q,
                                nullStartIdx, qrmanager.ldq,
                                memspace.workspace_double,
                                memspace.workspace_double.size(0),
                                cholmanager.FMat, cholmanager.ldm);
        } else if (alwaysPositiveDef) {
          nVars = qrmanager.mrows;
          internal::blas::xgemm(objective.nvar, mNull, objective.nvar, H,
                                objective.nvar, qrmanager.Q, nullStartIdx,
                                qrmanager.ldq, memspace.workspace_double,
                                memspace.workspace_double.size(0));
          i1 = objective.nvar + 1;
          for (A_maxDiag_idx = 0; A_maxDiag_idx < mNull; A_maxDiag_idx++) {
            for (LDimSizeP1 = i1; LDimSizeP1 <= nVars; LDimSizeP1++) {
              memspace.workspace_double[(LDimSizeP1 +
                                         memspace.workspace_double.size(0) *
                                             A_maxDiag_idx) -
                                        1] =
                  objective.beta *
                  qrmanager
                      .Q[(LDimSizeP1 + qrmanager.Q.size(0) *
                                           (A_maxDiag_idx + qrmanager.ncols)) -
                         1];
            }
          }
          internal::blas::xgemm(mNull, mNull, qrmanager.mrows, qrmanager.Q,
                                nullStartIdx, qrmanager.ldq,
                                memspace.workspace_double,
                                memspace.workspace_double.size(0),
                                cholmanager.FMat, cholmanager.ldm);
        }
        if (alwaysPositiveDef) {
          cholmanager.ndims = mNull;
          cholmanager.info = internal::lapack::xpotrf(mNull, cholmanager.FMat,
                                                      cholmanager.ldm);
        } else {
          LDimSizeP1 = cholmanager.ldm + 1;
          cholmanager.ndims = mNull;
          A_maxDiag_idx = 0;
          if (mNull > 1) {
            smax = std::abs(cholmanager.FMat[0]);
            for (k = 2; k <= mNull; k++) {
              double s;
              s = std::abs(cholmanager.FMat[(k - 1) * LDimSizeP1]);
              if (s > smax) {
                A_maxDiag_idx = k - 1;
                smax = s;
              }
            }
          }
          cholmanager.regTol_ = std::fmax(
              std::abs(cholmanager.FMat[A_maxDiag_idx +
                                        cholmanager.ldm * A_maxDiag_idx]) *
                  2.2204460492503131E-16,
              0.0);
          if ((cholmanager.FMat.size(0) * cholmanager.FMat.size(1) > 16384) &&
              (mNull > 128)) {
            boolean_T exitg1;
            k = 0;
            exitg1 = false;
            while ((!exitg1) && (k < mNull)) {
              A_maxDiag_idx = LDimSizeP1 * k + 1;
              nVars = mNull - k;
              if (k + 48 <= mNull) {
                DynamicRegCholManager::partialColLDL3_(cholmanager,
                                                       A_maxDiag_idx, nVars);
                k += 48;
              } else {
                DynamicRegCholManager::fullColLDL2_(cholmanager, A_maxDiag_idx,
                                                    nVars);
                exitg1 = true;
              }
            }
          } else {
            DynamicRegCholManager::fullColLDL2_(cholmanager, 1, mNull);
          }
          if (cholmanager.ConvexCheck) {
            b_idx = 0;
            int exitg2;
            do {
              exitg2 = 0;
              if (b_idx <= mNull - 1) {
                if (cholmanager.FMat[b_idx + cholmanager.ldm * b_idx] <= 0.0) {
                  cholmanager.info = -b_idx - 1;
                  exitg2 = 1;
                } else {
                  b_idx++;
                }
              } else {
                cholmanager.ConvexCheck = false;
                exitg2 = 1;
              }
            } while (exitg2 == 0);
          }
        }
        if (cholmanager.info != 0) {
          solution.state = -6;
        } else {
          b_idx = qrmanager.ldq;
          if (qrmanager.mrows != 0) {
            if (static_cast<int>(mNull < 200)) {
              for (int idx{0}; idx < mNull; idx++) {
                memspace.workspace_double[idx] = 0.0;
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (int idx = 0; idx < mNull; idx++) {
                memspace.workspace_double[idx] = 0.0;
              }
            }
            A_maxDiag_idx = 0;
            i1 = nullStartIdx + qrmanager.ldq * (mNull - 1);
            for (k = nullStartIdx; b_idx < 0 ? k >= i1 : k <= i1; k += b_idx) {
              smax = 0.0;
              nVars = k + nVar;
              for (int ia{k}; ia <= nVars; ia++) {
                smax += qrmanager.Q[ia - 1] * objective.grad[ia - k];
              }
              memspace.workspace_double[A_maxDiag_idx] =
                  memspace.workspace_double[A_maxDiag_idx] - smax;
              A_maxDiag_idx++;
            }
          }
          if (alwaysPositiveDef) {
            nVars = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (k = 0; k < nVars; k++) {
                A_maxDiag_idx = k * cholmanager.ldm;
                smax = memspace.workspace_double[k];
                for (int ia{0}; ia < k; ia++) {
                  smax -= cholmanager.FMat[A_maxDiag_idx + ia] *
                          memspace.workspace_double[ia];
                }
                memspace.workspace_double[k] =
                    smax / cholmanager.FMat[A_maxDiag_idx + k];
              }
            }
            nVars = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (k = nVars; k >= 1; k--) {
                A_maxDiag_idx = (k + (k - 1) * cholmanager.ldm) - 1;
                memspace.workspace_double[k - 1] =
                    memspace.workspace_double[k - 1] /
                    cholmanager.FMat[A_maxDiag_idx];
                for (int ia{0}; ia <= k - 2; ia++) {
                  LDimSizeP1 = (k - ia) - 2;
                  memspace.workspace_double[LDimSizeP1] =
                      memspace.workspace_double[LDimSizeP1] -
                      memspace.workspace_double[k - 1] *
                          cholmanager.FMat[(A_maxDiag_idx - ia) - 1];
                }
              }
            }
          } else {
            nVars = cholmanager.ndims - 2;
            if (cholmanager.ndims != 0) {
              for (k = 0; k <= nVars + 1; k++) {
                A_maxDiag_idx = k + k * cholmanager.ldm;
                i1 = nVars - k;
                for (int ia{0}; ia <= i1; ia++) {
                  LDimSizeP1 = (k + ia) + 1;
                  memspace.workspace_double[LDimSizeP1] =
                      memspace.workspace_double[LDimSizeP1] -
                      memspace.workspace_double[k] *
                          cholmanager.FMat[(A_maxDiag_idx + ia) + 1];
                }
              }
            }
            i1 = cholmanager.ndims;
            if (static_cast<int>(i1 < 200)) {
              for (int idx{0}; idx < i1; idx++) {
                memspace.workspace_double[idx] =
                    memspace.workspace_double[idx] /
                    cholmanager.FMat[idx + cholmanager.ldm * idx];
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (int idx = 0; idx < i1; idx++) {
                memspace.workspace_double[idx] =
                    memspace.workspace_double[idx] /
                    cholmanager.FMat[idx + cholmanager.ldm * idx];
              }
            }
            nVars = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (k = nVars; k >= 1; k--) {
                A_maxDiag_idx = (k - 1) * cholmanager.ldm;
                smax = memspace.workspace_double[k - 1];
                i1 = k + 1;
                for (int ia{nVars}; ia >= i1; ia--) {
                  smax -= cholmanager.FMat[(A_maxDiag_idx + ia) - 1] *
                          memspace.workspace_double[ia - 1];
                }
                memspace.workspace_double[k - 1] = smax;
              }
            }
          }
          if (qrmanager.mrows != 0) {
            if (i) {
              for (int idx{0}; idx <= nVar; idx++) {
                solution.searchDir[idx] = 0.0;
              }
            } else {
#pragma omp parallel for num_threads(                                          \
    4 > omp_get_max_threads() ? omp_get_max_threads() : 4)

              for (int idx = 0; idx <= nVar; idx++) {
                solution.searchDir[idx] = 0.0;
              }
            }
            LDimSizeP1 = 0;
            i = nullStartIdx + qrmanager.ldq * (mNull - 1);
            for (k = nullStartIdx; b_idx < 0 ? k >= i : k <= i; k += b_idx) {
              i1 = k + nVar;
              for (int ia{k}; ia <= i1; ia++) {
                nVars = ia - k;
                solution.searchDir[nVars] =
                    solution.searchDir[nVars] +
                    qrmanager.Q[ia - 1] * memspace.workspace_double[LDimSizeP1];
              }
              LDimSizeP1++;
            }
          }
        }
      }
    }
  }
}

} // namespace qpactiveset
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for compute_deltax.cpp
//
// [EOF]
//
