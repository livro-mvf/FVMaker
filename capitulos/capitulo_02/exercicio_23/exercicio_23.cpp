//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// AVISO LEGAL / LEGAL DISCLAIMER
//
// [PT-BR]
// O codigo-fonte e fornecido sob a licenca MIT, no estado em que se encontra
// ("as is"). Embora nos esforcemos para garantir o rigor matematico e a
// correcao das implementacoes, a natureza da computacao cientifica implica
// que inconsistencias pontuais possam ocorrer.
//
// Caso identifique algum erro, comportamento inesperado, ou tenha sugestoes de
// aprimoramento, seremos imensamente gratos se nos puder contactar atraves do
// e-mail livromvf@gmail.com. A sua contribuicao e inestimavel para o
// aperfeicoamento continuo deste livro e da sua biblioteca de apoio.
//
// [EN-GB]
// The source code is provided under the MIT Licence, on an "as is" basis.
// Whilst we endeavour to ensure mathematical rigour and correctness in the
// C++ implementations, the nature of scientific computing implies that
// occasional inconsistencies or errors may arise.
//
// Should you identify any bugs, unexpected behaviour, or have suggestions for
// improvement, we would be most grateful if you could reach out to us at
// livromvf@gmail.com. Your feedback is invaluable to the continuous refinement
// of this textbook and its accompanying library.
//==============================================================================

//==============================================================================
// Exercicio Computacional 2.3
// Design orientado a dados em um laco simples
//
// Este programa compara duas formas de armazenar os dados de N volumes.
//
// Na primeira forma, cada volume e uma estrutura contendo a posicao x, o valor
// da solucao phi e o termo fonte.
//
//     struct Volume {
//         double x;
//         double phi;
//         double fonte;
//     };
//
// Na segunda forma, as mesmas grandezas sao armazenadas em vetores separados.
// O laco principal atualiza somente phi usando fonte. Portanto, a coordenada x
// existe no problema, mas nao e necessaria durante esse laco.
//
// O objetivo e medir o tempo do laco principal nas duas organizacoes e discutir
// a diferenca em termos de localidade de memoria.
//
// O codigo e autocontido e nao depende da biblioteca em desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrao do C++
//==============================================================================

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

//==============================================================================
// Tipos usados no programa
//==============================================================================

using Real = double;
using Relogio = std::chrono::steady_clock;

//==============================================================================
// Constantes do experimento
//==============================================================================

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

//==============================================================================
// Programa principal
//==============================================================================

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
