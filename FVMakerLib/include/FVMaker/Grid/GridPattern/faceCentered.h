// ============================================================================
// Arquivo: faceCentered.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara face Centered no contexto de Grid / GridPattern.
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
 * @file FaceCentered.h
 * @brief Implementacao do padrao de malha com faces centradas
 * @ingroup GridPattern
 *
 * Calcula as coordenadas das faces com base nas posicoes dos centros,
 * utilizando um offset configuravel para controle preciso do posicionamento.
 * Integra com ParallelControl para operacoes paralelas quando disponivel.
 *
 * @author Joao Flavio Vasconcellos
 * @version 1.1
 * @date 2025-05-20
 * @copyright Copyright (c) 2026, João Flávio Vieira de Vasconcellos
 */

#include <FVMaker/Grid/GridPattern/abstractGridPattern.h>
#include <FVMaker/Misc/parallelControl.h>

GRID_NAMESPACE_OPEN

/**
 * @class FaceCentered
 * @brief Implementacao do padrao de malha com faces centradas
 *
 * Este padrao calcula as coordenadas das faces das celulas com base nas
 * posicoes dos centros, utilizando um offset configuravel. O valor padrao do
 * offset e 0.5, resultando em um posicionamento central exato entre os centros.
 *
 * @note Utiliza ParallelControl para operacoes paralelas na construcao da malha
 * @note Padrao frequentemente usado em esquemas de diferencas finitas
 *
 * @ingroup GridPattern
 */
class FaceCentered
{
public:
    FaceCentered() noexcept = default;
    explicit FaceCentered(double offset) noexcept : offset_{offset} {}
    FaceCentered(const FaceCentered&) noexcept = default;
    ~FaceCentered() noexcept = default;
    FaceCentered(FaceCentered&&) = delete;

    FaceCentered& operator=(const FaceCentered&) = delete;
    FaceCentered& operator=(FaceCentered&&) = delete;

//    template <typename T>
//    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T>*) const;
    
    template <typename T>
    [[nodiscard]] bool BuildMesh(AbstractGrid1D<T> *grid) const
    {
        
        if (!grid->GeraCentros()) return false;
        if (!grid->CalculaFaces(offset_)) return false;
        if (!grid->CalculaDistancias()) return false;
        
        return true;
        
    }    
    

    [[nodiscard]] std::shared_ptr<FaceCentered> Clone() const;
    std::string TipoPadraoMalha() const;
private:
    Real offset_{0.5};
};

GRID_NAMESPACE_CLOSE