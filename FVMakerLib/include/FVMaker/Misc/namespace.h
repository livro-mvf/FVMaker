//==============================================================================
// Nome        : Namespace.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Definição das macros para namespaces da biblioteca FVMaker.
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
 * @file Namespace.h
 * @brief Definições de macros para namespaces na biblioteca FVMaker.
 *
 * Este arquivo define macros para abertura e fechamento dos namespaces
 * principais utilizados na FVMaker, garantindo padronização no código.
 *
 * @author João Flávio Vasconcellos
 * @date 2025
 * @copyright GNU General Public License v3.0
 *
 * @ingroup Misc
 */

/** @brief Macro para abrir o namespace principal da FVMaker. */
#define FVMAKER_NAMESPACE_OPEN namespace fvm {

/** @brief Macro para fechar o namespace principal da FVMaker. */
#define FVMAKER_NAMESPACE_CLOSE }

/** @brief Macro para abrir o namespace da malha dentro do namespace FVMaker. */
#define GRID_NAMESPACE_OPEN namespace fvm { namespace grd {

/** @brief Macro para fechar o namespace da malha dentro do namespace FVMaker. */
#define GRID_NAMESPACE_CLOSE } }
