// ============================================================================
// Arquivo: misc.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara misc no contexto de Misc.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

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
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
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
#include <FVMaker/Misc/configure.h>

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