// ============================================================================
// Arquivo: ex_Poisson1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Poisson 1 D no contexto de projeto.
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
#include <iomanip>
#include <iostream>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>

namespace {

constexpr fvm::Real pi = 3.141592653589793238462643383279502884;

[[nodiscard]] fvm::Real phi_exact(fvm::Real x) {
    return std::sin(pi * x);
}

[[nodiscard]] fvm::Real source(fvm::Real x) {
    return -pi * pi * std::sin(pi * x);
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{20},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::Equation1D equation{
        grid,
        fvm::Laplacian1D{},
        fvm::function_source_1d(grid, source),
        fvm::BoundarySet1D{fvm::dirichlet_1d(0.0), fvm::dirichlet_1d(0.0)}
    };

    const fvm::TridiagonalSystem1D system = fvm::assemble_steady_1d(equation);
    const fvm::SolveResult result = fvm::TDMA::solve(system);

    fvm::DenseVector error{grid.num_volumes()};

    for (fvm::Size i = 0; i < grid.num_volumes(); ++i) {
        error[i] = result.solution[i] - phi_exact(grid.centers()[i]);
    }

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Poisson 1D: d2phi/dx2 = f(x)\n";
    std::cout << "==============================\n";
    std::cout << "N                 = " << grid.num_volumes() << '\n';
    std::cout << "residual infinity = " << result.residual_norm << '\n';
    std::cout << "error infinity    = " << fvm::norm_infinity(error) << "\n\n";

    std::cout << std::setw(5) << "i"
              << std::setw(14) << "x"
              << std::setw(16) << "phi_num"
              << std::setw(16) << "phi_exact"
              << '\n';

    for (fvm::Size i = 0; i < grid.num_volumes(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(14) << grid.centers()[i]
                  << std::setw(16) << result.solution[i]
                  << std::setw(16) << phi_exact(grid.centers()[i])
                  << '\n';
    }

    return result.converged && fvm::norm_infinity(error) < fvm::Real{3.0e-3}
        ? 0
        : 1;
}
