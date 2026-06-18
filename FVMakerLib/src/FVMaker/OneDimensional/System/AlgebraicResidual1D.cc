// ----------------------------------------------------------------------------
// File: AlgebraicResidual1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements A X - B residual evaluation for 1D systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

DenseVector algebraic_residual(
    const TridiagonalSystem1D& system,
    const DenseVector& solution
) {
    require(
        solution.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );

    return system * solution - system.rhs();
}

DenseVector algebraic_residual(
    const PentadiagonalSystem1D& system,
    const DenseVector& solution
) {
    require(
        solution.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    const Size n = system.size();
    const auto lower_second = system.lower_second();
    const auto lower_first = system.lower_first();
    const auto diagonal = system.diagonal();
    const auto upper_first = system.upper_first();
    const auto upper_second = system.upper_second();
    const auto rhs = system.rhs().values();

    DenseVector residual{n};

    for (Size row = 0; row < n; ++row) {
        Real ax = diagonal[row] * solution[row];

        if (row > 1) {
            ax += lower_second[row - 2] * solution[row - 2];
        }

        if (row > 0) {
            ax += lower_first[row - 1] * solution[row - 1];
        }

        if (row + 1 < n) {
            ax += upper_first[row] * solution[row + 1];
        }

        if (row + 2 < n) {
            ax += upper_second[row] * solution[row + 2];
        }

        residual[row] = ax - rhs[row];
    }

    return residual;
}

}  // namespace fvm
