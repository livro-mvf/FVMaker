// ----------------------------------------------------------------------------
// File: Interpolation1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines provisional 1D face interpolation schemes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <cmath>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

struct Interpolation1D final {
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "Interpolation1D", "fvm.1d.advection.Interpolation1D"};
    }
};

struct Upwind1D final {};
struct CentralDifference1D final {};
struct PowerLaw1D final {};
struct Exponential1D final {};
struct QUICK1D final {};

[[nodiscard]] inline Real central_face_value_1d(Real west, Real east) {
    return Real{0.5} * (west + east);
}

[[nodiscard]] inline Real upwind_face_value_1d(
    Real west,
    Real east,
    Real face_flux
) {
    return face_flux >= Real{} ? west : east;
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    CentralDifference1D,
    Real west,
    Real east,
    Real
) {
    return central_face_value_1d(west, east);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    Upwind1D,
    Real west,
    Real east,
    Real face_flux
) {
    return upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    PowerLaw1D,
    Real west,
    Real east,
    Real face_flux
) {
    return std::abs(face_flux) < Real{1.0}
        ? central_face_value_1d(west, east)
        : upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    Exponential1D,
    Real west,
    Real east,
    Real face_flux
) {
    return std::abs(face_flux) < Real{1.0}
        ? central_face_value_1d(west, east)
        : upwind_face_value_1d(west, east, face_flux);
}

[[nodiscard]] inline Real interpolate_internal_face_value_1d(
    QUICK1D,
    Real west,
    Real east,
    Real face_flux
) {
    return upwind_face_value_1d(west, east, face_flux);
}

template <class Scheme>
[[nodiscard]] DenseVector interpolate_faces_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    const FaceFlux1D& face_flux,
    Scheme scheme
) {
    require(
        cell_values.size() == grid.num_volumes(),
        error_catalog::kInvalidFieldSize,
        Interpolation1D::id()
    );

    require(
        face_flux.size() == grid.num_faces(),
        error_catalog::kInvalidArgument,
        Interpolation1D::id()
    );

    DenseVector face_values{grid.num_faces()};
    face_values[0] = cell_values[0];
    face_values[grid.num_faces() - 1] = cell_values[grid.num_volumes() - 1];

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        face_values[face] = interpolate_internal_face_value_1d(
            scheme,
            cell_values[face - 1],
            cell_values[face],
            face_flux.value(face)
        );
    }

    return face_values;
}

}  // namespace fvm
