// ============================================================================
// Arquivo: randomGrid1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara random Grid 1 D no contexto de Grid / Grid1D.
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
//      Includes da Biblioteca FVMaker
//==============================================================================
#include <FVMaker/Grid/Grid1D/abstractGrid1D.h>

GRID_NAMESPACE_OPEN

template<typename TypePattern>
class RandomGrid1D {

public:

    using DataType = Real;  // Define DataType para que GridDim e Is1DGrid funcionem corretamente
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    RandomGrid1D() noexcept = default;
    RandomGrid1D (const int&, const Real&, const Real& = 0.0);
    RandomGrid1D(const RandomGrid1D& _copia) noexcept
        : AbstractGrid1D<TypePattern>(*_copia.Clone()){};
    ~RandomGrid1D() noexcept = default;
//
    RandomGrid1D(RandomGrid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    RandomGrid1D& operator=(const RandomGrid1D&) = delete;
    RandomGrid1D& operator=(RandomGrid1D&&) = delete;
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] std::unique_ptr<AbstractGrid1D<TypePattern>> Clone() const;
    [[nodiscard]] bool GeraFaces ();
    [[nodiscard]] bool GeraCentros ();
//    
//==============================================================================
// Funções 
//==============================================================================

private :
        
    [[nodiscard]] bool GeraMalhaSequencial (VecReal*);
    [[nodiscard]] bool GeraMalhaParalelo (VecReal*);
    [[nodiscard]] bool GeraMalhaSIMD (VecReal*);

};

//============================================================================== 
//   typedef    
//============================================================================== 

template<typename TypePattern>
using SharedRandomGrid1D = std::shared_ptr<RandomGrid1D<TypePattern>>;
template<typename TypePattern>
using SharedConstRandomGrid1D = std::shared_ptr<RandomGrid1D<TypePattern> const>;
template<typename TypePattern>
using UniqueRandomGrid1D = std::unique_ptr<RandomGrid1D<TypePattern>>;
template<typename TypePattern>
using UniqueConstRandomGrid1D = std::unique_ptr<RandomGrid1D<TypePattern> const>;


GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/randomGrid1D.hpp>
