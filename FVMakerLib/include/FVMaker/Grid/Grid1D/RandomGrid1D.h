////==============================================================================
//// Name        : Random.h
//// Author      : Joao Flavio Vieira de Vasconcellos
//// Version     : 1.0
//// Description : Classe para gerar malha aleatoria em 1D
////
//// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
////               (jflavio at iprj.uerj.br)
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License.
////
//// This program is distributed in the hope that it will be useful,
//// but WITHOUT ANY WARRANTY; without even the implied warranty of
//// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//// GNU General Public License for more details.
////
//// You should have received a copy of the GNU General Public License
//// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////
////==============================================================================
//
//#ifndef __RANDOMGRID1D_MVF_H__
//#define __RANDOMGRID1D_MVF_H__
//
////==============================================================================
////   include do biblioteca
////==============================================================================
//
//
//
//#include <MVF++/Grid/Grid1D/CartesianGrid1D.h>
//
//MVF_NAMESPACE_OPEN
//
//template <typename TypePattern>
//class RandomGrid1D: public CartesianGrid1D<TypePattern> {
//
////==============================================================================
////   Identificacao da classe
////==============================================================================
//
//public:
//
//    DefineIdentity  (   "RandomGrid1D"
//                    ,   ID::RandomGrid1D
//                    );
//
//public:
//
//    RandomGrid1D<TypePattern> ()                            noexcept    = default;
//    RandomGrid1D<TypePattern> (const RandomGrid1D<TypePattern>&)        = default;
//    ~RandomGrid1D<TypePattern>()                                        = default;
//    
//    RandomGrid1D<TypePattern>       (   const int&                                  _nvol                                           // Numero de Volumes da malha
//                                    ,   const Real&                                 _lenght                                         // Comprimento da malha
//                                    ) : RandomGrid1D<TypePattern>(_nvol, _lenght, 0.0) {};
//
//    RandomGrid1D<TypePattern>   (   const int&              // Numero de Volumes da malha 
//                                ,   const Real&             // Comprimento da malha
//                                ,   const Real&             // Coordenada inicial da malha
//                                );
//    
////==============================================================================
//// Sobrecarga de operadores
////==============================================================================
//    
//public:
//
//    RandomGrid1D<TypePattern>&  operator = (const RandomGrid1D<TypePattern>&) noexcept = default;
//    
////==============================================================================
//// Funcoes
////==============================================================================
//    
//public:
//
//    [[nodiscard]]
//    virtual bool MainCoordinate (VecReal&);   
//
//};
//
////============================================================================== 
////   typedef    
////============================================================================== 
//
//template <typename T>
//using SharedRandomGrid1D = std::shared_ptr< RandomGrid1D<T> >;
//
//template <typename T>
//using SharedConstRandomGrid1D = std::shared_ptr< RandomGrid1D<T> const >;
//
//
//
//
//
//MVF_NAMESPACE_CLOSE
//
//#include <MVF++/Grid/Grid1D/RandomGrid1D.hpp>                  // RandomGrid1DGrid1D 
//
//
//#endif