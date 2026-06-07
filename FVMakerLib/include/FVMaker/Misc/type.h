//==============================================================================
// Nome        : Type.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Definicao de tipos comuns usados na biblioteca FVMaker,
//               incluindo tipos geometricos e numericos essenciais.
//
// Este programa e software livre: voce pode redistribui-lo e/ou
// modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3
// ou qualquer versao posterior.
//
// Este programa e distribuido na esperanca de que seja util,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de
// COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO.
// Consulte a Licenca Publica Geral GNU para mais detalhes.
//
// Voce deve ter recebido uma copia da Licenca Publica Geral GNU
// junto com este programa. Se nao, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file Type.h
 * @brief Definicoes de tipos numericos e conteineres na biblioteca FVMaker
 * @defgroup Tipos Tipos Fundamentais
 * @ingroup Misc
 *
 * Este arquivo contem definicoes essenciais para calculos numericos e
 * geometria computacional, incluindo aliases para tipos de precisao
 * e conteineres padrao.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <list>     ///< Para std::list
#include <vector>   ///< Para std::vector
#include <utility>  ///< Para std::pair

//==============================================================================
// Includes do FVMaker
//==============================================================================
#include <FVMaker/Misc/namespace.h>

//==============================================================================
// Definicoes de tipos numericos
//==============================================================================

/**
 * @brief Tipo de ponto flutuante padrao para calculos numericos
 * @details Define o tipo real utilizado em toda a biblioteca para calculos
 *          de precisao. Atualmente definido como double.
 * @ingroup Tipos
 */
using Real = double;

FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Definicoes de conteineres
//==============================================================================

/**
 * @brief Lista encadeada de inteiros
 * @ingroup Tipos
 */
using LstInt = std::list<int>;

/**
 * @brief Vetor dinamico de inteiros
 * @ingroup Tipos
 */
using VecInt = std::vector<int>;

/**
 * @brief Lista encadeada de numeros reais
 * @ingroup Tipos
 */
using LstReal = std::list<Real>;

/**
 * @brief Vetor dinamico de numeros reais
 * @ingroup Tipos
 */
using VecReal = std::vector<Real>;

/**
 * @brief Par de valores reais para armazenamento de dados
 * @details Usado frequentemente para armazenar pares coordenados (x,y)
 *          ou pares de valores relacionados.
 * @ingroup Tipos
 */
using PairData = std::pair<Real, Real>;

/**
 * @brief Vetor de pares de valores reais
 * @details Comumente utilizado para representar series de dados ou
 *          colecoes de pontos coordenados.
 * @ingroup Tipos
 */
using VecSource = std::vector<PairData>;

FVMAKER_NAMESPACE_CLOSE