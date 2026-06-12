// ----------------------------------------------------------------------------
// File: GaussSeidel.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the Gauss-Seidel solver for 1D tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

namespace {

void validate_options(IterativeSolverOptions options, ID source) {
    require(
        std::isfinite(options.tolerance) && options.tolerance > Real{},
        error_catalog::kInvalidTolerance,
        source
    );
    require(
        options.max_iterations > 0,
        error_catalog::kInvalidArgument,
        source
    );
}

}  // namespace

SolveResult GaussSeidel::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, GaussSeidel::id());

    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    for (Size row = 0; row < n; ++row) {
        require(
            std::abs(diagonal[row]) > Real{},
            error_catalog::kSingularSystem,
            GaussSeidel::id()
        );
    }

    DenseVector solution{n};
    DenseVector residual = algebraic_residual(system, solution);
    Real residual_norm = norm_infinity(residual);

    if (residual_norm <= options.tolerance) {
        return SolveResult{
            .solution = std::move(solution),
            .residual = std::move(residual),
            .converged = true,
            .iterations = 0,
            .residual_norm = residual_norm,
        };
    }

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        for (Size row = 0; row < n; ++row) {
            Real sum = rhs[row];

            if (row > 0) {
                sum -= lower[row - 1] * solution[row - 1];
            }

            if (row + 1 < n) {
                sum -= upper[row] * solution[row + 1];
            }

            solution[row] = sum / diagonal[row];
        }

        residual = algebraic_residual(system, solution);
        residual_norm = norm_infinity(residual);

        if (residual_norm <= options.tolerance) {
            return SolveResult{
                .solution = std::move(solution),
                .residual = std::move(residual),
                .converged = true,
                .iterations = iteration,
                .residual_norm = residual_norm,
            };
        }
    }

    return SolveResult{
        .solution = std::move(solution),
        .residual = std::move(residual),
        .converged = false,
        .iterations = options.max_iterations,
        .residual_norm = residual_norm,
    };
}

}  // namespace fvm
