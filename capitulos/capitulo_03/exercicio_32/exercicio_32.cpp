//==============================================================================
// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//==============================================================================
// AVISO LEGAL / LEGAL DISCLAIMER
//
// [PT-BR]
// O código-fonte é fornecido sob a licença MIT, no estado em que se encontra
// ("as is"). Embora nos esforcemos para garantir o rigor matemático e a
// correção das implementações, a natureza da computação científica implica
// que inconsistências pontuais possam ocorrer.
//
// Caso identifique algum erro, comportamento inesperado, ou tenha sugestões de
// aprimoramento, seremos imensamente gratos se nos puder contactar através do
// e-mail livromvf@gmail.com. A sua contribuição é inestimável para o
// aperfeiçoamento contínuo deste livro e da sua biblioteca de apoio.
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
// Exercício 3.2
// Diferença regressiva de primeira ordem
//
// Este programa resolve o Exercício 3.2 do Capítulo 3.
//
// O exercício pede estimar f'(2), para f(x) = ln(x), usando a diferença
// regressiva de primeira ordem:
//
//     f'(x_i) ~= (f_i - f_{i-1}) / Delta x
//
// O cálculo é feito para Delta x = 0.1 e Delta x = 0.05. O programa compara
// as aproximações com o valor exato f'(2) = 1/2, calcula os erros absolutos
// e avalia como o erro responde à redução do passo.
//
// O código é autocontido e não depende da biblioteca em desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrão do C++
//==============================================================================

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

//==============================================================================
// Tipo real usado no programa
//==============================================================================

using Real = double;

//==============================================================================
// Estrutura de dados para guardar o resultado de cada passo
//==============================================================================

struct ResultadoDiferenca {
    Real x{};
    Real passo{};
    Real aproximacao{};
    Real valor_exato{};
    Real erro_absoluto{};
};

//==============================================================================
// Função do exercício e derivada exata
//==============================================================================

[[nodiscard]] inline Real funcao_logaritmo(Real x)
{
    if (!(x > Real{0.0})) {
        throw std::invalid_argument(
            "A função ln(x) exige x > 0."
        );
    }

    return std::log(x);
}

[[nodiscard]] inline Real derivada_exata_logaritmo(Real x)
{
    if (!(x > Real{0.0})) {
        throw std::invalid_argument(
            "A derivada de ln(x) exige x > 0."
        );
    }

    return Real{1.0} / x;
}

//==============================================================================
// Diferença regressiva de primeira ordem
//==============================================================================

template <typename Funcao>
[[nodiscard]] inline Real diferenca_regressiva_primeira_ordem(
    Funcao funcao,
    Real x,
    Real passo
)
{
    if (!std::isfinite(x)) {
        throw std::invalid_argument("A coordenada x deve ser finita.");
    }

    if (!std::isfinite(passo)) {
        throw std::invalid_argument("O passo Delta x deve ser finito.");
    }

    if (!(passo > Real{0.0})) {
        throw std::invalid_argument(
            "O passo Delta x deve ser maior que zero."
        );
    }

    if (!(x - passo > Real{0.0})) {
        throw std::invalid_argument(
            "Para f(x) = ln(x), o ponto x - Delta x deve ser positivo."
        );
    }

    return (funcao(x) - funcao(x - passo)) / passo;
}

//==============================================================================
// Resolução do exercício
//==============================================================================

[[nodiscard]] inline ResultadoDiferenca calcular_resultado(
    Real x,
    Real passo
)
{
    const Real aproximacao =
        diferenca_regressiva_primeira_ordem(
            funcao_logaritmo,
            x,
            passo
        );

    const Real valor_exato = derivada_exata_logaritmo(x);
    const Real erro_absoluto = std::abs(aproximacao - valor_exato);

    return ResultadoDiferenca{
        x,
        passo,
        aproximacao,
        valor_exato,
        erro_absoluto
    };
}

//==============================================================================
// Impressão dos resultados
//==============================================================================

inline void imprimir_resultados(
    const std::vector<ResultadoDiferenca>& resultados
)
{
    std::cout << "Exercício 3.2\n";
    std::cout << "Diferença regressiva de primeira ordem\n";
    std::cout << "======================================\n\n";

    std::cout << "Função:        f(x) = ln(x)\n";
    std::cout << "Ponto:         x = 2\n";
    std::cout << "Derivada exata f'(2) = 1/2 = 0.5\n\n";

    std::cout << std::setw(14) << "Delta x"
              << std::setw(24) << "aproximacao"
              << std::setw(24) << "valor exato"
              << std::setw(24) << "erro absoluto" << '\n';

    for (const auto& resultado : resultados) {
        std::cout << std::setw(14) << resultado.passo
                  << std::setw(24) << resultado.aproximacao
                  << std::setw(24) << resultado.valor_exato
                  << std::setw(24) << resultado.erro_absoluto << '\n';
    }
}

inline void imprimir_comparacao_erros(
    const std::vector<ResultadoDiferenca>& resultados
)
{
    if (resultados.size() < 2) {
        return;
    }

    const Real passo_grosso = resultados[0].passo;
    const Real passo_fino = resultados[1].passo;

    const Real erro_grosso = resultados[0].erro_absoluto;
    const Real erro_fino = resultados[1].erro_absoluto;

    const Real razao_passos = passo_fino / passo_grosso;
    const Real razao_erros = erro_fino / erro_grosso;

    std::cout << "\nComparação dos erros\n";
    std::cout << "====================\n";
    std::cout << "Razão entre os passos h_fino/h_grosso = "
              << razao_passos << '\n';
    std::cout << "Razão entre os erros  E_fino/E_grosso = "
              << razao_erros << '\n';

    std::cout << "\nInterpretação\n";
    std::cout << "=============\n";
    std::cout << "A diferença regressiva usada neste exercício é de primeira ";
    std::cout << "ordem, isto é, seu erro dominante é proporcional a ";
    std::cout << "Delta x.\n";
    std::cout << "Por isso, ao reduzir Delta x pela metade, espera-se que ";
    std::cout << "o erro também caia aproximadamente pela metade.\n";
    std::cout << "A proporção não precisa ser exatamente 0.5, porque os ";
    std::cout << "termos de ordem superior da expansão de Taylor ainda ";
    std::cout << "participam do erro para passos finitos.\n";
}

//==============================================================================
// Funções auxiliares de teste
//==============================================================================

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

inline bool testar_condicao(
    const std::string& descricao,
    bool condicao
)
{
    std::cout << (condicao ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    return condicao;
}

inline bool testar_excecao(
    const std::string& descricao,
    Real x,
    Real passo
)
{
    bool excecao_lancada = false;

    try {
        (void)diferenca_regressiva_primeira_ordem(
            funcao_logaritmo,
            x,
            passo
        );
    } catch (const std::invalid_argument&) {
        excecao_lancada = true;
    } catch (...) {
        excecao_lancada = false;
    }

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!excecao_lancada) {
        std::cout << "  A chamada deveria ter lançado std::invalid_argument.\n";
    }

    return excecao_lancada;
}

inline unsigned executar_testes()
{
    unsigned testes_passaram = 0;
    unsigned testes_total = 0;

    std::cout << "\nTestes internos\n";
    std::cout << "===============\n";

    const auto funcao_linear = [](Real x) {
        return Real{3.0} * x + Real{2.0};
    };

    const Real derivada_linear =
        diferenca_regressiva_primeira_ordem(
            funcao_linear,
            Real{2.0},
            Real{0.4}
        );

    ++testes_total;
    if (
        testar_valor(
            "A diferença regressiva é exata para função linear",
            derivada_linear,
            Real{3.0},
            Real{1e-14}
        )
    ) {
        ++testes_passaram;
    }

    const ResultadoDiferenca resultado_01 =
        calcular_resultado(Real{2.0}, Real{0.1});

    ++testes_total;
    if (
        testar_valor(
            "Valor aproximado para Delta x = 0.1",
            resultado_01.aproximacao,
            Real{0.5129329438755059},
            Real{1e-14}
        )
    ) {
        ++testes_passaram;
    }

    const ResultadoDiferenca resultado_005 =
        calcular_resultado(Real{2.0}, Real{0.05});

    ++testes_total;
    if (
        testar_valor(
            "Valor aproximado para Delta x = 0.05",
            resultado_005.aproximacao,
            Real{0.5063561596857968},
            Real{1e-14}
        )
    ) {
        ++testes_passaram;
    }

    ++testes_total;
    if (
        testar_valor(
            "Derivada exata de ln(x) em x = 2",
            derivada_exata_logaritmo(Real{2.0}),
            Real{0.5},
            Real{0.0}
        )
    ) {
        ++testes_passaram;
    }

    const Real razao_erros =
        resultado_005.erro_absoluto / resultado_01.erro_absoluto;

    ++testes_total;
    if (
        testar_valor(
            "A razão entre os erros fica próxima de 0.5",
            razao_erros,
            Real{0.5},
            Real{0.03}
        )
    ) {
        ++testes_passaram;
    }

    testes_total += 3;

    if (
        testar_excecao(
            "Delta x = 0 deve ser rejeitado",
            Real{2.0},
            Real{0.0}
        )
    ) {
        ++testes_passaram;
    }

    if (
        testar_excecao(
            "Delta x < 0 deve ser rejeitado",
            Real{2.0},
            Real{-0.1}
        )
    ) {
        ++testes_passaram;
    }

    if (
        testar_excecao(
            "x - Delta x <= 0 deve ser rejeitado para ln(x)",
            Real{0.1},
            Real{0.2}
        )
    ) {
        ++testes_passaram;
    }

    std::cout << "\nResumo dos testes\n";
    std::cout << "=================\n";
    std::cout << "Testes aprovados: " << testes_passaram
              << " de " << testes_total << '\n';

    return (testes_passaram == testes_total) ? 0u : 1u;
}

//==============================================================================
// Mensagem final didática
//==============================================================================

inline void imprimir_mensagem_final()
{
    constexpr int size = 80;

    std::cout << "\nAplicações e recomendações\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. A diferença regressiva usa o ponto atual e o ponto ";
    std::cout << "imediatamente anterior.\n";
    std::cout << "2. Para f(x) = ln(x), todos os pontos avaliados precisam ";
    std::cout << "satisfazer x > 0.\n";
    std::cout << "3. Como o esquema é de primeira ordem, reduzir Delta x ";
    std::cout << "pela metade tende a reduzir o erro pela metade.\n";
    std::cout << "4. A proporção observada não é exatamente 0.5 porque ";
    std::cout << "ainda há termos de ordem superior no erro.\n";
    std::cout << "5. A comparação com o Exercício 3.1 mostra a diferença ";
    std::cout << "entre esquemas de primeira e segunda ordem.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Aplicação da diferença regressiva de primeira ordem.\n";
    std::cout << "2. Comparação entre aproximação numérica e derivada ";
    std::cout << "exata.\n";
    std::cout << "3. Cálculo do erro absoluto.\n";
    std::cout << "4. Avaliação da razão entre erros para passos diferentes.\n";
    std::cout << "5. Validação explícita do domínio de definição de ln(x).\n";
    std::cout << std::string(size, '=') << '\n';
}

//==============================================================================
// Programa principal
//==============================================================================

int main()
{
    try {
        std::cout << std::fixed << std::setprecision(16);

        const Real x = Real{2.0};

        const std::vector<Real> passos{
            Real{0.1},
            Real{0.05}
        };

        std::vector<ResultadoDiferenca> resultados;
        resultados.reserve(passos.size());

        for (const Real passo : passos) {
            resultados.push_back(calcular_resultado(x, passo));
        }

        imprimir_resultados(resultados);
        imprimir_comparacao_erros(resultados);

        const unsigned status_testes = executar_testes();

        imprimir_mensagem_final();

        return (status_testes == 0u) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}