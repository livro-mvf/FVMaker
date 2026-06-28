// ============================================================================
// Arquivo: BiCGSTAB.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Bi CGSTAB no contexto de OneDimensional / Solver.
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
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
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
    require(
        a.size() == b.size(),
        error_catalog::kInvalidSystemSize,
        BiCGSTAB::id()
    );

    Real value{};
    for (Size i = 0; i < a.size(); ++i) {
        value += a[i] * b[i];
    }
    return value;
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

SolveResult BiCGSTAB::solve(
    const TridiagonalSystem1D& system,
    IterativeSolverOptions options
) {
    validate_options(options, BiCGSTAB::id());

    const Size n = system.size();
    DenseVector solution{n};
    DenseVector residual = residual_b_minus_ax(system, solution);
    DenseVector shadow = residual;
    DenseVector direction{n};
    DenseVector v{n};
    DenseVector s{n};
    DenseVector t{n};

    Real rho = Real{1};
    Real alpha = Real{1};
    Real omega = Real{1};

    if (norm_infinity(residual) <= options.tolerance) {
        return make_result(system, std::move(solution), true, 0);
    }

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        const Real next_rho = dot(shadow, residual);
        require(
            std::abs(next_rho) > Real{},
            error_catalog::kSingularSystem,
            BiCGSTAB::id()
        );

        const Real beta = (next_rho / rho) * (alpha / omega);

        for (Size i = 0; i < n; ++i) {
            direction[i] =
                residual[i] + beta * (direction[i] - omega * v[i]);
        }

        multiply(system, direction, v);
        const Real alpha_denominator = dot(shadow, v);
        require(
            std::abs(alpha_denominator) > Real{},
            error_catalog::kSingularSystem,
            BiCGSTAB::id()
        );

        alpha = next_rho / alpha_denominator;

        for (Size i = 0; i < n; ++i) {
            s[i] = residual[i] - alpha * v[i];
        }

        if (norm_infinity(s) <= options.tolerance) {
            for (Size i = 0; i < n; ++i) {
                solution[i] += alpha * direction[i];
            }

            return make_result(system, std::move(solution), true, iteration);
        }

        multiply(system, s, t);
        const Real omega_denominator = dot(t, t);
        require(
            std::abs(omega_denominator) > Real{},
            error_catalog::kSingularSystem,
            BiCGSTAB::id()
        );

        omega = dot(t, s) / omega_denominator;
        require(
            std::abs(omega) > Real{},
            error_catalog::kSingularSystem,
            BiCGSTAB::id()
        );

        for (Size i = 0; i < n; ++i) {
            solution[i] += alpha * direction[i] + omega * s[i];
            residual[i] = s[i] - omega * t[i];
        }

        if (norm_infinity(residual) <= options.tolerance) {
            return make_result(system, std::move(solution), true, iteration);
        }

        rho = next_rho;
    }

    return make_result(system, std::move(solution), false, options.max_iterations);
}

}  // namespace fvm
