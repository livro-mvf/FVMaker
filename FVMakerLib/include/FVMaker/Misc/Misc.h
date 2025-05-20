//==============================================================================
// Nome        : Misc.h
// Autor       : Joao Flavio Vieira de Vasconcellos
// Versao      : 1.1
// Descricao   : Definicoes auxiliares para a biblioteca FVMaker,
//               incluindo utilitarios para strings e arquivos.
//
// Este programa e software livre: voce pode redistribui-lo e/ou
// modifica-lo sob os termos da Licenca Publica Geral GNU, versao 3
// ou qualquer versao posterior.
//
// Este programa e distribuido na esperanca de que seja util,
// mas SEM QUALQUER GARANTIA; sem mesmo a garantia implicita de
// COMERCIABILIDADE ou ADEQUACAO A UM DETERMINADO PROPOSITO.
// Consulte a Licenca Publica Geral GNU para mais detalhes.
//
// Voce deve ter recebido uma copia da Licenca Publica Geral GNU
// junto com este programa. Se nao, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file Misc.h
 * @brief Utilitarios e funcoes auxiliares da biblioteca FVMaker
 * @defgroup Utilitarios Funcoes Auxiliares
 * @ingroup Misc
 *
 * Este arquivo contem funcoes utilitarias para manipulacao de:
 * - Strings e conversoes de tipo
 * - Operacoes com arquivos e diretorios
 * - Ordenacao e verificacao de vetores
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright GNU General Public License v3.0
 */

//==============================================================================
// Includes da Biblioteca Padrao do C++
//==============================================================================
#include <algorithm>    ///< Para std::is_sorted
#include <string>       ///< Para std::string
#include <ostream>      ///< Para std::ostream

//==============================================================================
// Includes da FVMaker
//==============================================================================
#include <FVMaker/Misc/Configure.h>

FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Funcoes para manipulacao de strings
//==============================================================================

/**
 * @brief Converte uma string em valor booleano
 * @param str String a ser convertida ("true"/"false", "1"/"0")
 * @return true se a string representa valor verdadeiro
 * @note Comparacao case-insensitive
 * @ingroup Utilitarios
 */
[[nodiscard]] bool string2bool(std::string str);

//==============================================================================
// Funcoes para operacoes com arquivos
//==============================================================================

/**
 * @brief Obtem o diretorio de trabalho atual
 * @return Caminho completo do diretorio atual
 * @throws std::runtime_error Se nao for possivel obter o diretorio
 * @ingroup Utilitarios
 */
[[nodiscard]] std::string getCurrentWorkingDirectory();

/**
 * @brief Retorna o caminho absoluto de um arquivo
 * @param filename Nome do arquivo
 * @return Caminho absoluto completo
 * @ingroup Utilitarios
 */
[[nodiscard]] std::string getFullPath(const std::string& filename);

//==============================================================================
// Funcoes para vetores e ordenacao
//==============================================================================

/**
 * @brief Verifica se um vetor esta ordenado crescentemente
 * @tparam T Tipo do vetor (deve suportar iteradores)
 * @param _vector Vetor a ser verificado
 * @return true se o vetor estiver ordenado
 * @ingroup Utilitarios
 */
template<typename T>
[[nodiscard]] bool OrdemCrescente(const T& _vector) {
    return std::is_sorted(std::begin(_vector), std::end(_vector));
}

// Especializacao explicita para VecReal
template bool OrdemCrescente<VecReal>(const VecReal&);

//==============================================================================
// Funcoes de utilidade geral
//==============================================================================

/**
 * @brief Imprime uma linha de separacao
 * @param os Stream de saida
 * @param length Comprimento da linha (padrao: LSIZE)
 * @ingroup Utilitarios
 */
void PrintLine(std::ostream& os, const unsigned& length = LSIZE);

/**
 * @brief Gera hash de uma string (constexpr)
 * @param _str String de entrada
 * @param _h Parametro auxiliar para recursao
 * @return Valor hash calculado
 * @ingroup Utilitarios
 */
constexpr unsigned int str2int(const char* _str, const int& _h = 0) {
    return !_str[_h] ? 5381 : (str2int(_str, _h+1) * 33) ^ _str[_h];
}

FVMAKER_NAMESPACE_CLOSE