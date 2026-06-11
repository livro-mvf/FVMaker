// ----------------------------------------------------------------------------
// File: FieldView1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a non-owning mutable view over 1D field data.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Field/FieldView1D.h>

namespace fvm {

FieldView1D::FieldView1D(
    GridView1D grid,
    std::string_view name,
    std::span<Real> values
)
    : grid_(grid),
      name_(name),
      values_(values) {
    validate();
}

const GridView1D& FieldView1D::grid() const noexcept {
    return grid_;
}

std::string_view FieldView1D::name() const noexcept {
    return name_;
}

Size FieldView1D::size() const noexcept {
    return values_.size();
}

bool FieldView1D::empty() const noexcept {
    return values_.empty();
}

std::span<Real> FieldView1D::values() noexcept {
    return values_;
}

std::span<const Real> FieldView1D::values() const noexcept {
    return values_;
}

Real& FieldView1D::operator[](Size index) noexcept {
    return values_[index];
}

const Real& FieldView1D::operator[](Size index) const noexcept {
    return values_[index];
}

void FieldView1D::validate() const {
    require(
        values_.size() == grid_.num_volumes(),
        error_catalog::kInvalidFieldSize,
        FieldView1D::id()
    );
}

}  // namespace fvm
