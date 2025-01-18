//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inverseKinematicsOA_internal_types.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 10-Jan-2025 01:08:30
//

#ifndef INVERSEKINEMATICSOA_INTERNAL_TYPES_H
#define INVERSEKINEMATICSOA_INTERNAL_TYPES_H

// Include Files
#include "anonymous_function.h"
#include "inverseKinematicsOA_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include "coder_bounded_array.h"

// Type Definitions
struct struct_T {
  double penaltyParam;
  double threshold;
  int nPenaltyDecreases;
  double linearizedConstrViol;
  double initFval;
  double initConstrViolationEq;
  double initConstrViolationIneq;
  double phi;
  double phiPrimePlus;
  double phiFullStep;
  double feasRelativeFactor;
  double nlpPrimalFeasError;
  double nlpDualFeasError;
  double nlpComplError;
  double firstOrderOpt;
  boolean_T hasObjective;
};

struct b_struct_T {
  boolean_T gradOK;
  boolean_T fevalOK;
  boolean_T done;
  boolean_T stepAccepted;
  boolean_T failedLineSearch;
  int stepType;
};

struct d_struct_T {
  int mConstr;
  int mConstrOrig;
  int mConstrMax;
  int nVar;
  int nVarOrig;
  int nVarMax;
  int ldA;
  coder::array<double, 1U> Aineq;
  coder::array<double, 1U> bineq;
  coder::array<double, 1U> Aeq;
  coder::bounded_array<double, 6U, 1U> beq;
  coder::array<double, 1U> lb;
  coder::array<double, 1U> ub;
  coder::array<int, 1U> indexLB;
  coder::array<int, 1U> indexUB;
  coder::array<int, 1U> indexFixed;
  int mEqRemoved;
  coder::bounded_array<int, 6U, 1U> indexEqRemoved;
  coder::array<double, 1U> ATwset;
  coder::array<double, 1U> bwset;
  int nActiveConstr;
  coder::array<double, 1U> maxConstrWorkspace;
  int sizes[5];
  int sizesNormal[5];
  int sizesPhaseOne[5];
  int sizesRegularized[5];
  int sizesRegPhaseOne[5];
  int isActiveIdx[6];
  int isActiveIdxNormal[6];
  int isActiveIdxPhaseOne[6];
  int isActiveIdxRegularized[6];
  int isActiveIdxRegPhaseOne[6];
  coder::array<boolean_T, 1U> isActiveConstr;
  coder::array<int, 1U> Wid;
  coder::array<int, 1U> Wlocalidx;
  int nWConstr[5];
  int probType;
  double SLACK0;
};

struct e_struct_T {
  int ldq;
  coder::array<double, 2U> QR;
  coder::array<double, 2U> Q;
  coder::array<int, 1U> jpvt;
  int mrows;
  int ncols;
  coder::array<double, 1U> tau;
  int minRowCol;
  boolean_T usedPivoting;
};

struct f_struct_T {
  coder::array<double, 2U> FMat;
  int ldm;
  int ndims;
  int info;
  double scaleFactor;
  boolean_T ConvexCheck;
  double regTol_;
  double workspace_;
  double workspace2_;
};

struct g_struct_T {
  coder::array<double, 1U> grad;
  coder::array<double, 1U> Hx;
  boolean_T hasLinear;
  int nvar;
  int maxVar;
  double beta;
  double rho;
  int objtype;
  int prev_objtype;
  int prev_nvar;
  boolean_T prev_hasLinear;
  double gammaScalar;
};

struct h_struct_T {
  coder::array<double, 2U> workspace_double;
  coder::array<int, 1U> workspace_int;
  coder::array<int, 1U> workspace_sort;
};

struct i_struct_T {
  coder::array<double, 1U> FiniteDifferenceStepSize;
  int MaxIterations;
  int MaxFunctionEvaluations;
  coder::array<double, 1U> TypicalX;
  double ConstrRelTolFactor;
};

struct j_struct_T {
  int nVarMax;
  int mNonlinIneq;
  int mNonlinEq;
  int mIneq;
  int mEq;
  int iNonIneq0;
  int iNonEq0;
  double sqpFval;
  double sqpFval_old;
  coder::array<double, 1U> xstarsqp;
  coder::array<double, 1U> xstarsqp_old;
  coder::array<double, 1U> cIneq;
  coder::array<double, 1U> cIneq_old;
  coder::bounded_array<double, 6U, 1U> cEq;
  coder::bounded_array<double, 6U, 1U> cEq_old;
  coder::array<double, 1U> grad;
  coder::array<double, 1U> grad_old;
  int FunctionEvaluations;
  int sqpIterations;
  int sqpExitFlag;
  coder::array<double, 1U> lambdasqp;
  coder::array<double, 1U> lambdaStopTest;
  coder::array<double, 1U> lambdaStopTestPrev;
  double steplength;
  coder::array<double, 1U> delta_x;
  coder::array<double, 1U> socDirection;
  coder::array<int, 1U> workingset_old;
  coder::array<double, 1U> gradLag;
  coder::array<double, 1U> delta_gradLag;
  coder::array<double, 1U> xstar;
  double fstar;
  double firstorderopt;
  coder::array<double, 1U> lambda;
  int state;
  double maxConstr;
  int iterations;
  coder::array<double, 1U> searchDir;
};

struct k_struct_T {
  char SolverName[7];
  int MaxIterations;
  double StepTolerance;
  double ObjectiveLimit;
};

struct l_struct_T {
  coder::anonymous_function objfun;
  double f_1;
  double f_2;
  int nVar;
  int mIneq;
  int mEq;
  int numEvals;
  boolean_T SpecifyObjectiveGradient;
  boolean_T SpecifyConstraintGradient;
  boolean_T isEmptyNonlcon;
  coder::array<boolean_T, 1U> hasLB;
  coder::array<boolean_T, 1U> hasUB;
  boolean_T hasBounds;
  int FiniteDifferenceType;
};

#endif
//
// File trailer for inverseKinematicsOA_internal_types.h
//
// [EOF]
//
