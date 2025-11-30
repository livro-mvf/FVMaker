//==============================================================================
// Nome        : CellCentered.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Classe CellCentered, um padrão de malha que herda de GridPattern,
//               atualizada para permitir a definição de um offset.
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

#pragma once

/**
 * @file CellCentered.h
 * @brief Implementação do padrão de malha com volumes centrados
 * @ingroup GridPattern
 *
 * Calcula as coordenadas dos centros com base nas posições das faces,
 * utilizando um offset configurável para controle preciso do posicionamento.
 * Integra com ParallelControl para operações paralelas quando disponível.
 *
 * @author João Flávio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/AbstractGridPattern.h>
//#include <FVMaker/Misc/ParallelControl.h>

GRID_NAMESPACE_OPEN

/**
 * @class CellCentered
 * @brief Implementação do padrão de malha com volumes centrados
 *
 * Este padrão calcula as coordenadas dos centros das células com base nas
 * posições das faces, utilizando um offset configurável. O valor padrão do
 * offset é 0.5, resultando em um posicionamento central exato.
 *
 * @note Utiliza ParallelControl para operações paralelas na construção da malha
 *
 * @ingroup GridPattern
 */
class CellCentered : public AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão (offset = 0.5)
     * @post offset_ é inicializado com 0.5 (centro exato)
     */
    CellCentered() noexcept = default;

    /**
     * @brief Construtor com offset customizado
     * @param _offset Valor do offset para posicionamento (0.5 = centro exato)
     * @post offset_ é inicializado com o valor fornecido
     */
    explicit CellCentered(const Real& _offset) noexcept
        : AbstractGridPattern(_offset)
    {
    }

    /**
     * @brief Construtor de cópia (default)
     */
    CellCentered(const CellCentered&) noexcept = default;

    /**
     * @brief Destrutor virtual padrão
     */
    ~CellCentered() noexcept override = default;

    /**
     * @brief Construtor de movimento deletado
     */
    CellCentered(CellCentered&&) = delete;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    /**
     * @brief Operador de atribuição de cópia deletado
     */
    CellCentered& operator=(const CellCentered&) = delete;

    /**
     * @brief Operador de atribuição de movimento deletado
     */
    CellCentered& operator=(CellCentered&&) = delete;

//==============================================================================
// Funções virtuais
//==============================================================================
public:
    /**
     * @brief Constrói a malha no padrão CellCentered
     * @tparam T Tipo do grid
     * @param _grid Ponteiro para o grid a ser construído
     * @return true se bem-sucedido, false caso contrário
     *
     * @note Utiliza ParallelControl para operações paralelas quando disponível
     * @note Segue a sequência:
     *       1. Gera as faces da malha
     *       2. Calcula os centros com base no offset
     *       3. Calcula as distâncias entre elementos
     */
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>* _grid) const;

//==============================================================================
// Funções puramente virtuais (implementadas)
//==============================================================================
public:
    /**
     * @brief Cria uma cópia do objeto
     * @return shared_ptr para a nova instância
     *
     * @note Implementação do método virtual puro da classe base
     */
    [[nodiscard]] std::shared_ptr<AbstractGridPattern> Clone() const override;

    /**
     * @brief Obtém o nome do padrão de malha
     * @return "Volume Centrado"
     *
     * @note Implementação do método virtual puro da classe base
     */
    std::string TipoPadraoMalha() const override;
};

// Implementação do template
template <typename T>
bool CellCentered::BuildMesh(AbstractGrid1D<T>* _grid) const
{
    bool flag = _grid->GeraFaces();
    auto ptrXFace = _grid->AddressxFace();
    (*ptrXFace)[0] = _grid->XInit();
    (*ptrXFace)[_grid->NVol()] = _grid->XInit() + _grid->Length();
    flag = flag && _grid->CalculaCentros(offset_);
    flag = flag && _grid->CalculaDistancias();
    return flag;
}

GRID_NAMESPACE_CLOSE