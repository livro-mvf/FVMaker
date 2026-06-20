//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.7 - O efeito da estimativa inicial
//==============================================================================

#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
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

void imprimir_linha(
    std::string_view criterio,
    Real chute,
    const fvm::SolveResult& resultado,
    const fvm::DenseVector& exata
) {
    const Real erro = fvm::norm_infinity(resultado.solution - exata);
    std::cout << std::setw(20) << criterio
              << std::setw(14) << chute
              << std::setw(12) << resultado.iterations
              << std::setw(18) << resultado.initial_residual_norm
              << std::setw(18) << resultado.residual_norm
              << std::setw(18) << erro << '\n';
}

void executar(
    std::string_view nome,
    fvm::StopCriterionKind criterio,
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::DenseVector& exata
) {
    constexpr Real eps = 1.0e-8;
    for (Real chute : {0.0, 0.9, 0.99}) {
        const fvm::IterativeSolverOptions opcoes{
            .tolerance = eps,
            .max_iterations = 500000,
            .stop_criteria = fvm::StopCriteria{{fvm::StopCriterion{
                .kind = criterio,
                .tolerance = eps,
                .norm = fvm::NormType::infinity
            }}}
        };
        imprimir_linha(
            nome,
            chute,
            fvm::GaussSeidel::solve(
                sistema,
                fvm::DenseVector{sistema.size(), chute},
                opcoes
            ),
            exata
        );
    }
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Exercicio 6.7 - estimativa inicial\n\n";
    std::cout << std::setw(20) << "criterio"
              << std::setw(14) << "phi0"
              << std::setw(12) << "iter"
              << std::setw(18) << "residuo inicial"
              << std::setw(18) << "residuo final"
              << std::setw(18) << "erro" << '\n';

    constexpr Size n = 80;
    const fvm::TridiagonalSystem1D sistema = sistema_phi_um(n);
    const fvm::DenseVector exata(n, 1.0);

    executar("relativo", fvm::StopCriterionKind::residual_relative_initial,
             sistema, exata);
    std::cout << '\n';
    executar("absoluto", fvm::StopCriterionKind::residual_absolute,
             sistema, exata);
}
