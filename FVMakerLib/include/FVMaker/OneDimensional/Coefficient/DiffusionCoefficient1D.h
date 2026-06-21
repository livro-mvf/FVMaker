// ----------------------------------------------------------------------------
// File: DiffusionCoefficient1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines 1D diffusion coefficients stored on control-volume faces.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <span>
#include <string_view>
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {
struct FaceInterpolationData1D final {
    Real west_value{};
    Real east_value{};
    Real west_distance{};
    Real east_distance{};
};

struct ArithmeticFaceInterpolation1D final {
    [[nodiscard]] Real operator()(FaceInterpolationData1D data) const noexcept {
        const Real distance = data.west_distance + data.east_distance;
        return (data.east_distance * data.west_value
              + data.west_distance * data.east_value) / distance;
    }
};

struct HarmonicFaceInterpolation1D final {
    [[nodiscard]] Real operator()(FaceInterpolationData1D data) const noexcept {
        const Real distance = data.west_distance + data.east_distance;
        return distance / (data.west_distance / data.west_value
                         + data.east_distance / data.east_value);
    }
};

template <class Interpolator>
concept FaceInterpolator1D = requires(
    const Interpolator& interpolation,
    FaceInterpolationData1D data
) {
    { interpolation(data) } -> std::convertible_to<Real>;
};

class DiffusionCoefficient1D final {
public:
    explicit DiffusionCoefficient1D(DenseVector face_values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "DiffusionCoefficient1D",
            "fvm.1d.coefficient.DiffusionCoefficient1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_faces() const noexcept;
    [[nodiscard]] std::span<const Real> face_values() const noexcept;
    [[nodiscard]] Real face_value(Size face) const noexcept;

private:
    DenseVector face_values_;

    void validate() const;
};

[[nodiscard]] DiffusionCoefficient1D uniform_coefficient_1d(
    const GridView1D& grid,
    Real coefficient
);

template <ScalarFunction1D Function>
[[nodiscard]] DiffusionCoefficient1D function_coefficient_1d(
    const GridView1D& grid,
    Function function
) {
    DenseVector face_values{grid.num_faces()};

    for (Size face = 0; face < grid.num_faces(); ++face) {
        face_values[face] = function(grid.faces()[face]);
    }

    return DiffusionCoefficient1D{std::move(face_values)};
}

[[nodiscard]] DiffusionCoefficient1D face_coefficient_1d(
    const GridView1D& grid,
    DenseVector face_values
);

template <FaceInterpolator1D Interpolator>
[[nodiscard]] DiffusionCoefficient1D interpolated_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    Interpolator interpolation
) {
    require(
        cell_values.size() == grid.num_volumes(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    DenseVector face_values{grid.num_faces()};
    face_values[0] = cell_values[0];
    face_values[grid.num_faces() - 1] = cell_values[grid.num_volumes() - 1];

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        const Size west = face - 1;
        const Size east = face;
        const FaceInterpolationData1D data{
            .west_value = cell_values[west],
            .east_value = cell_values[east],
            .west_distance = grid.faces()[face] - grid.centers()[west],
            .east_distance = grid.centers()[east] - grid.faces()[face]
        };
        face_values[face] = static_cast<Real>(interpolation(data));
    }

    return DiffusionCoefficient1D{std::move(face_values)};
}

[[nodiscard]] DiffusionCoefficient1D arithmetic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

[[nodiscard]] DiffusionCoefficient1D harmonic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

[[nodiscard]] DiffusionCoefficient1D field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

}  // namespace fvm
