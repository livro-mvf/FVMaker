//==============================================================================
// Nome        : FaceCentered.cpp
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Implementacao dos metodos da classe FaceCentered
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

#include <FVMaker/Grid/GridPattern/faceCentered.h>

GRID_NAMESPACE_OPEN

/**
 * @brief Cria uma copia (clone) deste objeto FaceCentered
 * @return std::shared_ptr<AbstractGridPattern> apontando para nova instancia clonada
 *
 * @note Utiliza o construtor de copia padrao para criar nova instancia
 */
std::shared_ptr<AbstractGridPattern> FaceCentered::Clone() const {
    return std::make_shared<FaceCentered>(*this);
}

/**
 * @brief Retorna o tipo do padrao de malha
 * @return "Face Centrada" - string que identifica este padrao de malha
 *
 * @note A string retornada e usada para identificacao e logging do padrao
 */
std::string FaceCentered::TipoPadraoMalha() const {
    return "Face Centrada";
}

GRID_NAMESPACE_CLOSE