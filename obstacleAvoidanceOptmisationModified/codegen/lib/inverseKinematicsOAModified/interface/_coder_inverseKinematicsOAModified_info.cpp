//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_inverseKinematicsOAModified_info.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 04-Feb-2025 04:50:11
//

// Include Files
#include "_coder_inverseKinematicsOAModified_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
//
// Arguments    : void
// Return Type  : const mxArray *
//
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[8]{
      "789ced58cb6ed340147550a9bae05190406c90fa038d4b6942b32c49d34708e94b342d42"
      "613cbe4ea6f1ccb8f6386dba40fc00825f8055c5162496fc0b5f825d"
      "c7499dd67520aa1b8a6791f1f5f1cc199fb9ba671c29b5524e49927447f2daf194d7dfee"
      "c4939dfe86146cfd78aad3dfec8ba5eefdb1c0381fffd8e93167020e",
      "85173044a13b52e59430c4c456db00c9048beb2d504f108de8b045286c9e0e5eba112d9e"
      "82ba810bb9d7f906e0e6a64d25b361f556a89f0eba7a2ca7ce7fdfb1"
      "01f5f815a2c7641ffe7af18ddce01464abc195061172d144ac89a68112e7b7a25802611d"
      "a6518b1315310cf20657b8287315749db0ba6cba21ed86652474a4d4",
      "083574a0c00412843359b319762fac9ac6cd1a769eae03937967f2057fee8a2128b14e86"
      "38044423a0ca84b5c0b4a044185007c15665c1c7d23475015a438cb3"
      "22664f82babe0dd16d505dc74375f510aeec156d161f9f254c1b8b1edfc49079731cca17"
      "c4473d6fbc7d48d3a87db83ba02efd7deff9094f5ff85c77fbb8f8de",
      "8d7fbd15279fdfae8aef3064be41f3fa6108df641f9ec7a8b46d8396477c3dafcfce1fb4"
      "5f64f3c5de3ad62278a2d62185c471cd9fd487f3ebc3b0f9f5204217"
      "1f77ce1b1aa9db265274a8287be0acbfe59e39aeca2f1e0d990f3f42f982f8a8e7c3b9fb"
      "92a6f1d5bb4f87b1fa87dffe17beb8fc637f6d6ece28b0757be1a85a",
      "ddab96e6cd428e2e5d1fff48eac560f5e27d880e83eaf43842271fc79c1ab6803262c4b0"
      "9d57243a11ed5584b94290f319129bafa8dc7694e8f14d0d99273f43"
      "f982f8e8e7c905fb13a3bf7c69a6137fb944beb8fc25bbbcbf3423724f17f1f35d0699ea"
      "4e3b93e585ebe32f49ddf8b3ba316cdedd8bd0cbc73dea9a4618d26b",
      "ce2b373bf7afca5f863d877c0be50be2a39e2767f6c5c90db7c5f63fd4abc4572e932f2e"
      "5f59cbe4f0b3c226db3016778d99d9991d818e32f9c457125ff93bbd"
      "ee47e8e5e3cb60590ef31963f9577de57b285f101ff53c39bb2f9eb1c455073f24be72a9"
      "7c71f90ade367558318b957db5845538a864570f76af81affc065b0f",
      "9134",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 8384U, &nameCaptureInfo);
  return nameCaptureInfo;
}

//
// Arguments    : void
// Return Type  : mxArray *
//
mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[9]{"Version",
                                 "ResolvedFunctions",
                                 "Checksum",
                                 "EntryPoints",
                                 "CoverageInfo",
                                 "IsPolymorphic",
                                 "PropertyList",
                                 "UUID",
                                 "ClassEntryPointIsHandle"};
  const char_T *epFieldName[8]{
      "Name",     "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "FullPath", "TimeStamp",      "Constructor",     "Visible"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 8, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 11);
  emlrtSetField(xEntryPoints, 0, "Name",
                emlrtMxCreateString("inverseKinematicsOAModified"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(11.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString(
          "/home/shobhit/Franka-emika-Obstacle-avoidance/RobotModelling/"
          "robotmodelling/Matlab_implementation/functions_for_codegen/obstacle"
          "AvoidanceOptmisationModified/inverseKinematicsOAModified.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739652.17122685187));
  emlrtSetField(xEntryPoints, 0, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 9, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("23.2.0.2485118 (R2023b) Update 6"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("myIPVwBITrcP16xH1DJLaG"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

//
// File trailer for _coder_inverseKinematicsOAModified_info.cpp
//
// [EOF]
//
