//==============================================================================
// Nome        : FaceCentered.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Classe FaceCentered, um padrao de malha que herda de GridPattern,
//               atualizado para permitir a definicao de um offset.
//
// Este programa e software livre: voce pode redistribui-lo e/ou
// modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3
// da licenca, ou (a seu criterio) qualquer versao posterior.
//
// Este programa e distribuido na esperanca de que seja util, mas SEM
// QUALQUER GARANTIA; sem mesmo a garantia implicita de COMERCIABILIDADE
// ou ADEQUACAO A UM DETERMINADO PROPOSITO. Consulte a Licenca Publica
// Geral GNU para mais detalhes.
//
// Voce deve ter recebido uma copia da Licenca Publica Geral GNU junto
// com este programa. Caso contrario, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file FaceCentered.h
 * @brief Implementacao do padrao de malha com faces centradas
 * @ingroup GridPattern
 *
 * Calcula as coordenadas das faces com base nas posicoes dos centros,
 * utilizando um offset configuravel para controle preciso do posicionamento.
 * Integra com ParallelControl para operacoes paralelas quando disponivel.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

#include <FVMaker/Grid/GridPattern/abstractGridPattern.h>
#include <FVMaker/Misc/parallelControl.h>

GRID_NAMESPACE_OPEN

/**
 * @class FaceCentered
 * @brief Implementacao do padrao de malha com faces centradas
 *
 * Este padrao calcula as coordenadas das faces das celulas com base nas
 * posicoes dos centros, utilizando um offset configuravel. O valor padrao do
 * offset e 0.5, resultando em um posicionamento central exato entre os centros.
 *
 * @note Utiliza ParallelControl para operacoes paralelas na construcao da malha
 * @note Padrao frequentemente usado em esquemas de diferencas finitas
 *
 * @ingroup GridPattern
 */
class FaceCentered : public AbstractGridPattern
{
public:
    FaceCentered() noexcept = default;
    explicit FaceCentered(double offset) noexcept;
    FaceCentered(const FaceCentered&) noexcept = default;
    ~FaceCentered() noexcept override = default;
    FaceCentered(FaceCentered&&) = delete;

    FaceCentered& operator=(const FaceCentered&) = delete;
    FaceCentered& operator=(FaceCentered&&) = delete;

//    template <typename T>
//    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const;
    
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T> *grid) const
    {
        
        if (!grid->GeraCentros()) return false;
        if (!grid->CalculaFaces(offset_)) return false;
        if (!grid->CalculaDistancias()) return false;
        
        return true;
        
    }    
    

    [[nodiscard]] std::shared_ptr<AbstractGridPattern> Clone() const override;
    std::string TipoPadraoMalha() const override;
};

GRID_NAMESPACE_CLOSE