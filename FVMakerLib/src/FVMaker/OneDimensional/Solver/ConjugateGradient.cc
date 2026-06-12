// ----------------------------------------------------------------------------
// File: ConjugateGradient.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the Conjugate Gradient solver for 1D systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
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

[[nodiscard]] Real dot(const DenseVector& a, const DenseVector& b) {
    require(
        a.size() == b.size(),
        error_catalog::kInvalidSystemSize,
        ConjugateGradient::id()
    );

    Real value{};

    for (Size i = 0; i < a.size(); ++i) {
        value += a[i] * b[i];
    }

    return value;
}

[[nodiscard]] DenseVector multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x
) {
    require(
        x.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        ConjugateGradient::id()
    );

    DenseVector ax{system.size()};
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();

    for (Size row = 0; row < system.size(); ++row) {
        ax[row] = diagonal[row] * x[row];

        if (row > 0) {
            ax[row] += lower[row - 1] * x[row - 1];
        }

        if (row + 1 < system.size()) {
            ax[row] += upper[row] * x[row + 1];
        }
    }

    return ax;
}

}  // namespace

SolveResult ConjugateGradient::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, ConjugateGradient::id());

    const Size n = system.size();
    DenseVector solution{n};
    DenseVector residual{n};
    DenseVector direction{n};

    for (Size i = 0; i < n; ++i) {
        residual[i] = system.rhs()[i];
        direction[i] = residual[i];
    }

    Real residual_squared = dot(residual, residual);
    Real residual_norm = std::sqrt(residual_squared);

    if (residual_norm <= options.tolerance) {
        DenseVector algebraic = algebraic_residual(system, solution);
        return SolveResult{
            .solution = std::move(solution),
            .residual = std::move(algebraic),
            .converged = true,
            .iterations = 0,
            .residual_norm = residual_norm,
        };
    }

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        const DenseVector ad = multiply(system, direction);
        const Real denominator = dot(direction, ad);

        require(
            std::abs(denominator) > Real{},
            error_catalog::kSingularSystem,
            ConjugateGradient::id()
        );

        const Real alpha = residual_squared / denominator;

        for (Size i = 0; i < n; ++i) {
            solution[i] += alpha * direction[i];
            residual[i] -= alpha * ad[i];
        }

        const Real next_residual_squared = dot(residual, residual);
        residual_norm = std::sqrt(next_residual_squared);

        if (residual_norm <= options.tolerance) {
            DenseVector algebraic = algebraic_residual(system, solution);
            const Real algebraic_norm = norm_infinity(algebraic);

            return SolveResult{
                .solution = std::move(solution),
                .residual = std::move(algebraic),
                .converged = true,
                .iterations = iteration,
                .residual_norm = algebraic_norm,
            };
        }

        const Real beta = next_residual_squared / residual_squared;

        for (Size i = 0; i < n; ++i) {
            direction[i] = residual[i] + beta * direction[i];
        }

        residual_squared = next_residual_squared;
    }

    DenseVector algebraic = algebraic_residual(system, solution);
    const Real algebraic_norm = norm_infinity(algebraic);

    return SolveResult{
        .solution = std::move(solution),
        .residual = std::move(algebraic),
        .converged = false,
        .iterations = options.max_iterations,
        .residual_norm = algebraic_norm,
    };
}

}  // namespace fvm
