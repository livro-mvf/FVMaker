// ----------------------------------------------------------------------------
// File: DenseVector.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a contiguous dense vector for algebra kernels.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>

namespace fvm {

DenseVector::DenseVector(Size size) : values_(size) {}

DenseVector::DenseVector(Size size, Real value) : values_(size, value) {}

DenseVector::DenseVector(std::vector<Real> values) : values_(std::move(values)) {}

Size DenseVector::size() const noexcept {
    return values_.size();
}

bool DenseVector::empty() const noexcept {
    return values_.empty();
}

std::span<Real> DenseVector::values() noexcept {
    return values_;
}

std::span<const Real> DenseVector::values() const noexcept {
    return values_;
}

Real& DenseVector::operator[](Size index) noexcept {
    return values_[index];
}

const Real& DenseVector::operator[](Size index) const noexcept {
    return values_[index];
}

}  // namespace fvm
