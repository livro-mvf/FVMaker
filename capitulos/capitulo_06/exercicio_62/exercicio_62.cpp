// ============================================================================
// Arquivo: exercicio_62.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 62 no contexto de capitulos / capitulo_06 / exercicio_62.
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

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>

namespace {
using Real = fvm::Real;
using Size = fvm::Size;
constexpr Real kTolerance = 1.0e-10;

void imprimir_sistema(const fvm::TridiagonalSystem1D& sistema) {
    std::cout << std::setw(6) << "P"
              << std::setw(18) << "A_W"
              << std::setw(18) << "A_P"
              << std::setw(18) << "A_E"
              << std::setw(18) << "B_P" << '\n';
    for (Size p = 0; p < sistema.size(); ++p) {
        const Real aw = p > 0 ? sistema.lower()[p - 1] : 0.0;
        const Real ae = p + 1 < sistema.size() ? sistema.upper()[p] : 0.0;
        std::cout << std::setw(6) << p
                  << std::setw(18) << aw
                  << std::setw(18) << -sistema.diagonal()[p]
                  << std::setw(18) << ae
                  << std::setw(18) << -sistema.rhs()[p] << '\n';
    }
}

bool registrar(std::string_view descricao, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}
}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Exercicio 6.2 - sistema tridiagonal\n\n";

    // 1. Geracao da malha.
    const fvgrid::Axis1D eixo = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{5.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D malha{eixo};

    std::cout << "1. Malha gerada pela FVGridMaker\n"
              << "================================\n"
              << eixo << "\n\n";

    // 2. Condicoes de contorno explicitas.
    const fvm::BoundaryCondition1D contorno_esquerdo =
        fvm::dirichlet_1d(1.0);
    const fvm::BoundaryCondition1D contorno_direito =
        fvm::neumann_1d(0.0);
    const fvm::BoundarySet1D contornos{
        contorno_esquerdo,
        contorno_direito
    };

    std::cout << "2. Condicoes de contorno\n"
              << "=========================\n"
              << "oeste: Dirichlet, phi(0) = 1\n"
              << "leste: Neumann, dphi/dx(5) = 0\n\n";

    // 3. Definicao e montagem da equacao.
    const fvm::Equation1D equacao{
        malha,
        fvm::Laplacian1D{},
        fvm::function_source_1d(malha, [](Real x) { return -x * x; }),
        contornos
    };
    const fvm::TridiagonalSystem1D sistema =
        fvm::assemble_steady_1d(equacao);

    std::cout << "3. Sistema de equacoes montado pela FVMaker\n"
              << "===========================================\n";
    imprimir_sistema(sistema);
    std::cout << '\n';

    // 4. Solucao numerica pelo TDMA.
    const fvm::SolveResult resultado = fvm::TDMA::solve(sistema);
    std::cout << "4. Solver fvm::TDMA\n"
              << "===================\n"
              << "convergiu = " << (resultado.converged ? "sim" : "nao") << '\n'
              << "residuo   = " << resultado.residual_norm << "\n\n";

    // 5. Coeficientes da varredura direta.
    const fvm::TDMAForwardSweep varredura =
        fvm::TDMA::forward_sweep(sistema);
    std::cout << "5. Coeficientes T e Q fornecidos por fvm::TDMA\n"
              << "==============================================\n"
              << std::setw(6) << "P"
              << std::setw(18) << "T[P]"
              << std::setw(18) << "Q[P]" << '\n';
    for (Size p = 0; p < varredura.t.size(); ++p) {
        std::cout << std::setw(6) << p
                  << std::setw(18) << varredura.t[p]
                  << std::setw(18) << varredura.q[p] << '\n';
    }

    // 6. Solucoes numerica e analitica.
    fvm::Variable1D phi{malha, "phi"};
    phi.update_phi(resultado);
    phi.assign_analytic([](Real x) {
        return -std::pow(x, Real{4}) / Real{12}
             + Real{125} * x / Real{3}
             + Real{1};
    });

    std::cout << "\n6. Solucao analitica e solucao numerica\n"
              << "=======================================\n"
              << std::setprecision(8) << phi << '\n';

    const fvm::VariableDifferenceNorms1D erro = phi.norms_to_analytic();
    std::cout << "\nNormas de phi - phi_analit\n"
              << "infinita = " << erro.infinity << '\n'
              << "L1       = " << erro.l1 << '\n'
              << "L2       = " << erro.l2 << '\n';

    const bool solver_ok = registrar("fvm::TDMA convergiu", resultado.converged);
    const bool residuo_ok = registrar(
        "residuo do sistema e pequeno",
        resultado.residual_norm <= kTolerance
    );
    const bool t_ok = registrar(
        "ultimo coeficiente T e nulo",
        std::abs(varredura.t[varredura.t.size() - 1]) <= kTolerance
    );
    return solver_ok && residuo_ok && t_ok ? 0 : 1;
}
