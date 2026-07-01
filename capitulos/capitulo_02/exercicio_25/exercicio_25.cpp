// ============================================================================
// Arquivo: exercicio_25.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Compara AoS e SoA em um laco simples de atualizacao de phi.
// Autor: Joao Flavio Vieira de Vasconcellos
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
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>

using Real = double;
using Relogio = std::chrono::steady_clock;

namespace {

constexpr std::size_t numero_volumes = 1'000'000;
constexpr std::size_t numero_repeticoes = 60;
constexpr Real dt = Real{0.01};

// Representa Volume, um conjunto de dados usado neste exercicio.
struct Volume
{
    Real x{};
    Real phi{};
    Real fonte{};
};

// Representa CamposSeparados, um conjunto de dados usado neste exercicio.
struct CamposSeparados
{
    std::vector<Real> x;
    std::vector<Real> phi;
    std::vector<Real> fonte;
};

// Representa EstatisticaTempo, um conjunto de dados usado neste exercicio.
struct EstatisticaTempo
{
    double minimo{};
    double media{};
    double maximo{};
    double desvio_padrao{};
};

// Representa ResultadoMedicao, um conjunto de dados usado neste exercicio.
struct ResultadoMedicao
{
    std::vector<double> tempos_ms;
    EstatisticaTempo estatistica;
    Real soma_final{};
    Real soma_controle{};
};

// Calcula a coordenada do volume usado no ensaio de desempenho.
[[nodiscard]] Real coordenada(std::size_t indice)
{
    return (static_cast<Real>(indice) + Real{0.5}) / static_cast<Real>(numero_volumes);
}

// Define o valor inicial de phi para preencher as duas organizacoes de memoria.
[[nodiscard]] Real valor_inicial_phi(Real x)
{
    return std::sin(x);
}

// Define o termo fonte usado no laco de atualizacao.
[[nodiscard]] Real valor_fonte(Real x)
{
    return Real{1.0} + x * x;
}

// Cria uma estrutura de dados inicial para o experimento.
[[nodiscard]] std::vector<Volume> criar_volumes()
{
    std::vector<Volume> volumes(numero_volumes);

    for (std::size_t i = 0; i < numero_volumes; ++i) {
        const Real x = coordenada(i);
        volumes[i] = Volume{x, valor_inicial_phi(x), valor_fonte(x)};
    }

    return volumes;
}

// Cria uma estrutura de dados inicial para o experimento.
[[nodiscard]] CamposSeparados criar_campos_separados()
{
    CamposSeparados campos;
    campos.x.resize(numero_volumes);
    campos.phi.resize(numero_volumes);
    campos.fonte.resize(numero_volumes);

    for (std::size_t i = 0; i < numero_volumes; ++i) {
        const Real x = coordenada(i);
        campos.x[i] = x;
        campos.phi[i] = valor_inicial_phi(x);
        campos.fonte[i] = valor_fonte(x);
    }

    return campos;
}

// Atualiza os campos numericos no laco estudado.
void atualizar_array_de_estruturas(std::vector<Volume>& volumes)
{
    for (Volume& volume : volumes) {
        volume.phi += dt * volume.fonte;
    }
}

// Atualiza os campos numericos no laco estudado.
void atualizar_vetores_separados(CamposSeparados& campos)
{
    for (std::size_t i = 0; i < campos.phi.size(); ++i) {
        campos.phi[i] += dt * campos.fonte[i];
    }
}

// Calcula uma soma de controle para comparar resultados.
[[nodiscard]] Real soma_phi(const std::vector<Volume>& volumes)
{
    Real soma{};
    for (const Volume& volume : volumes) {
        soma += volume.phi;
    }
    return soma;
}

// Calcula uma soma de controle para comparar resultados.
[[nodiscard]] Real soma_phi(const CamposSeparados& campos)
{
    return std::accumulate(campos.phi.begin(), campos.phi.end(), Real{});
}

// Calcula uma grandeza auxiliar do metodo numerico.
[[nodiscard]] EstatisticaTempo calcular_estatistica(const std::vector<double>& tempos_ms)
{
    const auto [menor, maior] = std::minmax_element(tempos_ms.begin(), tempos_ms.end());
    const double media = std::accumulate(tempos_ms.begin(), tempos_ms.end(), 0.0)
        / static_cast<double>(tempos_ms.size());

    double soma_quadrados = 0.0;
    for (double tempo : tempos_ms) {
        const double desvio = tempo - media;
        soma_quadrados += desvio * desvio;
    }

    return EstatisticaTempo{
        *menor,
        media,
        *maior,
        std::sqrt(soma_quadrados / static_cast<double>(tempos_ms.size()))
    };
}

// Mede repeticoes do laco e acumula uma soma de controle.
template <typename Atualizar, typename Amostra, typename SomaFinal>
[[nodiscard]] ResultadoMedicao medir(Atualizar atualizar, Amostra amostra, SomaFinal soma_final)
{
    std::vector<double> tempos_ms;
    tempos_ms.reserve(numero_repeticoes);

    Real soma_controle{};

    for (std::size_t repeticao = 0; repeticao < numero_repeticoes; ++repeticao) {
        const auto inicio = Relogio::now();
        atualizar();
        const auto fim = Relogio::now();

        const std::chrono::duration<double, std::milli> duracao = fim - inicio;
        tempos_ms.push_back(duracao.count());

        // A amostra de controle cria uma dependencia observavel do resultado do
        // laco medido. Sem algum uso posterior, otimizacoes agressivas poderiam
        // eliminar trabalho e transformar a medicao em ficcao.
        soma_controle += amostra(repeticao);
    }

    return ResultadoMedicao{
        tempos_ms,
        calcular_estatistica(tempos_ms),
        soma_final(),
        soma_controle
    };
}

// Compara valores reais dentro de uma tolerancia.
[[nodiscard]] bool aproximadamente_igual(Real a, Real b, Real tolerancia)
{
    return std::abs(a - b) <= tolerancia;
}

// Imprime os dados do exercicio em formato legivel.
void imprimir_resultado(const std::string& nome, const ResultadoMedicao& resultado)
{
    std::cout << std::setw(24) << nome
              << std::setw(14) << resultado.estatistica.minimo
              << std::setw(14) << resultado.estatistica.media
              << std::setw(14) << resultado.estatistica.maximo
              << std::setw(14) << resultado.estatistica.desvio_padrao
              << std::setw(18) << resultado.soma_controle << '\n';
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 2.5\n";
    std::cout << "Design orientado a dados em um laco simples\n";
    std::cout << "===========================================\n\n";

    std::cout << "N          = " << numero_volumes << '\n';
    std::cout << "repeticoes = " << numero_repeticoes << '\n';
    std::cout << "dt         = " << dt << "\n\n";

    std::vector<Volume> volumes = criar_volumes();
    CamposSeparados campos = criar_campos_separados();

    const ResultadoMedicao aos = medir(
        [&volumes] { atualizar_array_de_estruturas(volumes); },
        [&volumes](std::size_t repeticao) -> Real {
            return volumes[repeticao % volumes.size()].phi;
        },
        [&volumes] { return soma_phi(volumes); }
    );

    const ResultadoMedicao soa = medir(
        [&campos] { atualizar_vetores_separados(campos); },
        [&campos](std::size_t repeticao) -> Real {
            return campos.phi[repeticao % campos.phi.size()];
        },
        [&campos] { return soma_phi(campos); }
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::setw(24) << "organizacao"
              << std::setw(14) << "min(ms)"
              << std::setw(14) << "media(ms)"
              << std::setw(14) << "max(ms)"
              << std::setw(14) << "dp(ms)"
              << std::setw(18) << "controle" << '\n';
    imprimir_resultado("AoS", aos);
    imprimir_resultado("SoA", soa);

    const double razao = soa.estatistica.media > 0.0
        ? aos.estatistica.media / soa.estatistica.media
        : 0.0;

    std::cout << "\nrazao media(AoS)/media(SoA) = " << razao << '\n';
    std::cout << "soma final AoS = " << aos.soma_final << '\n';
    std::cout << "soma final SoA = " << soa.soma_final << '\n';

    const bool consistente = aproximadamente_igual(aos.soma_final, soa.soma_final, Real{1.0e-7});
    std::cout << (consistente ? "[PASSOU] " : "[FALHOU] ")
              << "AoS e SoA produzem a mesma soma final dentro da tolerancia\n";

    std::cout << "\nInterpretacao\n";
    std::cout << "-------------\n";
    if (soa.estatistica.media < aos.estatistica.media) {
        std::cout << "Neste experimento, SoA foi mais rapido em media. Isso e coerente\n";
        std::cout << "com o laco medido, que usa apenas phi e fonte e nao precisa carregar x.\n";
    } else {
        std::cout << "Neste experimento, SoA nao foi mais rapido em media. Isso pode ocorrer\n";
        std::cout << "por ruido de medicao, vetorizacao, cache ou detalhes do compilador.\n";
    }
    std::cout << "O ponto metodologico permanece: a conclusao deve nascer da medicao,\n";
    std::cout << "nao da intuicao sobre desempenho.\n";

    return consistente ? 0 : 1;
}