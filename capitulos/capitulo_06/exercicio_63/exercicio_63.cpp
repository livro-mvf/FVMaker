// ============================================================================
// Arquivo: exercicio_63.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 63 no contexto de capitulos / capitulo_06 / exercicio_63.
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

#include <iomanip>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/Solver/StopCriteria.h>

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


template <class Step>
void imprimir_seis_iteracoes(
    std::string_view nome,
    const fvm::TridiagonalSystem1D& sistema,
    Step&& step
) {
    fvm::DenseVector phi{sistema.size()};
    std::cout << nome << '\n';
    for (Size k = 0; k <= 6; ++k) {
        std::cout << "k = " << k << "  ";
        for (Real valor : phi.values()) std::cout << std::setw(12) << valor;
        std::cout << '\n';
        if (k < 6) step(phi, sistema, k + 1);
    }
    std::cout << '\n';
}

void imprimir_resultado(
    std::string_view nome,
    const fvm::SolveResult& resultado,
    const fvm::DenseVector& exata
) {
    const Real erro = fvm::norm_infinity(resultado.solution - exata);
    std::cout << std::setw(22) << nome
              << std::setw(12) << (resultado.converged ? "sim" : "nao")
              << std::setw(14) << resultado.iterations
              << std::setw(18) << resultado.stop_value
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

    std::cout << "Exercicio 6.3 - Jacobi e Gauss-Seidel\n\n";
    fvm::DenseVector workspace{sistema.size()};
    imprimir_seis_iteracoes("Jacobi", sistema,
        [&workspace](auto& phi, const auto& a, Size) {
            fvm::Jacobi::sweep(a, phi, workspace);
        });
    imprimir_seis_iteracoes("GS_crescente", sistema,
        [](auto& phi, const auto& a, Size) {
            fvm::GaussSeidel::sweep(a, phi, fvm::GaussSeidelSweep::forward);
        });
    imprimir_seis_iteracoes("GS_decrescente", sistema,
        [](auto& phi, const auto& a, Size) {
            fvm::GaussSeidel::sweep(a, phi, fvm::GaussSeidelSweep::backward);
        });
    imprimir_seis_iteracoes("GS_simetrico", sistema,
        [](auto& phi, const auto& a, Size) {
            fvm::GaussSeidel::sweep(a, phi, fvm::GaussSeidelSweep::hybrid);
        });
    const fvm::IterativeSolverOptions base{
        .tolerance = 1.0e-10,
        .max_iterations = 10000,
        .stop_criteria = fvm::StopCriteria::residual_absolute(
            1.0e-10, fvm::NormType::rms
        )
    };
    const auto jacobi = fvm::Jacobi::solve(sistema, base);

    auto opcoes = base;
    opcoes.gauss_seidel_sweep = fvm::GaussSeidelSweep::forward;
    const auto crescente = fvm::GaussSeidel::solve(sistema, opcoes);
    opcoes.gauss_seidel_sweep = fvm::GaussSeidelSweep::backward;
    const auto decrescente = fvm::GaussSeidel::solve(sistema, opcoes);
    opcoes.gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid;
    const auto simetrico = fvm::GaussSeidel::solve(sistema, opcoes);

    std::cout << std::setw(22) << "metodo"
              << std::setw(12) << "conv."
              << std::setw(14) << "iteracoes"
              << std::setw(18) << "residuo"
              << std::setw(18) << "erro vs 1" << '\n';
    imprimir_resultado("Jacobi", jacobi, exata);
    imprimir_resultado("GS crescente", crescente, exata);
    imprimir_resultado("GS decrescente", decrescente, exata);
    imprimir_resultado("GS simetrico", simetrico, exata);

    const bool convergiram = registrar(
        "todos convergem para phi = 1",
        jacobi.converged && crescente.converged
            && decrescente.converged && simetrico.converged
    );
    const bool gs_mais_rapido = registrar(
        "as tres varreduras de Gauss-Seidel superam Jacobi",
        crescente.iterations < jacobi.iterations
            && decrescente.iterations < jacobi.iterations
            && simetrico.iterations < jacobi.iterations
    );
    return convergiram && gs_mais_rapido ? 0 : 1;
}
