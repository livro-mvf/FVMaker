#pragma once

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

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <iostream>
#include <string>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/AbstractGridPattern.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Padrão de malha CellCentered.
 *
 * Implementa a lógica específica para um padrão de malha
 * em que o volume é centrado, calculando as coordenadas dos centros
 * com base nas faces.
 */
class CellCentered : public AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão de CellCentered.
     *
     * Utiliza o construtor padrão da classe base (offset_ = 0.5).
     */
    CellCentered() noexcept = default;

    /**
     * @brief Construtor que permite definir o offset para o cálculo do centro.
     *
     * @param offset Valor que determina a fração de deslocamento no posicionamento
     *               dos centros (0.5 = centro exato entre duas faces).
     */
    explicit CellCentered(double offset) noexcept
        : AbstractGridPattern(offset)
    {
    }

    /**
     * @brief Construtor de cópia (default).
     */
    CellCentered(const CellCentered&) noexcept = default;

    /**
     * @brief Destrutor (default).
     */
    ~CellCentered() noexcept override = default;

    /**
     * @brief Construtor de movimento é deletado para evitar problemas de cópia
     *        da classe base.
     */
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
     * @brief Constrói a malha em um Grid1D<GridPattern> de forma cell-centered.
     *
     * @param grid Ponteiro único para o Grid1D que será preenchido.
     * @return true caso a malha seja construída com sucesso, false caso contrário.
     */
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const;

//==============================================================================
// Funções puramente virtuais (implementadas aqui)
//==============================================================================
public:
    /**
     * @brief Cria uma cópia (clone) deste objeto.
     *
     * @return std::shared_ptr<GridPattern> apontando para uma nova instância.
     */
    [[nodiscard]] std::shared_ptr<AbstractGridPattern> Clone() const override;

    /**
     * @brief Retorna o tipo do padrão de malha.
     *
     * @return "Volume Centrado" (em português).
     */
    std::string TipoPadraoMalha() const override;
};

template <typename T>
bool CellCentered::BuildMesh(AbstractGrid1D<T> *grid) const
{

bool flag = grid->GeraFaces();
auto ptrXFace =  grid->AddressxFace();
    (*ptrXFace)[0] =  grid->XInit();
    (*ptrXFace)[grid->NVol()] = grid->XInit() + grid->Length();
    flag = flag && grid->CalculaCentros(offset_);
    flag = flag && grid->CalculaDistancias();
    return flag;
}

GRID_NAMESPACE_CLOSE
