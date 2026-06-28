// ============================================================================
// Arquivo: exercicio_65.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 65 no contexto de capitulos / capitulo_06 / exercicio_65.
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

[[nodiscard]] fvm::StopCriteria criterio(
    fvm::StopCriterionKind tipo,
    Real tolerancia
) {
    return fvm::StopCriteria{{fvm::StopCriterion{
        .kind = tipo,
        .tolerance = tolerancia,
        .norm = fvm::NormType::infinity
    }}};
}

void resolver(
    std::string_view nome,
    fvm::StopCriterionKind tipo,
    Real eps,
    const fvm::TridiagonalSystem1D& sistema,
    const fvm::DenseVector& exata
) {
    const fvm::IterativeSolverOptions opcoes{
        .tolerance = eps,
        .max_iterations = 200000,
        .gauss_seidel_sweep = fvm::GaussSeidelSweep::hybrid,
        .stop_criteria = criterio(tipo, eps)
    };
    const fvm::SolveResult r = fvm::GaussSeidel::solve(sistema, opcoes);
    const Real erro = fvm::norm_infinity(r.solution - exata);

    std::cout << std::setw(30) << nome
              << std::setw(14) << eps
              << std::setw(12) << r.iterations
              << std::setw(12) << (r.converged ? "sim" : "nao")
              << std::setw(18) << r.stop_value
              << std::setw(18) << r.residual_norm
              << std::setw(18) << erro << '\n';
}
}  // namespace

int main(int argc, char** argv) {
    std::cout << std::fixed << std::setprecision(6);
    const Size n = argc > 1 && std::string_view{argv[1]} == std::string_view{"--full"}
        ? 256u : 120u;
    const fvm::TridiagonalSystem1D sistema = sistema_phi_um(n);
    const fvm::DenseVector exata(n, 1.0);

    std::cout << "Exercicio 6.5 - criterios de parada\n\n";
    std::cout << std::setw(30) << "criterio"
              << std::setw(14) << "epsilon"
              << std::setw(12) << "iter"
              << std::setw(12) << "conv."
              << std::setw(18) << "valor crit."
              << std::setw(18) << "residuo"
              << std::setw(18) << "erro real" << '\n';

    const Real tolerancias[] = {
        1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11
    };
    const struct {
        std::string_view nome;
        fvm::StopCriterionKind tipo;
    } criterios[] = {
        {"dif. absoluta", fvm::StopCriterionKind::correction_absolute},
        {"dif. relativa", fvm::StopCriterionKind::correction_relative_solution},
        {"dif. relativa total", fvm::StopCriterionKind::correction_relative_range},
        {"residuo absoluto", fvm::StopCriterionKind::residual_absolute},
        {"residuo relativo inicial", fvm::StopCriterionKind::residual_relative_initial},
    };

    for (const auto& c : criterios) {
        for (Real eps : tolerancias) resolver(c.nome, c.tipo, eps, sistema, exata);
        std::cout << '\n';
    }
}
