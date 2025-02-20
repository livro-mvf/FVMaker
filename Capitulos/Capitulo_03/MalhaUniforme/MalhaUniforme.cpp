//==============================================================================
// Nome        : MalhaUniforme.cpp
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
 * @file MalhaUniforme.cpp
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

#include <algorithm>            // std::sort
#include <ctime>                // std::time
#include <iomanip>              // std::setw
#include <iostream>             // std::cout
#include <random>               // std::mt19937 e std::uniform_real_distribution
//==============================================================================
//      Includes do fvmmaker
//==============================================================================


#include <FVMAKER/Grid/Grid1D/UniformGrid1D.h>
#include <FVMAKER/Misc/Misc.h> 

//==============================================================================
//      typedef
//==============================================================================

using fvm::grd::UniformGrid1D;       

int main() {

//==============================================================================
//  dados constantes
//==============================================================================

const Real  lenght(1.0);            // Comprimento do dominio unidimensional
const Real  xInit(-4.0);             // Coordenada inicial do dominio
const int   nVol(10);               // Numero de volumes da malha

//==============================================================================
//  Impressao dos dados de entrada
//==============================================================================

auto flags  = std::cout.flags();
    std::cout << "Dados iniciais do Malha Uniforme\n";
    fvm::PrintLine(std::cout);
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Comprimento do dominio unidimencional " 
              << std::setw(10) << lenght << "\n";
    std::cout << "Coordenada inicial da malha           " 
              << std::setw(10) << xInit << "\n";
    std::cout << "Numero de volumes finitos             " 
              << std::setw(10) << nVol << "\n";
    fvm::PrintLine(std::cout);

    

UniformGrid1D      ug1D(nVol, lenght, xInit);

    
return EXIT_SUCCESS;

}
