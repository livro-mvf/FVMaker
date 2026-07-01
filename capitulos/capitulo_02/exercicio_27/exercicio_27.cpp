// ============================================================================
// Arquivo: exercicio_27.cpp
// Projeto: FVMaker
// Versao: consulte <FVMaker/Core/Version.h>
// Descricao: Demonstra semente, tolerancia e ordem de soma em ponto flutuante.
// Autor: Joao Flavio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 Joao Flavio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
// ============================================================================

#include <algorithm>
#include <cctype>
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
#include <vector>

using Real = double;

namespace {

// Representa Parametros, um conjunto de dados usado neste exercicio.
struct Parametros
{
    std::size_t numero_amostras{};
    std::uint64_t semente{};
    Real tolerancia{};
};

// Representa ResultadoSomas, um conjunto de dados usado neste exercicio.
struct ResultadoSomas
{
    Real soma_direta{};
    Real soma_inversa{};
};

// Remove espacos das extremidades de uma linha de configuracao.
[[nodiscard]] std::string trim(std::string texto)
{
    const auto nao_espaco = [](unsigned char c) { return !std::isspace(c); };

    texto.erase(texto.begin(), std::find_if(texto.begin(), texto.end(), nao_espaco));
    texto.erase(std::find_if(texto.rbegin(), texto.rend(), nao_espaco).base(), texto.end());

    return texto;
}

// Cria uma estrutura de dados inicial para o experimento.
void criar_configuracao_padrao(const std::string& nome_arquivo)
{
    std::ofstream arquivo{nome_arquivo};
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel criar " + nome_arquivo);
    }

    arquivo << "# Configuracao do exercicio 26\n";
    arquivo << "N = 100000\n";
    arquivo << "semente = 2026\n";
    arquivo << "tolerancia = 1e-8\n";
}

// Verifica se o arquivo de configuracao ja existe.
[[nodiscard]] bool arquivo_existe(const std::string& nome_arquivo)
{
    std::ifstream arquivo{nome_arquivo};
    return static_cast<bool>(arquivo);
}

// Le dados externos usados pelo exercicio.
[[nodiscard]] Parametros ler_configuracao(const std::string& nome_arquivo)
{
    if (!arquivo_existe(nome_arquivo)) {
        criar_configuracao_padrao(nome_arquivo);
    }

    std::ifstream arquivo{nome_arquivo};
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel ler " + nome_arquivo);
    }

    Parametros parametros{100000, 2026, Real{1.0e-8}};
    std::string linha;

    while (std::getline(arquivo, linha)) {
        const std::size_t comentario = linha.find('#');
        if (comentario != std::string::npos) {
            linha.erase(comentario);
        }

        linha = trim(linha);
        if (linha.empty()) {
            continue;
        }

        const std::size_t igual = linha.find('=');
        if (igual == std::string::npos) {
            throw std::invalid_argument("Linha de configuracao sem '=': " + linha);
        }

        const std::string chave = trim(linha.substr(0, igual));
        const std::string valor = trim(linha.substr(igual + 1));

        if (chave == "N") {
            parametros.numero_amostras = static_cast<std::size_t>(std::stoull(valor));
        } else if (chave == "semente") {
            parametros.semente = static_cast<std::uint64_t>(std::stoull(valor));
        } else if (chave == "tolerancia") {
            parametros.tolerancia = std::stod(valor);
        } else {
            throw std::invalid_argument("Parametro desconhecido: " + chave);
        }
    }

    if (parametros.numero_amostras == 0) {
        throw std::invalid_argument("N deve ser maior que zero.");
    }
    if (!std::isfinite(parametros.tolerancia) || parametros.tolerancia <= Real{0.0}) {
        throw std::invalid_argument("A tolerancia deve ser finita e positiva.");
    }

    return parametros;
}

// Gera os dados numericos usados no experimento.
[[nodiscard]] std::vector<Real> gerar_amostras(const Parametros& parametros)
{
    std::mt19937_64 gerador{parametros.semente};
    std::uniform_real_distribution<Real> distribuicao{Real{0.0}, Real{1.0}};
    std::vector<Real> valores(parametros.numero_amostras);
    for (Real& valor : valores) {
        valor = distribuicao(gerador);
    }

    return valores;
}

// Calcula uma grandeza auxiliar do metodo numerico.
[[nodiscard]] ResultadoSomas calcular_somas(const std::vector<Real>& valores)
{
    Real soma_direta{};
    for (Real valor : valores) {
        soma_direta += valor;
    }

    Real soma_inversa{};
    for (auto it = valores.rbegin(); it != valores.rend(); ++it) {
        soma_inversa += *it;
    }

    return ResultadoSomas{soma_direta, soma_inversa};
}

// Identifica o compilador para registrar a reprodutibilidade.
[[nodiscard]] std::string compilador()
{
#if defined(__clang__)
    return std::string{"Clang "} + __clang_version__;
#elif defined(__GNUC__)
    return std::string{"GCC "} + __VERSION__;
#elif defined(_MSC_VER)
    return std::string{"MSVC "} + std::to_string(_MSC_VER);
#else
    return "compilador nao identificado";
#endif
}

// Grava a saida de referencia ou o resultado do experimento.
void gravar_saida(
    const std::string& nome_arquivo,
    const Parametros& parametros,
    const ResultadoSomas& resultado,
    bool igualdade_exata,
    bool igualdade_tolerancia
)
{
    std::ofstream arquivo{nome_arquivo};
    if (!arquivo) {
        throw std::runtime_error("Nao foi possivel criar " + nome_arquivo);
    }

    const Real diferenca = std::abs(resultado.soma_direta - resultado.soma_inversa);

    arquivo << std::setprecision(17);
    arquivo << "N = " << parametros.numero_amostras << '\n';
    arquivo << "semente = " << parametros.semente << '\n';
    arquivo << "tolerancia = " << parametros.tolerancia << '\n';
    arquivo << "compilador = " << compilador() << '\n';
    arquivo << "soma_direta = " << resultado.soma_direta << '\n';
    arquivo << "soma_inversa = " << resultado.soma_inversa << '\n';
    arquivo << "diferenca_absoluta = " << diferenca << '\n';
    arquivo << "igualdade_exata = " << (igualdade_exata ? "verdadeira" : "falsa") << '\n';
    arquivo << "igualdade_com_tolerancia = "
            << (igualdade_tolerancia ? "verdadeira" : "falsa") << '\n';
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    const std::string arquivo_configuracao = "exercicio_27_config.txt";
    const std::string arquivo_saida = "exercicio_27_saida.txt";

    std::cout << "Exercicio Computacional 2.7\n";
    std::cout << "Parametros, semente e reprodutibilidade\n";
    std::cout << "=======================================\n\n";

    const Parametros parametros = ler_configuracao(arquivo_configuracao);
    const std::vector<Real> valores = gerar_amostras(parametros);
    const ResultadoSomas resultado = calcular_somas(valores);

    const bool igualdade_exata = resultado.soma_direta == resultado.soma_inversa;
    const Real diferenca = std::abs(resultado.soma_direta - resultado.soma_inversa);
    const bool igualdade_tolerancia = diferenca <= parametros.tolerancia;

    gravar_saida(arquivo_saida, parametros, resultado, igualdade_exata, igualdade_tolerancia);

    std::cout << std::setprecision(17);
    std::cout << "Arquivo de configuracao: " << arquivo_configuracao << '\n';
    std::cout << "Arquivo de saida        : " << arquivo_saida << "\n\n";
    std::cout << "N          = " << parametros.numero_amostras << '\n';
    std::cout << "semente    = " << parametros.semente << '\n';
    std::cout << "tolerancia = " << parametros.tolerancia << '\n';
    std::cout << "compilador = " << compilador() << "\n\n";
    std::cout << "soma direta  = " << resultado.soma_direta << '\n';
    std::cout << "soma inversa = " << resultado.soma_inversa << '\n';
    std::cout << "diferenca    = " << diferenca << "\n\n";

    std::cout << "Comparacao por ==         : "
              << (igualdade_exata ? "verdadeira" : "falsa") << '\n';
    std::cout << "Comparacao com tolerancia : "
              << (igualdade_tolerancia ? "verdadeira" : "falsa") << '\n';

    std::cout << "\nInterpretacao\n";
    std::cout << "-------------\n";
    std::cout << "A semente fixa reproduz os valores gerados. A ordem da soma, porem,\n";
    std::cout << "pode alterar os ultimos digitos porque a adicao em ponto flutuante\n";
    std::cout << "nao e associativa. Por isso a comparacao numerica usa tolerancia e\n";
    std::cout << "a saida registra os parametros que definem o experimento.\n";

    return igualdade_tolerancia ? 0 : 1;
}