// SPDX-FileCopyrightText: 2026 FVMaker Team
// SPDX-License-Identifier: MIT
//
// Exercicio Computacional 2.1
// Titulo: Refatoracao da funcao da abertura.
//
// Objetivo:
//   Refatorar a funcao calc apresentada na abertura do capitulo, preservando
//   o resultado numerico e melhorando nomes, validacao e flexibilidade.
//
// Modelo numerico:
//   Integral aproximada pela regra dos trapezios em um intervalo finito.
//
// Verificacoes:
//   O programa compara a versao original com a versao refatorada, testa uma
//   integral conhecida e confirma que parametros invalidos sao rejeitados.

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>

using Real = double;

namespace {

[[nodiscard]] inline Real f_da_abertura(Real x)
{
    return x * x;
}

[[nodiscard]] inline Real calc_original(Real a, Real b, int n)
{
    Real h = (b - a) / static_cast<Real>(n);
    Real s = Real{0.5} * (f_da_abertura(a) + f_da_abertura(b));

    for (int i = 1; i < n; ++i) {
        s += f_da_abertura(a + static_cast<Real>(i) * h);
    }

    return s * h;
}

//==============================================================================
// Regra dos trapezios refatorada
//==============================================================================

template <typename Funcao>
[[nodiscard]] inline Real integrar_trapezios(
    Funcao funcao,
    Real limite_inferior,
    Real limite_superior,
    std::size_t numero_subintervalos
)
{
    if (!std::isfinite(limite_inferior)) {
        throw std::invalid_argument("O limite inferior deve ser finito.");
    }

    if (!std::isfinite(limite_superior)) {
        throw std::invalid_argument("O limite superior deve ser finito.");
    }

    if (!(limite_superior > limite_inferior)) {
        throw std::invalid_argument(
            "O limite superior deve ser maior que o limite inferior."
        );
    }

    if (numero_subintervalos == 0) {
        throw std::invalid_argument(
            "O numero de subintervalos deve ser maior que zero."
        );
    }

    const Real passo =
        (limite_superior - limite_inferior)
        / static_cast<Real>(numero_subintervalos);

    const Real soma_extremos =
        Real{0.5}
        * (funcao(limite_inferior) + funcao(limite_superior));

    const auto indices_internos =
        std::views::iota(std::size_t{1}, numero_subintervalos);

    const Real soma = std::accumulate(
        indices_internos.begin(),
        indices_internos.end(),
        soma_extremos,
        [funcao, limite_inferior, passo](Real acumulado, std::size_t indice) {
            const Real x =
                limite_inferior + static_cast<Real>(indice) * passo;

            return acumulado + funcao(x);
        }
    );

    return soma * passo;
}

//==============================================================================
// Funcoes auxiliares de teste
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

inline bool testar_excecao(
    const std::string& descricao,
    Real limite_inferior,
    Real limite_superior,
    std::size_t numero_subintervalos
)
{
    bool excecao_lancada = false;

    try {
        const auto funcao = [](Real x) {
            return x * x;
        };

        (void)integrar_trapezios(
            funcao,
            limite_inferior,
            limite_superior,
            numero_subintervalos
        );
    } catch (const std::invalid_argument&) {
        excecao_lancada = true;
    } catch (...) {
        excecao_lancada = false;
    }

    std::cout << (excecao_lancada ? "[PASSOU] " : "[FALHOU] ")
              << descricao << '\n';

    if (!excecao_lancada) {
        std::cout << "  A chamada deveria ter lancado std::invalid_argument.\n";
    }

    return excecao_lancada;
}

//==============================================================================
// Mensagem final didatica
//==============================================================================

inline void imprimir_mensagem_final()
{
    constexpr int size = 80;

    std::cout << "\nAplicacoes e recomendacoes\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. A funcao refatorada recebe a integranda como ";
    std::cout << "argumento.\n";
    std::cout << "2. O somatorio dos pontos internos usa STL e uma ";
    std::cout << "lambda acumuladora.\n";
    std::cout << "3. Os nomes indicam o significado fisico ou numerico ";
    std::cout << "de cada parametro.\n";
    std::cout << "4. As pre-condicoes sao verificadas antes do calculo ";
    std::cout << "da integral.\n";
    std::cout << "5. A versao refatorada preserva o resultado numerico ";
    std::cout << "da versao original.\n";
    std::cout << "6. A separacao entre algoritmo e funcao integranda ";
    std::cout << "facilita novos testes.\n";
    std::cout << std::string(size, '=') << '\n';

    std::cout << "\nConceitos demonstrados\n";
    std::cout << std::string(size, '=') << '\n';
    std::cout << "1. Refatoracao de nomes sem alterar o algoritmo ";
    std::cout << "matematico.\n";
    std::cout << "2. Remocao da dependencia obrigatoria de uma funcao ";
    std::cout << "global.\n";
    std::cout << "3. Uso de uma funcao generica para aceitar lambdas, ";
    std::cout << "funcoes livres ou objetos chamaveis.\n";
    std::cout << "4. Uso de std::views::iota para representar os ";
    std::cout << "indices internos sem montar um vetor auxiliar.\n";
    std::cout << "5. Uso de std::accumulate para explicitar o ";
    std::cout << "somatorio da regra dos trapezios.\n";
    std::cout << "6. Validacao do numero de subintervalos antes da ";
    std::cout << "divisao por n.\n";
    std::cout << "7. Comparacao entre o resultado numerico e uma ";
    std::cout << "integral conhecida.\n";
    std::cout << std::string(size, '=') << '\n';
}

} // namespace

int main()
{
    try {
        std::cout << "Exercicio Computacional 2.1\n";
        std::cout << "Refatoracao da funcao da abertura\n";
        std::cout << "=================================\n\n";

        unsigned testes_passaram = 0;
        unsigned testes_total = 0;

        const Real limite_inferior = Real{0.0};
        const Real limite_superior = Real{1.0};
        const std::size_t numero_subintervalos = 1000;

        const auto quadratica = [](Real x) {
            return x * x;
        };

        const Real resultado_refatorado =
            integrar_trapezios(
                quadratica,
                limite_inferior,
                limite_superior,
                numero_subintervalos
            );

        const Real resultado_original =
            calc_original(
                limite_inferior,
                limite_superior,
                static_cast<int>(numero_subintervalos)
            );

        const Real integral_exata_quadratica = Real{1.0} / Real{3.0};

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "Integral aproximada de f(x) = x^2 em [0, 1]\n";
        std::cout << "================================================\n";
        std::cout << "Numero de subintervalos = " << numero_subintervalos
                  << '\n';
        std::cout << "Resultado original      = " << resultado_original
                  << '\n';
        std::cout << "Resultado refatorado    = " << resultado_refatorado
                  << '\n';
        std::cout << "Integral exata          = "
                  << integral_exata_quadratica << "\n\n";

        testes_total += 2;

        if (
            testar_valor(
                "A versao refatorada reproduz a versao original",
                resultado_refatorado,
                resultado_original,
                Real{1e-15}
            )
        ) {
            ++testes_passaram;
        }

        if (
            testar_valor(
                "A aproximacao fica proxima da integral exata",
                resultado_refatorado,
                integral_exata_quadratica,
                Real{2e-7}
            )
        ) {
            ++testes_passaram;
        }

        std::cout << '\n';
        std::cout << "Teste com outra funcao integranda\n";
        std::cout << "=================================\n";

        const auto linear = [](Real x) {
            return Real{2.0} * x + Real{1.0};
        };

        const Real integral_linear =
            integrar_trapezios(linear, Real{0.0}, Real{2.0}, 8);

        ++testes_total;

        if (
            testar_valor(
                "Integral de 2x + 1 em [0, 2]",
                integral_linear,
                Real{6.0},
                Real{1e-14}
            )
        ) {
            ++testes_passaram;
        }

        std::cout << '\n';
        std::cout << "Testes de validacao das pre-condicoes\n";
        std::cout << "=====================================\n";

        testes_total += 3;

        if (
            testar_excecao(
                "n = 0 deve ser rejeitado",
                Real{0.0},
                Real{1.0},
                0
            )
        ) {
            ++testes_passaram;
        }

        if (
            testar_excecao(
                "limite superior igual ao inferior deve ser rejeitado",
                Real{1.0},
                Real{1.0},
                10
            )
        ) {
            ++testes_passaram;
        }

        if (
            testar_excecao(
                "limite superior menor que o inferior deve ser rejeitado",
                Real{2.0},
                Real{1.0},
                10
            )
        ) {
            ++testes_passaram;
        }

        std::cout << '\n';
        std::cout << "Resumo\n";
        std::cout << "======\n";
        std::cout << "Testes aprovados: " << testes_passaram
                  << " de " << testes_total << '\n';

        imprimir_mensagem_final();

        return (testes_passaram == testes_total) ? 0 : 1;

    } catch (const std::exception& erro) {
        std::cerr << "Erro inesperado: " << erro.what() << '\n';
        return 1;
    }
}
