// ----------------------------------------------------------------------------
// File: TridiagonalSystem1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a tridiagonal linear system for 1D finite volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <memory>
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

TridiagonalSystem1D::TridiagonalSystem1D(Size size)
    : lower_(size > 0 ? size - 1 : 0),
      diagonal_(size),
      upper_(size > 0 ? size - 1 : 0),
      rhs_(size) {
    validate();
}

TridiagonalSystem1D::TridiagonalSystem1D(
    std::vector<Real> lower,
    std::vector<Real> diagonal,
    std::vector<Real> upper,
    DenseVector rhs
)
    : lower_(std::move(lower)),
      diagonal_(std::move(diagonal)),
      upper_(std::move(upper)),
      rhs_(std::move(rhs)) {
    validate();
}

Size TridiagonalSystem1D::size() const noexcept {
    return diagonal_.size();
}

std::span<Real> TridiagonalSystem1D::lower() noexcept {
    return lower_;
}

std::span<const Real> TridiagonalSystem1D::lower() const noexcept {
    return lower_;
}

std::span<Real> TridiagonalSystem1D::diagonal() noexcept {
    return diagonal_;
}

std::span<const Real> TridiagonalSystem1D::diagonal() const noexcept {
    return diagonal_;
}

std::span<Real> TridiagonalSystem1D::upper() noexcept {
    return upper_;
}

std::span<const Real> TridiagonalSystem1D::upper() const noexcept {
    return upper_;
}

DenseVector& TridiagonalSystem1D::rhs() noexcept {
    return rhs_;
}

const DenseVector& TridiagonalSystem1D::rhs() const noexcept {
    return rhs_;
}

void TridiagonalSystem1D::set_row(
    Size row,
    Real lower,
    Real diagonal,
    Real upper,
    Real rhs
) {
    require(row < size(), error_catalog::kOutOfRange, TridiagonalSystem1D::id());

    diagonal_[row] = diagonal;
    rhs_[row] = rhs;

    if (row > 0) {
        lower_[row - 1] = lower;
    }

    if (row + 1 < size()) {
        upper_[row] = upper;
    }
}

void TridiagonalSystem1D::validate() const {
    const Size system_size = diagonal_.size();

    require(
        system_size > 0,
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );

    require(
        lower_.size() + 1 == system_size,
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );

    require(
        upper_.size() + 1 == system_size,
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );

    require(
        rhs_.size() == system_size,
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );
}

void multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
) {
    require(
        x.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );
    require(
        y.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );
    require(
        std::addressof(x) != std::addressof(y),
        error_catalog::kInvalidArgument,
        TridiagonalSystem1D::id()
    );

    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();

    for (Size row = 0; row < system.size(); ++row) {
        Real value = diagonal[row] * x[row];

        if (row > 0) {
            value += lower[row - 1] * x[row - 1];
        }

        if (row + 1 < system.size()) {
            value += upper[row] * x[row + 1];
        }

        y[row] = value;
    }
}

void multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
) {
    require(
        x.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );
    require(
        y.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );
    require(
        std::addressof(x) != std::addressof(y),
        error_catalog::kInvalidArgument,
        TridiagonalSystem1D::id()
    );

    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();

    for (Size row = 0; row < system.size(); ++row) {
        Real value = diagonal[row] * x[row];

        if (row > 0) {
            value += upper[row - 1] * x[row - 1];
        }

        if (row + 1 < system.size()) {
            value += lower[row] * x[row + 1];
        }

        y[row] = value;
    }
}

DenseVector multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x
) {
    DenseVector y{system.size()};
    multiply(system, x, y);
    return y;
}

DenseVector multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x
) {
    DenseVector y{system.size()};
    multiply_transpose(system, x, y);
    return y;
}

DenseVector operator*(
    const TridiagonalSystem1D& system,
    const DenseVector& x
) {
    return multiply(system, x);
}

}  // namespace fvm
