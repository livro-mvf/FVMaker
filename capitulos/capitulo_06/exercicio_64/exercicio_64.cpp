//==============================================================================
// Exercicio Computacional 6.4 - Iteracoes nao sao tempo
//==============================================================================

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace {
using Real = fvm::Real;
using Size = fvm::Size;

[[nodiscard]] fvm::TridiagonalSystem1D sistema_phi_um(Size n) {
    std::vector<Real> lower(n - 1, -1.0);
    std::vector<Real> diagonal(n, 2.0);
    std::vector<Real> upper(n - 1, -1.0);
    fvm::DenseVector rhs(n);
    diagonal.front() = 3.0;
    diagonal.back() = 1.0;
    rhs[0] = 2.0;
    return {std::move(lower), std::move(diagonal), std::move(upper), std::move(rhs)};
}

template <class Function>
[[nodiscard]] Real menor_tempo_de(int repeticoes, Function&& f) {
    Real melhor = std::numeric_limits<Real>::max();
    for (int i = 0; i < repeticoes; ++i) {
        const auto inicio = std::chrono::steady_clock::now();
        f();
        const auto fim = std::chrono::steady_clock::now();
        melhor = std::min(
            melhor,
            std::chrono::duration<Real>(fim - inicio).count()
        );
    }
    return melhor;
}

template <class Solver>
void medir(
    std::string_view nome,
    Size n,
    Real flops_por_volume,
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::IterativeSolverOptions& opcoes
) {
    fvm::SolveResult resultado{};
    const Real tempo = menor_tempo_de(1, [&] {
        resultado = Solver::solve(sistema, opcoes);
    });
    const Real flops = flops_por_volume * static_cast<Real>(n)
                     * static_cast<Real>(resultado.iterations);
    std::cout << std::setw(12) << n
              << std::setw(16) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << flops
              << std::setw(16) << tempo << '\n';
}
}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Exercicio 6.4 - iteracoes nao sao tempo\n\n";
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
        const fvm::TridiagonalSystem1D sistema = sistema_phi_um(n);
        medir<fvm::Jacobi>("Jacobi", n, 7.0, sistema, opcoes);
        medir<fvm::GaussSeidel>("GS hibrido", n, 7.0, sistema, opcoes);
        medir<fvm::BiCG>("BiCG", n, 30.0, sistema, opcoes);
        medir<fvm::BiCGSTAB>("BiCGSTAB", n, 45.0, sistema, opcoes);
        std::cout << '\n';
    }
}
