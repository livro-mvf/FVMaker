//==============================================================================
// Nome        : Configure.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Arquivo de configuração global da biblioteca FVMaker,
//               com definições numéricas para precisão e cálculos.
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
 * @file Configure.h
 * @brief Configurações globais da biblioteca FVMaker.
 *
 * Este arquivo contém definições de constantes e configurações gerais
 * utilizadas em toda a biblioteca FVMaker, incluindo precisão numérica
 * e valores limite para cálculos.
 *
 * @author João Flávio Vasconcellos
 * @date 2025
 * @copyright GNU General Public License v3.0
 *
 * @ingroup Misc
 */

#include <FVMaker/Misc/Type.h>

//==============================================================================
// Includes da Biblioteca Padrão do C++
//==============================================================================
#include <cmath>  ///< Funções matemáticas como std::acos
#include <limits> ///< Limites numéricos da máquina

FVMAKER_NAMESPACE_OPEN

/** @brief Definição do valor zero, considerando precisão numérica. */
const Real ZERO = (1e250 * std::numeric_limits<Real>::min());

/** @brief Limite numérico usado para evitar erros de arredondamento. */
const Real LIMITE(1e-30);

/** @brief Pequeno valor usado para comparações de precisão numérica. */
const Real EPSILON(1e-6);

/** @brief Tamanho padrão usado em diversas configurações da biblioteca. */
const int LSIZE(0x50);

/**
 * @brief Retorna o valor de \f$ \pi \f$ com precisão longa.
 * @return Valor de \f$ \pi \f$ com precisão de long double.
 */
constexpr Real Pi() { return acos(-1.0L); }

FVMAKER_NAMESPACE_CLOSE
