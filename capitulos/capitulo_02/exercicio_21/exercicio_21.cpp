//==============================================================================
// SPDX-FileCopyrightText: 2026 Author Name
// SPDX-License-Identifier: MIT
//==============================================================================
// AVISO LEGAL / LEGAL DISCLAIMER
//
// [PT-BR]
// O código-fonte é fornecido sob a licença MIT, no estado em que se encontra
// ("as is"). Embora nos esforcemos para garantir o rigor matemático e a
// correção das implementações, a natureza da computação científica implica
// que inconsistências pontuais possam ocorrer. Caso identifique algum erro,
// comportamento inesperado, ou tenha sugestões de aprimoramento, seremos
// imensamente gratos se nos puder contactar através do e-mail
// livromvf@gmail.com. A sua contribuição é inestimável para o aperfeiçoamento
// contínuo deste livro e da sua biblioteca de apoio.
//
// [EN-GB]
// The source code is provided under the MIT Licence, on an "as is" basis.
// Whilst we endeavour to ensure mathematical rigour and correctness in the
// C++ implementations, the nature of scientific computing implies that
// occasional inconsistencies or errors may arise. Should you identify any
// bugs, unexpected behaviour, or have suggestions for improvement, we would
// be most grateful if you could reach out to us at livromvf@gmail.com.
// Your feedback is invaluable to the continuous refinement of this textbook
// and its accompanying library.
//==============================================================================

//==============================================================================
// Exercicio Computacional 2.1
// Refatoracao da funcao da abertura - Regra do Trapezio
//
// Este programa refatora a funcao calc original, que implementava a regra do
// trapezio para integracao numerica. A versao original tinha varios problemas:
//
// - Nomenclatura obscura (calc, h, s, n)
// - Dependencia de funcao f global
// - Ausencia de documentacao
// - Falta de protecao contra n = 0
//
// A versao refatorada aplica os principios de Codigo Limpo da Secao 2.2:
//
// - Nomes descritivos que explicam o proposito
// - Funcao de integracao recebe a funcao integranda como parametro
// - Documentacao completa explicando o que, como e por que
// - Validacao defensiva das entradas
//
// O programa calcula integrais definidas usando a regra do trapezio composta:
//
// integral de f(x) em [a, b] ~ (delta_x / 2) * [f(x0) + 2 f(x1) + ... + f(xn)]
//
// onde delta_x = (b - a) / n e x_i = a + i * delta_x.
//
// Dados do problema:
// a, limite inferior de integracao.
// b, limite superior de integracao.
// n, numero de subintervalos, com n > 0.
// f, funcao a ser integrada.
//
// O codigo e autocontido e nao depende da biblioteca em desenvolvimento.
//==============================================================================

//==============================================================================
// Bibliotecas padrao do C++
//==============================================================================

#include <cmath>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

//==============================================================================
// Tipo real usado no programa
//==============================================================================

using Real = double;

//==============================================================================
// Declaracoes das funcoes
//==============================================================================

// Calcula a integral definida de uma funcao usando a regra do trapezio composta.
[[nodiscard]] inline Real calcular_integral_trapezio(
    Real a,
    Real b,
    std::size_t numero_subintervalos,
    std::function<Real(Real)> funcao_integranda
);

// Imprime os dados iniciais da execucao.
inline void imprimir_dados_iniciais(
    Real a,
    Real b,
    std::size_t numero_subintervalos,
    const std::string& descricao_funcao
);

// Imprime o resultado do calculo da integral.
inline void imprimir_resultado(
    Real integral_aproximada,
    Real integral_exata,
    const std::string& descricao_funcao
);

//==============================================================================
// Funcoes de teste com integrais conhecidas
//==============================================================================

inline Real funcao_constante(Real /* x */) { return Real{1.0}; }
inline Real funcao_linear(Real x) { return x; }
inline Real funcao_quadratica(Real x) { return x * x; }
inline Real funcao_seno(Real x) { return std::sin(x); }
inline Real funcao_exponencial(Real x) { return std::exp(x); }

//==============================================================================
// Programa principal
//==============================================================================

int main()
{
    try {
        std::cout << std::fixed << std::setprecision(12);

        std::cout << "Regra do Trapezio - Versao Refatorada\n";
        std::cout << "======================================\n\n";

        //======================================================================
        // Caso de teste 1: Funcao constante
        //======================================================================
        {
            const Real a = 0.0;
            const Real b = 1.0;
            const std::size_t n = 10;
            const std::string descricao = "f(x) = 1";
            const Real integral_exata = 1.0;

            imprimir_dados_iniciais(a, b, n, descricao);
            const Real integral_aproximada = calcular_integral_trapezio(a, b, n, funcao_constante);
            imprimir_resultado(integral_aproximada, integral_exata, descricao);
        }

        //======================================================================
        // Caso de teste 2: Funcao linear
        //======================================================================
        {
            const Real a = 0.0;
            const Real b = 1.0;
            const std::size_t n = 10;
            const std::string descricao = "f(x) = x";
            const Real integral_exata = 0.5;

            imprimir_dados_iniciais(a, b, n, descricao);
            const Real integral_aproximada = calcular_integral_trapezio(a, b, n, funcao_linear);
            imprimir_resultado(integral_aproximada, integral_exata, descricao);
        }

        //======================================================================
        // Caso de teste 3: Funcao quadratica
        //======================================================================
        {
            const Real a = 0.0;
            const Real b = 1.0;
            const std::size_t n = 100;
            const std::string descricao = "f(x) = x^2";
            const Real integral_exata = 1.0 / 3.0;

            imprimir_dados_iniciais(a, b, n, descricao);
            const Real integral_aproximada = calcular_integral_trapezio(a, b, n, funcao_quadratica);
            imprimir_resultado(integral_aproximada, integral_exata, descricao);
        }

        //======================================================================
        // Caso de teste 4: Funcao seno
        //======================================================================
        {
            const Real a = 0.0;
            const Real b = 3.14159265358979323846; // pi
            const std::size_t n = 100;
            const std::string descricao = "f(x) = sen(x)";
            const Real integral_exata = 2.0;

            imprimir_dados_iniciais(a, b, n, descricao);
            const Real integral_aproximada = calcular_integral_trapezio(a, b, n, funcao_seno);
            imprimir_resultado(integral_aproximada, integral_exata, descricao);
        }

        //======================================================================
        // Caso de teste 5: Funcao exponencial
        //======================================================================
        {
            const Real a = 0.0;
            const Real b = 1.0;
            const std::size_t n = 1000;
            const std::string descricao = "f(x) = e^x";
            const Real integral_exata = std::exp(1.0) - 1.0;

            imprimir_dados_iniciais(a, b, n, descricao);
            const Real integral_aproximada = calcular_integral_trapezio(a, b, n, funcao_exponencial);
            imprimir_resultado(integral_aproximada, integral_exata, descricao);
        }

        return 0;

    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << '\n';
        return 1;
    }
}

//==============================================================================
// Calculo da integral pela regra do trapezio
//==============================================================================

[[nodiscard]] inline Real calcular_integral_trapezio(
    Real a,
    Real b,
    std::size_t numero_subintervalos,
    std::function<Real(Real)> funcao_integranda
)
{
    if (numero_subintervalos == 0) {
        throw std::invalid_argument(
            "O numero de subintervalos deve ser maior que zero."
        );
    }

    if (!std::isfinite(a)) {
        throw std::invalid_argument(
            "O limite inferior de integracao deve ser finito."
        );
    }

    if (!std::isfinite(b)) {
        throw std::invalid_argument(
            "O limite superior de integracao deve ser finito."
        );
    }

    const Real tamanho_subintervalo =
        (b - a) / static_cast<Real>(numero_subintervalos);

    Real soma = 0.5 * (funcao_integranda(a) + funcao_integranda(b));

    for (std::size_t i = 1; i < numero_subintervalos; ++i) {
        const Real x_i = a + static_cast<Real>(i) * tamanho_subintervalo;
        soma += funcao_integranda(x_i);
    }

    const Real integral = soma * tamanho_subintervalo;

    return integral;
}

//==============================================================================
// Impressao dos dados iniciais
//==============================================================================

inline void imprimir_dados_iniciais(
    Real a,
    Real b,
    std::size_t numero_subintervalos,
    const std::string& descricao_funcao
)
{
    std::cout << "Dados do problema\n";
    std::cout << "-----------------\n";
    std::cout << "Funcao              : " << descricao_funcao << '\n';
    std::cout << "Limite inferior (a) : " << a << '\n';
    std::cout << "Limite superior (b) : " << b << '\n';
    std::cout << "Subintervalos (n)   : " << numero_subintervalos << '\n';
    std::cout << '\n';
}

//==============================================================================
// Impressao do resultado
//==============================================================================

inline void imprimir_resultado(
    Real integral_aproximada,
    Real integral_exata,
    const std::string& descricao_funcao
)
{
    (void)descricao_funcao; 

    std::cout << "Resultado\n";
    std::cout << "---------\n";
    std::cout << "Integral aproximada : " << integral_aproximada << '\n';
    std::cout << "Integral exata      : " << integral_exata << '\n';

    const Real erro_absoluto = std::abs(integral_aproximada - integral_exata);
    const Real erro_relativo = (integral_exata != Real{0.0})
        ? erro_absoluto / std::abs(integral_exata)
        : erro_absoluto;

    std::cout << "Erro absoluto       : " << erro_absoluto << '\n';
    std::cout << "Erro relativo       : " << erro_relativo << '\n';
    std::cout << '\n';
}