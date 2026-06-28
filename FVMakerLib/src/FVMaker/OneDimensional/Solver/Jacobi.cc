// ============================================================================
// Arquivo: Jacobi.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Jacobi no contexto de OneDimensional / Solver.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <cmath>
#include <utility>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
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

}  // namespace

void Jacobi::sweep(
    const TridiagonalSystem1D& system,
    DenseVector& solution,
    DenseVector& workspace
) {
    const Size n = system.size();
    require(
        solution.size() == n && workspace.size() == n,
        error_catalog::kInvalidSystemSize,
        Jacobi::id()
    );
    const auto lower = system.lower();
    const auto diagonal = system.diagonal();
    const auto upper = system.upper();
    const auto rhs = system.rhs().values();

    for (Size row = 0; row < n; ++row) {
        require(
            std::abs(diagonal[row]) > Real{},
            error_catalog::kSingularSystem,
            Jacobi::id()
        );
        Real sum = rhs[row];
        if (row > 0) {
            sum -= lower[row - 1] * solution[row - 1];
        }
        if (row + 1 < n) {
            sum -= upper[row] * solution[row + 1];
        }
        workspace[row] = sum / diagonal[row];
    }

    using std::swap;
    swap(solution, workspace);
}

SolveResult Jacobi::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, Jacobi::id());

    const Size n = system.size();
    const auto diagonal = system.diagonal();

    for (Size row = 0; row < n; ++row) {
        require(
            std::abs(diagonal[row]) > Real{},
            error_catalog::kSingularSystem,
            Jacobi::id()
        );
    }

    DenseVector current{n};
    DenseVector next{n};
    DenseVector residual = algebraic_residual(system, current);
    Real residual_norm = norm_infinity(residual);
    const Real initial_residual_norm = residual_norm;
    const DenseVector initial_residual = residual;
    const StopCriteria criteria = options.stop_criteria.empty()
        ? StopCriteria::residual_absolute(options.tolerance)
        : options.stop_criteria;

    if (residual_norm <= options.tolerance) {
        return SolveResult{
            .solution = std::move(current),
            .residual = std::move(residual),
            .converged = true,
            .iterations = 0,
            .residual_norm = residual_norm,
            .initial_residual_norm = initial_residual_norm,
        };
    }

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        const DenseVector previous = current;

        sweep(system, current, next);
        residual = algebraic_residual(system, current);
        residual_norm = norm_infinity(residual);
        const DenseVector correction = difference(current, previous);
        const StopCriteriaEvaluation stop = criteria.evaluate(
            StopCriteriaState{
                .iteration = iteration,
                .max_iterations = options.max_iterations,
                .solution = &current,
                .correction = &correction,
                .residual = &residual,
                .initial_residual = &initial_residual,
                .initial_residual_norm = initial_residual_norm
            }
        );

        if (stop.converged) {
            SolveResult result{
                .solution = std::move(current),
                .residual = std::move(residual),
                .converged = true,
                .iterations = iteration,
                .residual_norm = residual_norm,
                .initial_residual_norm = initial_residual_norm,
            };
            apply_stop_evaluation(result, stop);
            return result;
        }
    }

    SolveResult result{
        .solution = std::move(current),
        .residual = std::move(residual),
        .converged = false,
        .iterations = options.max_iterations,
        .residual_norm = residual_norm,
        .initial_residual_norm = initial_residual_norm,
    };
    result.reached_iteration_limit = true;
    result.stop_criterion = StopCriterionKind::max_iterations;
    result.stop_value = static_cast<Real>(options.max_iterations);
    result.stop_tolerance = static_cast<Real>(options.max_iterations);
    return result;
}

}  // namespace fvm
