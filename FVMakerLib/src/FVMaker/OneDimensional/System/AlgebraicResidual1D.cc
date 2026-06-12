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

    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    DenseVector residual{n};

    for (Size row = 0; row < n; ++row) {
        Real ax = diagonal[row] * solution[row];

        if (row > 0) {
            ax += lower[row - 1] * solution[row - 1];
        }

        if (row + 1 < n) {
            ax += upper[row] * solution[row + 1];
        }

        residual[row] = ax - rhs[row];
    }

    return residual;
}

}  // namespace fvm
