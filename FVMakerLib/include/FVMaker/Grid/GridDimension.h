//==============================================================================
// Nome        : GridDimension.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.3
// Descricao   : Definicao de traits para dimensionamento de grids
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
 * @file GridDimension.h
 * @brief Definicao de traits para dimensionamento de grids na FVMaker
 * @defgroup GridTraits Traits de Dimensionalidade
 * @ingroup Grid
 *
 * Este arquivo contem traits que permitem identificar automaticamente
 * a dimensao de uma malha e restringir o uso de templates apenas
 * para malhas suportadas na biblioteca FVMaker.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.3
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <type_traits>

//==============================================================================
// Includes do FVMaker
//==============================================================================
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Type.h>
#include <FVMaker/Grid/Grid1D/AbstractGrid1D.h>
#include <FVMaker/Grid/GridPattern/CellCentered.h>

using fvm::grd::CellCentered;

//==============================================================================
// Abertura do namespace FVMaker
//==============================================================================
FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Trait para identificacao da dimensao de um grid 1D
//==============================================================================

/**
 * @struct GridDim
 * @brief Trait para determinar a dimensao de uma malha
 * @tparam T Tipo da malha a ser avaliado
 *
 * Esta estrutura de traits verifica se um tipo T e derivado de
 * AbstractGrid1D<CellCentered> ou AbstractGrid1D<typename T::DataType>.
 * Retorna a dimensao da malha (1 para 1D, -1 para nao suportado).
 *
 * @ingroup GridTraits
 */
template <typename T>
struct GridDim {
    using DataType = Real;  ///< Tipo base para calculos numericos
    
    /**
     * @brief Valor da dimensao da malha
     * @retval 1 Se T for uma malha 1D valida
     * @retval -1 Se T nao for uma malha suportada
     */
    static constexpr int value =
        std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ? 1 :
        std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T> ? 1 : -1;
};

//==============================================================================
// Concept para restringir templates a grids 1D
//==============================================================================

/**
 * @concept Is1DGrid
 * @brief Concept que restringe templates a malhas 1D suportadas
 * @tparam T Tipo a ser verificado
 *
 * Verifica se T e derivado de AbstractGrid1D<CellCentered> ou
 * AbstractGrid1D<typename T::DataType>.
 *
 * @ingroup GridTraits
 */
template<typename T>
concept Is1DGrid =
    std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ||
    std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T>;

FVMAKER_NAMESPACE_CLOSE