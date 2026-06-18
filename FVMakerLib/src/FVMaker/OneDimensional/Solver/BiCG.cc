// ----------------------------------------------------------------------------
// File: BiCG.cc
// Project: FVMaker
// Description: Implements the BiConjugate Gradient solver for 1D systems.
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

namespace {

void validate_options(IterativeSolverOptions options, ID source) {
    require(
        std::isfinite(options.tolerance) && options.tolerance > Real{},
        error_catalog::kInvalidTolerance,
        source
    );
    require(options.max_iterations > 0, error_catalog::kInvalidArgument, source);
}

[[nodiscard]] Real dot(const DenseVector& a, const DenseVector& b) {
    require(a.size() == b.size(), error_catalog::kInvalidSystemSize, BiCG::id());

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
        BiCG::id()
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

[[nodiscard]] DenseVector multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x
) {
    require(
        x.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        BiCG::id()
    );

    DenseVector atx{system.size()};
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();

    for (Size row = 0; row < system.size(); ++row) {
        atx[row] = diagonal[row] * x[row];

        if (row > 0) {
            atx[row] += upper[row - 1] * x[row - 1];
        }

        if (row + 1 < system.size()) {
            atx[row] += lower[row] * x[row + 1];
        }
    }

    return atx;
}

[[nodiscard]] DenseVector residual_b_minus_ax(
    const TridiagonalSystem1D& system,
    const DenseVector& solution
) {
    DenseVector residual = algebraic_residual(system, solution);

    for (Size i = 0; i < residual.size(); ++i) {
        residual[i] = -residual[i];
    }

    return residual;
}

[[nodiscard]] SolveResult make_result(
    const TridiagonalSystem1D& system,
    DenseVector solution,
    bool converged,
    Size iterations
) {
    DenseVector algebraic = algebraic_residual(system, solution);
    const Real algebraic_norm = norm_infinity(algebraic);

    return SolveResult{
        .solution = std::move(solution),
        .residual = std::move(algebraic),
        .converged = converged,
        .iterations = iterations,
        .residual_norm = algebraic_norm,
    };
}

}  // namespace

SolveResult BiCG::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, BiCG::id());

    const Size n = system.size();
    DenseVector solution{n};
    DenseVector residual = residual_b_minus_ax(system, solution);
    DenseVector shadow = residual;
    DenseVector direction = residual;
    DenseVector shadow_direction = shadow;

    Real rho = dot(shadow, residual);
    require(std::abs(rho) > Real{}, error_catalog::kSingularSystem, BiCG::id());

    if (norm_infinity(residual) <= options.tolerance) {
        return make_result(system, std::move(solution), true, 0);
    }

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        const DenseVector ad = multiply(system, direction);
        const DenseVector at_shadow = multiply_transpose(system, shadow_direction);
        const Real denominator = dot(shadow_direction, ad);

        require(
            std::abs(denominator) > Real{},
            error_catalog::kSingularSystem,
            BiCG::id()
        );

        const Real alpha = rho / denominator;

        for (Size i = 0; i < n; ++i) {
            solution[i] += alpha * direction[i];
            residual[i] -= alpha * ad[i];
            shadow[i] -= alpha * at_shadow[i];
        }

        if (norm_infinity(residual) <= options.tolerance) {
            return make_result(system, std::move(solution), true, iteration);
        }

        const Real next_rho = dot(shadow, residual);
        require(
            std::abs(next_rho) > Real{},
            error_catalog::kSingularSystem,
            BiCG::id()
        );

        const Real beta = next_rho / rho;

        for (Size i = 0; i < n; ++i) {
            direction[i] = residual[i] + beta * direction[i];
            shadow_direction[i] = shadow[i] + beta * shadow_direction[i];
        }

        rho = next_rho;
    }

    return make_result(system, std::move(solution), false, options.max_iterations);
}

}  // namespace fvm
