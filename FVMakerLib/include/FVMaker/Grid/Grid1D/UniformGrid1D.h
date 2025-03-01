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

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMAKER/Grid/Grid1D/AbstractGrid1D.h>
#include <FVMAKER/Grid/GridPattern/CellCentered.h>


GRID_NAMESPACE_OPEN


class UniformGrid1D : public AbstractGrid1D<CellCentered> {
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    UniformGrid1D() noexcept = default;
    UniformGrid1D (const int&, const Real&, const Real& = 0.0);
    UniformGrid1D(const UniformGrid1D& _copia) noexcept
        : AbstractGrid1D(*_copia.Clone()){};
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
    
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D<CellCentered>> Clone() const;
    [[nodiscard]] virtual bool GeraFaces ();
    [[nodiscard]] virtual bool GeraCentros ();
    
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

using SharedUniformGrid1D = std::shared_ptr<UniformGrid1D>;
using SharedConstUniformGrid1D = std::shared_ptr<UniformGrid1D const>;
using UniqueUniformGrid1D = std::unique_ptr<UniformGrid1D>;
using UniqueConstUniformGrid1D = std::unique_ptr<UniformGrid1D const>;


GRID_NAMESPACE_CLOSE
