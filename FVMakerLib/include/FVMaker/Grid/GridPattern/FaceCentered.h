#pragma once

//==============================================================================
// Nome        : FaceCentered.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Classe FaceCentered, um padrão de malha que herda de GridPattern,
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
#include <string>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/AbstractGridPattern.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Padrão de malha FaceCentered.
 *
 * Implementa a lógica específica para um padrão de malha
 * centrado nas faces, calculando as coordenadas das faces
 * com base em posições de centros.
 */
class FaceCentered : public AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão de FaceCentered.
     *
     * Utiliza o construtor padrão da classe base (offset_ = 0.5).
     */
    FaceCentered() noexcept = default;

    /**
     * @brief Construtor que permite definir o offset para o cálculo das faces.
     *
     * @param offset Valor que determina a fração de deslocamento ao posicionar
     *               as faces (0.5 = meio exato entre dois centros).
     */
    explicit FaceCentered(double offset) noexcept
        : AbstractGridPattern(offset)
    {
    }

    /**
     * @brief Construtor de cópia (default).
     */
    FaceCentered(const FaceCentered&) noexcept = default;

    /**
     * @brief Destrutor (default).
     */
    ~FaceCentered() noexcept override = default;

    /**
     * @brief Construtor de movimento deletado para evitar problemas de cópia
     *        da classe base.
     */
    FaceCentered(FaceCentered&&) = delete;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    FaceCentered& operator=(const FaceCentered&) = delete;
    FaceCentered& operator=(FaceCentered&&) = delete;

//==============================================================================
// Funções virtuais
//==============================================================================
public:
    /**
     * @brief Constrói a malha em um Grid1D<GridPattern> de forma face-centered.
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
     * @return "Face Centrada".
     */
    std::string TipoPadraoMalha() const override;
};


template <typename T>
bool FaceCentered::BuildMesh(AbstractGrid1D<T> *grid) const
{
bool flag = grid->GeraCentros();
    flag = flag && grid->CalculaFaces(offset_);
    flag = flag && grid->CalculaDistancias();
    return flag;
}

GRID_NAMESPACE_CLOSE
