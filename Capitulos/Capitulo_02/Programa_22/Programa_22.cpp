//==============================================================================
// Nome        : Programa_22.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas dos nós para uma malha
//               aleatória unidimensional de diferenças finitas
// Livro       : Código 2.2 do livro
//               Solução Numérica de Equações Diferenciais -
//               O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 12 / mar / 2025
//
// Direitos autorais : Copyright (C) <2025> Joao Flavio Vasconcellos
//                    (jflavio at iprj.uerj.br)
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, conforme
// publicada pela Free Software Foundation, na versão 3 da Licença ou
// (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil, mas SEM
// NENHUMA GARANTIA; sem mesmo a garantia implícita de COMERCIABILIDADE
// ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM PARTICULAR. Consulte a Licença
// Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
// com este programa. Se não, veja <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file Programa_22.cpp
 * @brief Programa para calcular as coordenadas dos nós de uma malha
 *        aleatória.
 *
 * Este programa gera uma malha unidimensional aleatória utilizando
 * diferenças finitas. Os nós são distribuídos aleatoriamente dentro do
 * comprimento especificado.
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

#include <algorithm>   // std::sort
#include <chrono>      // std::chrono::system_clock::now()
#include <iomanip>     // std::setw, std::fixed, std::setprecision
#include <iostream>    // std::cout, std::endl
#include <random>      // std::default_random_engine,
#include <vector>      // std::vector

//==============================================================================
//      typedef
//==============================================================================

using Real      = double;
using VetorReal = std::vector<Real>;

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal do programa.
 *
 * Esta função gera uma malha aleatória unidimensional utilizando
 * diferenças finitas e imprime as coordenadas dos nós da malha.
 *
 * @return int Indicação de sucesso na execução do programa.
 */
int main() {

    //==============================================================================
    //      Definição das constantes
    //==============================================================================

    const Real XINIT = 0.0;      // Coordenada inicial da malha
    const Real LENGTH = 2.0;     // Comprimento da malha
    const int  NNOS   = 10;      // Número de nós
    constexpr unsigned LSIZE = 25;

    //==============================================================================
    //      Impressão dos dados iniciais da malha
    //==============================================================================

    std::cout << "Dados iniciais da malha aleatória\n";
    std::cout << std::string(LSIZE + 24, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: "
              << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial da malha:           "
              << std::setw(10) << XINIT << "\n";
    std::cout << "Número de nós da malha:                "
              << std::setw(10) << NNOS << "\n";
    std::cout << std::string(LSIZE + 24, '=') << "\n\n\n";

    //==============================================================================
    //      Inicialização do gerador de números aleatórios
    //==============================================================================

    unsigned seed = std::chrono::system_clock::now().time_since_epoch()
                    .count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<Real> distribution(XINIT, XINIT + LENGTH);

    //==============================================================================
    //      Criação do vetor para armazenar as coordenadas dos nós
    //==============================================================================

    VetorReal xNo(NNOS, 0.0);   // Vetor com as coordenadas dos nós da malha

    //==============================================================================
    //      Geração das coordenadas dos nós
    //==============================================================================

    // Lambda para gerar um número aleatório
    auto Aleatorio = [&distribution, &generator]() {
        return distribution(generator);
    };

    // Gera coordenadas aleatórias para os nós, exceto para o primeiro e o último
    std::generate_n(std::next(xNo.begin()), NNOS - 2, Aleatorio);

    xNo.front() = XINIT;          // Coordenada do primeiro nó
    xNo.back()  = XINIT + LENGTH; // Coordenada do último nó

    // Ordena as coordenadas em ordem crescente
    std::ranges::sort(xNo);


    //==============================================================================
    //      Impressão da malha gerada
    //==============================================================================

    std::cout << "Impressão das coordenadas dos nós da malha aleatória\n";
    std::cout << std::string(LSIZE, '=') << "\n";
    std::cout << std::setw(5) << "No" << std::setw(20) << "xNo" << "\n";
    std::cout << std::string(LSIZE, '=') << "\n";

    unsigned no = 0;
    std::cout << std::scientific << std::setprecision(4);

    for (const auto &xno : xNo) {
        std::cout << std::setw(5) << no++ << std::setw(20) << xno 
                  << std::endl;
    }

    std::cout << std::string(LSIZE, '=') << "\n\n";

    //==============================================================================
    //     Fim do programa
    //==============================================================================

    return EXIT_SUCCESS;
}
