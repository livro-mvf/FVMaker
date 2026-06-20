//==============================================================================
// Exercicio Computacional 6.8 - Quando a dominancia fraca ainda converge
//==============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>

namespace {
using Size = fvm::Size;
using Real = fvm::Real;
constexpr Real kTolerance = 1.0e-10;

[[nodiscard]] fvm::EquationContribution1D coeficientes_phi_um(Size n = 5) {
    fvm::EquationContribution1D c{n};
    for (Size i = 0; i < n; ++i) {
        c.aw()[i] = i > 0 ? 1.0 : 0.0;
        c.ae()[i] = i + 1 < n ? 1.0 : 0.0;
        c.ap()[i] = c.aw()[i] + c.ae()[i];
    }
    c.ap()[0] = 3.0;
    c.bp()[0] = 2.0;
    return c;
}

[[nodiscard]] bool linha_fracamente_dominante(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i]) + kTolerance
           >= std::abs(c.aw()[i]) + std::abs(c.ae()[i]);
}

[[nodiscard]] bool linha_estritamente_dominante(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i])
           > std::abs(c.aw()[i]) + std::abs(c.ae()[i]) + kTolerance;
}

[[nodiscard]] fvm::EquationContribution1D coeficientes_nao_dominantes() {
    fvm::EquationContribution1D c{2};
    c.ap()[0] = 1.0;
    c.ae()[0] = -3.0;
    c.bp()[0] = 1.0;
    c.ap()[1] = 1.0;
    c.aw()[1] = -2.0;
    c.bp()[1] = 1.0;
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
    const auto sistema = fvm::to_tridiagonal_system(c);
    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-10,
        .max_iterations = 5000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };
    const auto jacobi = fvm::Jacobi::solve(sistema, opcoes);
    const auto gs = fvm::GaussSeidel::solve(sistema, opcoes);

    std::cout << titulo << '\n';
    std::cout << std::setw(16) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(12) << "iter"
              << std::setw(18) << "residuo" << '\n';
    for (const auto& item : {
        std::pair<std::string_view, const fvm::SolveResult&>{"Jacobi", jacobi},
        {"Gauss-Seidel", gs}
    }) {
        std::cout << std::setw(16) << item.first
                  << std::setw(12) << (item.second.converged ? "sim" : "nao")
                  << std::setw(12) << item.second.iterations
                  << std::setw(18) << item.second.residual_norm << '\n';
    }
    std::cout << '\n';
}
}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Exercicio 6.8 - dominancia fraca\n\n";

    const auto fraca = coeficientes_phi_um();
    const auto ruim = coeficientes_nao_dominantes();

    std::cout << "Matriz do livro\n";
    imprimir_dominancia(fraca);
    std::cout << '\n';
    resolver_e_imprimir(fraca, "Sistema fracamente dominante e irredutivel");

    std::cout << "Matriz nao dominante\n";
    imprimir_dominancia(ruim);
    std::cout << '\n';
    resolver_e_imprimir(ruim, "Sistema nao dominante");
}
