#pragma once

//==============================================================================
// Name        : RMGrid1D.h
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

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMAKER/Grid/Grid1D/AbstractGrid1D.h>
#include <FVMAKER/Grid/GridPattern/CellCentered.h>


GRID_NAMESPACE_OPEN


template <typename T>
class RMGrid1D : public AbstractGrid1D<T> {

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
    virtual ~RMGrid1D() noexcept override = default;

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
    
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D<T>> Clone() const;
    [[nodiscard]] virtual bool GeraFaces ();
    [[nodiscard]] virtual bool GeraCentros ();
    
//==============================================================================
// Funções 
//==============================================================================

private :
        
    [[nodiscard]] bool GeraMalhaSequencial (const Real&, VecReal*);
    [[nodiscard]] bool GeraMalhaParalelo (const Real&, VecReal*);
    [[nodiscard]] bool GeraMalhaSIMD (const Real&, VecReal*);
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
