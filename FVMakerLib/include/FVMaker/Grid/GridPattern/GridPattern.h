#pragma once

//==============================================================================
// Nome        : GridPattern.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Definição da classe GridPattern, que fornece a interface
//               para padrões de malha na biblioteca FVMaker.
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, versão 3
// da licença, ou (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil, mas SEM
// QUALQUER GARANTIA; sem mesmo a garantia implícita de COMERCIABILIDADE
// ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO. Consulte a Licença Pública
// Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
// com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <string_view>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Grid/Grid1D/Grid1D.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Classe base para padrões de malha.
 *
 * Esta classe define a interface que todos os padrões de malha devem
 * implementar. Ela permite que o código que a utiliza trabalhe de forma
 * genérica com diferentes padrões, chamando métodos comuns a todos eles.
 *
 * Cada padrão concreto, como FaceCentered ou CellCentered, deve herdar
 * de GridPattern e implementar os métodos aqui definidos.
 */
class GridPattern {

//==============================================================================
// Construtores/Destrutora
//==============================================================================
    
public:
    
    GridPattern() noexcept = default;
    GridPattern(const GridPattern&) noexcept = default;
    virtual ~GridPattern() noexcept = default;

    GridPattern(GridPattern&&) = delete;

    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================

public:
        
    GridPattern& operator=(const GridPattern&) = delete;
    GridPattern& operator=(GridPattern&&) = delete;

//==============================================================================
// Funções puramente virtuais
//==============================================================================

public:

    
    [[nodiscard]] virtual bool GenerateCoordinates (void*) = 0;
    [[nodiscard]] virtual std::shared_ptr<GridPattern> Clone() const = 0;
    
    /**
     * @brief Retorna o tipo do padrão de malha.
     *
     * Exemplo: "FaceCentered", "CellCentered".
     */
      [[nodiscard]] virtual std::string TipoPadraoMalha() const = 0;

      
};

using ConstUniqueGridPattern = std::unique_ptr<const GridPattern>;

GRID_NAMESPACE_CLOSE

