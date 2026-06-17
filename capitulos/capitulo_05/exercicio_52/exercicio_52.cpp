//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.2
// Condicoes de contorno em uma rotina unica
//==============================================================================

#include "../comum/mvf_capitulo_05.h"

#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>

#include <iomanip>
#include <iostream>
#include <string_view>

namespace {

using Real = capitulo_05::Real;

void testar(
    std::string_view nome,
    fvm::BoundarySide1D lado,
    const fvm::BoundaryCondition1D& condicao,
    Real x_contorno,
    Real distancia,
    Real termo_fonte,
    Real coeficiente_vizinho
) {
    const capitulo_05::CoeficientesContorno c =
        capitulo_05::coeficientes_contorno(
            lado,
            condicao,
            x_contorno,
            distancia,
            termo_fonte,
            coeficiente_vizinho
        );

    std::cout << nome << '\n';
    std::cout << "  alpha = " << condicao.alpha(x_contorno)
              << ", beta = " << condicao.beta(x_contorno)
              << ", gamma = " << condicao.gamma(x_contorno) << '\n';
    std::cout << "  A_W = " << c.aw
              << ", A_E = " << c.ae
              << ", A_P = " << c.ap
              << ", B_P = " << c.bp << "\n\n";
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Exercicio 5.2 - contornos por uma rotina unica\n\n";

    testar(
        "Dirichlet a esquerda: phi(0) = 1",
        fvm::BoundarySide1D::left,
        fvm::dirichlet_1d(1.0),
        0.0,
        0.5,
        0.25,
        1.0
    );

    testar(
        "Neumann a direita: phi'(5) = 0",
        fvm::BoundarySide1D::right,
        fvm::neumann_1d(0.0),
        5.0,
        0.5,
        20.25,
        1.0
    );

    testar(
        "Robin a esquerda: 2 phi - phi' = 3",
        fvm::BoundarySide1D::left,
        fvm::robin_1d(2.0, -1.0, 3.0),
        0.0,
        0.25,
        0.50,
        4.0
    );

    std::cout << "A mesma rotina mudou apenas alpha, beta, gamma e o lado.\n";

    return 0;
}
