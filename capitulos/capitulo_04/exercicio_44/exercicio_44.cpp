// ============================================================================
// Arquivo: exercicio_44.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 44 no contexto de capitulos / capitulo_04 / exercicio_44.
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

//==============================================================================
// Header c++
//==============================================================================
#include <iomanip>
#include <iostream>

//==============================================================================
// Header FVGridMaker
//==============================================================================
#include <FVGridMaker/OneDimensional/Distribution1D/Random1D.h>
#include <FVGridMaker/Output/CSV/Axis1DCSVWriter.h>


namespace {

using Real = fvgrid::Real;
using Size = fvgrid::Size;

constexpr Real kTol = 1.0e-9;

// -----------------------------------------------------------------------------
// Métricas de qualidade da malha, seguindo a Seção 4.4 do texto.
// -----------------------------------------------------------------------------
struct Metricas {
    Real dx_min{};
    Real dx_max{};
    Real media{};
    Real desvio_padrao{};
    Real desvio_relativo{};
    Real razao{};
};

[[nodiscard]] bool registrar(const std::string& texto, bool passou) {
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << texto << '\n';
    return passou;
}

[[nodiscard]] Real soma(std::span<const Real> valores) {
    return std::accumulate(valores.begin(), valores.end(), Real{});
}

[[nodiscard]] bool perto(Real a, Real b, Real tol = kTol) {
    return std::abs(a - b) <= tol * std::max<Real>({Real{1}, std::abs(a), std::abs(b)});
}

[[nodiscard]] Metricas calcular_metricas(std::span<const Real> Dx) {
    const auto [it_min, it_max] = std::minmax_element(Dx.begin(), Dx.end());

    Metricas m;
    m.dx_min = *it_min;
    m.dx_max = *it_max;
    m.media = soma(Dx) / static_cast<Real>(Dx.size());

    Real soma_quadrados{};
    for (const Real dx : Dx) {
        const Real desvio = dx - m.media;
        soma_quadrados += desvio * desvio;
    }
    m.desvio_padrao = std::sqrt(soma_quadrados / static_cast<Real>(Dx.size()));
    m.desvio_relativo = m.desvio_padrao / m.media;
    m.razao = m.dx_max / m.dx_min;

    return m;
}

// -----------------------------------------------------------------------------
// PONTO CENTRAL DO EXERCÍCIO
// A malha é gerada pela FVGridMaker, sorteando as faces internas e mantendo
// fixas as duas faces externas em xI e xI + L. VolumeCentered1D informa que as
// faces sorteadas são a coordenada primária, e os centros são calculados como
// o ponto médio de cada volume.
// -----------------------------------------------------------------------------
[[nodiscard]] fvgrid::Axis1D gerar_malha(
    Size n,
    Real comprimento,
    Real x_inicial,
    std::uint64_t semente
) {
    return fvgrid::Random1D::make(
        fvgrid::NVol{n},
        fvgrid::Length{comprimento},
        fvgrid::XInit{x_inicial},
        fvgrid::Seed{semente},
        fvgrid::VolumeCentered1D{}
    );
}

// Tarefa (a): admissibilidade geométrica da malha.
[[nodiscard]] bool malha_admissivel(const fvgrid::Axis1D& eixo, Real comprimento) {
    const auto xF = eixo.faces();
    const auto Dx = eixo.cell_lengths();

    bool faces_crescentes = true;
    for (std::size_t i = 0; i + 1 < xF.size(); ++i) {
        faces_crescentes = faces_crescentes && (xF[i + 1] > xF[i]);
    }

    const bool comprimentos_positivos =
        std::all_of(Dx.begin(), Dx.end(), [](Real dx) { return dx > Real{}; });

    const bool soma_ok = perto(soma(Dx), comprimento);

    return faces_crescentes && comprimentos_positivos && soma_ok;
}

// Tarefa (d): maior transição local S_i = max(Dx[i+1]/Dx[i], Dx[i]/Dx[i+1]).
struct Transicao {
    Real valor{};
    std::size_t indice{};
};

[[nodiscard]] Transicao maior_transicao(std::span<const Real> Dx) {
    Transicao pior{};

    for (std::size_t i = 0; i + 1 < Dx.size(); ++i) {
        const Real s = std::max(Dx[i + 1] / Dx[i], Dx[i] / Dx[i + 1]);
        if (s > pior.valor) {
            pior.valor = s;
            pior.indice = i;
        }
    }

    return pior;
}

// Tarefa (g): critério simples de rejeição automática.
//
// Uma malha é aceita quando, simultaneamente:
//   a razão global r não excede 5 (Seção 4.4 do texto);
//   o desvio padrão relativo sigma_rel não excede 0,3;
//   nenhuma transição local S_i excede 3.
//
// Os dois primeiros limites vêm diretamente das faixas de alerta do texto; o
// terceiro evita que uma malha "aparentemente boa" em r e sigma_rel esconda
// um único salto muito abrupto entre dois volumes vizinhos.
[[nodiscard]] bool malha_aceitavel(const Metricas& m, Real maior_transicao_local) {
    return (m.razao <= 5.0) && (m.desvio_relativo <= 0.3) &&
           (maior_transicao_local <= 3.0);
}

[[nodiscard]] std::string classificar(Real razao) {
    if (razao <= 2.0) {
        return "boa";
    }
    if (razao <= 5.0) {
        return "intermediaria";
    }
    return "ruim";
}

void imprimir_metricas(const Metricas& m) {
    std::cout << "  Dx_min         = " << m.dx_min << '\n';
    std::cout << "  Dx_max         = " << m.dx_max << '\n';
    std::cout << "  media          = " << m.media << '\n';
    std::cout << "  desvio padrao  = " << m.desvio_padrao << '\n';
    std::cout << "  sigma_rel      = " << m.desvio_relativo << '\n';
    std::cout << "  razao r        = " << m.razao << '\n';
}

// Tarefa (e): distâncias de contorno versus distâncias internas em dx_centers.
void comparar_distancias_contorno(const fvgrid::Axis1D& eixo) {
    const auto dx = eixo.dx_centers();

    if (dx.size() < 3) {
        return;
    }

    const std::span<const Real> internas{dx.data() + 1, dx.size() - 2};
    const auto [it_min, it_max] = std::minmax_element(internas.begin(), internas.end());
    const Real media_interna = soma(internas) / static_cast<Real>(internas.size());

    const Real dx_contorno_w = dx.front();
    const Real dx_contorno_e = dx.back();

    std::cout << "  dx[0]  (contorno esquerdo) = " << dx_contorno_w << '\n';
    std::cout << "  dx[N]  (contorno direito)  = " << dx_contorno_e << '\n';
    std::cout << "  min/media/max internos     = "
              << *it_min << " / " << media_interna << " / " << *it_max << '\n';

    const bool contorno_w_fora_da_faixa =
        (dx_contorno_w < 0.5 * (*it_min)) || (dx_contorno_w > 2.0 * (*it_max));
    const bool contorno_e_fora_da_faixa =
        (dx_contorno_e < 0.5 * (*it_min)) || (dx_contorno_e > 2.0 * (*it_max));

    if (contorno_w_fora_da_faixa) {
        std::cout << "  aviso: dx[0] destoa claramente das distancias internas.\n";
    }
    if (contorno_e_fora_da_faixa) {
        std::cout << "  aviso: dx[N] destoa claramente das distancias internas.\n";
    }
    if (!contorno_w_fora_da_faixa && !contorno_e_fora_da_faixa) {
        std::cout << "  as distancias de contorno estao dentro da faixa interna.\n";
    }
}

struct Representante {
    std::string categoria;
    std::uint64_t semente{};
    fvgrid::Axis1D malha;
    Metricas metricas;
};

}  // namespace

int main() {
    std::cout << std::fixed << std::setprecision(8);

    constexpr Real x_inicial = 0.0;
    constexpr Real comprimento = 1.0;
    constexpr Size n_padrao = 10;
    constexpr std::uint64_t semente_base = 2026;

    std::cout << "Exercicio 4.4 - gerador de volumes aleatorios\n";
    std::cout << "xI = " << x_inicial << ", L = " << comprimento
              << ", N = " << n_padrao << ", semente base = " << semente_base
              << "\n\n";

    unsigned aprovados{};
    unsigned total{};

    // ---------------------------------------------------------------------
    // Tarefa (a): admissibilidade da malha gerada com a semente base.
    // ---------------------------------------------------------------------
    const fvgrid::Axis1D malha_base = gerar_malha(n_padrao, comprimento, x_inicial, semente_base);

    std::cout << "Tarefa (a) - admissibilidade geometrica\n";
    std::cout << "========================================\n";
    ++total; aprovados += registrar(
        "faces crescentes, Dx > 0 e soma(Dx) = L",
        malha_admissivel(malha_base, comprimento)
    );

    // ---------------------------------------------------------------------
    // Tarefa (b): medidas de qualidade da malha base.
    // ---------------------------------------------------------------------
    const Metricas metricas_base = calcular_metricas(malha_base.cell_lengths());

    std::cout << "\nTarefa (b) - medidas de qualidade (malha base)\n";
    std::cout << "===============================================\n";
    imprimir_metricas(metricas_base);
    std::cout << "  classificacao  = " << classificar(metricas_base.razao) << '\n';

    // ---------------------------------------------------------------------
    // Tarefa (c): varredura de sementes para achar exemplos representativos
    // de malha boa, intermediaria e ruim, todos com o mesmo N.
    // ---------------------------------------------------------------------
    std::cout << "\nTarefa (c) - varredura de sementes\n";
    std::cout << "===================================\n";

    std::vector<Representante> representantes;
    std::array<bool, 3> encontrado{false, false, false};
    constexpr std::array<std::string_view, 3> categorias{"boa", "intermediaria", "ruim"};
    constexpr std::uint64_t max_sementes = 2000;

    for (std::uint64_t semente = 0; semente < max_sementes; ++semente) {
        const fvgrid::Axis1D malha = gerar_malha(n_padrao, comprimento, x_inicial, semente);
        const Metricas m = calcular_metricas(malha.cell_lengths());
        const std::string categoria = classificar(m.razao);

        const auto pos = std::find(categorias.begin(), categorias.end(), categoria);
        const std::size_t indice = static_cast<std::size_t>(pos - categorias.begin());

        if (!encontrado[indice]) {
            encontrado[indice] = true;
            representantes.push_back(Representante{categoria, semente, malha, m});
        }

        if (encontrado[0] && encontrado[1] && encontrado[2]) {
            break;
        }
    }

    for (const auto& rep : representantes) {
        std::cout << "categoria " << rep.categoria << " (semente = " << rep.semente << ")\n";
        imprimir_metricas(rep.metricas);
        std::cout << '\n';
    }
    for (std::size_t i = 0; i < categorias.size(); ++i) {
        if (!encontrado[i]) {
            std::cout << "nenhuma semente entre 0 e " << (max_sementes - 1)
                      << " produziu uma malha da categoria \"" << categorias[i]
                      << "\" para N = " << n_padrao << ".\n";
        }
    }

    // ---------------------------------------------------------------------
    // Tarefas (d) e (e): saltos bruscos e distancias de contorno em cada
    // representante encontrado.
    // ---------------------------------------------------------------------
    std::cout << "\nTarefas (d) e (e) - transicoes locais e distancias de contorno\n";
    std::cout << "================================================================\n";

    for (const auto& rep : representantes) {
        const Transicao pior = maior_transicao(rep.malha.cell_lengths());

        std::cout << "categoria " << rep.categoria << " (semente = " << rep.semente << ")\n";
        std::cout << "  maior transicao local S_i = " << pior.valor
                  << ", entre os volumes " << pior.indice << " e " << (pior.indice + 1) << '\n';
        if (pior.valor > 2.0) {
            std::cout << "  esse salto e abrupto e pode degradar a interpolacao ";
            std::cout << "de valores e a aproximacao de fluxos na face correspondente.\n";
        }

        comparar_distancias_contorno(rep.malha);
        std::cout << '\n';
    }

    // ---------------------------------------------------------------------
    // Tarefa (f): variacao de N com semente fixa.
    // ---------------------------------------------------------------------
    std::cout << "Tarefa (f) - efeito do numero de volumes N\n";
    std::cout << "===========================================\n";

    constexpr std::array<Size, 5> valores_de_n{5, 10, 20, 40, 80};

    for (const Size n : valores_de_n) {
        const fvgrid::Axis1D malha = gerar_malha(n, comprimento, x_inicial, semente_base);
        const Metricas m = calcular_metricas(malha.cell_lengths());

        std::cout << "N = " << std::setw(3) << n
                  << "  r = " << std::setprecision(4) << m.razao
                  << "  sigma_rel = " << m.desvio_relativo
                  << "  classificacao = " << classificar(m.razao) << '\n';
    }
    std::cout << std::setprecision(8);
    std::cout << "\nAumentar N nao melhora automaticamente a qualidade da malha: ";
    std::cout << "com mais volumes sorteados de forma independente, cresce a ";
    std::cout << "chance de sortear um intervalo muito pequeno ou muito grande, ";
    std::cout << "e a razao r tende a piorar, nao a melhorar, a medida que N cresce.\n";

    // ---------------------------------------------------------------------
    // Tarefa (g): aplicar o criterio de rejeicao automatica aos exemplos.
    // ---------------------------------------------------------------------
    std::cout << "\nTarefa (g) - criterio automatico de rejeicao\n";
    std::cout << "=============================================\n";
    std::cout << "Uma malha e aceita quando r <= 5, sigma_rel <= 0,3 e a maior ";
    std::cout << "transicao local S_i <= 3, simultaneamente.\n\n";

    for (const auto& rep : representantes) {
        const Transicao pior = maior_transicao(rep.malha.cell_lengths());
        const bool aceita = malha_aceitavel(rep.metricas, pior.valor);

        std::cout << "  malha \"" << rep.categoria << "\" (semente " << rep.semente
                  << ") -> " << (aceita ? "aceita" : "rejeitada") << '\n';

        // A categoria "ruim" tem, por definicao, r > 5; o criterio precisa
        // rejeita-la sempre, o que serve de verificacao automatica minima.
        if (rep.categoria == "ruim") {
            ++total; aprovados += registrar(
                "criterio automatico rejeita a malha \"ruim\"",
                !aceita
            );
        }
    }

    // ---------------------------------------------------------------------
    // Saidas auxiliares e verificacoes finais.
    // ---------------------------------------------------------------------
    fvgrid::Axis1DCSVWriter::write(malha_base, "exercicio_44_malha_base.csv");
    std::cout << "\nCSV gerado: exercicio_44_malha_base.csv\n";

    for (const auto& rep : representantes) {
        const std::string nome_arquivo = "exercicio_44_malha_" + rep.categoria + ".csv";
        fvgrid::Axis1DCSVWriter::write(rep.malha, nome_arquivo);
        std::cout << "CSV gerado: " << nome_arquivo << '\n';
    }

    std::cout << "\nResumo\n";
    std::cout << "======\n";
    std::cout << "Verificacoes aprovadas: " << aprovados << " de " << total << '\n';

    return aprovados == total ? 0 : 1;
}
