// ============================================================================
// Arquivo: abstractGrid.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara abstract Grid no contexto de Grid.
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
 * @file AbstractGrid.h
 * @brief Classe base para geracao de malhas na biblioteca FVMaker
 * @defgroup Grid Classes de Malha
 *
 * Define a interface comum para classes derivadas de malhas (1D, 2D, 3D, etc.)
 * que serao utilizadas nas simulacoes numericas.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.0
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

//==============================================================================
// Includes do C++
//==============================================================================
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>

//==============================================================================
// Abertura do namespace FVMaker::Grid
//==============================================================================
GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid
 * @brief Classe base abstrata para geracao de malhas
 * @tparam TypePattern Tipo do padrao de discretizacao (ex: CellCentered)
 *
 * Esta classe define a interface comum para todas as malhas da biblioteca.
 * As classes derivadas devem implementar a logica especifica para geracao
 * e armazenamento das coordenadas.
 *
 * @ingroup Grid
 */
template<typename TypePattern>
class AbstractGrid {
public:
    //==========================================================================
    // Construtores/Destrutores
    //==========================================================================
    
    /**
     * @brief Construtor padrao
     */
    AbstractGrid() noexcept = default;
    
    /**
     * @brief Construtor de copia
     */
    AbstractGrid(const AbstractGrid&) noexcept = default;
    
    /**
     * @brief Destrutor */
    ~AbstractGrid() noexcept = default;
    
    /**
     * @brief Construtor de movimento deletado
     */
    AbstractGrid(AbstractGrid&&) = delete;
    
    //==========================================================================
    // Operadores
    //==========================================================================
    
    /**
     * @brief Operador de atribuicao de copia deletado
     */
    AbstractGrid& operator=(const AbstractGrid&) = delete;
    
    /**
     * @brief Operador de atribuicao de movimento deletado
     */
    AbstractGrid& operator=(AbstractGrid&&) = delete;
    
    //==========================================================================
    // Interface Publica
    //==========================================================================
    
    /**
     * @brief Verifica se a grade esta vazia
     * @return true se a grade nao contem elementos
     */
    [[nodiscard]] bool empty() const { return true; }
};

GRID_NAMESPACE_CLOSE