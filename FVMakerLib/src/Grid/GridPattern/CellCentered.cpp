//==============================================================================
// Nome        : CellCentered.cpp
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.1
// Descrição   : Implementação dos métodos da classe CellCentered
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

/**
 * @file CellCentered.cpp
 * @brief Implementação dos métodos da classe CellCentered
 * @ingroup GridPattern
 *
 * Contém as implementações dos métodos declarados na classe CellCentered,
 * incluindo a criação de clones e identificação do tipo de padrão de malha.
 *
 * @author João Flávio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

#include <FVMaker/Grid/GridPattern/CellCentered.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Cria uma cópia (clone) deste objeto CellCentered
 * @return std::shared_ptr<AbstractGridPattern> apontando para a nova instância clonada
 *
 * @details Utiliza o construtor de cópia padrão para criar uma nova instância idêntica.
 * A instância criada é gerenciada por um shared_ptr para permitir gerenciamento automático
 * de memória.
 *
 * @note Este método é uma implementação do método virtual puro da classe base AbstractGridPattern
 * @post O objeto retornado é uma cópia exata do objeto original
 *
 * @see AbstractGridPattern::Clone()
 */
std::shared_ptr<AbstractGridPattern> CellCentered::Clone() const {
    return std::make_shared<CellCentered>(*this);
}

/**
 * @brief Retorna o tipo do padrão de malha
 * @return "Volume Centrado" - string que identifica este padrão de malha
 *
 * @details A string retornada é utilizada para:
 * - Identificação do tipo de padrão em logs e mensagens
 * - Serialização/deserialização de objetos
 * - Propósitos de depuração e diagnóstico
 *
 * @note Este método é uma implementação do método virtual puro da classe base AbstractGridPattern
 * @see AbstractGridPattern::TipoPadraoMalha()
 */
std::string CellCentered::TipoPadraoMalha() const {
    return "Volume Centrado";
}

GRID_NAMESPACE_CLOSE