//==============================================================================
// Exercicio Computacional 6.7 - O efeito da estimativa inicial
//==============================================================================

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
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

struct ResultadoExercicio {
    fvm::SolveResult solver;
    Real residuo_inicial{};
    Real residuo_final{};
    Real erro{};
};

[[nodiscard]] ResultadoExercicio resolver(
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::DenseVector& exata,
    Real chute,
    fvm::StopCriterionKind criterio,
    Real eps
) {
    fvm::DenseVector inicial{sistema.size(), chute};
    const Real r0 = fvm::norm_l2(fvm::algebraic_residual(sistema, inicial));

    const fvm::IterativeSolverOptions opcoes{
        .tolerance = eps,
        .max_iterations = 500000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::forward,
        .stop_criteria = fvm::StopCriteria{{fvm::StopCriterion{
            .kind = criterio,
            .tolerance = eps,
            .norm = fvm::NormType::l2
        }}}
    };
    fvm::SolveResult resultado =
        fvm::GaussSeidel::solve(sistema, std::move(inicial), opcoes);
    const Real rf = fvm::norm_l2(resultado.residual);
    const Real erro = fvm::norm_infinity(resultado.solution - exata);
    return {std::move(resultado), r0, rf, erro};
}

void imprimir_linha(
    std::string_view criterio,
    Real chute,
    const ResultadoExercicio& r
) {
    std::cout << std::setw(18) << criterio
              << std::setw(12) << chute
              << std::setw(12) << r.solver.iterations
              << std::setw(18) << r.residuo_inicial
              << std::setw(18) << r.residuo_final
              << std::setw(18) << r.solver.stop_value
              << std::setw(18) << r.erro << '\n';
}
}  // namespace

int main() {
    std::cout << std::scientific << std::setprecision(8);
    std::cout << "Exercicio 6.7 - estimativa inicial\n\n";
    std::cout << std::setw(18) << "criterio"
              << std::setw(12) << "phi0"
              << std::setw(12) << "iter"
              << std::setw(18) << "||R0||2"
              << std::setw(18) << "||R||2"
              << std::setw(18) << "valor criterio"
              << std::setw(18) << "erro inf." << '\n';

    constexpr Size n = 80;
    constexpr Real eps = 1.0e-8;
    const fvm::TridiagonalSystem1D sistema = sistema_phi_um(n);
    const fvm::DenseVector exata(n, 1.0);
    const Real chutes[] = {0.0, 0.9, 0.99};

    std::vector<Size> iter_relativo;
    for (Real chute : chutes) {
        const auto r = resolver(
            sistema, exata, chute,
            fvm::StopCriterionKind::residual_relative_initial, eps
        );
        iter_relativo.push_back(r.solver.iterations);
        imprimir_linha("relativo a R0", chute, r);
    }

    std::cout << '\n';
    std::vector<Size> iter_absoluto;
    for (Real chute : chutes) {
        const auto r = resolver(
            sistema, exata, chute,
            fvm::StopCriterionKind::residual_absolute, eps
        );
        iter_absoluto.push_back(r.solver.iterations);
        imprimir_linha("absoluto", chute, r);
    }

    const auto [min_rel, max_rel] =
        std::minmax_element(iter_relativo.begin(), iter_relativo.end());
    std::cout << "\nObservacao:\n";
    if (*min_rel == *max_rel) {
        std::cout << "Os tres chutes exigiram o mesmo numero de iteracoes "
                  << "pelo criterio relativo. Neste problema linear, os erros "
                  << "iniciais sao proporcionais e a normalizacao cancela a escala.\n";
    } else {
        std::cout << "O criterio relativo alterou a contagem em "
                  << (*max_rel - *min_rel)
                  << " iteracoes; compare a reducao exigida de cada ||R0||2.\n";
    }
    std::cout << "Pelo criterio absoluto, o chute mais proximo nao e penalizado: "
              << iter_absoluto.front() << " -> " << iter_absoluto.back()
              << " iteracoes.\n";
}
