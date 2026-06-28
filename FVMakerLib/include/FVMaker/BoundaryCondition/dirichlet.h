// ============================================================================
// Arquivo: dirichlet.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara dirichlet no contexto de BoundaryCondition.
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

//==============================================================================
// FVMAKER includes
//==============================================================================
#include <FVMaker/BoundaryCondition/bc.h>

FVMAKER_NAMESPACE_OPEN

/**
 * @brief Classe genérica para representar uma função matemática definida
 * sobre os centros dos volumes de uma grid.
 *
 * A classe assume que o grid fornece um método (ex.: PtrSCCentreCoordinate())
 * que retorna um ponteiro para um container dos centros dos volumes.
 * Cada centro é convertido para um std::vector<Real> contendo as coordenadas,
 * possibilitando usar a mesma interface para grid 1D, 2D, 3D, etc.
 */

class Dirichlet : public BC{

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:
    
    Dirichlet()                noexcept = default;
    Dirichlet(const Dirichlet&)   noexcept = default;
    virtual ~Dirichlet()       noexcept = default;
    Dirichlet (const Real& _gamma) : BC(1,0, _gamma) {};
    
    Dirichlet(Dirichlet&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Dirichlet& operator=(const Dirichlet&) = delete;
    Dirichlet& operator=(Dirichlet&&) = delete;
    
};

FVMAKER_NAMESPACE_CLOSE
