// ============================================================================
// Arquivo: RMGrid1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara RMGrid 1 D no contexto de Grid / Grid1D.
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
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Grid/Grid1D/abstractGrid1D.h>
#include <FVMaker/Grid/GridPattern/cellCentered.h>


GRID_NAMESPACE_OPEN


template <typename T>
class RMGrid1D {

public:

    using DataType = Real;  // Define DataType para que GridDim e Is1DGrid funcionem corretamente
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    RMGrid1D() noexcept = default;
    RMGrid1D (const Real&, const int&, const Real&,  const Real& = 0.0);
    RMGrid1D (const RMGrid1D& _copia) noexcept
        : AbstractGrid1D<T>(*_copia.Clone()){};
    ~RMGrid1D() noexcept = default;

    RMGrid1D(RMGrid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    RMGrid1D& operator=(const RMGrid1D&) = delete;
    RMGrid1D& operator=(RMGrid1D&&) = delete;
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] std::unique_ptr<AbstractGrid1D<T>> Clone() const;
    [[nodiscard]] bool GeraFaces ();
    [[nodiscard]] bool GeraCentros ();
    
//==============================================================================
// Funções 
//==============================================================================

private :
        
    [[nodiscard]] bool GeraMalhaSequencial (VecReal*);
    [[nodiscard]] bool GeraMalhaParalelo (VecReal*);
    [[nodiscard]] bool GeraMalhaSIMD (VecReal*);
    [[nodiscard]] Real Funcao (const Real&) const;

//==============================================================================
// Dados da classe 
//==============================================================================

private:

Real    beta_= 1.001;
Real    auxiBeta_;
static  constexpr Real ALPHA_ = 0.5;
};
GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/RMGrid1D.hpp>
