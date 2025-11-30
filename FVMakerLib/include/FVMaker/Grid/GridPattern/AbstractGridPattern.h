//==============================================================================
// Nome        : AbstractGridPattern.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.2
// Descrição   : Definição da classe AbstractGridPattern, que fornece a interface
//               para padrões de malha na biblioteca FVMaker.
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, versão 3
// ou qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO.
// Consulte a Licença Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU
// junto com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file AbstractGridPattern.h
 * @brief Classe base abstrata para padrões de malha na biblioteca FVMaker
 * @defgroup GridPattern Padrões de Malha
 * @ingroup Grid
 *
 * Define a interface comum para todos os padrões de malha (FaceCentered, CellCentered, etc.)
 * incluindo controle de offset para posicionamento preciso e integração com ParallelControl
 * para operações paralelas.
 *
 * @author João Flávio Vasconcellos
 * @version 1.2
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <memory>
#include <string_view>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>

GRID_NAMESPACE_OPEN

// Forward declaration
template<typename TypePattern>
class AbstractGrid1D;

/**
 * @class AbstractGridPattern
 * @brief Classe base abstrata para padrões de malha
 *
 * Fornece a interface comum para implementação de padrões de malha como:
 * - CellCentered: volumes centrados nas células
 * - FaceCentered: volumes centrados nas faces
 *
 * Inclui controle de offset para posicionamento preciso e integração com
 * ParallelControl para operações paralelas quando disponível.
 *
 * @note Todas as classes derivadas devem implementar Clone() e TipoPadraoMalha()
 *
 * @ingroup GridPattern
 */
class AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão (offset = 0.5)
     * @post offset_ é inicializado com 0.5 (centro exato)
     */
    AbstractGridPattern() noexcept = default;

    /**
     * @brief Construtor com offset customizado
     * @param offset Valor do offset (0.0 a 1.0)
     * @post offset_ é inicializado com o valor fornecido
     * @throw Nenhuma (noexcept)
     */
    explicit AbstractGridPattern(const Real& offset) noexcept
        : offset_{offset}
    {
    }

    /**
     * @brief Construtor de cópia (default)
     */
    AbstractGridPattern(const AbstractGridPattern&) noexcept = default;

    /**
     * @brief Destrutor virtual padrão
     */
    virtual ~AbstractGridPattern() noexcept = default;

    /**
     * @brief Construtor de movimento deletado
     */
    AbstractGridPattern(AbstractGridPattern&&) = delete;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    /**
     * @brief Operador de atribuição de cópia deletado
     */
    AbstractGridPattern& operator=(const AbstractGridPattern&) = delete;

    /**
     * @brief Operador de atribuição de movimento deletado
     */
    AbstractGridPattern& operator=(AbstractGridPattern&&) = delete;

//==============================================================================
// Atributo offset e métodos de acesso
//==============================================================================
protected:
    /**
     * @brief Valor do offset para posicionamento (padrão: 0.5)
     * @details Controla o posicionamento relativo entre faces e centros:
     * - 0.5: posicionamento central exato
     * - <0.5: deslocado para a esquerda
     * - >0.5: deslocado para a direita
     */
    Real offset_{0.5};

public:
    /**
     * @brief Obtém o valor atual do offset
     * @return Valor do offset (0.0 a 1.0)
     */
    Real offset() const noexcept
    {
        return offset_;
    }

    /**
     * @brief Define um novo valor para o offset
     * @param newOffset Novo valor (0.0 a 1.0)
     * @post offset_ é atualizado com o novo valor
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
     *
     * @note Utiliza ParallelControl para operações paralelas quando disponível
     */
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const {return true;};

//==============================================================================
// Funções puramente virtuais
//==============================================================================
public:
    /**
     * @brief Cria uma cópia do objeto (método clone)
     * @return shared_ptr para a nova instância
     *
     * @note Implementação obrigatória nas classes derivadas
     */
    [[nodiscard]] virtual std::shared_ptr<AbstractGridPattern> Clone() const = 0;

    /**
     * @brief Obtém o nome do padrão de malha
     * @return String com o nome descritivo do padrão
     *
     * @note Implementação obrigatória nas classes derivadas
     */
    [[nodiscard]] virtual std::string TipoPadraoMalha() const = 0;
};

GRID_NAMESPACE_CLOSE