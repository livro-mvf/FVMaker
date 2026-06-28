// ============================================================================
// Arquivo: LinearSolverOptions1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Linear Solver Options 1 D no contexto de OneDimensional / Solver.
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
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>

namespace fvm {

std::string_view name(LinearSolverKind1D kind) noexcept {
    switch (kind) {
        case LinearSolverKind1D::tdma:
            return "TDMA";
        case LinearSolverKind1D::jacobi:
            return "Jacobi";
        case LinearSolverKind1D::gauss_seidel:
            return "Gauss-Seidel";
        case LinearSolverKind1D::conjugate_gradient:
            return "Conjugate Gradient";
        case LinearSolverKind1D::bicg:
            return "BiCG";
        case LinearSolverKind1D::bicgstab:
            return "BiCGSTAB";
    }

    return "unknown";
}

SteadyState LinearSolverOptions1D::steady_state() const {
    return SteadyState{
        .absolute_tolerance = absolute_tolerance,
        .relative_tolerance = relative_tolerance,
        .max_iterations = max_iterations
    };
}

void LinearSolverOptions1D::validate() const {
    steady_state().validate();

    require(
        solver == LinearSolverKind1D::tdma
            || solver == LinearSolverKind1D::jacobi
            || solver == LinearSolverKind1D::gauss_seidel
            || solver == LinearSolverKind1D::conjugate_gradient
            || solver == LinearSolverKind1D::bicg
            || solver == LinearSolverKind1D::bicgstab,
        error_catalog::kInvalidArgument,
        id()
    );
}

}  // namespace fvm
