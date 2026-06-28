// ============================================================================
// Arquivo: exercicio_23.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 23 no contexto de capitulos / capitulo_02 / exercicio_23.
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

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>

using Real = double;
using Relogio = std::chrono::steady_clock;

namespace {

constexpr std::size_t numero_volumes = 1'000'000;
constexpr std::size_t numero_repeticoes = 50;
constexpr Real dt = Real{0.01};

//==============================================================================
// Organizacao inicial: array de estruturas
//==============================================================================

struct Volume
{
    Real x;
    Real phi;
    Real fonte;
};

//==============================================================================
// Organizacao orientada a dados: estrutura de arrays
//==============================================================================

struct CamposSeparados
{
    std::vector<Real> x;
    std::vector<Real> phi;
    std::vector<Real> fonte;
};

//==============================================================================
// Funcoes de inicializacao
//==============================================================================

[[nodiscard]] inline Real coordenada(std::size_t indice)
{
    return (static_cast<Real>(indice) + Real{0.5})
        / static_cast<Real>(numero_volumes);
}

[[nodiscard]] inline Real valor_inicial_phi(Real x)
{
    return std::sin(x);
}

[[nodiscard]] inline Real valor_fonte(Real x)
{
    return Real{1.0} + x * x;
}

[[nodiscard]] inline std::vector<Volume> criar_volumes()
{
    std::vector<Volume> volumes(numero_volumes);

    for (std::size_t i = 0; i < numero_volumes; ++i) {
        const Real x = coordenada(i);

        volumes[i] = Volume{
            x,
            valor_inicial_phi(x),
            valor_fonte(x)
        };
    }

    return volumes;
}

[[nodiscard]] inline CamposSeparados criar_campos_separados()
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

//==============================================================================
// Lacos principais
//==============================================================================

inline void atualizar_array_de_estruturas(std::vector<Volume>& volumes)
{
    for (Volume& volume : volumes) {
        volume.phi += dt * volume.fonte;
    }
}

inline void atualizar_vetores_separados(CamposSeparados& campos)
{
    for (std::size_t i = 0; i < campos.phi.size(); ++i) {
        campos.phi[i] += dt * campos.fonte[i];
    }
}

//==============================================================================
// Medicao de tempo
//==============================================================================

template <typename Funcao>
[[nodiscard]] inline double medir_tempo(Funcao funcao)
{
    const auto inicio = Relogio::now();

    for (std::size_t repeticao = 0; repeticao < numero_repeticoes; ++repeticao) {
        funcao();
    }

    const auto fim = Relogio::now();
    const std::chrono::duration<double> tempo = fim - inicio;

    return tempo.count();
}

//==============================================================================
// Funcoes auxiliares de teste e impressao
//==============================================================================

[[nodiscard]] inline Real soma_phi(const std::vector<Volume>& volumes)
{
    Real soma = Real{};

    for (const Volume& volume : volumes) {
        soma += volume.phi;
    }

    return soma;
}

[[nodiscard]] inline Real soma_phi(const CamposSeparados& campos)
{
    return std::accumulate(campos.phi.begin(), campos.phi.end(), Real{});
}

[[nodiscard]] inline bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia
)
{
    return std::abs(a - b) <= tolerancia;
}

inline bool testar_valor(
    const std::string& descricao,
    Real obtido,
    Real esperado,
    Real tolerancia
)
{
    const bool passou = aproximadamente_igual(obtido, esperado, tolerancia);

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!passou) {
        std::cout << std::fixed << std::setprecision(16)
                  << "  obtido     = " << obtido << '\n'
                  << "  esperado   = " << esperado << '\n'
                  << "  tolerancia = " << tolerancia << '\n';
    }

    return passou;
}

inline void imprimir_mensagem_final(double tempo_aos, double tempo_soa)
{
    constexpr int size = 80;

    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << std::string(size, '-') << '\n';
    std::cout << "1. A organizacao com struct Volume e simples de ler, ";
    std::cout << "mas mistura grandezas com usos diferentes.\n";
    std::cout << "2. A organizacao com vetores separados deixa explicito ";
    std::cout << "quais dados sao usados por cada laco.\n";
    std::cout << "3. Quando o laco usa apenas phi e fonte, o vetor x nao ";
    std::cout << "precisa ser trazido para perto da CPU.\n";
    std::cout << "4. Em programas de volumes finitos, essa separacao ";
    std::cout << "ajuda a escrever kernels numericos mais previsiveis.\n";
    std::cout << std::string(size, '-') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '-') << '\n';
    std::cout << "1. Array de estruturas: std::vector<Volume>.\n";
    std::cout << "2. Estrutura de arrays: vetores separados para x, phi ";
    std::cout << "e fonte.\n";
    std::cout << "3. Medicao do tempo do laco principal com ";
    std::cout << "std::chrono::steady_clock.\n";
    std::cout << "4. Comparacao entre dois codigos que realizam o mesmo ";
    std::cout << "calculo numerico.\n";
    std::cout << "5. Interpretacao do desempenho usando localidade de ";
    std::cout << "memoria.\n";
    std::cout << std::string(size, '-') << '\n';

    std::cout << "\nInterpretacao\n";
    std::cout << std::string(size, '-') << '\n';

    if (tempo_soa < tempo_aos) {
        std::cout << "Neste teste, os vetores separados foram mais rapidos. ";
        std::cout << "Isso confirma a expectativa discutida na secao sobre ";
        std::cout << "design orientado a dados.\n";
    } else {
        std::cout << "Neste teste, os vetores separados nao foram mais ";
        std::cout << "rapidos. Isso pode ocorrer por ruido de medicao, ";
        std::cout << "otimizacoes do compilador, cache do processador ou ";
        std::cout << "caracteristicas da maquina usada.\n";
    }

    std::cout << "A razao conceitual continua a mesma: no array de ";
    std::cout << "estruturas, cada linha de cache pode carregar tambem x, ";
    std::cout << "embora x nao participe do laco. Nos vetores separados, ";
    std::cout << "o laco percorre principalmente os dados necessarios: ";
    std::cout << "phi e fonte.\n";
    std::cout << std::string(size, '-') << '\n';
}

} // namespace

int main()
{
    try {
        std::cout << "Exercicio Computacional 2.3\n";
        std::cout << "Design orientado a dados em um laco simples\n";
        std::cout << "===========================================\n\n";

        std::cout << "Dados do experimento\n";
        std::cout << "--------------------\n";
        std::cout << "N                  = " << numero_volumes << '\n';
        std::cout << "repeticoes         = " << numero_repeticoes << '\n';
        std::cout << "dt                 = " << dt << "\n\n";

        std::vector<Volume> volumes = criar_volumes();
        CamposSeparados campos = criar_campos_separados();

        std::cout << "Medindo o laco principal...\n\n";

        const double tempo_aos = medir_tempo(
            [&volumes]() {
                atualizar_array_de_estruturas(volumes);
            }
        );

        const double tempo_soa = medir_tempo(
            [&campos]() {
                atualizar_vetores_separados(campos);
            }
        );

        const Real soma_aos = soma_phi(volumes);
        const Real soma_soa = soma_phi(campos);
        const double razao = tempo_soa > 0.0 ? tempo_aos / tempo_soa : 0.0;

        std::cout << std::fixed << std::setprecision(8);
        std::cout << "Resultados\n";
        std::cout << "----------\n";
        std::cout << std::setw(30) << "organizacao"
                  << std::setw(18) << "tempo total (s)"
                  << std::setw(22) << "tempo por laco (ms)"
                  << '\n';
        std::cout << std::setw(30) << "array de estruturas"
                  << std::setw(18) << tempo_aos
                  << std::setw(22)
                  << (1000.0 * tempo_aos / static_cast<double>(numero_repeticoes))
                  << '\n';
        std::cout << std::setw(30) << "vetores separados"
                  << std::setw(18) << tempo_soa
                  << std::setw(22)
                  << (1000.0 * tempo_soa / static_cast<double>(numero_repeticoes))
                  << '\n';
        std::cout << "razao tempo(AoS)/tempo(SoA) = " << razao << "\n\n";

        unsigned testes_passaram = 0;
        unsigned testes_total = 0;

        std::cout << "Testes de consistencia\n";
        std::cout << "----------------------\n";

        ++testes_total;
        if (
            testar_valor(
                "as duas organizacoes produzem a mesma soma de phi",
                soma_aos,
                soma_soa,
                Real{1e-7}
            )
        ) {
            ++testes_passaram;
        }

        std::cout << '\n';

        std::cout << "Resumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: " << testes_passaram
                  << " de " << testes_total << '\n';

        imprimir_mensagem_final(tempo_aos, tempo_soa);

        return (testes_passaram == testes_total) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}
