// ----------------------------------------------------------------------------
// File: DiffusionCoefficient1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements 1D diffusion coefficients stored on faces.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>

namespace fvm {

DiffusionCoefficient1D::DiffusionCoefficient1D(DenseVector face_values)
    : face_values_(std::move(face_values)) {
    validate();
}

Size DiffusionCoefficient1D::num_faces() const noexcept {
    return face_values_.size();
}

std::span<const Real> DiffusionCoefficient1D::face_values() const noexcept {
    return face_values_.values();
}

Real DiffusionCoefficient1D::face_value(Size face) const noexcept {
    return face_values_[face];
}

void DiffusionCoefficient1D::validate() const {
    require(
        !face_values_.empty(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    for (const Real value : face_values_.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidCoefficient,
            DiffusionCoefficient1D::id()
        );
    }
}

DiffusionCoefficient1D uniform_coefficient_1d(
    const GridView1D& grid,
    Real coefficient
) {
    return DiffusionCoefficient1D{DenseVector{grid.num_faces(), coefficient}};
}

DiffusionCoefficient1D face_coefficient_1d(
    const GridView1D& grid,
    DenseVector face_values
) {
    require(
        face_values.size() == grid.num_faces(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    return DiffusionCoefficient1D{std::move(face_values)};
}

DiffusionCoefficient1D arithmetic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    return interpolated_field_coefficient_1d(
        grid, cell_values, ArithmeticFaceInterpolation1D{}
    );
}

DiffusionCoefficient1D harmonic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    for (const Real value : cell_values.values()) {
        require(
            value > Real{},
            error_catalog::kInvalidCoefficient,
            DiffusionCoefficient1D::id()
        );
    }
    return interpolated_field_coefficient_1d(
        grid, cell_values, HarmonicFaceInterpolation1D{}
    );
}

DiffusionCoefficient1D field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    return arithmetic_field_coefficient_1d(grid, cell_values);
}

}  // namespace fvm
