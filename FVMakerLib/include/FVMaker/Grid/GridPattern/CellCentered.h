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

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/AbstractGridPattern.h>

GRID_NAMESPACE_OPEN

/**
 * @class CellCentered
 * @brief Implementação do padrão de malha com volumes centrados
 *
 * Calcula as coordenadas dos centros com base nas posições das faces,
 * utilizando um offset configurável para controle preciso do posicionamento.
 */
class CellCentered : public AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão (offset = 0.5)
     */
    CellCentered() noexcept = default;

    /**
     * @brief Construtor com offset customizado
     * @param _offset Valor do offset para posicionamento (0.5 = centro exato)
     */
    explicit CellCentered(const Real& _offset) noexcept
        : AbstractGridPattern(_offset)
    {
    }

    CellCentered(const CellCentered&) noexcept = default;
    ~CellCentered() noexcept override = default;
    CellCentered(CellCentered&&) = delete;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    CellCentered& operator=(const CellCentered&) = delete;
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
     */
    [[nodiscard]] std::shared_ptr<AbstractGridPattern> Clone() const override;

    /**
     * @brief Obtém o nome do padrão de malha
     * @return "Volume Centrado"
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