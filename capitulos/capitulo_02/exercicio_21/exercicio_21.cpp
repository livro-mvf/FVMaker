// ============================================================================
// Arquivo: exercicio_21.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa exercicio 21 no contexto de capitulos / capitulo_02 / exercicio_21.
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
#include <stdexcept>
#include <string>

using Real = double;

namespace {

// ----------------------------------------------------------------------------
// Integranda usada em todos os testes deste exercicio.
//
// E a mesma funcao f(x) = x^2 da abertura do capitulo. Definimo-la uma
// unica vez, aqui, para nao repetir a expressao ao longo do programa.
// ----------------------------------------------------------------------------
[[nodiscard]] Real quadratica(Real x)
{
    return x * x;
}

// ----------------------------------------------------------------------------
// Regra dos trapezios composta.
//
// 'funcao' e a integranda; pode ser uma funcao livre, uma lambda ou
// qualquer objeto chamavel que receba um Real e devolva um Real. O tipo
// e mantido generico (template) justamente para que o mesmo algoritmo
// sirva a qualquer integranda, sem reescrita.
//
// As quatro verificacoes iniciais garantem que o calculo so prossegue
// com dados validos: limites finitos, intervalo bem orientado e ao menos
// um subintervalo (o que tambem evita a divisao por zero no passo h).
// ----------------------------------------------------------------------------
template <typename Funcao>
[[nodiscard]] Real integrar_trapezios(
    Funcao funcao,
    Real limite_inferior,
    Real limite_superior,
    std::size_t numero_subintervalos
)
{
    if (!std::isfinite(limite_inferior) || !std::isfinite(limite_superior)) {
        throw std::invalid_argument("Os limites de integracao devem ser finitos.");
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

    // As extremidades entram com peso 1/2; os pontos internos, com peso 1.
    Real soma = Real{0.5} * (funcao(limite_inferior) + funcao(limite_superior));

    for (std::size_t i = 1; i < numero_subintervalos; ++i) {
        soma += funcao(limite_inferior + static_cast<Real>(i) * passo);
    }

    return soma * passo;
}

// ----------------------------------------------------------------------------
// Contagem dos testes.
//
// Em vez de espalhar contadores pelo programa, guardamos os totais nesta
// struct simples e deixamos as funcoes de teste atualiza-los. Nao ha
// necessidade de classe nem de encapsulamento: sao apenas dois numeros.
// ----------------------------------------------------------------------------
struct Placar {
    unsigned aprovados = 0;
    unsigned total     = 0;
};

// Registra o resultado de um teste e imprime PASSOU ou FALHOU.
void registrar(Placar& placar, const std::string& descricao, bool passou)
{
    ++placar.total;
    if (passou) {
        ++placar.aprovados;
    }
    std::cout << (passou ? "[PASSOU] " : "[FALHOU] ") << descricao << '\n';
}

// Teste de valor: 'obtido' deve ficar dentro de 'tolerancia' do 'esperado'.
void testar_valor(
    Placar& placar,
    const std::string& descricao,
    Real obtido,
    Real esperado,
    Real tolerancia
)
{
    const bool passou = std::abs(obtido - esperado) <= tolerancia;
    registrar(placar, descricao, passou);

    if (!passou) {
        std::cout << std::fixed << std::setprecision(16)
                  << "    obtido     = " << obtido     << '\n'
                  << "    esperado   = " << esperado   << '\n'
                  << "    tolerancia = " << tolerancia << '\n';
    }
}

// Teste de excecao: a chamada 'acao' deve lancar std::invalid_argument.
template <typename Acao>
void testar_excecao(Placar& placar, const std::string& descricao, Acao acao)
{
    bool lancou = false;
    try {
        acao();
    } catch (const std::invalid_argument&) {
        lancou = true;
    } catch (...) {
        lancou = false;
    }
    registrar(placar, descricao, lancou);

    if (!lancou) {
        std::cout << "    a chamada deveria ter lancado std::invalid_argument.\n";
    }
}

} // namespace

int main()
{
    std::cout << "Exercicio Computacional 2.1 -- Regra dos trapezios\n\n";
    std::cout << std::fixed << std::setprecision(12);

    Placar placar;

    // -- Caso principal: integral de x^2 em [0, 1], cujo valor exato e 1/3. --
    const Real aproximada = integrar_trapezios(quadratica, Real{0.0}, Real{1.0}, 1000);
    const Real exata      = Real{1.0} / Real{3.0};

    std::cout << "Integral de x^2 em [0, 1] com 1000 subintervalos:\n";
    std::cout << "    aproximada = " << aproximada << '\n';
    std::cout << "    exata      = " << exata      << "\n\n";

    testar_valor(placar, "aproximacao de x^2 em [0,1] proxima de 1/3",
                 aproximada, exata, Real{2e-7});

    // -- A regra e exata para retas: integral de 2x + 1 em [0, 2] vale 6. --
    const Real reta = integrar_trapezios(
        [](Real x) { return Real{2.0} * x + Real{1.0}; },
        Real{0.0}, Real{2.0}, 8
    );
    testar_valor(placar, "integral de 2x + 1 em [0,2] igual a 6 (exata para retas)",
                 reta, Real{6.0}, Real{1e-14});

    // -- Dados invalidos devem ser rejeitados. --
    // Aqui a chamada deve falhar antes de retornar, entao descartamos o
    // valor de retorno de proposito com (void), ja que a funcao e nodiscard.
    testar_excecao(placar, "n = 0 e rejeitado",
        [] { (void)integrar_trapezios(quadratica, Real{0.0}, Real{1.0}, 0); });
    testar_excecao(placar, "intervalo de comprimento nulo e rejeitado",
        [] { (void)integrar_trapezios(quadratica, Real{1.0}, Real{1.0}, 10); });
    testar_excecao(placar, "intervalo invertido e rejeitado",
        [] { (void)integrar_trapezios(quadratica, Real{2.0}, Real{1.0}, 10); });

    std::cout << "\nResumo: " << placar.aprovados << " de " << placar.total
              << " testes aprovados.\n";

    return (placar.aprovados == placar.total) ? 0 : 1;
}