// ============================================================================
// Arquivo: namespace.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara namespace no contexto de Misc.
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
 * @file Namespace.h
 * @brief Definicoes de macros para namespaces na biblioteca FVMaker
 * @defgroup Namespaces Macros de Namespace
 * @ingroup Misc
 *
 * Este arquivo define macros para abertura e fechamento dos namespaces
 * principais utilizados na FVMaker, garantindo padronizacao no codigo.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

/**
 * @brief Macro para abrir o namespace principal da FVMaker
 * @details Define o inicio do namespace 'fvm' que engloba toda a biblioteca
 * @ingroup Namespaces
 */
#define FVMAKER_NAMESPACE_OPEN namespace fvm {

/**
 * @brief Macro para fechar o namespace principal da FVMaker
 * @details Define o fechamento do namespace 'fvm'
 * @ingroup Namespaces
 */
#define FVMAKER_NAMESPACE_CLOSE }

/**
 * @brief Macro para abrir o namespace da malha dentro do FVMaker
 * @details Define o inicio do namespace 'grd' para operacoes com malhas
 * @ingroup Namespaces
 */
#define GRID_NAMESPACE_OPEN namespace fvm { namespace grd {

/**
 * @brief Macro para fechar o namespace da malha dentro do FVMaker
 * @details Define o fechamento do namespace 'grd'
 * @ingroup Namespaces
 */
#define GRID_NAMESPACE_CLOSE } }