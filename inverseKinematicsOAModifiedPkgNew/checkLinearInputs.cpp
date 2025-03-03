//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: checkLinearInputs.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "checkLinearInputs.h"
#include "all.h"
#include "eml_int_forloop_overflow_check.h"
#include "inverseKinematicsOAModified_rtwutil.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>

// Function Declarations
static void b_rtErrorWithMessageID(const int i, const char *aFcnName,
                                   int aLineNum);

static void c_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void d_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void e_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void f_rtErrorWithMessageID(const char *aFcnName, int aLineNum);

static void rtErrorWithMessageID(const int i, const char *aFcnName,
                                 int aLineNum);

static void rtErrorWithMessageID(const char *r, const int i,
                                 const char *aFcnName, int aLineNum);

// Function Definitions
//
// Arguments    : const int i
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void b_rtErrorWithMessageID(const int i, const char *aFcnName,
                                   int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "Aeq must have ") << i) << " column(s).";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void c_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Row dimension of A is inconsistent with length of b.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void d_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "Row dimension of Aeq is inconsistent with length of beq.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void e_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "\'LB\' must contain only finite values or -Inf.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void f_rtErrorWithMessageID(const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  outStream << "\'UB\' must contain only finite values or Inf.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const int i
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void rtErrorWithMessageID(const int i, const char *aFcnName,
                                 int aLineNum)
{
  std::stringstream outStream;
  ((outStream << "A must have ") << i) << " column(s).";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : const char *r
//                const int i
//                const char *aFcnName
//                int aLineNum
// Return Type  : void
//
static void rtErrorWithMessageID(const char *r, const int i,
                                 const char *aFcnName, int aLineNum)
{
  std::stringstream outStream;
  ((((outStream << "Invalid bounds. ") << r) << " must be empty or have ") << i)
      << " elements.";
  outStream << "\n";
  ((((outStream << "Error in ") << aFcnName) << " (line ") << aLineNum) << ")";
  throw std::runtime_error(outStream.str());
}

//
// Arguments    : int nVar
//                const array<double, 2U> &Aineq
//                const array<double, 2U> &bineq
//                const array<double, 2U> &Aeq
//                const double beq_data[]
//                const int beq_size[2]
//                const array<double, 2U> &lb
//                const array<double, 2U> &ub
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace validate {
double checkLinearInputs(int nVar, const array<double, 2U> &Aineq,
                         const array<double, 2U> &bineq,
                         const array<double, 2U> &Aeq, const double beq_data[],
                         const int beq_size[2], const array<double, 2U> &lb,
                         const array<double, 2U> &ub)
{
  static rtBoundsCheckInfo w_emlrtBCI{
      -1,            // iFirst
      -1,            // iLast
      1,             // lineNo
      1,             // colNo
      "",            // aName
      "checkBounds", // fName
      "/usr/local/MATLAB/R2023b/toolbox/shared/optimlib/+optim/+coder/"
      "+validate/checkBounds.p", // pName
      0                          // checkKind
  };
  static rtRunTimeErrorInfo e_emlrtRTEI{
      1,                  // lineNo
      "checkLinearInputs" // fName
  };
  static rtRunTimeErrorInfo f_emlrtRTEI{
      1,            // lineNo
      "checkBounds" // fName
  };
  array<boolean_T, 2U> b_lb;
  array<boolean_T, 1U> r;
  double exitflag;
  int Aineq_idx_0;
  boolean_T exitg1;
  boolean_T y;
  if ((Aineq.size(0) != 0) && (Aineq.size(1) != 0)) {
    Aineq_idx_0 = Aineq.size(0) * Aineq.size(1);
    r.set_size(Aineq_idx_0);
    for (int i{0}; i < Aineq_idx_0; i++) {
      r[i] = ((!std::isinf(Aineq[i])) && (!std::isnan(Aineq[i])));
    }
    if (!all(r)) {
      rtErrorWithMessageID("A", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
    }
  }
  if ((bineq.size(0) != 0) && (bineq.size(1) != 0)) {
    r.set_size(bineq.size(0));
    Aineq_idx_0 = bineq.size(0);
    for (int i{0}; i < Aineq_idx_0; i++) {
      r[i] = ((!std::isinf(bineq[i])) && (!std::isnan(bineq[i])));
    }
    if (!all(r)) {
      rtErrorWithMessageID("B", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
    }
  }
  if ((Aeq.size(0) != 0) && (Aeq.size(1) != 0)) {
    Aineq_idx_0 = Aeq.size(0) * Aeq.size(1);
    r.set_size(Aineq_idx_0);
    for (int i{0}; i < Aineq_idx_0; i++) {
      r[i] = ((!std::isinf(Aeq[i])) && (!std::isnan(Aeq[i])));
    }
    if (!all(r)) {
      rtErrorWithMessageID("Aeq", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
    }
  }
  if ((beq_size[0] != 0) && (beq_size[1] != 0)) {
    r.set_size(beq_size[0]);
    Aineq_idx_0 = beq_size[0];
    for (int i{0}; i < Aineq_idx_0; i++) {
      double d;
      d = beq_data[i];
      r[i] = ((!std::isinf(d)) && (!std::isnan(d)));
    }
    if (!all(r)) {
      rtErrorWithMessageID("Beq", e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
    }
  }
  y = ((Aineq.size(0) != 0) && (Aineq.size(1) != 0));
  if (y && (Aineq.size(1) != nVar)) {
    rtErrorWithMessageID(nVar, e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
  if (y && (Aineq.size(0) != bineq.size(0) * bineq.size(1))) {
    c_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
  y = ((Aeq.size(0) != 0) && (Aeq.size(1) != 0));
  if (y && (Aeq.size(1) != nVar)) {
    b_rtErrorWithMessageID(nVar, e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
  if (y && (Aeq.size(0) != beq_size[0] * beq_size[1])) {
    d_rtErrorWithMessageID(e_emlrtRTEI.fName, e_emlrtRTEI.lineNo);
  }
  if ((lb.size(0) != 0) && (lb.size(0) != nVar)) {
    rtErrorWithMessageID("lb", nVar, f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
  }
  b_lb.set_size(lb.size(0), 1);
  Aineq_idx_0 = lb.size(0);
  for (int i{0}; i < Aineq_idx_0; i++) {
    b_lb[i] = (lb[i] >= rtInf);
  }
  y = false;
  if (b_lb.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  Aineq_idx_0 = 1;
  exitg1 = false;
  while ((!exitg1) && (Aineq_idx_0 <= b_lb.size(0))) {
    if (b_lb[Aineq_idx_0 - 1]) {
      y = true;
      exitg1 = true;
    } else {
      Aineq_idx_0++;
    }
  }
  if (y) {
    e_rtErrorWithMessageID(f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
  } else {
    b_lb.set_size(lb.size(0), 1);
    Aineq_idx_0 = lb.size(0);
    for (int i{0}; i < Aineq_idx_0; i++) {
      b_lb[i] = std::isnan(lb[i]);
    }
    y = false;
    if (b_lb.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    Aineq_idx_0 = 1;
    exitg1 = false;
    while ((!exitg1) && (Aineq_idx_0 <= b_lb.size(0))) {
      if (b_lb[Aineq_idx_0 - 1]) {
        y = true;
        exitg1 = true;
      } else {
        Aineq_idx_0++;
      }
    }
    if (y) {
      e_rtErrorWithMessageID(f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
    }
  }
  if ((ub.size(0) != 0) && (ub.size(0) != nVar)) {
    rtErrorWithMessageID("ub", nVar, f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
  }
  b_lb.set_size(ub.size(0), 1);
  Aineq_idx_0 = ub.size(0);
  for (int i{0}; i < Aineq_idx_0; i++) {
    b_lb[i] = (ub[i] <= rtMinusInf);
  }
  y = false;
  if (b_lb.size(0) > 2147483646) {
    check_forloop_overflow_error();
  }
  Aineq_idx_0 = 1;
  exitg1 = false;
  while ((!exitg1) && (Aineq_idx_0 <= b_lb.size(0))) {
    if (b_lb[Aineq_idx_0 - 1]) {
      y = true;
      exitg1 = true;
    } else {
      Aineq_idx_0++;
    }
  }
  if (y) {
    f_rtErrorWithMessageID(f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
  } else {
    b_lb.set_size(ub.size(0), 1);
    Aineq_idx_0 = ub.size(0);
    for (int i{0}; i < Aineq_idx_0; i++) {
      b_lb[i] = std::isnan(ub[i]);
    }
    y = false;
    if (b_lb.size(0) > 2147483646) {
      check_forloop_overflow_error();
    }
    Aineq_idx_0 = 1;
    exitg1 = false;
    while ((!exitg1) && (Aineq_idx_0 <= b_lb.size(0))) {
      if (b_lb[Aineq_idx_0 - 1]) {
        y = true;
        exitg1 = true;
      } else {
        Aineq_idx_0++;
      }
    }
    if (y) {
      f_rtErrorWithMessageID(f_emlrtRTEI.fName, f_emlrtRTEI.lineNo);
    }
  }
  exitflag = rtInf;
  y = (lb.size(0) == 0);
  if ((!y) && (ub.size(0) != 0)) {
    if (nVar > 2147483646) {
      check_forloop_overflow_error();
    }
    Aineq_idx_0 = 0;
    exitg1 = false;
    while ((!exitg1) && (Aineq_idx_0 <= nVar - 1)) {
      if ((Aineq_idx_0 + 1 < 1) || (Aineq_idx_0 + 1 > lb.size(0))) {
        rtDynamicBoundsError(Aineq_idx_0 + 1, 1, lb.size(0), w_emlrtBCI);
      }
      if ((Aineq_idx_0 + 1 < 1) || (Aineq_idx_0 + 1 > ub.size(0))) {
        rtDynamicBoundsError(Aineq_idx_0 + 1, 1, ub.size(0), w_emlrtBCI);
      }
      if (lb[Aineq_idx_0] > ub[Aineq_idx_0]) {
        exitflag = -2.0;
        exitg1 = true;
      } else {
        Aineq_idx_0++;
      }
    }
  }
  return exitflag;
}

} // namespace validate
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for checkLinearInputs.cpp
//
// [EOF]
//
