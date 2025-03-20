//==============================================================================
// Nome        : Misc.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.0
// Descrição   : Arquivo com definições auxiliares para a biblioteca FVMaker,
//               incluindo utilitários para manipulação de strings e arquivos.
//
// Este programa é software livre: você pode redistribuí-lo e/ou
// modificá-lo sob os termos da Licença Pública Geral GNU, versão 3
// da licença, ou (a seu critério) qualquer versão posterior.
//
// Este programa é distribuído na esperança de que seja útil, mas SEM
// QUALQUER GARANTIA; sem mesmo a garantia implícita de
// COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO.
// Consulte a Licença Pública Geral GNU para mais detalhes.
//
// Você deve ter recebido uma cópia da Licença Pública Geral GNU junto
// com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

#pragma once

/**
 * @file Misc.h
 * @brief Definições auxiliares para a biblioteca FVMaker.
 *
 * Este arquivo contém funções auxiliares para manipulação de strings,
 * caminhos de arquivos e ordenação de vetores.
 *
 * @author João Flávio Vasconcellos
 * @date 2025
 * @copyright GNU General Public License v3.0
 *
 * @ingroup Misc
 */

//==============================================================================
// Includes da Biblioteca Padrão do C++
//==============================================================================
#include <algorithm>            // Para std::is_sorted

//==============================================================================
// Includes da FVMaker
//==============================================================================
#include <FVMaker/Misc/Configure.h>

FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Funções auxiliares
//==============================================================================

/** @brief Imprime uma linha no stream de saída. */
void PrintLine(std::ostream&, const unsigned& = LSIZE);

/** @brief Converte uma string em booleano. */
[[nodiscard]] bool string2bool(std::string);

/** @brief Obtém o diretório de trabalho atual. */
[[nodiscard]] std::string getCurrentWorkingDirectory();

/** @brief Retorna o caminho absoluto de um arquivo. */
[[nodiscard]] std::string getFullPath(const std::string&);

/** @brief Renomeia um arquivo. */
[[nodiscard]] std::string RenameFile(const std::string&, const std::string&);

/** @brief Verifica se um vetor está ordenado de forma crescente. */
template<typename T>
[[nodiscard]] bool OrdemCrescente(const T& _vector) {
    return std::is_sorted(std::begin(_vector), std::end(_vector));
}

/** @brief Implementação explícita da função OrdemCrescente para VecReal. */
template bool OrdemCrescente<VecReal>(const VecReal&);

//==============================================================================
// Funções importantes
//==============================================================================

/**
 * @brief Converte uma string em um inteiro hash.
 *
 * @param _str String de entrada.
 * @param _h Parâmetro auxiliar para recursão.
 * @return Valor hash gerado a partir da string.
 */
constexpr unsigned int str2int(const char* _str, const int& _h = 0)
{
    return !_str[_h] ? 5381 : (str2int(_str, _h+1) * 33) ^ _str[_h];
}

FVMAKER_NAMESPACE_CLOSE
