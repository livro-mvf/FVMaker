#pragma once

#pragma once

//==============================================================================
// Name        : RandomGrid1D.h
// Author      : Joao Flavio Vieira de Vasconcellos
// Version     : 1.0
// Description : Classe para geracao de malha aleatoria (distribuicao uniforme)
//
// Copyright   : Copyright (C) <2025>  Joao Flavio Vasconcellos
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
//      Includes da Biblioteca FVMaker
//==============================================================================
#include <FVMAKER/Grid/Grid1D/AbstractGrid1D.h>

GRID_NAMESPACE_OPEN

template<typename TypePattern>
class RandomGrid1D : public AbstractGrid1D<TypePattern> {
    
//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    RandomGrid1D() noexcept = default;
    RandomGrid1D (const int&, const Real&, const Real& = 0.0);
    RandomGrid1D(const RandomGrid1D& _copia) noexcept
        : AbstractGrid1D<TypePattern>(*_copia.Clone()){};
    virtual ~RandomGrid1D() noexcept override = default;
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
    
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D<TypePattern>> Clone() const;
    [[nodiscard]] virtual bool GeraFaces ();
    [[nodiscard]] virtual bool GeraCentros ();
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

#include <FVMaker/Grid/Grid1D/RandomGrid1D.hpp>
