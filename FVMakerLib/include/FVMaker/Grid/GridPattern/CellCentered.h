#pragma once

//==============================================================================
// Nome        : CellCentered.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Classe CellCentered, um padrão de malha que herda de GridPattern.
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
#include <string>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/GridPattern.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Padrão de malha CellCentered.
 *
 * Implementa a lógica específica para um padrão de malha
 * centrado nas células.
 */
class CellCentered : public GridPattern {
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    CellCentered() noexcept = default;
    CellCentered (const CellCentered&) noexcept = default;
    virtual ~CellCentered() noexcept override = default;

    CellCentered (CellCentered&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================

public:
    
    CellCentered& operator=(const CellCentered&) = delete;
    CellCentered& operator=(CellCentered&&) = delete;
    
//==============================================================================
// Funções puramente virtuais
//==============================================================================

public:
    
    [[nodiscard]] std::shared_ptr<GridPattern> Clone() const override;
    [[nodiscard]] virtual bool GenerateCoordinates (void*) override;
    
    /**
     * @brief Retorna o tipo do padrão de malha.
     *
     * @return "CellCentered".
     */
    std::string TipoPadraoMalha() const override ;

};

GRID_NAMESPACE_CLOSE

