// ----------------------------------------------------------------------------
// File: FaceFlux1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements preliminary 1D face/mass flux containers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>

namespace fvm {

FaceFlux1D::FaceFlux1D(DenseVector values) : values_(std::move(values)) {
    validate();
}

Size FaceFlux1D::size() const noexcept {
    return values_.size();
}

std::span<const Real> FaceFlux1D::values() const noexcept {
    return values_.values();
}

Real FaceFlux1D::value(Size face) const noexcept {
    return values_[face];
}

void FaceFlux1D::validate() const {
    require(!values_.empty(), error_catalog::kInvalidArgument, FaceFlux1D::id());

    for (const Real value : values_.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidArgument,
            FaceFlux1D::id()
        );
    }
}

FaceFlux1D uniform_face_flux_1d(const GridView1D& grid, Real value) {
    return FaceFlux1D{DenseVector{grid.num_faces(), value}};
}

FaceFlux1D vector_face_flux_1d(
    const GridView1D& grid,
    DenseVector values
) {
    require(
        values.size() == grid.num_faces(),
        error_catalog::kInvalidArgument,
        FaceFlux1D::id()
    );

    return FaceFlux1D{std::move(values)};
}

Real face_peclet_1d(Real mass_flux, Real diffusion) {
    require(
        std::isfinite(mass_flux) && std::isfinite(diffusion)
            && std::abs(diffusion) > Real{},
        error_catalog::kInvalidArgument,
        FaceFlux1D::id()
    );

    return mass_flux / diffusion;
}

}  // namespace fvm
