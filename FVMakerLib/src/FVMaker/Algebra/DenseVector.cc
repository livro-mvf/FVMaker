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
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

namespace fvm {

namespace {

void require_same_size(const DenseVector& left, const DenseVector& right) {
    require(
        left.size() == right.size(),
        error_catalog::kInvalidSystemSize,
        DenseVector::id()
    );
}

}  // namespace

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

DenseVector& operator+=(DenseVector& left, const DenseVector& right) {
    require_same_size(left, right);

    for (Size i = 0; i < left.size(); ++i) {
        left[i] += right[i];
    }

    return left;
}

DenseVector& operator-=(DenseVector& left, const DenseVector& right) {
    require_same_size(left, right);

    for (Size i = 0; i < left.size(); ++i) {
        left[i] -= right[i];
    }

    return left;
}

DenseVector& operator*=(DenseVector& vector, Real scalar) {
    for (Real& value : vector.values()) {
        value *= scalar;
    }

    return vector;
}

DenseVector operator+(const DenseVector& left, const DenseVector& right) {
    DenseVector result = left;
    result += right;
    return result;
}

DenseVector operator-(const DenseVector& left, const DenseVector& right) {
    DenseVector result = left;
    result -= right;
    return result;
}

DenseVector operator-(const DenseVector& vector) {
    DenseVector result = vector;
    result *= Real{-1};
    return result;
}

DenseVector operator*(Real scalar, const DenseVector& vector) {
    DenseVector result = vector;
    result *= scalar;
    return result;
}

DenseVector operator*(const DenseVector& vector, Real scalar) {
    return scalar * vector;
}

}  // namespace fvm
