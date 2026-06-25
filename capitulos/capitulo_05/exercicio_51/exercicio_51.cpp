//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.1
// Imprima os coeficientes do Exemplo 5.2
//==============================================================================

//==============================================================================
// Header c++
//==============================================================================
// #include <algorithm>
// #include <cmath>
#include <iomanip>
#include <iostream>
// #include <numeric>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

int main() {

using Real = fvm::Real;
using Size = fvm::Size;

    std::cout << std::fixed << std::setprecision(12);

//==============================================================================
// Geração e Impressao da malha
//==============================================================================

    const fvgrid::Axis1D eixo = 
        fvgrid::Uniform1D::make(    fvgrid::NVol{5},
                                    fvgrid::Length{5.0},
                                    fvgrid::XInit{0.0}
    );


    std::cout << "Exercicio 5.1 - malha do Exemplo 5.2\n\n";
    std::cout << eixo << '\n';

//==============================================================================
// Definição das condições de contorno
//==============================================================================

    const fvm::BoundaryCondition1D esquerda = fvm::dirichlet_1d(1.0);
    const fvm::BoundaryCondition1D direita = fvm::neumann_1d(0.0);

    std::cout << esquerda << '\n';


    // const auto fonte = [](Real x) { return x * x; };

    // const fvm::EquationContribution1D coeficientes =
    //     capitulo_05::montar_coeficientes_poisson(
    //         eixo,
    //         fonte,
    //         esquerda,
    //         direita
    //     );

    // const fvm::TridiagonalSystem1D sistema =
    //     fvm::to_tridiagonal_system(coeficientes);

    // const std::vector<std::vector<Real>> matriz =
    //     capitulo_05::matriz_densa(coeficientes);

    // const std::vector<std::vector<Real>> matriz_esperada{
    //     {3.0, -1.0, 0.0, 0.0, 0.0},
    //     {-1.0, 2.0, -1.0, 0.0, 0.0},
    //     {0.0, -1.0, 2.0, -1.0, 0.0},
    //     {0.0, 0.0, -1.0, 2.0, -1.0},
    //     {0.0, 0.0, 0.0, -1.0, 1.0}
    // };

    // const std::array<Real, 5> rhs_esperado{
    //     2.25,
    //     2.25,
    //     6.25,
    //     12.25,
    //     20.25
    // };

    // std::cout << "Exercicio 5.1 - coeficientes do Exemplo 5.2\n\n";
    // std::cout << coeficientes << '\n';

    // std::cout << "\nMatriz reconstruida a partir dos vetores\n";
    // std::cout << "=======================================\n";
    // capitulo_05::imprimir_matriz(matriz, sistema.rhs().values());

    // unsigned aprovados{};
    // unsigned total{};

    // ++total;
    // aprovados += capitulo_05::registrar(
    //     "matriz coincide com a Eq. do exemplo",
    //     capitulo_05::conferir_matriz(matriz, matriz_esperada)
    // );

    // ++total;
    // aprovados += capitulo_05::registrar(
    //     "lado direito coincide com a Eq. do exemplo",
    //     capitulo_05::conferir_vetor(sistema.rhs().values(), rhs_esperado)
    // );

    // bool tridiagonal = true;
    // for (Size i = 0; i < matriz.size(); ++i) {
    //     for (Size j = 0; j < matriz[i].size(); ++j) {
    //         const bool na_diagonal =
    //             i == j || (i > 0 && j + 1 == i) || (j > 0 && i + 1 == j);

    //         if (!na_diagonal && !capitulo_05::perto(matriz[i][j], 0.0)) {
    //             tridiagonal = false;
    //         }
    //     }
    // }

    // ++total;
    // aprovados += capitulo_05::registrar(
    //     "entradas fora das tres diagonais sao zero",
    //     tridiagonal
    // );

    // std::cout << "\nDiagonais nao nulas: inferior (i,i-1), principal (i,i), "
    //           << "superior (i,i+1)\n";

    // return aprovados == total ? 0 : 1;
    return 0;
}
