// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 2.5
// Titulo: Reprodutibilidade de um experimento numerico.
//
// Objetivo:
//   Gerar numeros pseudoaleatorios, calcular a media e registrar os dados que
//   tornam o experimento repetivel: N, semente e resultado obtido.
//
// Modelo numerico:
//   Media aritmetica de N amostras uniformes no intervalo [0, 1).
//
// Verificacoes:
//   O programa compara duas execucoes com a mesma semente e uma terceira com
//   semente diferente, usando igualdade exata e uma tolerancia numerica.

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>

using Real = double;

namespace {

struct Parametros
{
    std::size_t numero_amostras{100000};
    std::uint64_t semente_a{2026};
    std::uint64_t semente_b{2027};
    Real tolerancia{1.0e-12};
};

struct ResultadoExperimento
{
    std::size_t numero_amostras{};
    std::uint64_t semente{};
    Real media{};
};

[[nodiscard]] Real calcular_media_uniforme(
    std::size_t numero_amostras,
    std::uint64_t semente
)
{
    if (numero_amostras == 0) {
        throw std::invalid_argument("N deve ser maior que zero.");
    }

    std::mt19937_64 gerador{semente};
    std::uniform_real_distribution<Real> distribuicao{Real{0.0}, Real{1.0}};

    Real soma = Real{};

    for (std::size_t i = 0; i < numero_amostras; ++i) {
        soma += distribuicao(gerador);
    }

    return soma / static_cast<Real>(numero_amostras);
}

[[nodiscard]] ResultadoExperimento executar_experimento(
    std::size_t numero_amostras,
    std::uint64_t semente
)
{
    return ResultadoExperimento{
        numero_amostras,
        semente,
        calcular_media_uniforme(numero_amostras, semente)
    };
}

[[nodiscard]] std::string formatar_resultado(
    const ResultadoExperimento& resultado
)
{
    std::ostringstream saida;

    saida << std::setprecision(17);
    saida << "N       = " << resultado.numero_amostras << '\n';
    saida << "semente = " << resultado.semente << '\n';
    saida << "media   = " << resultado.media << '\n';

    return saida.str();
}

void gravar_resultado(
    const std::string& nome_arquivo,
    const ResultadoExperimento& resultado
)
{
    std::ofstream arquivo{nome_arquivo};

    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel abrir " + nome_arquivo);
    }

    arquivo << formatar_resultado(resultado);
}

[[nodiscard]] bool aproximadamente_igual(
    Real a,
    Real b,
    Real tolerancia
)
{
    return std::abs(a - b) <= tolerancia;
}

void registrar_teste(
    const std::string& descricao,
    bool passou,
    unsigned& aprovados,
    unsigned& total
)
{
    ++total;

    if (passou) {
        ++aprovados;
    }

    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';
}

void imprimir_resultado(
    const std::string& nome,
    const ResultadoExperimento& resultado
)
{
    std::cout << nome << '\n';
    std::cout << "  N       = " << resultado.numero_amostras << '\n';
    std::cout << "  semente = " << resultado.semente << '\n';
    std::cout << std::setprecision(17);
    std::cout << "  media   = " << resultado.media << '\n';
}

} // namespace

int main()
{
    try {
        const Parametros parametros;

        std::cout << "Exercicio Computacional 2.5\n";
        std::cout << "Reprodutibilidade de um experimento numerico\n";
        std::cout << "============================================\n\n";

        const ResultadoExperimento primeira_execucao =
            executar_experimento(
                parametros.numero_amostras,
                parametros.semente_a
            );

        const ResultadoExperimento segunda_execucao =
            executar_experimento(
                parametros.numero_amostras,
                parametros.semente_a
            );

        const ResultadoExperimento terceira_execucao =
            executar_experimento(
                parametros.numero_amostras,
                parametros.semente_b
            );

        gravar_resultado("exercicio_25_mesma_semente_a.txt", primeira_execucao);
        gravar_resultado("exercicio_25_mesma_semente_b.txt", segunda_execucao);
        gravar_resultado("exercicio_25_semente_diferente.txt", terceira_execucao);

        imprimir_resultado("Primeira execucao", primeira_execucao);
        imprimir_resultado("Segunda execucao, mesma semente", segunda_execucao);
        imprimir_resultado("Terceira execucao, semente diferente", terceira_execucao);

        const std::string texto_primeira = formatar_resultado(primeira_execucao);
        const std::string texto_segunda = formatar_resultado(segunda_execucao);

        unsigned testes_aprovados = 0;
        unsigned testes_total = 0;

        std::cout << "\nVerificacoes automaticas\n";
        std::cout << "========================\n";

        registrar_teste(
            "arquivos de duas execucoes com mesma semente sao identicos",
            texto_primeira == texto_segunda,
            testes_aprovados,
            testes_total
        );

        registrar_teste(
            "media com mesma semente passa no operador ==",
            primeira_execucao.media == segunda_execucao.media,
            testes_aprovados,
            testes_total
        );

        registrar_teste(
            "media com mesma semente passa com tolerancia",
            aproximadamente_igual(
                primeira_execucao.media,
                segunda_execucao.media,
                parametros.tolerancia
            ),
            testes_aprovados,
            testes_total
        );

        registrar_teste(
            "media com sementes diferentes muda neste experimento",
            !aproximadamente_igual(
                primeira_execucao.media,
                terceira_execucao.media,
                parametros.tolerancia
            ),
            testes_aprovados,
            testes_total
        );

        std::cout << "\nResumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: "
                  << testes_aprovados << " de " << testes_total << '\n';

        std::cout << "\nArquivos gravados\n";
        std::cout << "-----------------\n";
        std::cout << "1. exercicio_25_mesma_semente_a.txt\n";
        std::cout << "2. exercicio_25_mesma_semente_b.txt\n";
        std::cout << "3. exercicio_25_semente_diferente.txt\n";

        std::cout << "\nAplicacoes e recomendacoes\n";
        std::cout << "--------------------------\n";
        std::cout << "1. Registrar a semente permite repetir a mesma sequencia.\n";
        std::cout << "2. Registrar N define o tamanho exato do experimento.\n";
        std::cout << "3. Comparar com tolerancia e mais robusto para resultados ";
        std::cout << "de ponto flutuante produzidos por caminhos numericos distintos.\n";

        return (testes_aprovados == testes_total) ? 0 : 1;
    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}
