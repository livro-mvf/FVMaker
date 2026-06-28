// ============================================================================
// Arquivo: ex_PentadiagonalAndGhostCells1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Pentadiagonal And Ghost Cells 1 D no contexto de projeto.
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
#include <vector>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>

int main() {
    using fvm::DenseVector;
    using fvm::GhostCells1D;
    using fvm::PentadiagonalSystem1D;
    using fvm::Real;

    const GhostCells1D ghost_cells{2, 2};

    const PentadiagonalSystem1D system{
        std::vector<Real>{-0.5, -0.5, -0.5},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{4.0, 4.0, 4.0, 4.0, 4.0},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{-0.5, -0.5, -0.5},
        DenseVector{std::vector<Real>{2.5, 1.5, 1.0, 1.5, 2.5}}
    };

    const DenseVector solution{std::vector<Real>{1.0, 1.0, 1.0, 1.0, 1.0}};
    const DenseVector residual = fvm::algebraic_residual(system, solution);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Ghost cells 1D\n";
    std::cout << "==============\n";
    std::cout << "left                  = " << ghost_cells.left() << '\n';
    std::cout << "right                 = " << ghost_cells.right() << '\n';
    std::cout << "physical cells        = " << system.size() << '\n';
    std::cout << "storage cells         = "
              << ghost_cells.storage_size(system.size()) << '\n';
    std::cout << "first physical index  = "
              << ghost_cells.first_physical_index() << "\n\n";

    std::cout << "Pentadiagonal residual\n";
    std::cout << "======================\n";
    std::cout << "||A x - b||_inf       = "
              << fvm::norm_infinity(residual) << '\n';

    return fvm::norm_infinity(residual) == Real{} ? 0 : 1;
}
