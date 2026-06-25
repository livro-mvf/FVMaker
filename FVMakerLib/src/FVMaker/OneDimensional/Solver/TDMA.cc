// ----------------------------------------------------------------------------
// File: TDMA.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the TDMA solver for 1D tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

namespace {

[[nodiscard]] bool is_zero_pivot(Real value) noexcept {
    return std::abs(value) <= Real{};
}

}  // namespace

TDMAForwardSweep TDMA::forward_sweep(
    const TridiagonalSystem1D& system
) {
    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    TDMAForwardSweep sweep{
        .t = DenseVector{n},
        .q = DenseVector{n}
    };

    Real pivot = diagonal[0];
    require(!is_zero_pivot(pivot), error_catalog::kSingularSystem, TDMA::id());

    if (n > 1) {
        sweep.t[0] = - upper[0] / pivot;
    }

    sweep.q[0] = rhs[0] / pivot;

    for (Size row = 1; row < n; ++row) {
    
        pivot = diagonal[row] + lower[row - 1] * sweep.t[row - 1];

        require(
            !is_zero_pivot(pivot),
            error_catalog::kSingularSystem,
            TDMA::id()
        );

        if (row + 1 < n) {
            sweep.t[row] = -upper[row] / pivot;
        }

        sweep.q[row] =
            (rhs[row] - lower[row - 1] * sweep.q[row - 1]) / pivot;
    }

    return sweep;
}

SolveResult TDMA::solve(const TridiagonalSystem1D& system) {
    TDMAForwardSweep sweep = forward_sweep(system);
    const Size n = system.size();
    DenseVector solution{n};
    solution[n - 1] = sweep.q[n - 1];

    for (Size reverse_index = n - 1; reverse_index > 0; --reverse_index) {
        const Size row = reverse_index - 1;
        solution[row] =
            sweep.t[row] * solution[row + 1] + sweep.q[row];
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
