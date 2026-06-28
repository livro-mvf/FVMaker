// ============================================================================
// Arquivo: type.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara type no contexto de Misc.
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
 * @file Type.h
 * @brief Definicoes de tipos numericos e conteineres na biblioteca FVMaker
 * @defgroup Tipos Tipos Fundamentais
 * @ingroup Misc
 *
 * Este arquivo contem definicoes essenciais para calculos numericos e
 * geometria computacional, incluindo aliases para tipos de precisao
 * e conteineres padrao.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <list>     ///< Para std::list
#include <vector>   ///< Para std::vector
#include <utility>  ///< Para std::pair

//==============================================================================
// Includes do FVMaker
//==============================================================================
#include <FVMaker/Misc/namespace.h>

//==============================================================================
// Definicoes de tipos numericos
//==============================================================================

/**
 * @brief Tipo de ponto flutuante padrao para calculos numericos
 * @details Define o tipo real utilizado em toda a biblioteca para calculos
 *          de precisao. Atualmente definido como double.
 * @ingroup Tipos
 */
using Real = double;

FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Definicoes de conteineres
//==============================================================================

/**
 * @brief Lista encadeada de inteiros
 * @ingroup Tipos
 */
using LstInt = std::list<int>;

/**
 * @brief Vetor dinamico de inteiros
 * @ingroup Tipos
 */
using VecInt = std::vector<int>;

/**
 * @brief Lista encadeada de numeros reais
 * @ingroup Tipos
 */
using LstReal = std::list<Real>;

/**
 * @brief Vetor dinamico de numeros reais
 * @ingroup Tipos
 */
using VecReal = std::vector<Real>;

/**
 * @brief Par de valores reais para armazenamento de dados
 * @details Usado frequentemente para armazenar pares coordenados (x,y)
 *          ou pares de valores relacionados.
 * @ingroup Tipos
 */
using PairData = std::pair<Real, Real>;

/**
 * @brief Vetor de pares de valores reais
 * @details Comumente utilizado para representar series de dados ou
 *          colecoes de pontos coordenados.
 * @ingroup Tipos
 */
using VecSource = std::vector<PairData>;

FVMAKER_NAMESPACE_CLOSE