/*
Original Author: https://github.com/dkkim1005/

Code adapted from:

https://github.com/dkkim1005/Monotone_Cubic_Interpolation/blob/master/cubic_hermite_spline.cpp

Changes:
 -Split into .cpp and .h files.
 -Defined Copy and Assign Constructors as delete

*/

#include "cubichermitespline.h"

CubicHermiteSpline::CubicHermiteSpline(const double* xVec,
		const double* yVec, const double* mVec, const size_t size) :
		xVec_(nullptr), yVec_(nullptr), mVec_(nullptr), size_(0)
{
  allocate_heap_memory_(xVec, yVec, mVec, size);
}

CubicHermiteSpline::~CubicHermiteSpline() {
  delete_heap_memory_();
}

double CubicHermiteSpline::operator()(const double x) const {
  const size_t idx = binary_search_(x);

  if(idx == size_-1) {
    return yVec_[size_-1];
  }

  const double t = (x - xVec_[idx])/(xVec_[idx+1] - xVec_[idx]);

  return interp_f_(t, idx);
}

CubicHermiteSpline::CubicHermiteSpline() : xVec_(nullptr), yVec_(nullptr),
				mVec_(nullptr), size_(0) {}

void CubicHermiteSpline::delete_heap_memory_() {
  if(xVec_ != nullptr) {
    delete [] xVec_;
  }
  if(yVec_ != nullptr) {
    delete [] yVec_;
  }
  if(mVec_ != nullptr) {
    delete [] mVec_;
  }
}

void CubicHermiteSpline::allocate_heap_memory_(const double* xVec,
		const double* yVec, const double* mVec, const size_t size)
{
  delete_heap_memory_();

  xVec_ = new double[size];
  yVec_ = new double[size];
  mVec_ = new double[size];
  size_ = size;

  std::memcpy(xVec_, xVec, sizeof(double)*size);
  std::memcpy(yVec_, yVec, sizeof(double)*size);
  std::memcpy(mVec_, mVec, sizeof(double)*size);
}

size_t CubicHermiteSpline::binary_search_(const double& x) const {
  assert(xVec_[0] <= x and x <= xVec_[size_-1]);
  size_t idx_l = 0, idx_r = size_ - 1, idx = size_/2;

  auto is_x_in_Boundary = [this, &x](const size_t& idx) -> bool
		{ return this->xVec_[idx] <= x and x < this->xVec_[idx+1]; };

  while(1) {
    if(idx_r - idx_l == 1) {
      if(is_x_in_Boundary(idx)) {
        return idx;
      }
      else {
        return idx+1;
      }
    }

    if(is_x_in_Boundary(idx)) {
      return idx;
    }
    else if(xVec_[idx+1] <= x) {
      idx_l = idx;
      idx = (idx_r - idx_l)/2 + idx_l;
    }
    else {
      idx_r = idx;
      idx = (idx_r - idx_l)/2 + idx_l;
    }
  }
}

double CubicHermiteSpline::interp_f_(const double& t, const size_t& idx) const {
  return (2*std::pow(t,3) - 3*std::pow(t,2) + 1)*yVec_[idx] +
     (std::pow(t,3) - 2*std::pow(t,2) + t)*(xVec_[idx+1]-xVec_[idx])*
     mVec_[idx] + (-2*std::pow(t,3) + 3*std::pow(t,2))*yVec_[idx+1] +
     (std::pow(t,3) - std::pow(t,2))*(xVec_[idx+1] - xVec_[idx])*mVec_[idx+1];
}


MonotoneCubicInterpolation::MonotoneCubicInterpolation(const double* xVec,
		const double* yVec, const size_t size) : CubicHermiteSpline() {
    std::vector<double> delta(size, 0);
    std::vector<double> mVec(size, 0);

    for(int i=0; i<(int)size-1; i++) {
      delta[i] = (yVec[i+1] - yVec[i])/(xVec[i+1] - xVec[i]);
    }

    for(int i=1; i<(int)size-1; i++) {
      mVec[i] = (delta[i-1] + delta[i])/2.;
    }

    mVec[0] = delta[0]; mVec[size-1] = delta[size-2];

    for(int i=0; i<(int)size-1; i++) {
      if(std::abs(delta[i]) < 1e-30) {
        mVec[i] = mVec[i+1] = 0.;
      }
    }

    allocate_heap_memory_(xVec, yVec, &mVec[0], size);
  }

MonotoneCubicInterpolation::~MonotoneCubicInterpolation() {
}
