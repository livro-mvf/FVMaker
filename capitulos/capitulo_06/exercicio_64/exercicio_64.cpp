//==============================================================================
// Exercicio Computacional 6.4 - Iteracoes nao sao tempo
//==============================================================================

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

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
    constexpr int repeticoes = 3;
    const Real tempo = menor_tempo_de(repeticoes, [&] {
        resultado = Solver::solve(sistema, opcoes);
    });
    const Real flops = flops_por_volume * static_cast<Real>(n)
                     * static_cast<Real>(resultado.iterations);

    std::cout << std::setw(12) << n
              << std::setw(18) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << flops
              << std::setw(16) << tempo << '\n';
}
}  // namespace

int main(int argc, char** argv) {
    const bool campanha_completa =
        argc > 1 && std::string_view{argv[1]} == "--full";
    const std::vector<Size> tamanhos = campanha_completa
        ? std::vector<Size>{100u, 1000u, 10000u, 100000u}
        : std::vector<Size>{20u, 50u, 100u};

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Exercicio 6.4 - iteracoes nao sao tempo\n\n"
              << "compilador: " << __VERSION__ << '\n'
              << "threads informadas pelo hardware: "
              << std::thread::hardware_concurrency() << '\n'
              << "build: "
#ifdef NDEBUG
              << "Release"
#else
              << "Debug"
#endif
              << "\nrepeticoes por medida: 3\n"
              << "campanha: "
              << (campanha_completa ? "10^2, 10^3, 10^4, 10^5"
                                    : "rapida; use --full para a campanha do texto")
              << "\n\n";

    std::cout << std::setw(12) << "N"
              << std::setw(18) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(14) << "iter"
              << std::setw(18) << "flops aprox."
              << std::setw(16) << "tempo[s]" << '\n';

    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-8,
        .max_iterations = campanha_completa ? 200000u : 50000u,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };

    for (Size n : tamanhos) {
        const fvm::TridiagonalSystem1D sistema = sistema_phi_um(n);
        medir<fvm::Jacobi>("Jacobi", n, 7.0, sistema, opcoes);
        medir<fvm::GaussSeidel>("GS hibrido", n, 7.0, sistema, opcoes);
        std::cout << '\n';
    }
}
