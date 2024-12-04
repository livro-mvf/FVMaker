//==============================================================================
// Nome        : MalhaAleatoria.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas dos nós
//                para uma malha aleatória unidimensional de
//                diferenças finitas
// Livro       : Código 2.2 do livro
//                Solução Numérica de Equações Diferenciais - 
//                O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 17 / fev / 2024 
//
// Direitos autorais : Copyright (C) <2024> Joao Flavio Vasconcellos 
//                    (jflavio at iprj.uerj.br)
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU como
// publicada pela Free Software Foundation, tanto a versão 3 da
// Licença como (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM NENHUMA GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A QUALQUER PROPÓSITO EM
// PARTICULAR. Consulte a Licença Pública Geral GNU para mais
// detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU
// junto com este programa. Se não, veja <http://www.gnu.org/licenses/>.
//==============================================================================

/**
 * @file MalhaAleatoria.cpp
 * @brief Programa para calcular as coordenadas dos nós de uma malha
 * aleatória.
 *
 * Este programa gera uma malha unidimensional aleatória utilizando
 * diferenças finitas. Os nós são distribuídos aleatoriamente dentro do
 * comprimento especificado.
 *
 * @version 1.1
 * @date 2024-11-30
 * @author João Flávio Vasconcellos
 *
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//==============================================================================
//      Includes da Biblioteca Padrão do C++
//==============================================================================

#include <algorithm>            // std::generate_n, std::sort
#include <chrono>               // std::chrono::system_clock::now()
#include <iomanip>              // std::setw
#include <iostream>             // std::cout , std::endl
#include <random>               // std::default_random_engine,
                                // std::uniform_real_distribution<Real>
#include <vector>               // std::vector
#include <ranges>               // std::views::iota

//==============================================================================
//      typedef
//==============================================================================

using Real = double;
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
 * @return int Indicação de sucesso ou falha na execução do programa.
 */
int main() {

//==============================================================================
//      Definição das variáveis constantes
//==============================================================================

const Real XINIT = 0.0;           // Coordenada inicial da malha
const Real LENGHT = 2.0;          // Comprimento da malha
const int NUNNODES = 10;          // Número de nós

//==============================================================================
//      Definição das variáveis para geração de números aleatórios
//==============================================================================

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::uniform_real_distribution<Real> distribution(XINIT, XINIT + LENGHT);

//==============================================================================
//      Definição das variáveis
//==============================================================================

VetorReal xNo(NUNNODES, 0.0);  // Vetor com as coordenadas dos nós da malha

//==============================================================================
//      Geração das coordenadas dos nós
//==============================================================================

// Lambda para gerar números aleatórios
auto Aleatorio = [&distribution, &generator]() {
    return distribution(generator);
};

std::generate_n(std::next(xNo.begin()), NUNNODES - 2, Aleatorio);

xNo.front() = XINIT;                // Coordenada do primeiro nó
xNo.back() = XINIT + LENGHT;        // Coordenada do último nó

std::ranges::sort(xNo);             // Colocando todas as coordenadas em ordem
                                    // crescente

//==============================================================================
//      Impressão da malha
//==============================================================================

constexpr unsigned LSIZE = 25;

std::cout << std::string(LSIZE, '=') << "\n";
std::cout << std::setw(5) << "No" << std::setw(20) << "xNo" << "\n";
std::cout << std::string(LSIZE, '=') << "\n\n";

unsigned no = 0;
std::cout << std::scientific << std::setprecision(4);

for (const auto& xno : xNo) {
    std::cout << std::setw(5) << no++ << std::setw(20) << xno << std::endl;
}

//==============================================================================
//     Fim do programa
//==============================================================================

return EXIT_SUCCESS;
}
