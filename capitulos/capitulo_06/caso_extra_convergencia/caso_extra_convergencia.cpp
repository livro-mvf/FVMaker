// ============================================================================
// Arquivo: caso_extra_convergencia.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa caso extra convergencia no contexto de capitulos / capitulo_06 / caso_extra_convergencia.
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

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

namespace {

struct Estado {
    double x{};
    double y{};
};

[[nodiscard]] double residuo_convergente(const Estado e)
{
    return std::abs(3.0 - (2.0 * e.x + e.y));
}

[[nodiscard]] double residuo_divergente(const Estado e)
{
    return std::abs(5.0 - (e.x + 4.0 * e.y));
}

[[nodiscard]] Estado proximo_convergente(const Estado e)
{
    const double x = (3.0 - e.y) / 2.0;
    const double y = (5.0 - x) / 4.0;
    return {x, y};
}

[[nodiscard]] Estado proximo_divergente(const Estado e)
{
    const double x = 5.0 - 4.0 * e.y;
    const double y = 3.0 - 2.0 * x;
    return {x, y};
}

void cabecalho(std::string_view titulo)
{
    std::cout << "\n" << titulo << "\n";
    std::cout << "------------------------------------------------\n";
    std::cout << std::setw(10) << "Iteracao"
              << std::setw(14) << "x"
              << std::setw(14) << "y"
              << std::setw(14) << "||R_k||" << "\n";
}

void imprime_linha(const int k, const Estado e, const double residuo, const int casas_x,
                   const int casas_r)
{
    std::cout << std::setw(10) << k
              << std::fixed << std::setprecision(casas_x)
              << std::setw(14) << e.x
              << std::setw(14) << e.y
              << std::setprecision(casas_r)
              << std::setw(14) << residuo << "\n";
}

[[nodiscard]] std::string trunca_decimal(const double valor, const int casas)
{
    const double escala = std::pow(10.0, casas);
    const double sinal = valor < 0.0 ? -1.0 : 1.0;
    const double truncado = sinal * std::floor(std::abs(valor) * escala) / escala;

    std::ostringstream texto;
    texto << std::fixed << std::setprecision(casas) << truncado;
    return texto.str();
}

void imprime_linha_truncada(const int k, const Estado e, const double residuo)
{
    std::cout << std::setw(10) << k
              << std::fixed << std::setprecision(5)
              << std::setw(14) << e.x
              << std::setw(14) << e.y
              << std::setw(14) << trunca_decimal(residuo, 6) << "\n";
}

void tabela_convergente()
{
    cabecalho("Tabela: arranjo que converge");

    Estado estado{};
    for (int k = 0; k <= 7; ++k) {
        if (k <= 4 || k == 7) {
            imprime_linha_truncada(k, estado, residuo_convergente(estado));
        }

        if (k == 4) {
            std::cout << std::setw(10) << "..."
                      << std::setw(14) << "..."
                      << std::setw(14) << "..."
                      << std::setw(14) << "..." << "\n";
        }

        estado = proximo_convergente(estado);
    }
}

void tabela_divergente()
{
    cabecalho("Tabela: arranjo que diverge");

    Estado estado{};
    for (int k = 0; k <= 3; ++k) {
        imprime_linha(k, estado, residuo_divergente(estado), 3, 3);
        estado = proximo_divergente(estado);
    }
}

} // namespace

int main()
{
    std::cout << "Caso extra: criterio de convergencia\n";
    tabela_convergente();
    tabela_divergente();

    return 0;
}
