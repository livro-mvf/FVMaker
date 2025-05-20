//==============================================================================
// Nome        : GridPattern.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.2
// Descrição   : Definição da classe GridPattern, que fornece a interface
//               para padrões de malha na biblioteca FVMaker. Agora também
//               inclui o atributo offset_, que por padrão é 0.5, permitindo
//               variações no posicionamento de faces/centros.
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
// Includes da biblioteca padrão do C++
//==============================================================================
#include <memory>
#include <string_view>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>

GRID_NAMESPACE_OPEN

template<typename TypePattern>
class AbstractGrid1D;

/**
 * @class AbstractGridPattern
 * @brief Classe base abstrata para padrões de malha
 *
 * Define a interface comum para todos os padrões de malha (FaceCentered, CellCentered, etc.)
 * Inclui um atributo offset_ configurável para controle preciso do posicionamento.
 */
class AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão (offset = 0.5)
     */
    AbstractGridPattern() noexcept = default;

    /**
     * @brief Construtor com offset customizado
     * @param offset Valor do offset (0.0 a 1.0)
     */
    explicit AbstractGridPattern(const Real& offset) noexcept
        : offset_{offset}
    {
    }

    AbstractGridPattern(const AbstractGridPattern&) noexcept = default;
    AbstractGridPattern(AbstractGridPattern&&) = delete;
    virtual ~AbstractGridPattern() noexcept = default;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    AbstractGridPattern& operator=(const AbstractGridPattern&) = delete;
    AbstractGridPattern& operator=(AbstractGridPattern&&) = delete;

//==============================================================================
// Atributo offset e métodos de acesso
//==============================================================================
protected:
    /**
     * @brief Valor do offset para posicionamento (padrão: 0.5)
     */
    Real offset_{0.5};

public:
    /**
     * @brief Obtém o valor atual do offset
     * @return Valor do offset
     */
    Real offset() const noexcept
    {
        return offset_;
    }

    /**
     * @brief Define um novo valor para o offset
     * @param newOffset Novo valor (0.0 a 1.0)
     */
    void setOffset(const Real& newOffset) noexcept
    {
        offset_ = newOffset;
    }

//==============================================================================
// Funções virtuais
//==============================================================================
public:
    /**
     * @brief Constrói a malha em um Grid1D
     * @tparam T Tipo do padrão de malha
     * @param grid Ponteiro para o Grid1D a ser construído
     * @return true se bem-sucedido, false caso contrário
     */
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const {return true;};

//==============================================================================
// Funções puramente virtuas
//==============================================================================
public:
    /**
     * @brief Cria uma cópia do objeto
     * @return shared_ptr para a nova instância
     */
    [[nodiscard]] virtual std::shared_ptr<AbstractGridPattern> Clone() const = 0;

    /**
     * @brief Obtém o nome do padrão de malha
     * @return String com o nome do padrão
     */
    [[nodiscard]] virtual std::string TipoPadraoMalha() const = 0;
};

GRID_NAMESPACE_CLOSE