// ----------------------------------------------------------------------------
// File: NonlinearCoefficient1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a generic 1D coefficient Gamma(phi, x).
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Coefficient/NonlinearCoefficient1D.h>

namespace fvm {

NonlinearCoefficient1D::NonlinearCoefficient1D(Function function)
    : function_(std::move(function)) {
    require(
        static_cast<bool>(function_),
        error_catalog::kInvalidCoefficient,
        NonlinearCoefficient1D::id()
    );
}

DenseVector NonlinearCoefficient1D::cell_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    require(
        phi.size() == grid.num_volumes(),
        error_catalog::kInvalidCoefficient,
        NonlinearCoefficient1D::id()
    );

    DenseVector values{grid.num_volumes()};

    for (Size cell = 0; cell < grid.num_volumes(); ++cell) {
        values[cell] = function_(phi[cell], grid.centers()[cell]);

        require(
            std::isfinite(values[cell]),
            error_catalog::kInvalidCoefficient,
            NonlinearCoefficient1D::id()
        );
    }

    return values;
}

DiffusionCoefficient1D NonlinearCoefficient1D::arithmetic_face_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    return arithmetic_field_coefficient_1d(grid, cell_values(grid, phi));
}

DiffusionCoefficient1D NonlinearCoefficient1D::harmonic_face_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    return harmonic_field_coefficient_1d(grid, cell_values(grid, phi));
}

}  // namespace fvm
