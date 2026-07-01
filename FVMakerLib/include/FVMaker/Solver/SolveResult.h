// ============================================================================
// Arquivo: SolveResult.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Solve Result no contexto de Solver.
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

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/Solver/StopCriteria.h>

namespace fvm {

// Representa o conceito de solve result dentro da biblioteca FVMaker.
struct SolveResult final {
    DenseVector solution;
    DenseVector residual;
    bool converged{false};
    Size iterations{};
    Real residual_norm{};
    Real initial_residual_norm{};
    Real relative_residual_norm{};
    Real correction_norm{};
    Real requested_tolerance{};
    bool reached_absolute_tolerance{false};
    bool reached_relative_tolerance{false};
    StopCriterionKind stop_criterion{StopCriterionKind::max_iterations};
    Real stop_value{};
    Real stop_tolerance{};
    bool reached_iteration_limit{false};
};

}  // namespace fvm
