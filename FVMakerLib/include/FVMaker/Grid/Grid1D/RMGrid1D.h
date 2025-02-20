//////==============================================================================
////// Name        : RobertsMaliskaI.h
////// Author      : Joao Flavio Vieira de Vasconcellos
////// Version     : 1.0
////// Description : Classe para gera��o de malha n�o uniforme
//////               segundo a equa��o de Roberts presente
//////               no livro do Maliska
//////               doi: 10.1016/j.envsoft.2022.105563
//////
////// Copyright   : Copyright (C) <2023>  Joao Flavio Vasconcellos
//////                                      (jflavio at iprj.uerj.br)
//////
////// This program is free software: you can redistribute it and/or modify
////// it under the terms of the GNU General Public License as published by
////// the Free Software Foundation, either version 3 of the License.
//////
////// This program is distributed in the hope that it will be useful,
////// but WITHOUT ANY WARRANTY; without even the implied warranty of
////// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
////// GNU General Public License for more details.
//////
////// You should have received a copy of the GNU General Public License
////// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////
//////==============================================================================
////
////
////#ifndef __MFV_ROBERTSMALISKAI_H__
////#define __MFV_ROBERTSMALISKAI_H__
////
//////==============================================================================
//////   include do biblioteca
//////==============================================================================
////#include <MVF++/Mesh/Mesh1D/AbstractMesh1D.h>
////
////MVF_NAMESPACE_OPEN
////
////class RobertsMaliskaI : public AbstractMesh1D  {
////    
//////==============================================================================
//////   Identificacao da classe
//////==============================================================================
////
////public:
////
////    DefineIdentity  (   "RobertsMaliskaI"
////                    ,   ID::RobertsMaliskaI
////                    );    
////
////public:
////    
////    RobertsMaliskaI()                       noexcept    = default;
////    RobertsMaliskaI(const RobertsMaliskaI&)             = default;
////    virtual ~RobertsMaliskaI()                          = default;
////
////    RobertsMaliskaI (   const int&                                      _nvol                               // Numero de Volumes da malha
////                    ,   const Real&                                     _lenght                             // Comprimento da malha
////                    ) 
////                    : RobertsMaliskaI (_nvol, _lenght, 0.0, AbstractMeshPattern::FACECENTRADA, BETA) {};
////
////    
//////    RobertsMaliskaI (   const int&                                     _nvol                               // Numero de Volumes da malha
//////                    ,   const Real&                                     _lenght                             // Comprimento da malha
//////                    ,   const AbstractMeshPattern::TIPOVOLUME&          _typeVol = AbstractMeshPattern::FACECENTRADA  // Tipo de Arranjo da malha
//////                    ,   const Real&                                     _alpha = BETA
//////                    ) : RobertsMaliskaI (_nvol, _lenght, 0.0, _typeVol, _alpha) {};
////
////                    
////private: 
////    
////    RobertsMaliskaI (   const int&                                                         // Numero de Volumes da malha 
////                    ,   const Real&                                                         // Comprimento da malha
////                    ,   const Real&                                                         // Coordenada inicial da malha
////                    ,   const AbstractMeshPattern::TIPOVOLUME&                              // Tipo de Arranjo da malha
////                    ,   const Real&                                                         
////                    );
////    
//////==============================================================================
////// Sobrecarga de operadores
//////==============================================================================
////    
////public:
////
////    RobertsMaliskaI&  operator = (const RobertsMaliskaI&) noexcept = default;
////    
////    
//////==============================================================================
////// Funcoes virtuais
//////==============================================================================
////        
////protected:
////        
////    virtual void MeshGeneration (VecReal&)  override;    
////    
////
//////==============================================================================
////// Funcoes da classe
//////==============================================================================
////        
////    
////public:
////
////    [[nodiscard]]    
////    virtual std::shared_ptr<AbstractMesh1D const > SharedPtr() const override {return std::make_shared <RobertsMaliskaI const> (*this);};    
////
//////==============================================================================
////// Variaveis da classe
//////==============================================================================
////        
////private:
////
////    const Real          beta = 2;
////    static Real const   BETA;
////};
////
//////============================================================================== 
//////   typedef    
//////============================================================================== 
////
////typedef std::shared_ptr<RobertsMaliskaI>                             SharedRobertsMaliskaI;
////typedef std::shared_ptr<RobertsMaliskaI const>                        SharedConstRobertsMaliskaI;
////
////
////MVF_NAMESPACE_CLOSE
////
////#endif 
////
