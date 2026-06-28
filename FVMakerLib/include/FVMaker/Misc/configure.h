// ============================================================================
// Arquivo: configure.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara configure no contexto de Misc.
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
 * @file Configure.h
 * @brief Configuracoes globais e constantes fundamentais da biblioteca FVMaker
 * @defgroup Configuracoes Configuracoes Globais
 * @ingroup Misc
 *
 * Este arquivo contem definicoes de constantes e configuracoes essenciais
 * utilizadas em toda a biblioteca FVMaker, incluindo:
 * - Precisao numerica
 * - Valores limite para calculos
 * - Constantes matematicas fundamentais
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.2
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

#include <FVMaker/Misc/type.h>

//==============================================================================
// Includes da Biblioteca Padrao do C++
//==============================================================================
#include <cmath>   ///< Para funcoes matematicas como std::acos
#include <limits>  ///< Para limites numericos da maquina

FVMAKER_NAMESPACE_OPEN

/**
 * @brief Valor zero com tratamento de precisao numerica
 * @details Define um valor zero seguro para comparacoes numericas,
 *          considerando a precisao da maquina e evitando underflow.
 * @ingroup Configuracoes
 */
const Real ZERO = (1e250 * std::numeric_limits<Real>::min());

/**
 * @brief Limite numerico para evitar erros de arredondamento
 * @details Valor usado como limite inferior para consideracoes numericas.
 *          Qualquer valor absoluto menor que este pode ser tratado como zero.
 * @ingroup Configuracoes
 */
const Real LIMITE(1e-30);

/**
 * @brief Tolerancia para comparacoes de ponto flutuante
 * @details Pequeno valor usado como margem de erro em comparacoes numericas.
 *          Define a precisao padrao para operacoes na biblioteca.
 * @ingroup Configuracoes
 */
const Real EPSILON(1e-6);

/**
 * @brief Tamanho padrao para buffers e configuracoes
 * @details Tamanho padrao usado em diversas estruturas e configuracoes
 *          da biblioteca (0x50 = 80 em decimal).
 * @ingroup Configuracoes
 */
const int LSIZE(0x50);

/**
 * @brief Constante matematica π com precisao estendida
 * @return O valor de π com precisao de long double
 * @note Calculado como arccos(-1) para maxima precisao
 * @ingroup Configuracoes
 * @code
 *   Real area = Pi() * raio * raio; // Exemplo de uso
 * @endcode
 */
constexpr Real Pi() { return acos(-1.0L); }

FVMAKER_NAMESPACE_CLOSE