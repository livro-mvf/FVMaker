//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.6
// Imprima os coeficientes de um problema seu
//==============================================================================

#include "../comum/mvf_capitulo_05.h"

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>

#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    using capitulo_05::Real;

    std::cout << std::fixed << std::setprecision(12);

    const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0},
        fvgrid::FaceCentered1D{}
    );

    const fvm::EquationContribution1D coeficientes =
        capitulo_05::montar_coeficientes_poisson(
            eixo,
            [](Real) { return Real{1}; },
            fvm::dirichlet_1d(0.0),
            fvm::neumann_1d(0.0)
        );

    const fvm::TridiagonalSystem1D sistema =
        fvm::to_tridiagonal_system(coeficientes);
    const std::vector<std::vector<Real>> matriz =
        capitulo_05::matriz_densa(coeficientes);

    std::cout << "Exercicio 5.6 - problema proprio\n\n";
    capitulo_05::imprimir_vetores(coeficientes);

    std::cout << "\nMatriz reconstruida\n";
    std::cout << "===================\n";
    capitulo_05::imprimir_matriz(matriz, sistema.rhs().values());

    std::cout << "\nEste sistema ainda nao e resolvido aqui; a resolucao fica "
              << "para o capitulo 6.\n";

    return 0;
}
