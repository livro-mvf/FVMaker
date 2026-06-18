//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.5
// Termo fonte: a aproximacao contra o valor exato
//==============================================================================

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include "../comum/mvf_capitulo_05.h"

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

namespace {

using Real = capitulo_05::Real;
using Size = capitulo_05::Size;

[[nodiscard]] Real integral_x2(Real a, Real b) {
    return (b * b * b - a * a * a) / Real{3};
}

[[nodiscard]] Real primitiva_exp_sin(Real x) {
    return Real{0.5} * std::exp(x) * (std::sin(x) - std::cos(x));
}

[[nodiscard]] Real integral_exp_sin(Real a, Real b) {
    return primitiva_exp_sin(b) - primitiva_exp_sin(a);
}

template <class Function, class Integral>
[[nodiscard]] Real erro_total_fonte(
    const fvgrid::Axis1D& eixo,
    Function&& f,
    Integral&& integral
) {
    Real erro_total{};

    for (Size p = 0; p < eixo.num_cells(); ++p) {
        const Real aproximado = capitulo_05::fonte_um_ponto(eixo, p, f);
        const Real exato = integral(eixo.faces()[p], eixo.faces()[p + 1]);
        erro_total += std::abs(aproximado - exato);
    }

    return erro_total;
}

template <class Function, class Integral>
void imprimir_fonte_por_volume(
    std::string_view titulo,
    const fvgrid::Axis1D& eixo,
    Function&& f,
    Integral&& integral
) {
    std::cout << titulo << '\n';
    std::cout << "P              B_P          integral          erro\n";

    for (Size p = 0; p < eixo.num_cells(); ++p) {
        const Real aproximado = capitulo_05::fonte_um_ponto(eixo, p, f);
        const Real exato = integral(eixo.faces()[p], eixo.faces()[p + 1]);

        std::cout << std::setw(2) << p
                  << std::setw(16) << aproximado
                  << std::setw(16) << exato
                  << std::setw(16) << std::abs(aproximado - exato)
                  << '\n';
    }
}

void caso_x2_uniforme() {
    const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
        fvgrid::NVol{5},
        fvgrid::Length{5.0},
        fvgrid::XInit{0.0},
        fvgrid::FaceCentered1D{}
    );

    imprimir_fonte_por_volume(
        "f(x) = x^2, malha uniforme, N = 5",
        eixo,
        [](Real x) { return x * x; },
        integral_x2
    );
}

void ordem_x2_uniforme() {
    std::cout << "\nErro local maximo de x^2 contra Delta x^3\n";
    std::cout << "N              dx        erro_max     erro/dx^3\n";

    for (Size n : {5u, 10u, 20u, 40u}) {
        const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
            fvgrid::NVol{n},
            fvgrid::Length{5.0},
            fvgrid::XInit{0.0},
            fvgrid::FaceCentered1D{}
        );

        Real erro_max{};
        for (Size p = 0; p < eixo.num_cells(); ++p) {
            const Real aproximado =
                capitulo_05::fonte_um_ponto(eixo, p, [](Real x) { return x * x; });
            const Real exato = integral_x2(eixo.faces()[p], eixo.faces()[p + 1]);
            erro_max = std::max(erro_max, std::abs(aproximado - exato));
        }

        const Real dx = eixo.cell_lengths()[0];
        std::cout << std::setw(2) << n
                  << std::setw(16) << dx
                  << std::setw(16) << erro_max
                  << std::setw(16) << erro_max / (dx * dx * dx)
                  << '\n';
    }
}

void ordem_x2_nao_uniforme() {
    std::cout << "\nf(x) = x^2 em malha face centrada nao uniforme\n";
    std::cout << "N        erro_total      ordem_obs\n";

    Real erro_anterior{};
    for (Size n : {10u, 20u, 40u, 80u}) {
        const fvgrid::Axis1D eixo =
            capitulo_05::malha_face_centrada_customizada(
                capitulo_05::centros_potencia(n, 5.0, 0.0, 1.35),
                0.0,
                5.0
            );

        const Real erro = erro_total_fonte(
            eixo,
            [](Real x) { return x * x; },
            integral_x2
        );

        Real ordem{};
        if (erro_anterior > Real{}) {
            ordem = std::log(erro_anterior / erro) / std::log(Real{2});
        }

        std::cout << std::setw(2) << n
                  << std::setw(18) << erro
                  << std::setw(15) << ordem << '\n';

        erro_anterior = erro;
    }
}

void caso_exp_sin() {
    std::cout << "\nf(x) = exp(x) sin(x), erro total\n";
    std::cout << "N              erro      ordem_obs\n";

    Real erro_anterior{};
    for (Size n : {10u, 20u, 40u, 80u}) {
        const fvgrid::Axis1D eixo = fvgrid::Uniform1D::make(
            fvgrid::NVol{n},
            fvgrid::Length{1.0},
            fvgrid::XInit{0.0},
            fvgrid::FaceCentered1D{}
        );

        const Real erro_total = erro_total_fonte(
            eixo,
            [](Real x) { return std::exp(x) * std::sin(x); },
            integral_exp_sin
        );

        Real ordem{};
        if (erro_anterior > Real{}) {
            ordem = std::log(erro_anterior / erro_total) / std::log(Real{2});
        }

        std::cout << std::setw(2) << n
                  << std::setw(18) << erro_total
                  << std::setw(15) << ordem << '\n';

        erro_anterior = erro_total;
    }
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Exercicio 5.5 - termo fonte\n\n";

    caso_x2_uniforme();
    ordem_x2_uniforme();
    ordem_x2_nao_uniforme();
    caso_exp_sin();

    return 0;
}
