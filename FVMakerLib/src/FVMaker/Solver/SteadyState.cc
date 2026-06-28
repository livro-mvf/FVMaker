// ============================================================================
// Arquivo: SteadyState.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Steady State no contexto de Solver.
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

#include <algorithm>
#include <cmath>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Solver/SteadyState.h>

namespace fvm {

Real SteadyState::effective_tolerance(Real reference_norm) const {
    validate();

    require(
        std::isfinite(reference_norm) && reference_norm >= Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    return std::max(absolute_tolerance, relative_tolerance * reference_norm);
}

void SteadyState::validate() const {
    require(
        std::isfinite(absolute_tolerance) && absolute_tolerance > Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    require(
        std::isfinite(relative_tolerance) && relative_tolerance >= Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    require(
        max_iterations > 0,
        error_catalog::kInvalidArgument,
        SteadyState::id()
    );
}

}  // namespace fvm
