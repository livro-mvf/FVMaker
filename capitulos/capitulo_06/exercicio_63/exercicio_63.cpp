//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.3
// Os quatro metodos iterativos, lado a lado
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

void imprimir_primeiras_iteracoes_jacobi(
    const fvm::TridiagonalSystem1D& sistema,
    Size numero
) {
    fvm::DenseVector atual{sistema.size()};
    fvm::DenseVector proximo{sistema.size()};

    const auto aW = sistema.lower();
    const auto aP = sistema.diagonal();
    const auto aE = sistema.upper();
    const auto b = sistema.rhs().values();

    std::cout << "Jacobi: primeiras iteracoes\n";

    for (Size k = 0; k <= numero; ++k) {
        std::cout << "k = " << k << "  ";
        for (Size i = 0; i < atual.size(); ++i) {
            std::cout << std::setw(12) << atual[i];
        }
        std::cout << '\n';

        for (Size i = 0; i < atual.size(); ++i) {
            Real soma = b[i];
            if (i > 0) {
                soma -= aW[i - 1] * atual[i - 1];
            }
            if (i + 1 < atual.size()) {
                soma -= aE[i] * atual[i + 1];
            }
            proximo[i] = soma / aP[i];
        }
        atual = proximo;
    }
}

void imprimir_resultado(
    std::string_view nome,
    const fvm::SolveResult& resultado,
    const fvm::DenseVector& exata
) {
    const Real erro = capitulo_06::erro_infinito(resultado.solution, exata);

    std::cout << std::setw(16) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << resultado.residual_norm
              << std::setw(18) << erro << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);

    const fvm::EquationContribution1D coeficientes =
        capitulo_06::coeficientes_phi_um();
    const fvm::TridiagonalSystem1D sistema =
        fvm::to_tridiagonal_system(coeficientes);
    const fvm::DenseVector exata =
        capitulo_06::vetor_constante(sistema.size(), 1.0);

    std::cout << "Exercicio 6.3 - quatro metodos iterativos\n\n";
    std::cout << coeficientes << "\n\n";

    imprimir_primeiras_iteracoes_jacobi(sistema, 6);
    std::cout << '\n';

    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-10,
        .max_iterations = 10000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };

    const fvm::SolveResult jacobi = fvm::Jacobi::solve(sistema, opcoes);
    const fvm::SolveResult gs = fvm::GaussSeidel::solve(sistema, opcoes);
    const fvm::SolveResult bicg = fvm::BiCG::solve(sistema, opcoes);
    const fvm::SolveResult bicgstab = fvm::BiCGSTAB::solve(sistema, opcoes);

    std::cout << "Resumo de convergencia\n";
    std::cout << "======================\n";
    std::cout << std::setw(16) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(14) << "iteracoes"
              << std::setw(18) << "residuo"
              << std::setw(18) << "erro vs 1" << '\n';

    imprimir_resultado("Jacobi", jacobi, exata);
    imprimir_resultado("GS hibrido", gs, exata);
    imprimir_resultado("BiCG", bicg, exata);
    imprimir_resultado("BiCGSTAB", bicgstab, exata);

    unsigned aprovados{};
    unsigned total{};

    ++total;
    aprovados += capitulo_06::registrar(
        "todos convergem para phi = 1",
        jacobi.converged && gs.converged && bicg.converged && bicgstab.converged
    );

    ++total;
    aprovados += capitulo_06::registrar(
        "Gauss-Seidel converge em menos iteracoes que Jacobi",
        gs.iterations < jacobi.iterations
    );

    ++total;
    aprovados += capitulo_06::registrar(
        "BiCGSTAB converge em menos iteracoes que Gauss-Seidel",
        bicgstab.iterations < gs.iterations
    );

    return aprovados == total ? 0 : 1;
}
