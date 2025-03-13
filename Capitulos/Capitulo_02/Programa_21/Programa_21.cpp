//==============================================================================
// Nome        : Programa_21.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas dos nós para uma malha
//               unidimensional de diferenças finitas
// Livro       : Código 2.1 do livro
//               Solução Numérica de Equações Diferenciais - 
//               O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 12 / mar / 2025
//
// Direitos autorais : Copyright (C) <2025> Joao Flavio Vasconcellos 
//                    (jflavio at iprj.uerj.br)
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU como publicada
// pela Free Software Foundation, tanto a versão 3 da Licença como (a
// seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM PARTICULAR.
// Consulte a Licença Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU
// junto com este programa. Se não, veja <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file Programa_21.cpp
 * @brief Programa para calcular as coordenadas dos nós de uma malha.
 *
 * Este programa gera uma malha unidimensional utilizando diferenças finitas.
 * Os nós são distribuídos segundo uma progressão geométrica especificada.
 *
 * @version 1.1
 * @date 2025-03-12
 * @author João Flávio Vasconcellos
 *
 * @copyright Copyright (c) 2025
 * @license GNU GPL v3
 */

//==============================================================================
//      Inclusões da Biblioteca Padrão do C++
//==============================================================================

#include <algorithm>    // std::for_each
#include <iomanip>      // std::setw, std::setprecision, std::fixed, std::scientific
#include <iostream>     // std::cout, std::endl
#include <iterator>     // std::ostream_iterator
#include <vector>       // std::vector

//==============================================================================
//      typedef
//==============================================================================

using Real      = double;
using VetorReal = std::vector<Real>;

//==============================================================================
//      Função para o cálculo de real elevado a um inteiro
//==============================================================================

/**
 * @brief Calcula a potência de um número real com expoente inteiro.
 *
 * @param _base  Base real.
 * @param _exp   Expoente inteiro.
 * @param _result Resultado parcial (usado na recursão).
 * @return Real  Resultado de _base elevado a _exp.
 */
constexpr Real ipow(const Real& _base, const int& _exp, 
                      const Real& _result = 1) {
    return _exp < 1 ? _result 
                    : ipow(_base * _base, _exp >> 1,
                           (_exp % 2) ? _result * _base : _result);
}

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal do programa.
 *
 * Esta função gera uma malha unidimensional utilizando diferenças finitas e
 * imprime as coordenadas dos nós, os quais são distribuídos segundo uma
 * progressão geométrica.
 *
 * @return int Código de saída do programa. Retorna EXIT_SUCCESS se a 
 *             execução for bem-sucedida.
 */
int main() {

//==============================================================================
//      Definição das variáveis
//==============================================================================

    const unsigned NNOS  = 11;      // Número de nós da malha
    const Real     LENGTH = 4.0;     // Comprimento do domínio
    const Real     RATIO  = 1.05;     // Razão da progressão geométrica
    const Real     XINIT  = -2.0;    // Coordenada do primeiro nó
    constexpr unsigned LSIZE = 25;

    Real Dx;                      // Distância entre os nós
    VetorReal xCentro(NNOS);      // Coordenadas x dos nós

//==============================================================================
//      Impressão dos dados da malha
//==============================================================================

    std::cout << "Dados iniciais da malha geométrica\n";
    std::cout << std::string(LSIZE + 24, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " 
              << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial da malha:           " 
              << std::setw(10) << XINIT << "\n";
    std::cout << "Razão da progressão geométrica:        " 
              << std::setw(10) << RATIO << "\n";
    std::cout << "Número de nós da malha:                " 
              << std::setw(10) << NNOS << "\n";
    std::cout << std::string(LSIZE + 24, '=') << "\n\n\n";

//==============================================================================
//      Geração das coordenadas dos nós segundo uma progressão geométrica
//==============================================================================

    Real x = XINIT;
    Dx = LENGTH * (RATIO - 1.0) / (ipow(RATIO, NNOS - 1) - 1.0);

    // Lambda para gerar a progressão geométrica
    auto GeometricProgression = [&x, &Dx, RATIO](auto& _x) -> void {
        _x = x;
        x += Dx;
        Dx *= RATIO;
    };

    std::for_each(std::begin(xCentro), std::end(xCentro),
                  GeometricProgression);

//==============================================================================
//      Impressão do resultado
//==============================================================================

    std::cout << "Impressão das coordenadas dos nós da malha geométrica\n";
    std::cout << std::string(LSIZE, '=') << "\n";
    std::cout << std::setw(5) << "NO" 
              << std::setw(20) << "xNo" << "\n";
    std::cout << std::string(LSIZE, '=') << "\n";

    unsigned no = 0;
    auto Print = [&no](const auto& _xC) {
        std::stringstream ss;
        ss << std::setw(5) << no 
           << std::scientific 
           << std::setw(20) << _xC;
        no++;
        return ss.str();
    };

    std::transform  (   std::begin(xCentro)
                    ,   std::end(xCentro)
                    ,   std::ostream_iterator<std::string>(std::cout, "\n")
                    ,   Print
                    );

    std::cout << std::string(LSIZE, '=') << "\n\n";

//==============================================================================
//      Limpeza do vetor
//==============================================================================

    xCentro.clear();

//==============================================================================
//     Fim do programa
//==============================================================================

    return EXIT_SUCCESS;
}
