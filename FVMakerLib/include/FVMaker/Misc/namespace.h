//==============================================================================
// Nome        : Namespace.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Definicao das macros para namespaces da biblioteca FVMaker
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
 * @file Namespace.h
 * @brief Definicoes de macros para namespaces na biblioteca FVMaker
 * @defgroup Namespaces Macros de Namespace
 * @ingroup Misc
 *
 * Este arquivo define macros para abertura e fechamento dos namespaces
 * principais utilizados na FVMaker, garantindo padronizacao no codigo.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

/**
 * @brief Macro para abrir o namespace principal da FVMaker
 * @details Define o inicio do namespace 'fvm' que engloba toda a biblioteca
 * @ingroup Namespaces
 */
#define FVMAKER_NAMESPACE_OPEN namespace fvm {

/**
 * @brief Macro para fechar o namespace principal da FVMaker
 * @details Define o fechamento do namespace 'fvm'
 * @ingroup Namespaces
 */
#define FVMAKER_NAMESPACE_CLOSE }

/**
 * @brief Macro para abrir o namespace da malha dentro do FVMaker
 * @details Define o inicio do namespace 'grd' para operacoes com malhas
 * @ingroup Namespaces
 */
#define GRID_NAMESPACE_OPEN namespace fvm { namespace grd {

/**
 * @brief Macro para fechar o namespace da malha dentro do FVMaker
 * @details Define o fechamento do namespace 'grd'
 * @ingroup Namespaces
 */
#define GRID_NAMESPACE_CLOSE } }