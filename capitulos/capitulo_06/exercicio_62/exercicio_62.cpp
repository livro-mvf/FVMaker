//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.2
// Implementar o TDMA
//==============================================================================

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_06.h"
#include <FVMaker/OneDimensional/Solver/TDMA.h>



int main() {
    using capitulo_06::Real;
    using capitulo_06::Size;

    std::cout << std::fixed << std::setprecision(12);

    const fvm::EquationContribution1D coeficientes =
        capitulo_06::coeficientes_exemplo();
    const fvm::TridiagonalSystem1D sistema =
        fvm::to_tridiagonal_system(coeficientes);

    const capitulo_06::TDMATrace rastro =
        capitulo_06::tdma_com_rastro(coeficientes);
    const fvm::SolveResult resultado_fvm = fvm::TDMA::solve(sistema);

    std::cout << "Exercicio 6.2 - TDMA passo a passo\n\n";
    std::cout << "Coeficientes do sistema\n";
    std::cout << "=======================\n";
    std::cout << coeficientes << "\n\n";

    std::cout << "Primeira fase: vetores T e Q\n";
    std::cout << "============================\n";
    std::cout << "indice"
              << std::setw(18) << "T"
              << std::setw(18) << "Q" << '\n';

    for (Size i = 0; i < rastro.t.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(18) << rastro.t[i]
                  << std::setw(18) << rastro.q[i] << '\n';
    }

    std::cout << "\nRetrosubstituicao\n";
    std::cout << "=================\n";
    capitulo_06::imprimir_vetor("phi", rastro.phi);

    fvm::DenseVector diferenca = rastro.phi - resultado_fvm.solution;
    const Real erro_fvm = fvm::norm_infinity(diferenca);

    std::cout << "\nComparacao com fvm::TDMA::solve\n";
    std::cout << "===============================\n";
    std::cout << "residuo FVMaker = " << resultado_fvm.residual_norm << '\n';
    std::cout << "|phi_didatico - phi_fvm|_inf = " << erro_fvm << '\n';

    unsigned aprovados{};
    unsigned total{};

    ++total;
    aprovados += capitulo_06::registrar(
        "TDMA didatico coincide com fvm::TDMA",
        capitulo_06::perto(erro_fvm, Real{})
    );

    ++total;
    aprovados += capitulo_06::registrar(
        "ultimo T e nulo",
        capitulo_06::perto(rastro.t[rastro.t.size() - 1], Real{})
    );

    return aprovados == total ? 0 : 1;
}
