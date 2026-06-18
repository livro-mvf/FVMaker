//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 6.7
// O efeito da estimativa inicial
//==============================================================================


//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_06.h"

namespace {

using Real = capitulo_06::Real;
using Size = capitulo_06::Size;

[[nodiscard]] Real residuo_infinito_phi_um(const fvm::DenseVector& phi) {
    const Size n = phi.size();
    Real maximo{};

    for (Size i = 0; i < n; ++i) {
        Real r{};
        if (i == 0) {
            r = Real{3} * phi[i] - phi[i + 1] - Real{2};
        } else if (i + 1 == n) {
            r = phi[i] - phi[i - 1];
        } else {
            r = Real{2} * phi[i] - phi[i - 1] - phi[i + 1];
        }
        maximo = std::max(maximo, std::abs(r));
    }

    return maximo;
}

struct Resultado {
    Size iteracoes{};
    Real residual_inicial{};
    Real residual_final{};
    Real erro{};
};

Resultado resolver(
    Real chute,
    bool relativo,
    Real eps
) {
    constexpr Size n = 80;
    fvm::DenseVector phi{n, chute};
    const Real r0 = residuo_infinito_phi_um(phi);

    for (Size iter = 1; iter <= 500000; ++iter) {
        for (Size i = 0; i < n; ++i) {
            if (i == 0) {
                phi[i] = (Real{2} + phi[i + 1]) / Real{3};
            } else if (i + 1 == n) {
                phi[i] = phi[i - 1];
            } else {
                phi[i] = Real{0.5} * (phi[i - 1] + phi[i + 1]);
            }
        }

        const Real r = residuo_infinito_phi_um(phi);
        const Real valor = relativo ? r / r0 : r;
        if (valor <= eps) {
            const fvm::DenseVector exata =
                capitulo_06::vetor_constante(n, Real{1});
            return Resultado{
                iter,
                r0,
                r,
                capitulo_06::erro_infinito(phi, exata)
            };
        }
    }

    const fvm::DenseVector exata = capitulo_06::vetor_constante(80, Real{1});
    return Resultado{
        500000,
        r0,
        residuo_infinito_phi_um(phi),
        capitulo_06::erro_infinito(phi, exata)
    };
}

void imprimir_linha(std::string_view criterio, Real chute, const Resultado& r) {
    std::cout << std::setw(20) << criterio
              << std::setw(14) << chute
              << std::setw(12) << r.iteracoes
              << std::setw(18) << r.residual_inicial
              << std::setw(18) << r.residual_final
              << std::setw(18) << r.erro << '\n';
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

    constexpr Real eps = 1.0e-8;

    for (Real chute : {0.0, 0.9, 0.99}) {
        imprimir_linha("relativo", chute, resolver(chute, true, eps));
    }

    std::cout << '\n';

    for (Real chute : {0.0, 0.9, 0.99}) {
        imprimir_linha("absoluto", chute, resolver(chute, false, eps));
    }

    return 0;
}
