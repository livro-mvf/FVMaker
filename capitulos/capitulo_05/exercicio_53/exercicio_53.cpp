//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.3
// O coeficiente de uma face, visto dos dois lados
//==============================================================================


//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_05.h"


int main() {
    using capitulo_05::Real;
    using capitulo_05::Size;

    std::cout << std::fixed << std::setprecision(12);

    const Real x_inicial = 0.0;
    const Real comprimento = 5.0;
    const Size n = 8;

    const fvgrid::Axis1D eixo =
        capitulo_05::malha_face_centrada_customizada(
            capitulo_05::centros_potencia(n, comprimento, x_inicial, 1.35),
            x_inicial,
            comprimento
        );

    const fvm::EquationContribution1D coeficientes =
        capitulo_05::montar_coeficientes_poisson(
            eixo,
            [](Real x) { return x * x; },
            fvm::dirichlet_1d(1.0),
            fvm::neumann_1d(0.0)
        );

    std::cout << "Exercicio 5.3 - coeficiente de uma face interna\n\n";
    std::cout << eixo << "\n\n";
    std::cout << coeficientes << "\n\n";

    std::cout << "Comparacao A_E[i] contra A_W[i+1]\n";
    std::cout << "=================================\n";
    std::cout << "face"
              << std::setw(18) << "A_E[i]"
              << std::setw(18) << "A_W[i+1]"
              << std::setw(18) << "diferenca" << '\n';

    bool identidade = true;

    for (Size i = 0; i + 1 < n; ++i) {
        const Real diferenca =
            std::abs(coeficientes.ae()[i] - coeficientes.aw()[i + 1]);

        identidade = identidade && capitulo_05::perto(diferenca, Real{});

        std::cout << std::setw(4) << i
                  << std::setw(18) << coeficientes.ae()[i]
                  << std::setw(18) << coeficientes.aw()[i + 1]
                  << std::setw(18) << diferenca << '\n';
    }

    const bool contornos_zerados =
        capitulo_05::perto(coeficientes.aw()[0], Real{}) &&
        capitulo_05::perto(coeficientes.ae()[n - 1], Real{});

    bool erro_provocado_detectado = false;
    for (Size i = 0; i + 1 < n; ++i) {
        const Real ae_errado = coeficientes.ae()[i] * Real{1.05};
        if (!capitulo_05::perto(ae_errado, coeficientes.aw()[i + 1])) {
            erro_provocado_detectado = true;
            break;
        }
    }

    unsigned aprovados{};
    unsigned total{};

    ++total;
    aprovados += capitulo_05::registrar(
        "A_E[i] = A_W[i+1] em todas as faces internas",
        identidade
    );

    ++total;
    aprovados += capitulo_05::registrar(
        "A_W[0] e A_E[N-1] sao zeros de contorno",
        contornos_zerados
    );

    ++total;
    aprovados += capitulo_05::registrar(
        "teste de regressao detecta erro provocado",
        erro_provocado_detectado
    );

    std::cout << "\nOs zeros nas extremidades nao sao coeficientes de faces "
              << "internas; eles indicam que nao ha volume vizinho fora "
              << "do dominio.\n";

    return aprovados == total ? 0 : 1;
}
