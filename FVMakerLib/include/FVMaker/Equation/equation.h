// ============================================================================
// Arquivo: equation.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara equation no contexto de Equation.
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
#include <iostream>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/abstractGrid.h>     // Definições da Grid; 
#include <FVMaker/BoundaryCondition/boundaryCondition.h>        ///< Definição da classe Dirichlet para as condições de contorno
#include <FVMaker/Utilities/function.h>        ///< Definição da classe Function

using fvm::grd::AbstractGrid;

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
// Representa uma equacao formada por termos discretos do nucleo legado.
template <typename T>
class Equation {

public:
        
    // Cria um objeto Equation com os dados fornecidos.
    Equation(const Equation&) noexcept = default;
    // Finaliza um objeto Equation.
    ~Equation() noexcept = default;
    // Cria um objeto Equation com os dados fornecidos.
    Equation(Equation&&) = delete; 
    // Cria um objeto Equation com os dados fornecidos.
    Equation    (   const T&                        _grid
                ,   const AbstractCoefficient<T>    _coeff
                ,   const BoundaryConditions<T>     _bc
                ,   const Function<T>*              _funcao = nullptr
                ) :
        grid_(_grid), bc_(_bc)
    {
        
    try {
        if (_coeff.empty())  throw fvm::FVMakerException(ErrorCode::UndefiniedCoefficient);
        
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
        this->coeffPtr_ = std::make_shared<Coefficient1D<T>>(_coeff);    
    };
    

    
    // Escreve uma representacao textual do objeto no fluxo de saida.
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const Equation<U>&);


//==============================================================================
// Funções inline
//==============================================================================

  [[nodiscard]] inline size_t NVol() const {
    return  this->grid_.NVol();
  }
    
//==============================================================================
// Variaveis da classe
//==============================================================================
    
protected :    
    
    SharedAbstractCoefficient<T>        coeffPtr_;   
    const T&                            grid_;  ///< Referência para a grid usada
    
    VecReal                             aP_, 
                                        aE_, 
                                        aW_, 
                                        aN_, 
                                        aS_, 
                                        aF_, 
                                        aB_, 
                                        BP_, 
                                        SP_, 
                                        SC_;
    
    const BoundaryConditions<T>         bc_;
};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Equation/equation.hpp>