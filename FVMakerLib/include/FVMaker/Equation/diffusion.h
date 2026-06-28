// ============================================================================
// Arquivo: diffusion.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara diffusion no contexto de Equation.
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
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/abstractCoefficient.h>               ///< Definições da classe base de coeficientes; 
#include <FVMaker/Equation/equation.h>                              ///< Definições da Equation; 
#include <FVMaker/Grid/gridDimension.h>
using fvm::grd::AbstractGrid;
using fvm::GridDim;


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
        
template <typename T>
requires Is1DGrid<T>        
class Diffusion : public Equation <T>
{

//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    Diffusion(const Diffusion&) noexcept = default;
    virtual ~Diffusion() noexcept override = default;
    Diffusion(Diffusion&&) = delete;    

    Diffusion   (   const T& _grid
                ,   const AbstractCoefficient<T>    _coeff
                ,   const BoundaryConditions<T>     _bc
                ,   const Function<T>*              _funcao = nullptr  
                );
 
    
//==============================================================================
// Funções
//==============================================================================
public  :
        
    [[nodiscard]] bool ComputeCoefficient();
    
private:    
    [[nodiscard]] bool ComputeCoefficient1D();
    
//==============================================================================
// dados da classe
//==============================================================================
    
    
};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Equation/diffusion.hpp>                              ///< Definições da Equation; 
