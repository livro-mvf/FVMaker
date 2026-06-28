// ============================================================================
// Arquivo: exercicio_68.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 68 no contexto de capitulos / capitulo_06 / exercicio_68.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string_view>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
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

[[nodiscard]] bool dominante_fraca(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i]) + kTolerance
        >= std::abs(c.aw()[i]) + std::abs(c.ae()[i]);
}

[[nodiscard]] bool dominante_estrita(
    const fvm::EquationContribution1D& c,
    Size i
) {
    return std::abs(c.ap()[i])
        > std::abs(c.aw()[i]) + std::abs(c.ae()[i]) + kTolerance;
}

void imprimir_dominancia(const fvm::EquationContribution1D& c) {
    std::cout << "linha"
              << std::setw(18) << "A_P"
              << std::setw(18) << "|A_W|+|A_E|"
              << std::setw(14) << "fraca"
              << std::setw(14) << "estrita" << '\n';
    for (Size i = 0; i < c.size(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(18) << c.ap()[i]
                  << std::setw(18) << std::abs(c.aw()[i]) + std::abs(c.ae()[i])
                  << std::setw(14) << (dominante_fraca(c, i) ? "sim" : "nao")
                  << std::setw(14) << (dominante_estrita(c, i) ? "sim" : "nao")
                  << '\n';
    }
}

void imprimir_historico_divergente(const fvm::TridiagonalSystem1D& a) {
    fvm::DenseVector jacobi{a.size()};
    fvm::DenseVector gs{a.size()};

    std::cout << "Evolucao do residuo no sistema nao dominante\n"
              << std::setw(8) << "k"
              << std::setw(20) << "Jacobi ||R||inf"
              << std::setw(20) << "GS ||R||inf" << '\n';

    for (Size k = 0; k <= 8; ++k) {
        std::cout << std::setw(8) << k
                  << std::setw(20)
                  << fvm::norm_infinity(fvm::algebraic_residual(a, jacobi))
                  << std::setw(20)
                  << fvm::norm_infinity(fvm::algebraic_residual(a, gs))
                  << '\n';
        if (k == 8) break;

        const fvm::DenseVector anterior = jacobi;
        for (Size i = 0; i < a.size(); ++i) {
            Real soma = a.rhs()[i];
            if (i > 0) soma -= a.lower()[i - 1] * anterior[i - 1];
            if (i + 1 < a.size()) soma -= a.upper()[i] * anterior[i + 1];
            jacobi[i] = soma / a.diagonal()[i];
        }
        for (Size i = 0; i < a.size(); ++i) {
            Real soma = a.rhs()[i];
            if (i > 0) soma -= a.lower()[i - 1] * gs[i - 1];
            if (i + 1 < a.size()) soma -= a.upper()[i] * gs[i + 1];
            gs[i] = soma / a.diagonal()[i];
        }
    }
    std::cout << '\n';
}

void resolver_e_imprimir(
    const fvm::EquationContribution1D& c,
    std::string_view titulo,
    Size max_iterations
) {
    const auto sistema = fvm::to_tridiagonal_system(c);
    const fvm::IterativeSolverOptions opcoes{
        .tolerance = 1.0e-10,
        .max_iterations = max_iterations,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::forward
    };
    const auto jacobi = fvm::Jacobi::solve(sistema, opcoes);
    const auto gs = fvm::GaussSeidel::solve(sistema, opcoes);

    std::cout << titulo << '\n'
              << std::setw(16) << "metodo"
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
    std::cout << std::scientific << std::setprecision(8);
    std::cout << "Exercicio 6.8 - dominancia fraca\n\n";

    const auto fraca = coeficientes_phi_um();
    const auto ruim = coeficientes_nao_dominantes();

    std::cout << "Matriz do livro\n";
    imprimir_dominancia(fraca);
    std::cout << '\n';
    resolver_e_imprimir(fraca, "Sistema fracamente dominante e irredutivel", 5000);

    std::cout << "Matriz nao dominante\n";
    imprimir_dominancia(ruim);
    std::cout << '\n';
    const auto sistema_ruim = fvm::to_tridiagonal_system(ruim);
    imprimir_historico_divergente(sistema_ruim);
    resolver_e_imprimir(ruim, "Sistema nao dominante", 50);
}
