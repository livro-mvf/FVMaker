// ============================================================================
// Arquivo: exercicio_33.cpp
// Projeto: FVMaker
// Descricao: Mede automaticamente a ordem observada de formulas finitas.
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
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using Real = double;

namespace {

// Guarda uma linha da tabela de refinamento de uma formula numerica.
struct LinhaRefinamento
{
    Real passo{};
    Real aproximacao{};
    Real erro{};
    Real razao{};
    Real ordem{};
};

// Avalia a funcao escolhida para o experimento de ordem.
[[nodiscard]] Real funcao(Real x)
{
    return std::sin(x);
}

// Avalia a primeira derivada exata da funcao escolhida.
[[nodiscard]] Real derivada_primeira_exata(Real x)
{
    return std::cos(x);
}

// Avalia a segunda derivada exata da funcao escolhida.
[[nodiscard]] Real derivada_segunda_exata(Real x)
{
    return -std::sin(x);
}

// Aproxima f''(x) pela formula centrada de segunda ordem.
[[nodiscard]] Real segunda_centrada(Real x, Real h)
{
    return (funcao(x + h) - Real{2.0} * funcao(x) + funcao(x - h)) / (h * h);
}

// Aproxima f'(x) pela formula progressiva de primeira ordem.
[[nodiscard]] Real primeira_progressiva(Real x, Real h)
{
    return (funcao(x + h) - funcao(x)) / h;
}

// Calcula a ordem observada a partir da razao de erros entre passos metades.
[[nodiscard]] Real ordem_por_razao(Real razao)
{
    if (!(razao > Real{0.0})) {
        return std::numeric_limits<Real>::quiet_NaN();
    }
    return std::log(razao) / std::log(Real{2.0});
}

// Monta uma tabela de erro para uma formula e uma derivada exata no ponto fixo.
template <typename Formula, typename Exata>
[[nodiscard]] std::vector<LinhaRefinamento> medir_ordem(
    Formula formula,
    Exata exata,
    Real x,
    const std::vector<Real>& passos
)
{
    std::vector<LinhaRefinamento> tabela;
    tabela.reserve(passos.size());

    Real erro_anterior = Real{};
    for (Real h : passos) {
        const Real aproximacao = formula(x, h);
        const Real erro = std::abs(aproximacao - exata(x));
        const Real razao = tabela.empty() ? Real{} : erro_anterior / erro;
        const Real ordem = tabela.empty() ? Real{} : ordem_por_razao(razao);
        tabela.push_back(LinhaRefinamento{h, aproximacao, erro, razao, ordem});
        erro_anterior = erro;
    }

    return tabela;
}

// Imprime uma tabela de refinamento com erro, razao e ordem observada.
void imprimir_tabela(const std::string& titulo, const std::vector<LinhaRefinamento>& tabela)
{
    std::cout << titulo << '\n';
    std::cout << std::setw(16) << "h"
              << std::setw(22) << "aproximacao"
              << std::setw(18) << "erro"
              << std::setw(18) << "razao"
              << std::setw(18) << "ordem" << '\n';

    for (const LinhaRefinamento& linha : tabela) {
        std::cout << std::setw(16) << linha.passo
                  << std::setw(22) << linha.aproximacao
                  << std::setw(18) << linha.erro;
        if (linha.razao == Real{}) {
            std::cout << std::setw(18) << "--" << std::setw(18) << "--";
        } else {
            std::cout << std::setw(18) << linha.razao
                      << std::setw(18) << linha.ordem;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Localiza a linha de menor erro para destacar o limite do refinamento.
[[nodiscard]] std::size_t indice_menor_erro(const std::vector<LinhaRefinamento>& tabela)
{
    return static_cast<std::size_t>(
        std::distance(
            tabela.begin(),
            std::min_element(
                tabela.begin(),
                tabela.end(),
                [](const LinhaRefinamento& a, const LinhaRefinamento& b) {
                    return a.erro < b.erro;
                }
            )
        )
    );
}

// Verifica se um valor esta dentro de uma faixa fechada.
[[nodiscard]] bool dentro(Real valor, Real minimo, Real maximo)
{
    return valor >= minimo && valor <= maximo;
}

} // namespace

// Executa o roteiro completo do exercicio.
int main()
{
    std::cout << "Exercicio Computacional 3.3\n";
    std::cout << "Ordem observada de forma automatica\n";
    std::cout << "===================================\n\n";

    const Real x = Real{1.0};
    const std::vector<Real> passos{0.4, 0.2, 0.1, 0.05, 0.025, 0.0125};

    std::cout << std::scientific << std::setprecision(8);

    const auto tabela_segunda = medir_ordem(
        segunda_centrada,
        derivada_segunda_exata,
        x,
        passos
    );
    imprimir_tabela("Derivada segunda centrada", tabela_segunda);

    const auto tabela_primeira = medir_ordem(
        primeira_progressiva,
        derivada_primeira_exata,
        x,
        passos
    );
    imprimir_tabela("Derivada primeira progressiva", tabela_primeira);

    std::vector<Real> passos_muito_pequenos;
    for (Real h = Real{1.0e-1}; h >= Real{1.0e-12}; h *= Real{0.1}) {
        passos_muito_pequenos.push_back(h);
    }
    const auto tabela_arredondamento = medir_ordem(
        segunda_centrada,
        derivada_segunda_exata,
        x,
        passos_muito_pequenos
    );
    imprimir_tabela("Refinamento alem do necessario", tabela_arredondamento);

    const std::size_t minimo = indice_menor_erro(tabela_arredondamento);
    std::cout << "Menor erro observado em h = " << tabela_arredondamento[minimo].passo
              << ", erro = " << tabela_arredondamento[minimo].erro << "\n\n";

    const Real razao_segunda = tabela_segunda.back().razao;
    const Real razao_primeira = tabela_primeira.back().razao;
    const bool segunda_ok = dentro(razao_segunda, Real{3.8}, Real{4.2});
    const bool primeira_ok = dentro(razao_primeira, Real{1.8}, Real{2.2});
    const bool arredondamento_ok = tabela_arredondamento.back().erro > tabela_arredondamento[minimo].erro;

    std::cout << (segunda_ok ? "[PASSOU] " : "[FALHOU] ")
              << "a formula centrada mostra razao proxima de 4\n";
    std::cout << (primeira_ok ? "[PASSOU] " : "[FALHOU] ")
              << "a formula progressiva mostra razao proxima de 2\n";
    std::cout << (arredondamento_ok ? "[PASSOU] " : "[FALHOU] ")
              << "o erro volta a crescer quando o passo fica pequeno demais\n";

    return (segunda_ok && primeira_ok && arredondamento_ok) ? 0 : 1;
}