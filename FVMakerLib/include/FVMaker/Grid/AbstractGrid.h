//==============================================================================
// Nome        : AbstractGrid.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.0
// Descricao   : Classe base para todas as classes de geracao de malhas
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
 * @file AbstractGrid.h
 * @brief Classe base para geracao de malhas na biblioteca FVMaker
 * @defgroup Grid Classes de Malha
 *
 * Define a interface comum para classes derivadas de malhas (1D, 2D, 3D, etc.)
 * que serao utilizadas nas simulacoes numericas.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.0
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <FVMaker/Misc/Namespace.h>
#include <FVMaker/Misc/Type.h>

//==============================================================================
// Abertura do namespace FVMaker::Grid
//==============================================================================
GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid
 * @brief Classe base abstrata para geracao de malhas
 * @tparam TypePattern Tipo do padrao de discretizacao (ex: CellCentered)
 *
 * Esta classe define a interface comum para todas as malhas da biblioteca.
 * As classes derivadas devem implementar a logica especifica para geracao
 * e armazenamento das coordenadas.
 *
 * @ingroup Grid
 */
template<typename TypePattern>
class AbstractGrid {
public:
    //==========================================================================
    // Construtores/Destrutores
    //==========================================================================
    
    /**
     * @brief Construtor padrao
     */
    AbstractGrid() noexcept = default;
    
    /**
     * @brief Construtor de copia
     */
    AbstractGrid(const AbstractGrid&) noexcept = default;
    
    /**
     * @brief Destrutor virtual
     */
    virtual ~AbstractGrid() noexcept = default;
    
    /**
     * @brief Construtor de movimento deletado
     */
    AbstractGrid(AbstractGrid&&) = delete;
    
    //==========================================================================
    // Operadores
    //==========================================================================
    
    /**
     * @brief Operador de atribuicao de copia deletado
     */
    AbstractGrid& operator=(const AbstractGrid&) = delete;
    
    /**
     * @brief Operador de atribuicao de movimento deletado
     */
    AbstractGrid& operator=(AbstractGrid&&) = delete;
    
    //==========================================================================
    // Interface Publica
    //==========================================================================
    
    /**
     * @brief Verifica se a grade esta vazia
     * @return true se a grade nao contem elementos
     */
    [[nodiscard]] virtual bool empty() const = 0;
};

GRID_NAMESPACE_CLOSE