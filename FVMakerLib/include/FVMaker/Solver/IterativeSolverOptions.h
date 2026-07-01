// ============================================================================
// Arquivo: IterativeSolverOptions.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Iterative Solver Options no contexto de Solver.
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

#include <FVMaker/Core/Types.h>
#include <FVMaker/Solver/StopCriteria.h>

namespace fvm {

// Define a direcao de varredura usada pelo metodo de Gauss-Seidel.
enum class GaussSeidelSweep {
    forward,
    backward,
    hybrid,
};

// Representa o conceito de iterative solver options dentro da biblioteca FVMaker.
struct IterativeSolverOptions final {
    Real tolerance{1.0e-10};
    Size max_iterations{10000};
    GaussSeidelSweep gauss_seidel_sweep{GaussSeidelSweep::hybrid};
    StopCriteria stop_criteria{};
};

}  // namespace fvm
