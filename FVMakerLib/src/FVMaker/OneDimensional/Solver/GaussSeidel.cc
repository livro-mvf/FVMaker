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
#include <FVMaker/Solver/StopCriteria.h>

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

[[nodiscard]] DenseVector difference(
    const DenseVector& a,
    const DenseVector& b
) {
    DenseVector result{a.size()};

    for (Size i = 0; i < a.size(); ++i) {
        result[i] = a[i] - b[i];
    }

    return result;
}

void apply_stop_evaluation(
    SolveResult& result,
    const StopCriteriaEvaluation& evaluation
) {
    result.stop_criterion = evaluation.criterion;
    result.stop_value = evaluation.value;
    result.stop_tolerance = evaluation.tolerance;
    result.reached_iteration_limit = evaluation.reached_iteration_limit;
}

void sweep_forward(
    DenseVector& solution,
    const TridiagonalSystem1D& system
) {
    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

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
}

void sweep_backward(
    DenseVector& solution,
    const TridiagonalSystem1D& system
) {
    const Size n = system.size();
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    for (Size reverse_index = n; reverse_index > 0; --reverse_index) {
        const Size row = reverse_index - 1;
        Real sum = rhs[row];

        if (row > 0) {
            sum -= lower[row - 1] * solution[row - 1];
        }

        if (row + 1 < n) {
            sum -= upper[row] * solution[row + 1];
        }

        solution[row] = sum / diagonal[row];
    }
}

}  // namespace

SolveResult GaussSeidel::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, GaussSeidel::id());

    const Size n = system.size();
    const auto diagonal = system.diagonal();

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
    const Real initial_residual_norm = residual_norm;
    const StopCriteria criteria = options.stop_criteria.empty()
        ? StopCriteria::residual_absolute(options.tolerance)
        : options.stop_criteria;

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
        const DenseVector previous = solution;

        if (options.gauss_seidel_sweep == GaussSeidelSweep::backward) {
            sweep_backward(solution, system);
        } else if (options.gauss_seidel_sweep == GaussSeidelSweep::hybrid
                   && iteration % 2 == 0) {
            sweep_backward(solution, system);
        } else {
            sweep_forward(solution, system);
        }

        residual = algebraic_residual(system, solution);
        residual_norm = norm_infinity(residual);
        const DenseVector correction = difference(solution, previous);
        const StopCriteriaEvaluation stop = criteria.evaluate(
            StopCriteriaState{
                .iteration = iteration,
                .max_iterations = options.max_iterations,
                .solution = &solution,
                .correction = &correction,
                .residual = &residual,
                .initial_residual_norm = initial_residual_norm
            }
        );

        if (stop.converged) {
            SolveResult result{
                .solution = std::move(solution),
                .residual = std::move(residual),
                .converged = true,
                .iterations = iteration,
                .residual_norm = residual_norm,
            };
            apply_stop_evaluation(result, stop);
            return result;
        }
    }

    SolveResult result{
        .solution = std::move(solution),
        .residual = std::move(residual),
        .converged = false,
        .iterations = options.max_iterations,
        .residual_norm = residual_norm,
    };
    result.reached_iteration_limit = true;
    result.stop_criterion = StopCriterionKind::max_iterations;
    result.stop_value = static_cast<Real>(options.max_iterations);
    result.stop_tolerance = static_cast<Real>(options.max_iterations);
    return result;
}

}  // namespace fvm
