//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.8
// Quando a dominancia apenas fraca ainda converge
//==============================================================================



//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_06.h"
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>

namespace {

using Size = capitulo_06::Size;
using Real = capitulo_06::Real;

[[nodiscard]] bool linha_fracamente_dominante(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i]) + capitulo_06::kTol
           >= std::abs(c.aw()[i]) + std::abs(c.ae()[i]);
}

[[nodiscard]] bool linha_estritamente_dominante(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i])
           > std::abs(c.aw()[i]) + std::abs(c.ae()[i]) + capitulo_06::kTol;
}

[[nodiscard]] fvm::EquationContribution1D coeficientes_nao_dominantes() {
    fvm::EquationContribution1D c{2};

    c.ap()[0] = Real{1};
    c.ae()[0] = Real{-3};
    c.aw()[0] = Real{};
    c.bp()[0] = Real{1};

    c.ap()[1] = Real{1};
    c.ae()[1] = Real{};
    c.aw()[1] = Real{-2};
    c.bp()[1] = Real{1};

    return c;
}

void imprimir_dominancia(const fvm::EquationContribution1D& c) {
    std::cout << "linha"
              << std::setw(18) << "A_P"
              << std::setw(18) << "A_W+A_E"
              << std::setw(14) << "fraca"
              << std::setw(14) << "estrita" << '\n';

    for (Size i = 0; i < c.size(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(18) << c.ap()[i]
                  << std::setw(18) << std::abs(c.aw()[i]) + std::abs(c.ae()[i])
                  << std::setw(14) << (linha_fracamente_dominante(c, i) ? "sim" : "nao")
                  << std::setw(14) << (linha_estritamente_dominante(c, i) ? "sim" : "nao")
                  << '\n';
    }
}

void resolver_e_imprimir(
    const fvm::EquationContribution1D& c,
    std::string_view titulo
) {
    const fvm::TridiagonalSystem1D sistema = fvm::to_tridiagonal_system(c);
    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-10,
        .max_iterations = 5000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };

    const fvm::SolveResult jacobi = fvm::Jacobi::solve(sistema, opcoes);
    const fvm::SolveResult gs = fvm::GaussSeidel::solve(sistema, opcoes);

    std::cout << titulo << '\n';
    std::cout << std::setw(16) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(12) << "iter"
              << std::setw(18) << "residuo" << '\n';
    std::cout << std::setw(16) << "Jacobi"
              << std::setw(12) << (jacobi.converged ? "sim" : "nao")
              << std::setw(12) << jacobi.iterations
              << std::setw(18) << jacobi.residual_norm << '\n';
    std::cout << std::setw(16) << "Gauss-Seidel"
              << std::setw(12) << (gs.converged ? "sim" : "nao")
              << std::setw(12) << gs.iterations
              << std::setw(18) << gs.residual_norm << "\n\n";
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Exercicio 6.8 - dominancia fraca\n\n";

    const fvm::EquationContribution1D fraca =
        capitulo_06::coeficientes_phi_um();
    const fvm::EquationContribution1D ruim =
        coeficientes_nao_dominantes();

    std::cout << "Matriz do livro\n";
    imprimir_dominancia(fraca);
    std::cout << '\n';
    resolver_e_imprimir(fraca, "Sistema fracamente dominante e irredutivel");

    std::cout << "Matriz nao dominante\n";
    imprimir_dominancia(ruim);
    std::cout << '\n';
    resolver_e_imprimir(ruim, "Sistema nao dominante");

    return 0;
}
