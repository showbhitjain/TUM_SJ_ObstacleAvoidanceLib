//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: compute_deltax.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

// Include Files
#include "compute_deltax.h"
#include "factor.h"
#include "fullColLDL2_.h"
#include "inverseKinematicsOA_internal_types.h"
#include "partialColLDL3_.h"
#include "rt_nonfinite.h"
#include "solve.h"
#include "xgemm.h"
#include "xpotrf.h"
#include "coder_array.h"
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
  int mNull_tmp;
  int nVar_tmp;
  nVar_tmp = qrmanager.mrows - 1;
  mNull_tmp = qrmanager.mrows - qrmanager.ncols;
  if (mNull_tmp <= 0) {
    for (int ix{0}; ix <= nVar_tmp; ix++) {
      solution.searchDir[ix] = 0.0;
    }
  } else {
    int ix;
    for (ix = 0; ix <= nVar_tmp; ix++) {
      solution.searchDir[ix] = -objective.grad[ix];
    }
    if (qrmanager.ncols <= 0) {
      switch (objective.objtype) {
      case 5:
        break;
      case 3: {
        int LD_diagOffset;
        int LDimSizeP1;
        if (alwaysPositiveDef) {
          cholmanager.ndims = qrmanager.mrows;
          if ((H.size(0) != 0) && (H.size(1) != 0)) {
            for (ix = 0; ix <= nVar_tmp; ix++) {
              LDimSizeP1 = (nVar_tmp + 1) * ix;
              LD_diagOffset = cholmanager.ldm * ix;
              for (int k{0}; k <= nVar_tmp; k++) {
                cholmanager.FMat[LD_diagOffset + k] = H[LDimSizeP1 + k];
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
          int i;
          int nVars;
          LD_diagOffset = cholmanager.ndims - 2;
          if ((solution.searchDir.size(0) != 0) && (cholmanager.ndims != 0)) {
            for (LDimSizeP1 = 0; LDimSizeP1 <= LD_diagOffset + 1;
                 LDimSizeP1++) {
              nVars = LDimSizeP1 + LDimSizeP1 * cholmanager.ldm;
              i = LD_diagOffset - LDimSizeP1;
              for (int iac{0}; iac <= i; iac++) {
                ix = (LDimSizeP1 + iac) + 1;
                solution.searchDir[ix] =
                    solution.searchDir[ix] -
                    solution.searchDir[LDimSizeP1] *
                        cholmanager.FMat[(nVars + iac) + 1];
              }
            }
          }
          i = cholmanager.ndims;
          for (ix = 0; ix < i; ix++) {
            solution.searchDir[ix] =
                solution.searchDir[ix] /
                cholmanager.FMat[ix + cholmanager.ldm * ix];
          }
          LD_diagOffset = cholmanager.ndims;
          if ((solution.searchDir.size(0) != 0) && (cholmanager.ndims != 0)) {
            for (LDimSizeP1 = LD_diagOffset; LDimSizeP1 >= 1; LDimSizeP1--) {
              double smax;
              nVars = (LDimSizeP1 - 1) * cholmanager.ldm;
              smax = solution.searchDir[LDimSizeP1 - 1];
              i = LDimSizeP1 + 1;
              for (int iac{LD_diagOffset}; iac >= i; iac--) {
                smax -= cholmanager.FMat[(nVars + iac) - 1] *
                        solution.searchDir[iac - 1];
              }
              solution.searchDir[LDimSizeP1 - 1] = smax;
            }
          }
        }
      } break;
      default: {
        if (alwaysPositiveDef) {
          int LDimSizeP1;
          int nVars;
          nVars = objective.nvar;
          cholmanager.ndims = objective.nvar;
          if ((H.size(0) != 0) && (H.size(1) != 0)) {
            for (ix = 0; ix < nVars; ix++) {
              int LD_diagOffset;
              LDimSizeP1 = nVars * ix;
              LD_diagOffset = cholmanager.ldm * ix;
              for (int k{0}; k < nVars; k++) {
                cholmanager.FMat[LD_diagOffset + k] = H[LDimSizeP1 + k];
              }
            }
          }
          cholmanager.info = internal::lapack::xpotrf(
              objective.nvar, cholmanager.FMat, cholmanager.ldm);
          if (cholmanager.info != 0) {
            solution.state = -6;
          } else {
            double smax;
            int i;
            CholManager::solve(cholmanager, solution.searchDir);
            smax = 1.0 / objective.beta;
            LDimSizeP1 = objective.nvar + 1;
            i = qrmanager.mrows;
            for (int k{LDimSizeP1}; k <= i; k++) {
              solution.searchDir[k - 1] = smax * solution.searchDir[k - 1];
            }
          }
        }
      } break;
      }
    } else {
      int nullStartIdx_tmp;
      nullStartIdx_tmp = qrmanager.ldq * qrmanager.ncols + 1;
      if (objective.objtype == 5) {
        int LD_diagOffset;
        for (ix = 0; ix < mNull_tmp; ix++) {
          memspace.workspace_double[ix] =
              -qrmanager.Q[nVar_tmp + qrmanager.ldq * (qrmanager.ncols + ix)];
        }
        LD_diagOffset = qrmanager.ldq;
        if (qrmanager.mrows != 0) {
          int i;
          int nVars;
          for (nVars = 0; nVars <= nVar_tmp; nVars++) {
            solution.searchDir[nVars] = 0.0;
          }
          ix = 0;
          i = nullStartIdx_tmp + qrmanager.ldq * (mNull_tmp - 1);
          for (int iac{nullStartIdx_tmp};
               LD_diagOffset < 0 ? iac >= i : iac <= i; iac += LD_diagOffset) {
            int LDimSizeP1;
            LDimSizeP1 = iac + nVar_tmp;
            for (int ia{iac}; ia <= LDimSizeP1; ia++) {
              nVars = ia - iac;
              solution.searchDir[nVars] =
                  solution.searchDir[nVars] +
                  qrmanager.Q[ia - 1] * memspace.workspace_double[ix];
            }
            ix++;
          }
        }
      } else {
        double smax;
        int LD_diagOffset;
        int LDimSizeP1;
        int i;
        int k;
        int nVars;
        if (objective.objtype == 3) {
          internal::blas::xgemm(qrmanager.mrows, mNull_tmp, qrmanager.mrows, H,
                                qrmanager.mrows, qrmanager.Q, nullStartIdx_tmp,
                                qrmanager.ldq, memspace.workspace_double,
                                memspace.workspace_double.size(0));
          internal::blas::xgemm(mNull_tmp, mNull_tmp, qrmanager.mrows,
                                qrmanager.Q, nullStartIdx_tmp, qrmanager.ldq,
                                memspace.workspace_double,
                                memspace.workspace_double.size(0),
                                cholmanager.FMat, cholmanager.ldm);
        } else if (alwaysPositiveDef) {
          nVars = qrmanager.mrows;
          internal::blas::xgemm(objective.nvar, mNull_tmp, objective.nvar, H,
                                objective.nvar, qrmanager.Q, nullStartIdx_tmp,
                                qrmanager.ldq, memspace.workspace_double,
                                memspace.workspace_double.size(0));
          i = objective.nvar + 1;
          for (LD_diagOffset = 0; LD_diagOffset < mNull_tmp; LD_diagOffset++) {
            for (LDimSizeP1 = i; LDimSizeP1 <= nVars; LDimSizeP1++) {
              memspace.workspace_double[(LDimSizeP1 +
                                         memspace.workspace_double.size(0) *
                                             LD_diagOffset) -
                                        1] =
                  objective.beta *
                  qrmanager
                      .Q[(LDimSizeP1 + qrmanager.Q.size(0) *
                                           (LD_diagOffset + qrmanager.ncols)) -
                         1];
            }
          }
          internal::blas::xgemm(mNull_tmp, mNull_tmp, qrmanager.mrows,
                                qrmanager.Q, nullStartIdx_tmp, qrmanager.ldq,
                                memspace.workspace_double,
                                memspace.workspace_double.size(0),
                                cholmanager.FMat, cholmanager.ldm);
        }
        if (alwaysPositiveDef) {
          cholmanager.ndims = mNull_tmp;
          cholmanager.info = internal::lapack::xpotrf(
              mNull_tmp, cholmanager.FMat, cholmanager.ldm);
        } else {
          LDimSizeP1 = cholmanager.ldm + 1;
          cholmanager.ndims = mNull_tmp;
          nVars = 0;
          if (mNull_tmp > 1) {
            smax = std::abs(cholmanager.FMat[0]);
            for (k = 2; k <= mNull_tmp; k++) {
              double s;
              s = std::abs(cholmanager.FMat[(k - 1) * LDimSizeP1]);
              if (s > smax) {
                nVars = k - 1;
                smax = s;
              }
            }
          }
          cholmanager.regTol_ = std::fmax(
              std::abs(cholmanager.FMat[nVars + cholmanager.ldm * nVars]) *
                  2.2204460492503131E-16,
              0.0);
          if ((cholmanager.FMat.size(0) * cholmanager.FMat.size(1) > 16384) &&
              (mNull_tmp > 128)) {
            boolean_T exitg1;
            k = 0;
            exitg1 = false;
            while ((!exitg1) && (k < mNull_tmp)) {
              LD_diagOffset = LDimSizeP1 * k + 1;
              nVars = mNull_tmp - k;
              if (k + 48 <= mNull_tmp) {
                DynamicRegCholManager::partialColLDL3_(cholmanager,
                                                       LD_diagOffset, nVars);
                k += 48;
              } else {
                DynamicRegCholManager::fullColLDL2_(cholmanager, LD_diagOffset,
                                                    nVars);
                exitg1 = true;
              }
            }
          } else {
            DynamicRegCholManager::fullColLDL2_(cholmanager, 1, mNull_tmp);
          }
          if (cholmanager.ConvexCheck) {
            ix = 0;
            int exitg2;
            do {
              exitg2 = 0;
              if (ix <= mNull_tmp - 1) {
                if (cholmanager.FMat[ix + cholmanager.ldm * ix] <= 0.0) {
                  cholmanager.info = -ix - 1;
                  exitg2 = 1;
                } else {
                  ix++;
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
          k = qrmanager.ldq;
          if (qrmanager.mrows != 0) {
            for (nVars = 0; nVars < mNull_tmp; nVars++) {
              memspace.workspace_double[nVars] = 0.0;
            }
            nVars = 0;
            i = nullStartIdx_tmp + qrmanager.ldq * (mNull_tmp - 1);
            for (int iac{nullStartIdx_tmp}; k < 0 ? iac >= i : iac <= i;
                 iac += k) {
              smax = 0.0;
              LDimSizeP1 = iac + nVar_tmp;
              for (int ia{iac}; ia <= LDimSizeP1; ia++) {
                smax += qrmanager.Q[ia - 1] * objective.grad[ia - iac];
              }
              memspace.workspace_double[nVars] =
                  memspace.workspace_double[nVars] - smax;
              nVars++;
            }
          }
          if (alwaysPositiveDef) {
            LD_diagOffset = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (LDimSizeP1 = 0; LDimSizeP1 < LD_diagOffset; LDimSizeP1++) {
                nVars = LDimSizeP1 * cholmanager.ldm;
                smax = memspace.workspace_double[LDimSizeP1];
                for (int iac{0}; iac < LDimSizeP1; iac++) {
                  smax -= cholmanager.FMat[nVars + iac] *
                          memspace.workspace_double[iac];
                }
                memspace.workspace_double[LDimSizeP1] =
                    smax / cholmanager.FMat[nVars + LDimSizeP1];
              }
            }
            LD_diagOffset = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (LDimSizeP1 = LD_diagOffset; LDimSizeP1 >= 1; LDimSizeP1--) {
                nVars = (LDimSizeP1 + (LDimSizeP1 - 1) * cholmanager.ldm) - 1;
                memspace.workspace_double[LDimSizeP1 - 1] =
                    memspace.workspace_double[LDimSizeP1 - 1] /
                    cholmanager.FMat[nVars];
                for (int iac{0}; iac <= LDimSizeP1 - 2; iac++) {
                  ix = (LDimSizeP1 - iac) - 2;
                  memspace.workspace_double[ix] =
                      memspace.workspace_double[ix] -
                      memspace.workspace_double[LDimSizeP1 - 1] *
                          cholmanager.FMat[(nVars - iac) - 1];
                }
              }
            }
          } else {
            LD_diagOffset = cholmanager.ndims - 2;
            if (cholmanager.ndims != 0) {
              for (LDimSizeP1 = 0; LDimSizeP1 <= LD_diagOffset + 1;
                   LDimSizeP1++) {
                nVars = LDimSizeP1 + LDimSizeP1 * cholmanager.ldm;
                i = LD_diagOffset - LDimSizeP1;
                for (int iac{0}; iac <= i; iac++) {
                  ix = (LDimSizeP1 + iac) + 1;
                  memspace.workspace_double[ix] =
                      memspace.workspace_double[ix] -
                      memspace.workspace_double[LDimSizeP1] *
                          cholmanager.FMat[(nVars + iac) + 1];
                }
              }
            }
            i = cholmanager.ndims;
            for (ix = 0; ix < i; ix++) {
              memspace.workspace_double[ix] =
                  memspace.workspace_double[ix] /
                  cholmanager.FMat[ix + cholmanager.ldm * ix];
            }
            LD_diagOffset = cholmanager.ndims;
            if (cholmanager.ndims != 0) {
              for (LDimSizeP1 = LD_diagOffset; LDimSizeP1 >= 1; LDimSizeP1--) {
                nVars = (LDimSizeP1 - 1) * cholmanager.ldm;
                smax = memspace.workspace_double[LDimSizeP1 - 1];
                i = LDimSizeP1 + 1;
                for (int iac{LD_diagOffset}; iac >= i; iac--) {
                  smax -= cholmanager.FMat[(nVars + iac) - 1] *
                          memspace.workspace_double[iac - 1];
                }
                memspace.workspace_double[LDimSizeP1 - 1] = smax;
              }
            }
          }
          if (qrmanager.mrows != 0) {
            for (nVars = 0; nVars <= nVar_tmp; nVars++) {
              solution.searchDir[nVars] = 0.0;
            }
            ix = 0;
            i = nullStartIdx_tmp + qrmanager.ldq * (mNull_tmp - 1);
            for (int iac{nullStartIdx_tmp}; k < 0 ? iac >= i : iac <= i;
                 iac += k) {
              LDimSizeP1 = iac + nVar_tmp;
              for (int ia{iac}; ia <= LDimSizeP1; ia++) {
                nVars = ia - iac;
                solution.searchDir[nVars] =
                    solution.searchDir[nVars] +
                    qrmanager.Q[ia - 1] * memspace.workspace_double[ix];
              }
              ix++;
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
