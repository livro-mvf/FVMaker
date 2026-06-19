//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.2
// Malha, montagem do sistema e solucao pelo TDMA
//==============================================================================

#include <cmath>
#include <iomanip>
#include <iostream>

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
    const auto inferior = sistema.lower();
    const auto principal = sistema.diagonal();
    const auto superior = sistema.upper();
    const auto fonte = sistema.rhs().values();

    std::cout << std::setw(6) << "P"
              << std::setw(18) << "A_W"
              << std::setw(18) << "A_P"
              << std::setw(18) << "A_E"
              << std::setw(18) << "B_P" << '\n';

    for (Size p = 0; p < sistema.size(); ++p) {
        const Real aw = p > 0 ? inferior[p - 1] : Real{};
        const Real ae = p + 1 < sistema.size() ? superior[p] : Real{};

        std::cout << std::setw(6) << p
                  << std::setw(18) << aw
                  << std::setw(18) << principal[p]
                  << std::setw(18) << ae
                  << std::setw(18) << fonte[p] << '\n';
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


//==============================================================================
// Geração e impressão da malha
//==============================================================================

    // 1. Geracao da malha pela FVGridMaker.
    const fvgrid::Axis1D eixo = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{5.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D malha{eixo};

    std::cout << "1. Malha gerada pela FVGridMaker\n";
    std::cout << "================================\n";
    std::cout << eixo << "\n\n";

//==============================================================================
// Definição e impressão das condições de contorno
//==============================================================================


    // Condicoes de contorno declaradas antes da equacao.
    const fvm::BoundaryCondition1D contorno_esquerdo =
        fvm::dirichlet_1d(1.0);
    const fvm::BoundaryCondition1D contorno_direito =
        fvm::neumann_1d(0.0);

    std::cout << "Condicoes de contorno\n";
    std::cout << "======================\n";
    std::cout << "oeste: Dirichlet, phi(0) = 1\n";
    std::cout << "leste: Neumann, dphi/dx(5) = 0\n\n";

auto boundary_condition = fvm::BoundarySet1D{contorno_esquerdo, contorno_direito};

    std::cout << "1. Condicoes de contorno\n";
    // std::cout << boundary_condition << "\n\n";

//==============================================================================
// Definicao da função termo fonte do problema
//==============================================================================

auto source_function = [](Real x) { return -x * x;};

//==============================================================================
// Definicao e impressão dos coeficientes do sistema de equações para um laplaciano 1D
//==============================================================================

    const fvm::Equation1D 
        equacao(    malha,
                    fvm::Laplacian1D{},
                    source_function,
                    boundary_condition
        );


    // std::cout << equacao << "\n\n";

//==============================================================================
// Definicao dos coeficientes do sistema de equações para um laplaciano 1D
//==============================================================================


    const fvm::TridiagonalSystem1D sistema =
        fvm::assemble_steady_1d(equacao);

    std::cout << "2. Sistema de equacoes montado pela FVMaker\n";
    std::cout << "===========================================\n";
    imprimir_sistema(sistema);
    std::cout << '\n';

    // 3. Solucao numerica pelo TDMA da FVMaker.
    const fvm::SolveResult resultado = fvm::TDMA::solve(sistema);

    std::cout << "3. Solver fvm::TDMA\n";
    std::cout << "===================\n";
    std::cout << "convergiu = " << (resultado.converged ? "sim" : "nao") << '\n';
    std::cout << "residuo   = " << resultado.residual_norm << "\n\n";

    // 4. Coeficientes da varredura direta fornecidos pelo proprio TDMA.
    const fvm::TDMAForwardSweep varredura =
        fvm::TDMA::forward_sweep(sistema);

    std::cout << "4. Coeficientes T e Q fornecidos por fvm::TDMA\n";
    std::cout << "==============================================\n";
    std::cout << std::setw(6) << "P"
              << std::setw(18) << "T[P]"
              << std::setw(18) << "Q[P]" << '\n';

    for (Size p = 0; p < varredura.t.size(); ++p) {
        std::cout << std::setw(6) << p
                  << std::setw(18) << varredura.t[p]
                  << std::setw(18) << varredura.q[p] << '\n';
    }

    // 5. Solucoes numerica e analitica na Variable1D da FVMaker.
    fvm::Variable1D phi{malha, "phi"};
    phi.update_phi(resultado);
    phi.assign_analytic([](Real x) {
        return -std::pow(x, Real{4}) / Real{12}
             + Real{125} * x / Real{3}
             + Real{1};
    });

    std::cout << "\n5. Solucao analitica e solucao numerica\n";
    std::cout << "=======================================\n";
    std::cout << std::setprecision(8) << phi << '\n';

    const fvm::VariableDifferenceNorms1D erro = phi.norms_to_analytic();
    std::cout << "\nNormas de phi - phi_analit\n";
    std::cout << "infinita = " << erro.infinity << '\n';
    std::cout << "L1       = " << erro.l1 << '\n';
    std::cout << "L2       = " << erro.l2 << '\n';

    const bool solver_ok = registrar(
        "fvm::TDMA convergiu",
        resultado.converged
    );
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
