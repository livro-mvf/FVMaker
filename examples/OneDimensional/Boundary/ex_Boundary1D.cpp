// ============================================================================
// Arquivo: ex_Boundary1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Boundary 1 D no contexto de projeto.
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

#include <iomanip>
#include <iostream>

#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

int main() {
    using fvm::BoundarySet1D;
    using fvm::BoundarySide1D;
    using fvm::Real;

    const BoundarySet1D boundaries{
        fvm::dirichlet_1d(1.0),
        fvm::neumann_1d(0.0)
    };

    const Real dx = 0.25;
    const Real left_interior_value = 0.75;
    const Real right_interior_value = 2.0;

    const Real left_ghost = fvm::first_ghost_value(
        boundaries,
        BoundarySide1D::left,
        0.0,
        left_interior_value,
        dx
    );

    const Real right_ghost = fvm::first_ghost_value(
        boundaries,
        BoundarySide1D::right,
        1.0,
        right_interior_value,
        dx
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Boundary1D\n";
    std::cout << "==========\n";
    std::cout << "left interior value  = " << left_interior_value << '\n';
    std::cout << "left ghost value     = " << left_ghost << '\n';
    std::cout << "right interior value = " << right_interior_value << '\n';
    std::cout << "right ghost value    = " << right_ghost << '\n';

    return (left_ghost == Real{1.25} && right_ghost == right_interior_value)
        ? 0
        : 1;
}
