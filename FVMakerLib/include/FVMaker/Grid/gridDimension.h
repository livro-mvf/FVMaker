// ============================================================================
// Arquivo: gridDimension.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara grid Dimension no contexto de Grid.
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
 * @file GridDimension.h
 * @brief Definicao de traits para dimensionamento de grids na FVMaker
 * @defgroup GridTraits Traits de Dimensionalidade
 * @ingroup Grid
 *
 * Este arquivo contem traits que permitem identificar automaticamente
 * a dimensao de uma malha e restringir o uso de templates apenas
 * para malhas suportadas na biblioteca FVMaker.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.3
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <type_traits>

//==============================================================================
// Includes do FVMaker
//==============================================================================
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Grid/Grid1D/abstractGrid1D.h>
#include <FVMaker/Grid/GridPattern/cellCentered.h>

using fvm::grd::CellCentered;

//==============================================================================
// Abertura do namespace FVMaker
//==============================================================================
FVMAKER_NAMESPACE_OPEN

//==============================================================================
// Trait para identificacao da dimensao de um grid 1D
//==============================================================================

/**
 * @struct GridDim
 * @brief Trait para determinar a dimensao de uma malha
 * @tparam T Tipo da malha a ser avaliado
 *
 * Esta estrutura de traits verifica se um tipo T e derivado de
 * AbstractGrid1D<CellCentered> ou AbstractGrid1D<typename T::DataType>.
 * Retorna a dimensao da malha (1 para 1D, -1 para nao suportado).
 *
 * @ingroup GridTraits
 */
template <typename T>
struct GridDim {
    using DataType = Real;  ///< Tipo base para calculos numericos
    
    /**
     * @brief Valor da dimensao da malha
     * @retval 1 Se T for uma malha 1D valida
     * @retval -1 Se T nao for uma malha suportada
     */
    static constexpr int value =
        std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ? 1 :
        std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T> ? 1 : -1;
};

//==============================================================================
// Concept para restringir templates a grids 1D
//==============================================================================

/**
 * @concept Is1DGrid
 * @brief Concept que restringe templates a malhas 1D suportadas
 * @tparam T Tipo a ser verificado
 *
 * Verifica se T e derivado de AbstractGrid1D<CellCentered> ou
 * AbstractGrid1D<typename T::DataType>.
 *
 * @ingroup GridTraits
 */
template<typename T>
concept Is1DGrid =
    std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ||
    std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T>;

FVMAKER_NAMESPACE_CLOSE