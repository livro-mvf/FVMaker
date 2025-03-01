#pragma once

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
 * @brief Classe base para padrões de malha.
 *
 * Esta classe define a interface que todos os padrões de malha devem
 * implementar. Ela permite que o código que a utiliza trabalhe de forma
 * genérica com diferentes padrões, chamando métodos comuns a todos eles.
 *
 * Cada padrão concreto, como FaceCentered ou CellCentered, deve herdar
 * de GridPattern e implementar os métodos aqui definidos.
 */
class AbstractGridPattern
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:

    /**
     * @brief Construtor padrão.
     *
     * Inicializa offset_ com 0.5.
     */
    AbstractGridPattern() noexcept = default;

    /**
     * @brief Construtor que permite definir o offset.
     *
     * @param offset Valor que define a fração de deslocamento ao calcular
     *               centros ou faces, dependendo do padrão concreto.
     *               Padrão tipicamente é 0.5.
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
// Atributo offset e seus métodos de acesso
//==============================================================================
protected:

    /**
     * @brief Fração de deslocamento usada pelos padrões que calculam
     *        centros ou faces de forma não exatamente centralizada.
     *        O valor padrão é 0.5 (meio do intervalo).
     */
    Real offset_{0.5};

public:

    /**
     * @brief Retorna o valor atual de offset.
     */
    Real offset() const noexcept
    {
        return offset_;
    }

    /**
     * @brief Define um novo valor de offset.
     *
     * @param newOffset Novo valor para o deslocamento.
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
     * @brief Constrói a malha 1D (via Grid1D) de acordo com este padrão.
     *
     * @param grid Ponteiro único para um Grid1D<GridPattern> que será preenchido
     *             com as coordenadas calculadas (faces, centros, etc).
     * @return true se a malha foi construída com sucesso, false em caso contrário.
     */
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const {return true;};

//==============================================================================
// Funções puramente virtuais
//==============================================================================
public:

    /**
     * @brief Cria uma cópia (clone) deste objeto GridPattern.
     *
     * @return Um std::shared_ptr para a nova instância clonada.
     */
    [[nodiscard]] virtual std::shared_ptr<AbstractGridPattern> Clone() const = 0;

    /**
     * @brief Retorna o tipo do padrão de malha.
     *
     * Exemplos: "FaceCentered", "CellCentered".
     */
    [[nodiscard]] virtual std::string TipoPadraoMalha() const = 0;
};

GRID_NAMESPACE_CLOSE
