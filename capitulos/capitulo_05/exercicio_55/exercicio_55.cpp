//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercicio Computacional 5.5
// Consistencia vista pelo residuo
//==============================================================================

#include "../comum/mvf_capitulo_05.h"

#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Domain1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {

using Real = capitulo_05::Real;
using Size = capitulo_05::Size;

[[nodiscard]] Real solucao(Real x) {
    return -Real{0.5} * x * x + x;
}

[[nodiscard]] std::vector<Real> faces_nao_uniformes(Size n) {
    std::vector<Real> faces(n + 1);

    for (Size i = 0; i <= n; ++i) {
        const Real s = static_cast<Real>(i) / static_cast<Real>(n);
        faces[i] = s * s * s;
    }

    return faces;
}

[[nodiscard]] std::vector<Real> centros_nao_uniformes(Size n) {
    std::vector<Real> centros(n);

    for (Size i = 0; i < n; ++i) {
        const Real s =
            (static_cast<Real>(i) + Real{0.5}) / static_cast<Real>(n);
        centros[i] = s * s;
    }

    return centros;
}

[[nodiscard]] Real residuo_maximo(const fvgrid::Axis1D& eixo) {
    Real maximo{};

    for (Size p = 1; p + 1 < eixo.num_cells(); ++p) {
        const Real phi_w = solucao(eixo.centers()[p - 1]);
        const Real phi_p = solucao(eixo.centers()[p]);
        const Real phi_e = solucao(eixo.centers()[p + 1]);

        const Real fluxo_e =
            (phi_e - phi_p) / (eixo.centers()[p + 1] - eixo.centers()[p]);
        const Real fluxo_w =
            (phi_p - phi_w) / (eixo.centers()[p] - eixo.centers()[p - 1]);
        const Real bp = eixo.cell_lengths()[p];
        const Real residuo = fluxo_e - fluxo_w + bp;

        maximo = std::max(maximo, std::abs(residuo));
    }

    return maximo;
}

void imprimir_linha(Size n, Real erro_fc, Real erro_vc) {
    std::cout << std::setw(4) << n
              << std::setw(20) << erro_fc
              << std::setw(20) << erro_vc << '\n';
}

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Exercicio 5.5 - residuo de truncamento interno\n\n";
    std::cout << "N       face centrada     volume centrada\n";

    for (Size n : {8u, 16u, 32u, 64u}) {
        const fvgrid::Domain1D dominio =
            fvgrid::Domain1D::from_length(fvgrid::XInit{0.0}, fvgrid::Length{1.0});

        const fvgrid::Axis1D face_centrada = fvgrid::Custom1D::make(
            fvgrid::Coordinates1D::centers(centros_nao_uniformes(n)),
            fvgrid::FaceCentered1D{},
            dominio
        );

        const fvgrid::Axis1D volume_centrada = fvgrid::Custom1D::make(
            fvgrid::Coordinates1D::faces(faces_nao_uniformes(n)),
            fvgrid::VolumeCentered1D{},
            dominio
        );

        imprimir_linha(
            n,
            residuo_maximo(face_centrada),
            residuo_maximo(volume_centrada)
        );
    }

    std::cout << "\nA malha face centrada elimina o termo de primeira ordem "
              << "do fluxo neste teste quadratico.\n";
    std::cout << "Na volume centrada, a geometria das faces e dos centros "
              << "pode deixar residuo quando a malha nao uniforme e usada.\n";

    return 0;
}
