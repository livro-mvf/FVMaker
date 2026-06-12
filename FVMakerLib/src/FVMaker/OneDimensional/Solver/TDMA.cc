// ----------------------------------------------------------------------------
// File: TDMA.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the TDMA solver for 1D tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <cmath>
#include <utility>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

namespace {

[[nodiscard]] bool is_zero_pivot(Real value) noexcept {
    return std::abs(value) <= Real{};
}

}  // namespace

SolveResult TDMA::solve(const TridiagonalSystem1D& system) {
    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    std::vector<Real> modified_upper(n > 1 ? n - 1 : 0);
    std::vector<Real> modified_rhs(n);

    Real pivot = diagonal[0];
    require(!is_zero_pivot(pivot), error_catalog::kSingularSystem, TDMA::id());

    if (n > 1) {
        modified_upper[0] = upper[0] / pivot;
    }
    modified_rhs[0] = rhs[0] / pivot;

    for (Size row = 1; row < n; ++row) {
        pivot = diagonal[row] - lower[row - 1] * modified_upper[row - 1];

        require(
            !is_zero_pivot(pivot),
            error_catalog::kSingularSystem,
            TDMA::id()
        );

        if (row + 1 < n) {
            modified_upper[row] = upper[row] / pivot;
        }

        modified_rhs[row] =
            (rhs[row] - lower[row - 1] * modified_rhs[row - 1]) / pivot;
    }

    DenseVector solution{n};
    solution[n - 1] = modified_rhs[n - 1];

    for (Size reverse_index = n - 1; reverse_index > 0; --reverse_index) {
        const Size row = reverse_index - 1;
        solution[row] =
            modified_rhs[row] - modified_upper[row] * solution[row + 1];
    }

    DenseVector residual = algebraic_residual(system, solution);
    const Real residual_norm = norm_infinity(residual);

    return SolveResult{
        .solution = std::move(solution),
        .residual = std::move(residual),
        .converged = true,
        .iterations = 1,
        .residual_norm = residual_norm,
    };
}

}  // namespace fvm
