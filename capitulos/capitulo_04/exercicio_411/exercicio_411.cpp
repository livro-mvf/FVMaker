//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// Exercício Computacional 4.11
// Aleatórias: qual arranjo transiciona pior?
//==============================================================================

#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>

namespace {

using Real = fvgrid::Real;

constexpr Real kTol = 1.0e-12;

struct Estatisticas {
    Real minimo{};
    Real maximo{};
    Real soma{};
    Real razao{};
    Real g_max{};
    Real g_medio{};
};

[[nodiscard]] bool perto(Real a, Real b) {
    return std::abs(a - b) <= kTol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] Real g_local(std::span<const Real> dx, std::size_t i) {
    if (i == 0) {
        return Real{};
    }

    return std::abs(dx[i] / dx[i - 1] - Real{1});
}

[[nodiscard]] Estatisticas calcular_estatisticas(std::span<const Real> dx) {
    const auto [menor, maior] = std::minmax_element(dx.begin(), dx.end());
    const Real soma = std::accumulate(dx.begin(), dx.end(), Real{});

    Real g_soma{};
    Real g_max{};

    for (std::size_t i = 1; i < dx.size(); ++i) {
        const Real g = g_local(dx, i);
        g_soma += g;
        g_max = std::max(g_max, g);
    }

    return Estatisticas{
        *menor,
        *maior,
        soma,
        *maior / *menor,
        g_max,
        dx.size() > 1 ? g_soma / static_cast<Real>(dx.size() - 1) : Real{}
    };
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

void imprimir_estatisticas(const std::string& nome, const Estatisticas& e) {
    std::cout << nome << '\n';
    std::cout << "  min(Delta xP)               = " << e.minimo << '\n';
    std::cout << "  max(Delta xP)               = " << e.maximo << '\n';
    std::cout << "  sum(Delta xP)               = " << e.soma << '\n';
    std::cout << "  razão global max/min        = " << e.razao << '\n';
    std::cout << "  max G_i                     = " << e.g_max << '\n';
    std::cout << "  média G_i                   = " << e.g_medio << "\n\n";
}

void exportar_csv(const fvgrid::Axis1D& volume, const fvgrid::Axis1D& face) {
    std::ofstream arquivo{"exercicio_411_malhas_aleatorias.csv"};

    if (!arquivo) {
        throw std::runtime_error("Não foi possível criar exercicio_411_malhas_aleatorias.csv.");
    }

    arquivo << "arranjo,i,xC,dxP,G\n";

    const auto escrever = [&arquivo](const std::string& nome, const fvgrid::Axis1D& eixo) {
        for (std::size_t i = 0; i < eixo.num_cells(); ++i) {
            arquivo << nome << ',' << i << ',' << std::setprecision(17)
                    << eixo.centers()[i] << ','
                    << eixo.cell_lengths()[i] << ','
                    << g_local(eixo.cell_lengths(), i) << '\n';
        }
    };

    escrever("volume_centrada", volume);
    escrever("face_centrada", face);
}

}  // namespace

int main() {
    constexpr std::size_t n = 50;
    constexpr Real x_inicial = 0.0;
    constexpr Real comprimento = 1.0;
    constexpr std::uint64_t seed_volume = 20260411;
    constexpr std::uint64_t seed_face = 20260412;
    constexpr Real espacamento_minimo = 1.0e-4;

    const fvgrid::Axis1D volume_centrada = fvgrid::Random1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::Seed{seed_volume},
        fvgrid::MinSpacing{espacamento_minimo},
        fvgrid::VolumeCentered1D{}
    );

    const fvgrid::Axis1D face_centrada = fvgrid::Random1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::Seed{seed_face},
        fvgrid::MinSpacing{espacamento_minimo},
        fvgrid::FaceCentered1D{}
    );

    const Estatisticas estat_volume = calcular_estatisticas(volume_centrada.cell_lengths());
    const Estatisticas estat_face = calcular_estatisticas(face_centrada.cell_lengths());

    std::cout << "Exercício 4.11 - malhas aleatórias\n";
    std::cout << "N = " << n << ", domínio = [" << x_inicial << ", "
              << x_inicial + comprimento << "]\n\n";

    imprimir_estatisticas("Volume centrada", estat_volume);
    imprimir_estatisticas("Face centrada", estat_face);

    std::cout << "Verificações automáticas\n";
    std::cout << "========================\n";

    unsigned aprovados{};
    unsigned total{};
    ++total; aprovados += registrar(
        "volume centrada: sum Delta xP_i = L",
        perto(estat_volume.soma, comprimento)
    );
    ++total; aprovados += registrar(
        "face centrada: sum Delta xP_i = L",
        perto(estat_face.soma, comprimento)
    );

    const std::string pior =
        estat_face.g_max > estat_volume.g_max ? "face centrada" : "volume centrada";

    std::cout << "\nDiscussão automática\n";
    std::cout << "====================\n";
    std::cout << "Nesta realização, a maior transição local ocorreu na malha "
              << pior << ". Essa é a malha que tende a exigir mais cuidado nas "
              << "fórmulas não uniformes, pois saltos abruptos em Delta xP "
              << "aumentam os termos locais de erro.\n";

    exportar_csv(volume_centrada, face_centrada);
    std::cout << "CSV gerado: exercicio_411_malhas_aleatorias.csv\n";

    return aprovados == total ? 0 : 1;
}
