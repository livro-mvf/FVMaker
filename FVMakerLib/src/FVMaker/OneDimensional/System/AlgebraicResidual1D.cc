// ============================================================================
// Arquivo: AlgebraicResidual1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Algebraic Residual 1 D no contexto de OneDimensional / System.
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

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

namespace fvm {

DenseVector algebraic_residual(
    const TridiagonalSystem1D& system,
    const DenseVector& solution
) {
    require(
        solution.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        TridiagonalSystem1D::id()
    );

    return system * solution - system.rhs();
}

DenseVector algebraic_residual(
    const PentadiagonalSystem1D& system,
    const DenseVector& solution
) {
    require(
        solution.size() == system.size(),
        error_catalog::kInvalidSystemSize,
        PentadiagonalSystem1D::id()
    );

    const Size n = system.size();
    const auto lower_second = system.lower_second();
    const auto lower_first = system.lower_first();
    const auto diagonal = system.diagonal();
    const auto upper_first = system.upper_first();
    const auto upper_second = system.upper_second();
    const auto rhs = system.rhs().values();

    DenseVector residual{n};

    for (Size row = 0; row < n; ++row) {
        Real ax = diagonal[row] * solution[row];

        if (row > 1) {
            ax += lower_second[row - 2] * solution[row - 2];
        }

        if (row > 0) {
            ax += lower_first[row - 1] * solution[row - 1];
        }

        if (row + 1 < n) {
            ax += upper_first[row] * solution[row + 1];
        }

        if (row + 2 < n) {
            ax += upper_second[row] * solution[row + 2];
        }

        residual[row] = ax - rhs[row];
    }

    return residual;
}

}  // namespace fvm
