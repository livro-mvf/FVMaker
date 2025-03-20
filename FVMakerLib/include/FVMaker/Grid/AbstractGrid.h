//==============================================================================
// Nome        : AbstractGrid.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Classe base para todas as classes de geração de malhas.
//               Define a interface comum para as classes derivadas de malhas
//               1D, 2D, 3D.
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
 * @file AbstractGrid.h
 * @brief Classe base para geração de malhas na biblioteca FVMaker.
 *
 * Esta classe fornece uma interface comum para classes derivadas de malhas
 * (1D, 2D, 3D, etc.). As classes derivadas devem implementar a lógica
 * específica para geração e armazenamento das coordenadas.
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
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Type.h>

//==============================================================================
// Abertura do namespace FVMaker::AbstractGrid
//==============================================================================
GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid
 * @brief Classe base abstrata para geração de malhas (1D, 2D, 3D, etc.).
 *
 * Esta classe define uma interface comum para todas as malhas da biblioteca.
 * As classes derivadas devem implementar a lógica para gerar e armazenar
 * as coordenadas específicas.
 */
template<typename TypePattern>
class AbstractGrid {

public:
    /** @brief Construtor padrão. */
    AbstractGrid() noexcept = default;
    
    /** @brief Construtor de cópia. */
    AbstractGrid(const AbstractGrid&) noexcept = default;
    
    /** @brief Destrutor virtual. */
    virtual ~AbstractGrid() noexcept = default;
    
    /** @brief Construtor de movimento deletado. */
    AbstractGrid(AbstractGrid&&) = delete;
    
    /** @brief Operador de atribuição de cópia deletado. */
    AbstractGrid& operator=(const AbstractGrid&) = delete;
    
    /** @brief Operador de atribuição de movimento deletado. */
    AbstractGrid& operator=(AbstractGrid&&) = delete;
    
    /** @brief Verifica se a grade está vazia. */
    [[nodiscard]] virtual bool empty() const = 0;
};

#include <FVMaker/Grid/AbstractGrid.hpp>

GRID_NAMESPACE_CLOSE
