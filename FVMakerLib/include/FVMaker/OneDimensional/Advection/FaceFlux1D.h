// ----------------------------------------------------------------------------
// File: FaceFlux1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines preliminary 1D face/mass flux containers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <span>
#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class FaceFlux1D final {
public:
    explicit FaceFlux1D(DenseVector values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "FaceFlux1D", "fvm.1d.advection.FaceFlux1D"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;
    [[nodiscard]] Real value(Size face) const noexcept;

private:
    DenseVector values_;

    void validate() const;
};

using MassFlux1D = FaceFlux1D;

[[nodiscard]] FaceFlux1D uniform_face_flux_1d(
    const GridView1D& grid,
    Real value
);

[[nodiscard]] FaceFlux1D vector_face_flux_1d(
    const GridView1D& grid,
    DenseVector values
);

[[nodiscard]] Real face_peclet_1d(Real mass_flux, Real diffusion);

}  // namespace fvm
