// ============================================================================
// Arquivo: TDMA.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa TDMA no contexto de OneDimensional / Solver.
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
