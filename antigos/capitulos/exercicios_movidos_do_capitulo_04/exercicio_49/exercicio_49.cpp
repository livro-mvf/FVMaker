// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 4.9
// Titulo: Primeiro contato: malha face centrada.
//
// Objetivo:
//   Usar a FVGridMaker para gerar uma malha face centrada com N = 50 volumes
//   em [0, 1], obter faces e centros e verificar que cada face interna e o
//   ponto medio entre os centros vizinhos.
//
// Modelo numerico:
//   A distribuicao uniforme fornece os centros primarios da malha face
//   centrada. A FVGridMaker reconstrói as faces internas pela media entre
//   centros consecutivos e impoe as faces de contorno no dominio.
//
// Verificacoes:
//   O programa confirma o ponto medio nas faces internas, calcula a razao
//   global max(Delta xP)/min(Delta xP), a suavidade local G_i e gera os
//   arquivos de relatorio, CSV e grafico SVG.

#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/FaceCentered1D.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

using Real = fvgrid::Real;

constexpr Real kTol = 1.0e-12;

struct EstatisticasMalha
{
    Real dx_min{};
    Real dx_max{};
    Real soma{};
    Real razao_global{};
    Real suavidade_max{};
    Real suavidade_media{};
};

[[nodiscard]] bool perto(Real a, Real b)
{
    return std::abs(a - b)
        <= kTol * std::max<Real>({Real{1.0}, std::abs(a), std::abs(b)});
}

[[nodiscard]] bool registrar_teste(
    const std::string& descricao,
    bool passou
)
{
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
    return passou;
}

[[nodiscard]] Real suavidade_local(
    const std::vector<Real>& dx,
    std::size_t i
)
{
    if (i == 0) {
        return Real{};
    }

    return std::abs(dx[i] / dx[i - 1] - Real{1.0});
}

[[nodiscard]] EstatisticasMalha calcular_estatisticas(
    const std::vector<Real>& dx
)
{
    const auto [menor, maior] = std::minmax_element(dx.begin(), dx.end());
    const Real soma = std::accumulate(dx.begin(), dx.end(), Real{});

    Real soma_suavidade{};
    Real max_suavidade{};

    for (std::size_t i = 1; i < dx.size(); ++i) {
        const Real g = suavidade_local(dx, i);
        soma_suavidade += g;
        max_suavidade = std::max(max_suavidade, g);
    }

    return EstatisticasMalha{
        *menor,
        *maior,
        soma,
        *maior / *menor,
        max_suavidade,
        dx.size() > 1
            ? soma_suavidade / static_cast<Real>(dx.size() - 1)
            : Real{}
    };
}

[[nodiscard]] bool faces_internas_no_ponto_medio(
    const fvgrid::Axis1D& malha
)
{
    const std::vector<Real> faces{malha.faces().begin(), malha.faces().end()};
    const std::vector<Real> centros{
        malha.centers().begin(),
        malha.centers().end()
    };

    for (std::size_t i = 1; i + 1 < faces.size(); ++i) {
        const Real ponto_medio =
            Real{0.5} * (centros[i - 1] + centros[i]);

        if (!perto(faces[i], ponto_medio)) {
            return false;
        }
    }

    return true;
}

void imprimir_malha(const fvgrid::Axis1D& malha)
{
    std::cout << std::setw(6) << "P"
              << std::setw(20) << "xF[P]"
              << std::setw(20) << "xC[P]"
              << std::setw(20) << "xF[P+1]"
              << std::setw(20) << "Delta xP"
              << std::setw(20) << "G_i" << '\n';

    const std::vector<Real> dx{
        malha.cell_lengths().begin(),
        malha.cell_lengths().end()
    };

    for (std::size_t i = 0; i < malha.num_cells(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(20) << malha.faces()[i]
                  << std::setw(20) << malha.centers()[i]
                  << std::setw(20) << malha.faces()[i + 1]
                  << std::setw(20) << dx[i]
                  << std::setw(20) << suavidade_local(dx, i)
                  << '\n';
    }
}

void exportar_csv(const fvgrid::Axis1D& malha)
{
    std::ofstream arquivo{"exercicio_49_espacamentos.csv"};

    if (!arquivo) {
        throw std::runtime_error(
            "Nao foi possivel criar exercicio_49_espacamentos.csv."
        );
    }

    const std::vector<Real> dx{
        malha.cell_lengths().begin(),
        malha.cell_lengths().end()
    };

    arquivo << "P,xF_esquerda,xC,xF_direita,Delta_xP,G\n";

    for (std::size_t i = 0; i < malha.num_cells(); ++i) {
        arquivo << i << ',' << std::setprecision(17)
                << malha.faces()[i] << ','
                << malha.centers()[i] << ','
                << malha.faces()[i + 1] << ','
                << dx[i] << ','
                << suavidade_local(dx, i) << '\n';
    }
}

void exportar_relatorio(
    const fvgrid::Axis1D& malha,
    const EstatisticasMalha& estatisticas,
    bool faces_ok
)
{
    std::ofstream arquivo{"exercicio_49_relatorio.txt"};

    if (!arquivo) {
        throw std::runtime_error(
            "Nao foi possivel criar exercicio_49_relatorio.txt."
        );
    }

    arquivo << std::setprecision(17);
    arquivo << "Exercicio 4.9 - Primeiro contato: malha face centrada\n";
    arquivo << "N = " << malha.num_cells() << '\n';
    arquivo << "L = " << malha.length() << '\n';
    arquivo << "faces internas no ponto medio = "
            << (faces_ok ? "sim" : "nao") << '\n';
    arquivo << "min(Delta xP) = " << estatisticas.dx_min << '\n';
    arquivo << "max(Delta xP) = " << estatisticas.dx_max << '\n';
    arquivo << "razao global  = " << estatisticas.razao_global << '\n';
    arquivo << "max G_i       = " << estatisticas.suavidade_max << '\n';
    arquivo << "media G_i     = " << estatisticas.suavidade_media << '\n';
}

void exportar_svg(
    const fvgrid::Axis1D& malha,
    const EstatisticasMalha& estatisticas
)
{
    std::ofstream arquivo{"exercicio_49_distribuicao_dx.svg"};

    if (!arquivo) {
        throw std::runtime_error(
            "Nao foi possivel criar exercicio_49_distribuicao_dx.svg."
        );
    }

    constexpr int largura = 900;
    constexpr int altura = 360;
    constexpr int margem = 48;

    const std::vector<Real> dx{
        malha.cell_lengths().begin(),
        malha.cell_lengths().end()
    };

    const Real escala_x =
        static_cast<Real>(largura - 2 * margem) / static_cast<Real>(dx.size());

    const Real escala_y =
        static_cast<Real>(altura - 2 * margem) / estatisticas.dx_max;

    arquivo << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\""
            << largura << "\" height=\"" << altura << "\" viewBox=\"0 0 "
            << largura << ' ' << altura << "\">\n";
    arquivo << "<rect width=\"100%\" height=\"100%\" fill=\"white\"/>\n";
    arquivo << "<text x=\"48\" y=\"28\" font-family=\"Arial\" "
            << "font-size=\"18\">Exercicio 4.9 - distribuicao de Delta xP"
            << "</text>\n";
    arquivo << "<line x1=\"" << margem << "\" y1=\"" << altura - margem
            << "\" x2=\"" << largura - margem << "\" y2=\""
            << altura - margem << "\" stroke=\"black\"/>\n";
    arquivo << "<line x1=\"" << margem << "\" y1=\"" << margem
            << "\" x2=\"" << margem << "\" y2=\"" << altura - margem
            << "\" stroke=\"black\"/>\n";

    for (std::size_t i = 0; i < dx.size(); ++i) {
        const Real barra_altura = dx[i] * escala_y;
        const Real x = static_cast<Real>(margem) + static_cast<Real>(i) * escala_x;
        const Real y = static_cast<Real>(altura - margem) - barra_altura;

        arquivo << "<rect x=\"" << x << "\" y=\"" << y
                << "\" width=\"" << std::max<Real>(1.0, escala_x - 1.0)
                << "\" height=\"" << barra_altura
                << "\" fill=\"#377eb8\"/>\n";
    }

    arquivo << "</svg>\n";
}

void imprimir_mensagem_final()
{
    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << "--------------------------\n";
    std::cout << "1. A malha face centrada e definida pelos centros; ";
    std::cout << "as faces internas ficam no ponto medio entre centros ";
    std::cout << "vizinhos.\n";
    std::cout << "2. O relatorio resume a razao global e a suavidade ";
    std::cout << "local da malha.\n";
    std::cout << "3. O CSV e o SVG gerados servem como entregaveis ";
    std::cout << "para inspecao e pos-processamento.\n";
}

} // namespace

int main()
{
    try {
        constexpr std::size_t n = 50;
        constexpr Real x_inicial = 0.0;
        constexpr Real comprimento = 1.0;

        const fvgrid::Axis1D malha = fvgrid::Uniform1D::make(
            fvgrid::NVol{n},
            fvgrid::Length{comprimento},
            fvgrid::XInit{x_inicial},
            fvgrid::FaceCentered1D{}
        );

        const std::vector<Real> dx{
            malha.cell_lengths().begin(),
            malha.cell_lengths().end()
        };

        const EstatisticasMalha estatisticas = calcular_estatisticas(dx);
        const bool faces_ok = faces_internas_no_ponto_medio(malha);

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "Exercicio Computacional 4.9\n";
        std::cout << "Primeiro contato: malha face centrada\n";
        std::cout << "=====================================\n\n";
        std::cout << "N = " << n << ", dominio = ["
                  << x_inicial << ", " << x_inicial + comprimento
                  << "]\n\n";

        imprimir_malha(malha);

        unsigned aprovados{};
        unsigned total{};

        std::cout << "\nVerificacoes automaticas\n";
        std::cout << "========================\n";

        ++total;
        aprovados += registrar_teste(
            "faces internas no ponto medio entre centros vizinhos",
            faces_ok
        );

        ++total;
        aprovados += registrar_teste(
            "sum Delta xP = L",
            perto(estatisticas.soma, comprimento)
        );

        ++total;
        aprovados += registrar_teste(
            "Delta xP > 0",
            estatisticas.dx_min > Real{}
        );

        std::cout << "\nRelatorio numerico\n";
        std::cout << "==================\n";
        std::cout << "min(Delta xP) = " << estatisticas.dx_min << '\n';
        std::cout << "max(Delta xP) = " << estatisticas.dx_max << '\n';
        std::cout << "razao global  = " << estatisticas.razao_global << '\n';
        std::cout << "max G_i       = " << estatisticas.suavidade_max << '\n';
        std::cout << "media G_i     = " << estatisticas.suavidade_media << '\n';

        exportar_csv(malha);
        exportar_relatorio(malha, estatisticas, faces_ok);
        exportar_svg(malha, estatisticas);

        std::cout << "\nArquivos gerados\n";
        std::cout << "================\n";
        std::cout << "exercicio_49_espacamentos.csv\n";
        std::cout << "exercicio_49_relatorio.txt\n";
        std::cout << "exercicio_49_distribuicao_dx.svg\n";

        imprimir_mensagem_final();

        return (aprovados == total) ? 0 : 1;
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}
