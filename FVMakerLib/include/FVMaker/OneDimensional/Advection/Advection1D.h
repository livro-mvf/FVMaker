// ----------------------------------------------------------------------------
// File: Advection1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a provisional 1D advection helper.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Advection/Interpolation1D.h>

namespace fvm {

class Advection1D final {
public:
    explicit Advection1D(FaceFlux1D face_flux);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "Advection1D", "fvm.1d.advection.Advection1D"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const FaceFlux1D& face_flux() const noexcept;

private:
    FaceFlux1D face_flux_;
};

template <class Scheme>
[[nodiscard]] DenseVector advective_face_values_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    const Advection1D& advection,
    Scheme scheme
) {
    return interpolate_faces_1d(
        grid,
        cell_values,
        advection.face_flux(),
        scheme
    );
}

}  // namespace fvm
