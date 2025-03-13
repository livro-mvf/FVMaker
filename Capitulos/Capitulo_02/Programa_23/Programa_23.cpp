//==============================================================================
// Nome        : Programa_23.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.4
// Descrição   : Programa que calcula as coordenadas dos nós
//                para uma malha simétrica unidimensional de
//                diferenças finitas
// Livro       : Exercício Computacional 2.3
//                Solução Numérica de Equações Diferenciais - 
//                O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 17 / fev / 2024
//
// Direitos autorais : Copyright (C) <2024> Joao Flavio Vasconcellos 
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
 * @file Programa_23.cpp
 * @brief Programa para calcular as coordenadas dos nós de uma malha
 * simétrica.
 *
 * Este programa gera uma malha unidimensional simétrica utilizando
 * diferenças finitas. Os nós são distribuídos aleatoriamente dentro do
 * comprimento especificado, garantindo simetria em relação ao centro do
 * domínio.
 *
 * @version 1.4
 * @date 2024-11-30
 * @author João Flávio Vasconcellos
 *
 * @copyright Copyright (c) 2024
 * @license GNU GPL v3
 */

//==============================================================================
//      Includes da Biblioteca Padrão do C++
//==============================================================================

//#include <algorithm>            // std::generate_n, std::sort
#include <chrono>               // std::chrono::system_clock::now()
#include <iomanip>              // std::setw
#include <iostream>             // std::cout , std::endl
#include <random>               // std::default_random_engine,
                                // std::uniform_real_distribution<Real>
#include <vector>               // std::vector
//#include <ranges>               // std::views::iota

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
 * Esta função gera uma malha simétrica unidimensional utilizando
 * diferenças finitas e imprime as coordenadas dos nós da malha.
 *
 * @return int Indicação de sucesso ou falha na execução do programa.
 */
int main() {

//==============================================================================
//      Definição das variáveis constantes
//==============================================================================

const Real  XINIT = 2.16;           // Coordenada inicial da malha
const Real  LENGTH = 2.43;          // Comprimento da malha
const int   NNOS = 29;          // Número de nós
const Real  XCENTER = XINIT + LENGTH / 2.0; // Coordenada do centro do domínio
const Real  DESVIOPADRAO = 4;       // Desvio padrao
constexpr unsigned LSIZE = 45;

//==============================================================================
//      Impressão dos dados da malha
//==============================================================================

    std::cout << "Dados iniciais da malha geométrica\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do domínio unidimensional: " 
              << std::setw(10) << LENGTH << "\n";
    std::cout << "Coordenada inicial da malha:           " 
              << std::setw(10) << XINIT << "\n";
    std::cout << "Desvio Padrão:                         " 
              << std::setw(10) << DESVIOPADRAO << "\n";
    std::cout << "Número de nós da malha:                " 
              << std::setw(10) << NNOS << "\n";
    std::cout << std::string(LSIZE + 4, '=') << "\n\n\n";



//==============================================================================
//      Definição das variáveis para geração de números aleatórios
//==============================================================================

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::normal_distribution<Real> distribution(XCENTER / 2, DESVIOPADRAO);

//==============================================================================
//      Definição das variáveis
//==============================================================================

VetorReal xNo(NNOS, 0.0);  // Vetor com as coordenadas dos nós da malha

xNo.front() = XINIT;                // Coordenada do primeiro nó
xNo.back() = XINIT + LENGTH;        // Coordenada do último nó

//==============================================================================
//      Geração das coordenadas dos nós
//==============================================================================

// Lambda para gerar nós aleatórios até o centro do domínio
auto Aleatorio = [&distribution, &generator, XINIT, XCENTER]() {
    Real valor;
    do {
        valor = distribution(generator);
    } while (!(valor > XINIT && valor <  XCENTER));
    
    return valor;
};

int halfNodes;

    if (NNOS%2 != 0) {
        halfNodes = (NNOS - 1) / 2 - 1;
    } else {
       halfNodes = NNOS / 2 - 1;
    }
        
std::generate_n(std::next(xNo.begin()), halfNodes, Aleatorio);

// Gerar a parte simétrica dos nós em relação ao centro do domínio
const Real    dx = xNo.back()  + XINIT;
for (int i = 1; i <= halfNodes; ++i) {
    xNo[xNo.size() - 1 - i] =  dx - xNo[i];
}

// Se o número de nós for ímpar, o nó central é exatamente no centro do domínio
if (NNOS % 2 != 0) {
    xNo[halfNodes + 1] = XCENTER;
}

    std::sort(xNo.begin(), xNo.end());

//==============================================================================
//      Impressão da malha
//==============================================================================


std::cout << "Impressão das coordenadas dos nós da malha simétrica\n";
std::cout << std::string(LSIZE, '=') << "\n";
std::cout   << std::setw(5) << "No" 
            << std::setw(20) << "xNo"    
            << std::setw(20) << "xCentre - xNo" 
            <<"\n";
std::cout << std::string(LSIZE, '=') << "\n";

unsigned no = 0;
std::cout << std::scientific << std::setprecision(4);

for (const auto& xno : xNo) {
    std::cout << std::setw(5) << no++ 
              << std::setw(20) << xno
              << std::setw(20) << XCENTER - xno
              << std::endl;
}
    std::cout << std::string(LSIZE, '=') << "\n\n";
//==============================================================================
//     Fim do programa
//==============================================================================

return EXIT_SUCCESS;
}
