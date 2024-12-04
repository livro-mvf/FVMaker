//==============================================================================
// Nome        : MalhaGeometrica.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Programa que calcula as coordenadas dos nós
//                para uma malha unidimensional de diferenças finitas
// Livro       : Codigo 2.1 do livro
//                Solução Numérica de Equações Diferenciais - 
//                O Método de Volumes Finitos
// Testado     : gcc 10.2 usando c++20 em 30 / nov / 2024
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
 * @file MalhaGeometrica.cpp
 * @brief Programa para calcular as coordenadas dos nós de uma malha.
 *
 * Este programa gera uma malha unidimensional utilizando diferenças finitas.
 * Os nós são gerados com uma progressão geométrica especificada.
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

#include <algorithm>            // std::generate_n
#include <iomanip>              // std::setw
#include <iostream>             // std::cout , std::endl
#include <iterator>             // std::ostream_iterator
#include <vector>               // std::vector
#include <sstream>              // std::stringstream

//==============================================================================
//      typedef
//==============================================================================

using Real = double;
using VetorReal = std::vector<Real>;

//==============================================================================
//      Função para o cálculo de real elevado a um inteiro
//==============================================================================

/**
 * @brief Função para calcular potências de reais com expoente inteiro.
 *
 * @param _base Base real da potência.
 * @param _exp Expoente inteiro da potância.
 * @param _result Resultado parcial, usado para recursão interna.
 * @return Real Resultado da base elevada ao expoente.
 */
constexpr Real ipow(const Real& _base, const int& _exp, const Real& _result = 1) {
    return _exp < 1 ? _result : ipow(_base * _base, _exp >> 1,
                                     (_exp % 2) ? _result * _base : _result);
}

//==============================================================================
//      Função principal
//==============================================================================

/**
 * @brief Função principal do programa.
 *
 * Esta função gera uma malha unidimensional utilizando diferenças finitas
 * e imprime as coordenadas dos nós e das faces dos volumes.
 *
 * @return int Indicação de sucesso ou falha na execução do programa.
 */
int main() {

//==============================================================================
//      Definição das variáveis
//==============================================================================

const unsigned NVOL = 10;         // Número de nós da malha
const Real LENGHT = 1.0;          // Comprimento do domínio    
const Real RATIO = 1.5;           // Razão da progressão geométrica
const Real XINIT = 2.0;           // Coordenada do primeiro nó
    
Real Dx;                         // Distância entre os nós
VetorReal xCentro(NVOL);         // Coordenadas x dos nós
VetorReal xFace(NVOL + 1);       // Coordenadas das faces dos volumes    

//==============================================================================
//      Geração das coordenadas dos nós segundo uma progressão geométrica
//==============================================================================

Real x = XINIT;
Dx = LENGHT * (RATIO - 1.0) / (ipow(RATIO, NVOL) - 1.0);

// Lambda para gerar a progressão geométrica
auto GeometricProgression = [&x, &Dx, RATIO](auto& _x) -> void {
    _x = x;
    x += Dx;
    Dx *= RATIO;
};

std::for_each(std::begin(xCentro), std::end(xCentro), GeometricProgression);

// Calcula as coordenadas das faces
auto CoordFace = [](const auto& _xw, const auto& _xe) {
    return 0.5 * (_xe + _xw);
};

std::transform(std::begin(xCentro), std::end(xCentro) - 1,
               std::begin(xCentro) + 1, std::begin(xFace) + 1,
               CoordFace);

xFace.front() = XINIT;
xFace.back() = XINIT + LENGHT;

//==============================================================================
//      Impressão do resultado
//==============================================================================

constexpr unsigned LSIZE = 45;

std::cout << std::string(LSIZE, '=') << "\n";
std::cout << std::setw(5) << "vol" << std::setw(20) << "xCentro"
          << std::setw(20) << "xFace" << "\n";
std::cout << std::string(LSIZE, '=') << "\n\n";

unsigned volume = 0;

auto Print = [&volume](const auto& _xC, const auto& _xF) {
    std::stringstream ss;
    ss << std::setw(5) << volume << std::scientific << std::setw(20) << _xC
       << std::setw(20) << _xF;
    volume++;
    return ss.str();    
};

std::transform(std::begin(xCentro), std::end(xCentro),
               std::begin(xFace),
               std::ostream_iterator<std::string>(std::cout, "\n"),
               Print);

std::cout << std::setw(5) << volume << std::scientific << std::setw(40)
          << *(--std::end(xFace));

//==============================================================================
//      Apagando os vetores
//==============================================================================

xCentro.clear();
xFace.clear();

//==============================================================================
//     Fim do programa
//==============================================================================

return EXIT_SUCCESS;
}
