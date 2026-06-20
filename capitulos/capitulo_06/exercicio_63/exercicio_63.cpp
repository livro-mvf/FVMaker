//==============================================================================
// Exercicio Computacional 6.3 - Metodos iterativos lado a lado
//==============================================================================

#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace {
using Real = fvm::Real;
using Size = fvm::Size;

[[nodiscard]] fvm::TridiagonalSystem1D sistema_phi_um(Size n = 5) {
    std::vector<Real> lower(n - 1, -1.0);
    std::vector<Real> diagonal(n, 2.0);
    std::vector<Real> upper(n - 1, -1.0);
    fvm::DenseVector rhs(n);
    diagonal.front() = 3.0;
    diagonal.back() = 1.0;
    rhs[0] = 2.0;
    return {std::move(lower), std::move(diagonal), std::move(upper), std::move(rhs)};
}

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
        for (Real valor : atual.values()) {
            std::cout << std::setw(12) << valor;
        }
        std::cout << '\n';

        for (Size i = 0; i < atual.size(); ++i) {
            Real soma = b[i];
            if (i > 0) soma -= aW[i - 1] * atual[i - 1];
            if (i + 1 < atual.size()) soma -= aE[i] * atual[i + 1];
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
    const Real erro = fvm::norm_infinity(resultado.solution - exata);
    std::cout << std::setw(16) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << resultado.residual_norm
              << std::setw(18) << erro << '\n';
}

bool registrar(std::string_view texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}
}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);
    const fvm::TridiagonalSystem1D sistema = sistema_phi_um();
    const fvm::DenseVector exata(sistema.size(), 1.0);

    std::cout << "Exercicio 6.3 - quatro metodos iterativos\n\n";
    imprimir_primeiras_iteracoes_jacobi(sistema, 6);
    std::cout << '\n';

    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-10,
        .max_iterations = 10000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid
    };
    const auto jacobi = fvm::Jacobi::solve(sistema, opcoes);
    const auto gs = fvm::GaussSeidel::solve(sistema, opcoes);
    const auto bicg = fvm::BiCG::solve(sistema, opcoes);
    const auto bicgstab = fvm::BiCGSTAB::solve(sistema, opcoes);

    std::cout << std::setw(16) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(14) << "iteracoes"
              << std::setw(18) << "residuo"
              << std::setw(18) << "erro vs 1" << '\n';
    imprimir_resultado("Jacobi", jacobi, exata);
    imprimir_resultado("GS hibrido", gs, exata);
    imprimir_resultado("BiCG", bicg, exata);
    imprimir_resultado("BiCGSTAB", bicgstab, exata);

    const bool todos = registrar(
        "todos convergem para phi = 1",
        jacobi.converged && gs.converged && bicg.converged && bicgstab.converged
    );
    const bool gs_melhor = registrar(
        "Gauss-Seidel usa menos iteracoes que Jacobi",
        gs.iterations < jacobi.iterations
    );
    const bool bicgstab_melhor = registrar(
        "BiCGSTAB usa menos iteracoes que Gauss-Seidel",
        bicgstab.iterations < gs.iterations
    );
    return todos && gs_melhor && bicgstab_melhor ? 0 : 1;
}
