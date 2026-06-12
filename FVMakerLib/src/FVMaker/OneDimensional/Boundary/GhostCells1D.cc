// ----------------------------------------------------------------------------
// File: GhostCells1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the ghost-cell layout for 1D finite-volume problems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>

namespace fvm {

GhostCells1D::GhostCells1D(Size left, Size right)
    : left_(left), right_(right) {
    validate();
}

Size GhostCells1D::left() const noexcept {
    return left_;
}

Size GhostCells1D::right() const noexcept {
    return right_;
}

Size GhostCells1D::total() const noexcept {
    return left_ + right_;
}

bool GhostCells1D::empty() const noexcept {
    return total() == 0;
}

Size GhostCells1D::first_physical_index() const noexcept {
    return left_;
}

Size GhostCells1D::storage_size(Size physical_size) const noexcept {
    return physical_size + total();
}

Size GhostCells1D::storage_index(Size physical_index) const noexcept {
    return left_ + physical_index;
}

void GhostCells1D::validate() const {
    require(
        left_ <= max_per_side(),
        error_catalog::kInvalidBoundaryCondition,
        GhostCells1D::id()
    );

    require(
        right_ <= max_per_side(),
        error_catalog::kInvalidBoundaryCondition,
        GhostCells1D::id()
    );
}

}  // namespace fvm
