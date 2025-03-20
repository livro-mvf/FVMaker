//==============================================================================
// Nome        : Type.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Definição de tipos comuns usados na biblioteca FVMaker,
//               incluindo tipos geométricos e numéricos essenciais.
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, versão 3
// da licença, ou (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil, mas SEM
// QUALQUER GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO.
// Consulte a Licença Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
// com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file Type.h
 * @brief Definição de tipos numéricos e geométricos na biblioteca FVMaker.
 *
 * Este arquivo contém definições essenciais para cálculos de geometria
 * computacional e cálculos numéricos, incluindo vetores, listas e tipos
 * de precisão numérica utilizados na FVMaker.
 *
 * @author João Flávio Vasconcellos
 * @date 2025
 * @copyright GNU General Public License v3.0
 *
 * @ingroup Misc
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <list>             // Para std::list
#include <vector>           // Para std::vector

//==============================================================================
// Includes do FVMaker
//==============================================================================
#include <FVMaker/Misc/Namespace.h>


//==============================================================================
// Definições de tipos comuns
//==============================================================================

/** @brief Tipo Real utilizado para cálculos numéricos de precisão dupla. */
using Real = double;

FVMAKER_NAMESPACE_OPEN

/** @brief Lista de inteiros. */
using LstInt = std::list<int>;

/** @brief Vetor de inteiros. */
using VecInt = std::vector<int>;

/** @brief Lista de números reais. */
using LstReal = std::list<Real>;

/** @brief Vetor de números reais. */
using VecReal = std::vector<Real>;

/** @brief Par de valores reais, usado para armazenar dados numéricos. */
using PairData = std::pair<Real, Real>;

/** @brief Vetor de pares de valores reais. */
using VecSource = std::vector<PairData>;

FVMAKER_NAMESPACE_CLOSE

