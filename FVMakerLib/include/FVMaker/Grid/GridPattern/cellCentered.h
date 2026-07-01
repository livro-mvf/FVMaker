// ============================================================================
// Arquivo: cellCentered.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara cell Centered no contexto de Grid / GridPattern.
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
 * @file CellCentered.h
 * @brief Implementação do padrão de malha com volumes centrados
 * @ingroup GridPattern
 *
 * Calcula as coordenadas dos centros com base nas posições das faces,
 * utilizando um offset configurável para controle preciso do posicionamento.
 * Integra com ParallelControl para operações paralelas quando disponível.
 *
 * @author João Flávio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/GridPattern/abstractGridPattern.h>
//#include <FVMaker/Misc/ParallelControl.h>

GRID_NAMESPACE_OPEN

/**
 * @class CellCentered
 * @brief Implementação do padrão de malha com volumes centrados
 *
 * Este padrão calcula as coordenadas dos centros das células com base nas
 * posições das faces, utilizando um offset configurável. O valor padrão do
 * offset é 0.5, resultando em um posicionamento central exato.
 *
 * @note Utiliza ParallelControl para operações paralelas na construção da malha
 *
 * @ingroup GridPattern
 */
class CellCentered
{
//==============================================================================
// Construtores/Destrutora
//==============================================================================
public:
    /**
     * @brief Construtor padrão (offset = 0.5)
     * @post offset_ é inicializado com 0.5 (centro exato)
     */
    CellCentered() noexcept = default;

    /**
     * @brief Construtor com offset customizado
     * @param _offset Valor do offset para posicionamento (0.5 = centro exato)
     * @post offset_ é inicializado com o valor fornecido
     */
    explicit CellCentered(const Real& _offset) noexcept
        : offset_{_offset}
    {
    }

    /**
     * @brief Construtor de cópia (default)
     */
    CellCentered(const CellCentered&) noexcept = default;

    /**
     * @brief Destrutor padrão
     */
    ~CellCentered() noexcept = default;

    /**
     * @brief Construtor de movimento deletado
     */
    CellCentered(CellCentered&&) = delete;

//==============================================================================
// Sobrecarga de operadores
//==============================================================================
public:
    /**
     * @brief Operador de atribuição de cópia deletado
     */
    CellCentered& operator=(const CellCentered&) = delete;

    /**
     * @brief Operador de atribuição de movimento deletado
     */
    CellCentered& operator=(CellCentered&&) = delete;

//==============================================================================
// Funções virtuais
//==============================================================================
public:
    /**
     * @brief Constrói a malha no padrão CellCentered
     * @tparam T Tipo do grid
     * @param _grid Ponteiro para o grid a ser construído
     * @return true se bem-sucedido, false caso contrário
     *
     * @note Utiliza ParallelControl para operações paralelas quando disponível
     * @note Segue a sequência:
     *       1. Gera as faces da malha
     *       2. Calcula os centros com base no offset
     *       3. Calcula as distâncias entre elementos
     */
    // Monta a representacao algebrica associada aos dados fornecidos.
    template <typename Grid>
    [[nodiscard]] bool BuildMesh(Grid* _grid) const;

//==============================================================================
// Funções puramente virtuais (implementadas)
//==============================================================================
public:
    /**
     * @brief Cria uma cópia do objeto
     * @return shared_ptr para a nova instância
     *
     * @note Implementação do método puro da classe base
     */
    [[nodiscard]] std::shared_ptr<CellCentered> Clone() const;

    /**
     * @brief Obtém o nome do padrão de malha
     * @return "Volume Centrado"
     *
     * @note Implementação do método puro da classe base
     */
    std::string TipoPadraoMalha() const;
private:
    Real offset_{0.5};
};

// Implementação do template
// Monta a representacao algebrica associada aos dados fornecidos.
template <typename Grid>
bool CellCentered::BuildMesh(Grid* _grid) const
{
    bool flag = _grid->GeraFaces();
    auto ptrXFace = _grid->AddressxFace();
    (*ptrXFace)[0] = _grid->XInit();
    (*ptrXFace)[_grid->NVol()] = _grid->XInit() + _grid->Length();
    flag = flag && _grid->CalculaCentros(offset_);
    flag = flag && _grid->CalculaDistancias();
    return flag;
}

GRID_NAMESPACE_CLOSE