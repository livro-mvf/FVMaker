// ----------------------------------------------------------------------------
// File: DiffusionCoefficient1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines 1D diffusion coefficients stored on control-volume faces.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <span>
#include <string_view>
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

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
