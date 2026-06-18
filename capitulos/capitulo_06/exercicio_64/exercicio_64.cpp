//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.4
// Iteracoes nao sao tempo
//==============================================================================

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_06.h"
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>

namespace {

using Real = capitulo_06::Real;
using Size = capitulo_06::Size;

template <class Solver>
void medir(
    std::string_view nome,
    Size n,
    Real flops_por_volume,
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::IterativeSolverOptions& opcoes
) {
    fvm::SolveResult resultado{};
    const Real tempo = capitulo_06::menor_tempo_de(1, [&] {
        resultado = Solver::solve(sistema, opcoes);
    });

    const Real flops_estimados =
        flops_por_volume * static_cast<Real>(n) *
        static_cast<Real>(resultado.iterations);

    std::cout << std::setw(12) << n
              << std::setw(16) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << flops_estimados
              << std::setw(16) << tempo << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Exercicio 6.4 - iteracoes nao sao tempo\n\n";
    std::cout << "Tamanhos didaticos usados no build automatico: "
              << "100, 300, 1000.\n";
    std::cout << "Para uma campanha longa, troque por 1e2, 1e3, 1e4, 1e5.\n\n";

    std::cout << std::setw(12) << "N"
              << std::setw(16) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(14) << "iter"
              << std::setw(18) << "flops aprox."
              << std::setw(16) << "tempo[s]" << '\n';

    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-8,
        .max_iterations = 50000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };

    for (Size n : {100u, 300u, 1000u}) {
        const fvm::EquationContribution1D coeficientes =
            capitulo_06::coeficientes_phi_um(n);
        const fvm::TridiagonalSystem1D sistema =
            fvm::to_tridiagonal_system(coeficientes);

        medir<fvm::Jacobi>("Jacobi", n, 7.0, sistema, opcoes);
        medir<fvm::GaussSeidel>("GS hibrido", n, 7.0, sistema, opcoes);
        medir<fvm::BiCG>("BiCG", n, 30.0, sistema, opcoes);
        medir<fvm::BiCGSTAB>("BiCGSTAB", n, 45.0, sistema, opcoes);
        std::cout << '\n';
    }

    return 0;
}
