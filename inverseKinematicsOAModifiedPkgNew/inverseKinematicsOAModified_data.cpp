//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOAModified_data.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 03-Mar-2025 23:23:36
//

// Include Files
#include "inverseKinematicsOAModified_data.h"
#include "inverseKinematicsOAModified_types.h"
#include "rt_nonfinite.h"
#include <cstring>

// Variable Definitions
rtRunTimeErrorInfo emlrtRTEI{
    138,                  // lineNo
    "dynamic_size_checks" // fName
};

rtRunTimeErrorInfo b_emlrtRTEI{
    133,                  // lineNo
    "dynamic_size_checks" // fName
};

rtBoundsCheckInfo b_emlrtBCI{
    -1,                            // iFirst
    -1,                            // iLast
    1,                             // lineNo
    1,                             // colNo
    "",                            // aName
    "computeConstrViolationIneq_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+fminconsqp/"
    "+MeritFunction/computeConstrViolationIneq_.p", // pName
    0                                               // checkKind
};

rtRunTimeErrorInfo c_emlrtRTEI{
    13,                // lineNo
    "validatenonempty" // fName
};

rtRunTimeErrorInfo d_emlrtRTEI{
    18,                              // lineNo
    "eml_int_forloop_overflow_check" // fName
};

rtBoundsCheckInfo d_emlrtBCI{
    -1,               // iFirst
    -1,               // iLast
    1,                // lineNo
    1,                // colNo
    "",               // aName
    "setProblemType", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/setProblemType.p", // pName
    0                               // checkKind
};

rtBoundsCheckInfo g_emlrtBCI{
    -1,         // iFirst
    -1,         // iLast
    1,          // lineNo
    1,          // colNo
    "",         // aName
    "isActive", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/isActive.p", // pName
    0                         // checkKind
};

rtBoundsCheckInfo h_emlrtBCI{
    -1,             // iFirst
    -1,             // iLast
    1,              // lineNo
    1,              // colNo
    "",             // aName
    "removeConstr", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/removeConstr.p", // pName
    0                             // checkKind
};

rtBoundsCheckInfo i_emlrtBCI{
    1,              // iFirst
    6,              // iLast
    1,              // lineNo
    1,              // colNo
    "",             // aName
    "removeConstr", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/removeConstr.p", // pName
    0                             // checkKind
};

rtBoundsCheckInfo j_emlrtBCI{
    -1,                // iFirst
    -1,                // iLast
    1,                 // lineNo
    1,                 // colNo
    "",                // aName
    "moveConstraint_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/moveConstraint_.p", // pName
    0                                // checkKind
};

rtBoundsCheckInfo k_emlrtBCI{
    1,              // iFirst
    5,              // iLast
    1,              // lineNo
    1,              // colNo
    "",             // aName
    "removeConstr", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/removeConstr.p", // pName
    3                             // checkKind
};

rtBoundsCheckInfo l_emlrtBCI{
    -1,                     // iFirst
    -1,                     // iLast
    1,                      // lineNo
    1,                      // colNo
    "",                     // aName
    "RemoveDependentIneq_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+initialize/RemoveDependentIneq_.p", // pName
    0                                     // checkKind
};

rtBoundsCheckInfo m_emlrtBCI{
    -1,         // iFirst
    -1,         // iLast
    1,          // lineNo
    1,          // colNo
    "",         // aName
    "factorQR", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+QRManager/"
    "factorQR.p", // pName
    0             // checkKind
};

rtBoundsCheckInfo q_emlrtBCI{
    -1,                        // iFirst
    -1,                        // iLast
    1,                         // lineNo
    1,                         // colNo
    "",                        // aName
    "addConstrUpdateRecords_", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/addConstrUpdateRecords_.p", // pName
    0                                        // checkKind
};

rtBoundsCheckInfo r_emlrtBCI{
    -1,               // iFirst
    -1,               // iLast
    1,                // lineNo
    1,                // colNo
    "",               // aName
    "addAineqConstr", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/+qpactiveset/"
    "+WorkingSet/addAineqConstr.p", // pName
    0                               // checkKind
};

rtBoundsCheckInfo s_emlrtBCI{
    -1,       // iFirst
    -1,       // iLast
    1,        // lineNo
    1,        // colNo
    "",       // aName
    "factor", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
    "+DynamicRegCholManager/factor.p", // pName
    0                                  // checkKind
};

rtBoundsCheckInfo t_emlrtBCI{
    -1,      // iFirst
    -1,      // iLast
    1,       // lineNo
    1,       // colNo
    "",      // aName
    "solve", // fName
    "/usr/local/MATLAB/R2023b/toolbox/optim/+optim/+coder/"
    "+DynamicRegCholManager/solve.p", // pName
    0                                 // checkKind
};

//
// File trailer for inverseKinematicsOAModified_data.cpp
//
// [EOF]
//
