// ----------------------------------------------------------------------------
// File: PentadiagonalSystem1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a pentadiagonal linear system for 1D finite volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>

namespace fvm {

namespace {

[[nodiscard]] Size first_band_size(Size system_size) noexcept {
    return system_size > 0 ? system_size - 1 : 0;
}

[[nodiscard]] Size second_band_size(Size system_size) noexcept {
    return system_size > 1 ? system_size - 2 : 0;
}

}  // namespace

PentadiagonalSystem1D::PentadiagonalSystem1D(Size size)
    : lower_second_(second_band_size(size)),
      lower_first_(first_band_size(size)),
      diagonal_(size),
      upper_first_(first_band_size(size)),
      upper_second_(second_band_size(size)),
      rhs_(size) {
    validate();
}

PentadiagonalSystem1D::PentadiagonalSystem1D(
    std::vector<Real> lower_second,
    std::vector<Real> lower_first,
    std::vector<Real> diagonal,
    std::vector<Real> upper_first,
    std::vector<Real> upper_second,
    DenseVector rhs
)
    : lower_second_(std::move(lower_second)),
      lower_first_(std::move(lower_first)),
      diagonal_(std::move(diagonal)),
      upper_first_(std::move(upper_first)),
      upper_second_(std::move(upper_second)),
      rhs_(std::move(rhs)) {
    validate();
}

Size PentadiagonalSystem1D::size() const noexcept {
    return diagonal_.size();
}

std::span<Real> PentadiagonalSystem1D::lower_second() noexcept {
    return lower_second_;
}

std::span<const Real> PentadiagonalSystem1D::lower_second() const noexcept {
    return lower_second_;
}

std::span<Real> PentadiagonalSystem1D::lower_first() noexcept {
    return lower_first_;
}

std::span<const Real> PentadiagonalSystem1D::lower_first() const noexcept {
    return lower_first_;
}

std::span<Real> PentadiagonalSystem1D::diagonal() noexcept {
    return diagonal_;
}

std::span<const Real> PentadiagonalSystem1D::diagonal() const noexcept {
    return diagonal_;
}

std::span<Real> PentadiagonalSystem1D::upper_first() noexcept {
    return upper_first_;
}

std::span<const Real> PentadiagonalSystem1D::upper_first() const noexcept {
    return upper_first_;
}

std::span<Real> PentadiagonalSystem1D::upper_second() noexcept {
    return upper_second_;
}

std::span<const Real> PentadiagonalSystem1D::upper_second() const noexcept {
    return upper_second_;
}

DenseVector& PentadiagonalSystem1D::rhs() noexcept {
    return rhs_;
}

const DenseVector& PentadiagonalSystem1D::rhs() const noexcept {
    return rhs_;
}

void PentadiagonalSystem1D::set_row(
    Size row,
    Real lower_second,
    Real lower_first,
    Real diagonal,
    Real upper_first,
    Real upper_second,
    Real rhs
) {
    require(row < size(), error_catalog::kOutOfRange, PentadiagonalSystem1D::id());

    diagonal_[row] = diagonal;
    rhs_[row] = rhs;

    if (row > 1) {
        lower_second_[row - 2] = lower_second;
    }

    if (row > 0) {
        lower_first_[row - 1] = lower_first;
    }

    if (row + 1 < size()) {
        upper_first_[row] = upper_first;
    }

    if (row + 2 < size()) {
        upper_second_[row] = upper_second;
    }
}

void PentadiagonalSystem1D::validate() const {
    const Size system_size = diagonal_.size();

    require(
        system_size > 0,
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    require(
        lower_second_.size() == second_band_size(system_size),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    require(
        lower_first_.size() == first_band_size(system_size),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    require(
        upper_first_.size() == first_band_size(system_size),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    require(
        upper_second_.size() == second_band_size(system_size),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    require(
        rhs_.size() == system_size,
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );
}

}  // namespace fvm
