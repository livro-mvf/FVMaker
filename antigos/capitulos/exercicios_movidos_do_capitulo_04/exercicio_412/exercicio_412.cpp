// ============================================================================
// Arquivo: exercicio_412.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 412 no contexto de antigos / capitulos / exercicios_movidos_do_capitulo_04 / exercicio_412.
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

#include <FVGridMaker/OneDimensional/Distribution1D/Roberts1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <algorithm>
#include <array>
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

struct Caso {
    std::string nome;
    Real beta;
};

[[nodiscard]] bool perto(Real a, Real b, Real tol = kTol) {
    return std::abs(a - b) <=
           tol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] fvgrid::Axis1D eixo_roberts(
    std::size_t n,
    Real comprimento,
    Real beta
) {
    // -------------------------------------------------------------------------
    // PONTO CENTRAL DO EXERCÍCIO
    // A malha de Roberts é gerada diretamente pela FVGridMaker.
    //
    // Roberts1D transforma as coordenadas normalizadas eta em coordenadas
    // físicas. VolumeCentered1D informa que essas coordenadas primárias são
    // faces, e a biblioteca completa a Axis1D com xC, Dx e dx.
    // -------------------------------------------------------------------------
    return fvgrid::Roberts1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{0.0},
        fvgrid::Beta{beta},
        fvgrid::VolumeCentered1D{}
    );
}

[[nodiscard]] Real razao_min_max(std::span<const Real> dx) {
    const auto [menor, maior] = std::minmax_element(dx.begin(), dx.end());
    return *menor / *maior;
}

[[nodiscard]] Real soma(std::span<const Real> valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool simetria_faces(const fvgrid::Axis1D& eixo) {
    const Real comprimento = eixo.length();
    const std::size_t n = eixo.num_faces() - 1;

    for (std::size_t i = 0; i <= n; ++i) {
        if (!perto(eixo.faces()[i], comprimento - eixo.faces()[n - i], 1.0e-11)) {
            return false;
        }
    }

    return true;
}

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

void exportar_razao(std::size_t n, Real comprimento) {
    std::ofstream arquivo{"exercicio_412_razao_roberts.csv"};

    if (!arquivo) {
        throw std::runtime_error("Não foi possível criar exercicio_412_razao_roberts.csv.");
    }

    arquivo << "beta,razao_min_max\n";

    constexpr std::size_t amostras = 40;
    constexpr Real beta_min = 1.1;
    constexpr Real beta_max = 20.0;

    for (std::size_t k = 0; k < amostras; ++k) {
        const Real t = static_cast<Real>(k) / static_cast<Real>(amostras - 1);
        const Real beta = beta_min + t * (beta_max - beta_min);
        const fvgrid::Axis1D eixo = eixo_roberts(n, comprimento, beta);

        arquivo << std::setprecision(17) << beta << ','
                << razao_min_max(eixo.cell_lengths()) << '\n';
    }
}

void exportar_malhas(std::size_t n, Real comprimento, const std::array<Caso, 3>& casos) {
    std::ofstream arquivo{"exercicio_412_malhas_roberts.csv"};

    if (!arquivo) {
        throw std::runtime_error("Não foi possível criar exercicio_412_malhas_roberts.csv.");
    }

    arquivo << "caso,beta,i,xF,xC,Dx\n";

    for (const Caso& caso : casos) {
        const fvgrid::Axis1D eixo = eixo_roberts(n, comprimento, caso.beta);

        for (std::size_t i = 0; i < eixo.num_cells(); ++i) {
            arquivo << caso.nome << ',' << std::setprecision(17)
                    << caso.beta << ',' << i << ','
                    << eixo.faces()[i] << ','
                    << eixo.centers()[i] << ','
                    << eixo.cell_lengths()[i] << '\n';
        }

        arquivo << caso.nome << ',' << caso.beta << ',' << n << ','
                << eixo.faces()[n] << ",,\n";
    }
}

}  // namespace

int main() {
    constexpr std::size_t n = 80;
    constexpr Real comprimento = 1.0;

    const std::array<Caso, 3> casos{{
        {"quase_uniforme", 20.0},
        {"moderado", 4.0},
        {"extremo", 1.1}
    }};

    std::cout << "Exercício 4.12 - refinamento nas fronteiras de Roberts\n";
    std::cout << "N = " << n << ", L = " << comprimento
              << ", alpha = 0.5"
              << "\n\n";

    bool todos_simetria = true;
    bool todos_soma = true;

    for (const Caso& caso : casos) {
        const fvgrid::Axis1D eixo = eixo_roberts(n, comprimento, caso.beta);

        std::cout << caso.nome << " (beta = " << caso.beta << ")\n";
        std::cout << "  min(Dx)/max(Dx) = " << razao_min_max(eixo.cell_lengths()) << '\n';
        std::cout << "  sum(Dx)         = " << soma(eixo.cell_lengths()) << '\n';
        std::cout << "  simetria        = " << (simetria_faces(eixo) ? "sim" : "não") << "\n\n";

        todos_simetria = todos_simetria && simetria_faces(eixo);
        todos_soma = todos_soma && perto(soma(eixo.cell_lengths()), comprimento);
    }

    exportar_razao(n, comprimento);
    exportar_malhas(n, comprimento, casos);

    std::cout << "Verificações automáticas\n";
    std::cout << "========================\n";
    unsigned aprovados{};
    unsigned total{};
    ++total; aprovados += registrar("sum Dx[P] = L nos casos representativos", todos_soma);
    ++total; aprovados += registrar("xF[i] = L - xF[N-i] nos casos representativos", todos_simetria);

    std::cout << "\nCSVs gerados:\n";
    std::cout << "  exercicio_412_razao_roberts.csv\n";
    std::cout << "  exercicio_412_malhas_roberts.csv\n";

    return aprovados == total ? 0 : 1;
}
