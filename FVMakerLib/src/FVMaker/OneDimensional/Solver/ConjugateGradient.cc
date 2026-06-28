// ============================================================================
// Arquivo: ConjugateGradient.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Conjugate Gradient no contexto de OneDimensional / Solver.
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
    DenseVector ad{n};

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
        multiply(system, direction, ad);
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
