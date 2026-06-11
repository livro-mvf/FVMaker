// ----------------------------------------------------------------------------
// File: GridView1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a non-owning view over an FVGridMaker Axis1D.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

GridView1D::GridView1D(const fvgrid::Axis1D& axis) : axis_(&axis) {
    validate();
}

GridView1D::GridView1D(const fvgrid::Axis1D* axis) : axis_(axis) {
    validate();
}

Size GridView1D::num_volumes() const noexcept {
    return static_cast<Size>(axis_->num_cells());
}

Size GridView1D::num_cells() const noexcept {
    return num_volumes();
}

Size GridView1D::num_faces() const noexcept {
    return static_cast<Size>(axis_->num_faces());
}

std::span<const Real> GridView1D::faces() const noexcept {
    return axis_->faces();
}

std::span<const Real> GridView1D::centers() const noexcept {
    return axis_->centers();
}

std::span<const Real> GridView1D::dx_faces() const noexcept {
    return axis_->dx_faces();
}

std::span<const Real> GridView1D::dx_centers() const noexcept {
    return axis_->dx_centers();
}

std::span<const Real> GridView1D::cell_lengths() const noexcept {
    return axis_->cell_lengths();
}

Real GridView1D::xmin() const noexcept {
    return axis_->xmin();
}

Real GridView1D::xmax() const noexcept {
    return axis_->xmax();
}

Real GridView1D::length() const noexcept {
    return axis_->length();
}

std::string_view GridView1D::pattern_name() const noexcept {
    return axis_->pattern_name();
}

GridMetrics1D GridView1D::metrics() const noexcept {
    return GridMetrics1D{
        .num_volumes = num_volumes(),
        .num_faces = num_faces(),
        .faces = faces(),
        .centers = centers(),
        .dx_faces = dx_faces(),
        .dx_centers = dx_centers(),
        .cell_lengths = cell_lengths(),
        .xmin = xmin(),
        .xmax = xmax(),
        .length = length(),
        .pattern_name = pattern_name(),
    };
}

void GridView1D::validate() const {
    require(axis_ != nullptr, error_catalog::kMissingMesh, GridView1D::id());

    require(
        axis_->num_cells() > 0,
        error_catalog::kInvalidMeshDimension,
        GridView1D::id()
    );

    require(
        axis_->num_faces() == axis_->num_cells() + 1,
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->faces().size() == axis_->num_faces(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->centers().size() == axis_->num_cells(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->dx_faces().size() == axis_->num_cells(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->dx_centers().size() == axis_->num_faces(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );
}

}  // namespace fvm
