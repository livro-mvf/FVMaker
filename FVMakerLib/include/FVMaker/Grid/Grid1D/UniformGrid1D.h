#pragma once

//==============================================================================
// Name        : UniformGrid1D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Classe para gera��o de malha uniforme
//
// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
//                                      (jflavio at iprj.uerj.br)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//==============================================================================

#include <FVMAKER/Grid/Grid1D/Grid1D.h>
#include <FVMAKER/Grid/GridPattern/CellCentered.h>


GRID_NAMESPACE_OPEN


class UniformGrid1D : public Grid1D<CellCentered> {
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    UniformGrid1D() noexcept = default;
    UniformGrid1D (const int&, const Real&, const Real& = 0.0);
    UniformGrid1D(const UniformGrid1D& _copia) noexcept
        : Grid1D(*_copia.Clone()){};
    virtual ~UniformGrid1D() noexcept override = default;

    UniformGrid1D(UniformGrid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    UniformGrid1D& operator=(const UniformGrid1D&) = delete;
    UniformGrid1D& operator=(UniformGrid1D&&) = delete;
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] virtual std::unique_ptr<Grid1D<CellCentered>>Clone() const;
    [[nodiscard]] virtual bool GeraMalha ();
    
//==============================================================================
// Funções 
//==============================================================================

    [[nodiscard]] bool GeraMalhaSsequencial ();
    [[nodiscard]] bool GeraMalhaParalelo ();
    [[nodiscard]] bool GeraMalhaSIMD ();
    
//
//
//    
////==============================================================================
////   Identificacao da classe
////==============================================================================
//
//public:
//
//        DefineIdentity  (   "UniformGrid1D"
//                        ,   ID::UniformGrid1D 
//                        );
//
////==============================================================================
//// Construtoras / destrutora
////==============================================================================
//
//public:
//
//    UniformGrid1D   ()                      noexcept    = default;
//    UniformGrid1D   (const UniformGrid1D&)              = default;
//    ~UniformGrid1D  ()                                  = default;
//
//
//    UniformGrid1D   ( const int&,                       // Numero de volumes da malha
//                      const Real&,                      // Tamanho do dominio 1D
//                      const Real& = 0.0                 // Coordenada inicial
//                    );                  
//
//
////==============================================================================
////   Sobrecarga de operadores
////==============================================================================
//
//public:    
//    
//    [[nodiscard]]
//    UniformGrid1D& operator = (const UniformGrid1D&) noexcept = default;
//
////==============================================================================
//// Funcoes virtuais
////==============================================================================
//        
//public:  
//    
//    [[nodiscard]]
//    virtual bool MainCoordinate (VecReal&); 
// 
};




//============================================================================== 
//   typedef    
//============================================================================== 

using SharedUniformGrid1D = std::shared_ptr<UniformGrid1D>;
using SharedConstUniformGrid1D = std::shared_ptr<UniformGrid1D const>;


GRID_NAMESPACE_CLOSE
