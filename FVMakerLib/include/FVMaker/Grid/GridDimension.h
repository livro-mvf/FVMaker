//==============================================================================
// Nome        : GridDimension.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.3
// Descrição   : Definição de traits para dimensionamento de grids.
//               Permite identificar automaticamente a dimensão de uma malha
//               e restringir a utilização apenas para
//               malhas suportadas.
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
 * @file GridDimension.h
 * @brief Definição de traits para dimensionamento de grids na FVMaker.
 *
 * Este arquivo contém traits que permitem identificar automaticamente
 * a dimensão de uma malha e restringir o uso de templates apenas
 * para malhas suportadas na biblioteca FVMaker.
 *
 * @author João Flávio Vasconcellos
 * @date 2025
 * @copyright GNU General Public License v3.0
 *
 * @ingroup Grid
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
// Trait para identificar a dimensão de um grid 1D
//==============================================================================
/**
 * @brief Trait para determinar se um tipo é uma malha 1D válida.
 *
 * Esta estrutura de traits verifica se um tipo `T` é derivado de
 * `AbstractGrid1D<CellCentered>` ou `AbstractGrid1D<typename T::DataType>`.
 */
template <typename T>
struct GridDim {
    using DataType = Real;
    static constexpr int value =
        std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ? 1 :
        std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T> ? 1 : -1;
};

//==============================================================================
// Concept para restringir a Diffusion<T> a grids 1D
//==============================================================================
/**
 * @brief Concept que restringe `Diffusion<T>` para malhas 1D suportadas.
 *
 * Verifica se `T` é derivado de `AbstractGrid1D<CellCentered>` ou
 * `AbstractGrid1D<typename T::DataType>`.
 */
template<typename T>
concept Is1DGrid =
    std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ||
    std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T>;

FVMAKER_NAMESPACE_CLOSE
