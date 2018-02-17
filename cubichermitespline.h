/*
Original Author: https://github.com/dkkim1005/

Code adapted from:

https://github.com/dkkim1005/Monotone_Cubic_Interpolation/blob/master/cubic_hermite_spline.cpp

Changes:
 -Split into .cpp and .h files.
 -Defined Copy and Assign Constructors as delete

*/

#include <iostream>
#include <cstring>
#include <cmath>
#include <functional>
#include <cassert>
#include <vector>

class CubicHermiteSpline {
public:
  explicit CubicHermiteSpline();
  CubicHermiteSpline(const double*, const double*, const double*, const size_t);
  CubicHermiteSpline(const CubicHermiteSpline&) = delete;
  CubicHermiteSpline& operator=(const CubicHermiteSpline&) = delete;
  double operator()(const double) const;
  virtual ~CubicHermiteSpline();

protected:
  double* xVec_;
  double* yVec_;
  double* mVec_;
  size_t size_;
  void delete_heap_memory_();
  void allocate_heap_memory_(const double*, const double*,
			     const double*, const size_t);
  size_t binary_search_(const double&) const;
  double interp_f_(const double&, const size_t&) const;
};

class MonotoneCubicInterpolation : public CubicHermiteSpline {
public:
  MonotoneCubicInterpolation(const double*, const double*, const size_t);
  ~MonotoneCubicInterpolation();
};
