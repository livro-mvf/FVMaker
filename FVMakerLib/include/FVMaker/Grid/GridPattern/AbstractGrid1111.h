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
 * @date 2025-05-20   
 * @copyright GNU General Public License v3.0
 *
 * @note Todas as classes derivadas devem implementar o método empty()   
 */

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Type.h>

GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid
 * @brief Classe base abstrata para geração de malhas (1D, 2D, 3D, etc.).
 *
 * @tparam TypePattern Tipo do padrão de malha (FaceCentered, CellCentered, etc.)
 */
template<typename TypePattern>
class AbstractGrid {
    
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:

    AbstractGrid() noexcept = default;
    AbstractGrid(const AbstractGrid&) noexcept = default;
    virtual ~AbstractGrid() noexcept = default;
    AbstractGrid(AbstractGrid&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    
    AbstractGrid& operator=(const AbstractGrid&) = delete;
    AbstractGrid& operator=(AbstractGrid&&) = delete;
    
//==============================================================================
// Funções puramente virtuais
//==============================================================================
public:    
    /**
     * @brief Verifica se a grade está vazia
     * @return true se a grade estiver vazia, false caso contrário
     */
    [[nodiscard]] virtual bool empty() const = 0;
};

GRID_NAMESPACE_CLOSE