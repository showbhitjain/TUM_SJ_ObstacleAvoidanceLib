//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: quaternion.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 26-Oct-2024 20:50:12
//

#ifndef QUATERNION_H
#define QUATERNION_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include "coder_bounded_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace trajectoryGeneration {
namespace coder {
class c_quaternion;

}
} // namespace trajectoryGeneration

// Type Definitions
namespace trajectoryGeneration {
namespace coder {
class quaternion {
public:
  quaternion slerp(const quaternion q2) const;
  quaternion quaternionBase_slerp(const quaternion q2) const;
  void init();
  void b_log();
  void parenAssign(const c_quaternion rhs);
  double a;
  double b;
  double c;
  double d;
};

class b_quaternion {
public:
  ::coder::array<double, 1U> a;
  ::coder::array<double, 1U> b;
  ::coder::array<double, 1U> c;
  ::coder::array<double, 1U> d;
};

class c_quaternion {
public:
  ::coder::bounded_array<double, 1U, 2U> a;
  ::coder::bounded_array<double, 1U, 2U> b;
  ::coder::bounded_array<double, 1U, 2U> c;
  ::coder::bounded_array<double, 1U, 2U> d;
};

} // namespace coder
} // namespace trajectoryGeneration

#endif
//
// File trailer for quaternion.h
//
// [EOF]
//
