// ----------------------------------------------------------------------------
// File: Field1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements scalar fields stored on 1D control volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Field/Field1D.h>

namespace fvm {

Field1D::Field1D(GridView1D grid, std::string name)
    : Field1D(grid, std::move(name), Real{}, false) {}

Field1D::Field1D(
    GridView1D grid,
    std::string name,
    Real uniform_value,
    bool keep_history
)
    : grid_(grid),
      name_(std::move(name)),
      values_(grid_.num_volumes(), uniform_value),
      keep_history_(keep_history) {
    validate_size();
    record_initial_state();
}

Field1D::Field1D(
    GridView1D grid,
    std::string name,
    std::vector<Real> values,
    bool keep_history
)
    : grid_(grid),
      name_(std::move(name)),
      values_(std::move(values)),
      keep_history_(keep_history) {
    validate_size();
    record_initial_state();
}

const GridView1D& Field1D::grid() const noexcept {
    return grid_;
}

std::string_view Field1D::name() const noexcept {
    return name_;
}

void Field1D::rename(std::string name) {
    name_ = std::move(name);
}

Size Field1D::size() const noexcept {
    return values_.size();
}

bool Field1D::empty() const noexcept {
    return values_.empty();
}

std::span<Real> Field1D::values() noexcept {
    return values_;
}

std::span<const Real> Field1D::values() const noexcept {
    return values_;
}

Real& Field1D::operator[](Size index) noexcept {
    return values_[index];
}

const Real& Field1D::operator[](Size index) const noexcept {
    return values_[index];
}

void Field1D::fill(Real value) {
    std::ranges::fill(values_, value);
}

void Field1D::assign_values(std::vector<Real> values) {
    values_ = std::move(values);
    validate_size();
}

FieldView1D Field1D::view() noexcept {
    return FieldView1D{grid_, name_, values_};
}

bool Field1D::history_enabled() const noexcept {
    return keep_history_;
}

Size Field1D::history_size() const noexcept {
    return history_.size();
}

std::span<const Real> Field1D::history_step(Size index) const {
    require(
        index < history_.size(),
        error_catalog::kOutOfRange,
        Field1D::id()
    );

    return history_[index];
}

void Field1D::save_state() {
    if (keep_history_) {
        history_.push_back(values_);
    }
}

void Field1D::clear_history() {
    history_.clear();
}

void Field1D::validate_size() const {
    require(
        values_.size() == grid_.num_volumes(),
        error_catalog::kInvalidFieldSize,
        Field1D::id()
    );
}

void Field1D::record_initial_state() {
    if (keep_history_) {
        history_.push_back(values_);
    }
}

}  // namespace fvm
