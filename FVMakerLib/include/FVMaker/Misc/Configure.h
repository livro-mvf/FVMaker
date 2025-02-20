/**
 * @mainpage Documentação da Biblioteca GridVoronoi++
 *
 * @section intro_sec Introdução
 *
 *  Esta é a documentação da biblioteca GridVoronoi++. Ela foi projetada para 
 *  aqueles que necessitam desenvolver programas na área de geometria computacional
 *  envolvendo algoritmos de Voronoi em grades.
 *
 *  Por isso, desenvolvemos esta biblioteca que oferece uma variedade de funcionalidades
 *  relacionadas a diagramas de Voronoi em grades.
 *
 * @section dependencies Dependências
 *
 * Esta biblioteca depende de <a href="https://github.com/adafruit/Adafruit_Sensor">
 * Adafruit_Sensor</a> estar presente no seu sistema. Por favor, certifique-se de ter
 * instalado a versão mais recente antes de usar esta biblioteca.
 *
 * @section authors Autores
 *
 *  O código foi desenvolvido por:
 *  - Lara Botelho Brum
 *  - Luan Rodrigues Soares de Souza
 * 
 *  sob a supervisão de
 *  - João Flávio Vasconcellos
 *
 * @section license Licença
 * 
 * @copyright   : Copyright (C) 2021 João Flávio Vasconcellos 
 *                                      (jflavio at iprj.uerj.br)
 *
 * Este projeto é lançado sob a Licença Pública GNU. Veja o arquivo LICENSE
 * junto com este programa. Se não, veja <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <FVMaker/Misc/Type.h>

//==============================================================================
// Includes da Biblioteca Padrão do C++
//==============================================================================

#include <cmath>                // std::acos
#include <limits>               // std::numeric_limits

FVMAKER_NAMESPACE_OPEN


    const Real ZERO = (1e250 * std::numeric_limits<Real>::min());  ///< Valor de zero
    const Real LIMITE(1e-30);                                      ///< Limite
    const Real EPSILON(1e-6);                                      ///< Epsilon
    const int LSIZE(0x50);                                          ///< Tamanho padrão

    constexpr Real Pi() {return acos(-1.0L);};
FVMAKER_NAMESPACE_CLOSE 