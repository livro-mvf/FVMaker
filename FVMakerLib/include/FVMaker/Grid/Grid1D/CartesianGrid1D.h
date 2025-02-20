////==============================================================================
//// Name        : CartesianGrid1D.h
//// Author      : Joao Flavio Vieira de Vasconcellos
//// Version     : 1.0
//// Description : Classe base para definicao das malhas em uma dimensao
////
//// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
////                                      (jflavio at iprj.uerj.br)
////
//// This program is free software: you can redistribute it and/or modify
//// it under the terms of the GNU General Public License as published by
//// the Free Software Foundation, either version 3 of the License, or
//// (at your option) any later version.
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
//#ifndef __MVF_CARTESIAN_GRID_1D__
//#define __MVF_CARTESIAN_GRID_1D__
//
////==============================================================================
//// Includes da biblioteca
////==============================================================================
//
//#include <MVF++/Grid/GridPattern/GridPattern.h>
//#include <MVF++/Grid/CartesianGrid.h>
//
//MVF_NAMESPACE_OPEN
//
//template<typename T>
//struct TraitsPatternGrid;
//
//template<typename TypePattern>
//class CartesianGrid1D; 
//
//template<typename TypePattern>
//using SharedCartesianGrid1D = std::shared_ptr<CartesianGrid1D<TypePattern>>;
//
//template<typename TypePattern>
//using SharedConstCartesianGrid1D = std::shared_ptr<CartesianGrid1D<TypePattern> const>;
//
//template<typename TypePattern>
//class CartesianGrid1D : public CartesianGrid {
//public:
//
//    //==============================================================================
//    // Fun��es friends
//    //==============================================================================
//
//    template<typename U>
//    friend std::ostream& operator<<(std::ostream&, const CartesianGrid1D<U>&);
//
//    friend struct mvf::GridPatternTraits<TypePattern>;
//
//    //==============================================================================
//    // Identifica��o da classe
//    //==============================================================================
//
//    DefineIdentity("CartesianGrid1D", ID::CartesianGrid1D);
//
//    //==============================================================================
//    // Construtoras / destrutora
//    //==============================================================================
//
//public:
//    CartesianGrid1D<TypePattern>() noexcept = default;
//    CartesianGrid1D<TypePattern>(const CartesianGrid1D<TypePattern>&) = default;
//    ~CartesianGrid1D<TypePattern>() = default;
//    CartesianGrid1D<TypePattern>(const int&, const Real&, const Real&);
//
//    //==============================================================================
//    // Sobrecarga de operadores
//    //==============================================================================
//
//public:
//    CartesianGrid1D<TypePattern>& operator=(const CartesianGrid1D<TypePattern>&) noexcept = default;
//
//    //==============================================================================
//    // Fun��es inline
//    //==============================================================================
//
//public:
//    [[nodiscard]] inline int NVol() const { return nVol; }
//    [[nodiscard]] inline Real StartX() const { return startX; }
//    [[nodiscard]] inline Real EndX() const { return endX; }
//    [[nodiscard]] inline Real Lenght() const { return endX - startX; }
//    [[nodiscard]] inline const VecReal& FaceX() const { return faceX; }
//    [[nodiscard]] inline VecReal& FaceX() { return faceX; }
//    [[nodiscard]] inline const VecReal& FaceDX() const { return faceDX; }
//    [[nodiscard]] inline VecReal& FaceDX() { return faceDX; }
//    [[nodiscard]] inline const VecReal& CentreX() const { return centreX; }
//    [[nodiscard]] inline VecReal& CentreX() { return centreX; }
//    [[nodiscard]] inline const VecReal& CentreDX() const { return centreDX; }
//    [[nodiscard]] inline VecReal& CentreDX() { return centreDX; }
//    [[nodiscard]] inline int size() const { return centreX.size(); }
//    [[nodiscard]] inline bool empty() const { return centreX.empty(); }
//    [[nodiscard]] inline std::string GridPatternType() const { return TraitsPatternGrid<TypePattern>::GridPatternType(); }
//    inline SharedCartesianGrid1D<TypePattern> GetShared() { return std::make_shared<CartesianGrid1D<TypePattern>>(*this); }
//    [[nodiscard]] inline bool EvaluateOtherCoordinate() { return TraitsPatternGrid<TypePattern>::EvaluateOtherCoordinate(this->GetShared()); }
//    [[nodiscard]] inline bool EvaluateMainCoordinate() { return TraitsPatternGrid<TypePattern>::EvaluateMainCoordinate(this->GetShared()); }
//
//    //==============================================================================
//    // Fun��es
//    //==============================================================================
//
//public:
//    [[nodiscard]] bool EvaluateCoordinates();
//
//protected:
//    
//    [[nodiscard]] bool EvaluateCentreDx();
//    [[nodiscard]] bool EvaluateFaceDx();
//    friend struct TraitsPatternGrid<TypePattern>;
//
//    //==============================================================================
//    // Dados da classe
//    //==============================================================================
//
//protected:
//    int nVol = 0;
//    Real startX = 0;
//    Real endX = 0;
//
//    VecReal faceX{};
//    VecReal centreX{};
//    VecReal centreDX{};
//    VecReal faceDX{};
//};
//
//template<typename T>
//struct TraitsPatternGrid {
//    static void OtherCoordinateEvaluation(SharedCartesianGrid1D<T>&);
//    static void MainCoordinateEvaluation(SharedCartesianGrid1D<T>&);
//};
//
//
//
//MVF_NAMESPACE_CLOSE
//
//        
//        
//#include <MVF++/Grid/Grid1D/CartesianGrid1D.hpp> // CartesianGrid1D 
//
//#endif
//
//
//
//
//////==============================================================================
////// Name        : CartesianGrid1D.h
////// Author      : Joao Flavio Vieira de Vasconcellos
////// Version     : 1.0
////// Description : Classe base para definicao das malhas em uma dimensao
//////
////// Copyright   : Copyright (C) <2024>  Joao Flavio Vasconcellos
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
////#ifndef __MVF_CARTESIAN_GRID_1D__
////#define __MVF_CARTESIAN_GRID_1D__
////
//////==============================================================================
//////   includes da biblioteca
//////==============================================================================
////
////#include <MVF++/Grid/GridPattern/GridPattern.h>
////#include <MVF++/Grid/CartesianGrid.h>
////
////
////MVF_NAMESPACE_OPEN
////
////
////template<typename T>
////struct TraitsPatternGrid {
////    static void OtherCoordinateEvaluation ( std::unique_ptr< CartesianGrid1D<T> > &); 
////};
////
////
////template<typename TypePattern>
////class CartesianGrid1D : public CartesianGrid 
////{
////
////    using SharedCartesianGrid1D = std::shared_ptr< CartesianGrid1D < TypePattern > >;
////    using SharedConstCartesianGrid1D = std::shared_ptr< CartesianGrid1D< TypePattern > const >;
////
//////==============================================================================
//////   funcoes friends
//////==============================================================================
////
////public:
////
////    template<typename U>
////    friend std::ostream& operator << (std::ostream&, const CartesianGrid1D<U>&);
////    
////    friend struct mvf::GridPatternTraits<TypePattern>;
////    
//////==============================================================================
//////   Identificacao da classe
//////==============================================================================
////
////public:
////
////    DefineIdentity  (   "CartesianGrid1D"
////                    ,   ID::CartesianGrid1D
////                    );
////
//////==============================================================================
////// Construtoras / destrutora
//////==============================================================================
//////
//////
////public  :
////
////    CartesianGrid1D<TypePattern> ()                                  noexcept    = default;
////    CartesianGrid1D<TypePattern> ( const CartesianGrid1D<TypePattern>& )         = default;
////    ~CartesianGrid1D<TypePattern>()                                              = default;
////    
////    CartesianGrid1D<TypePattern>    (   const int&
////                                    ,   const Real&
////                                    ,   const Real&
////                                    );
////    
//////==============================================================================
////// Sobrecarga de operadores
//////==============================================================================
////
////public  :
////    
////    CartesianGrid1D<TypePattern>& operator = (const CartesianGrid1D<TypePattern>&) noexcept = default;
////
//////==============================================================================
////// Funcoes inline
//////==============================================================================
////
////public  :
////    
////    [[nodiscard]] 
////    inline size_t NVol () const {return nVol;};
////    
////    [[nodiscard]] 
////    inline Real StartX () const {return startX;};
////    
////    [[nodiscard]] 
////    inline Real EndX () const {return endX;};
////    
////    [[nodiscard]]    
////    inline Real Lenght () const {return endX - startX;};    
////    
////    [[nodiscard]] 
////    inline const VecReal& FaceX () const {return faceX;};
////
////    [[nodiscard]] 
////    inline  VecReal& FaceX () {return faceX;};
////
////    [[nodiscard]] 
////    inline const VecReal& FaceDX () const {return faceDX;};
////
////    [[nodiscard]] 
////    inline  VecReal& FaceDX () {return faceDX;};
//////    
////    [[nodiscard]] 
////    inline const VecReal& CentreX () const {return centreX;};
////
////    [[nodiscard]] 
////    inline  VecReal& CentreX () {return centreX;};
////
////    [[nodiscard]] 
////    inline const VecReal& CentreDX () const {return centreDX;};
////
////    [[nodiscard]] 
////    inline  VecReal& CentreDX () {return centreDX;};
////
////    [[nodiscard]] 
////    inline size_t size () const {return centreX.size();};
////   
////    [[nodiscard]] 
////    inline bool empty () const {return centreX.empty();};
////
////    [[nodiscard]]
////    inline std::string GridPatternType () const {
////        return TraitsPatternGrid<TypePattern>::GridPatternType(); 
////    }    
////
////    inline SharedCartesianGrid1D GetShared() {
////        return std::make_shared<CartesianGrid1D<TypePattern>>(*this);
////    }
////    [[nodiscard]]
////    inline bool EvaluateOtherCoordinate ()  {
////        return TraitsPatternGrid<TypePattern>::EvaluateOtherCoordinate(this->GetShared()); 
////    }    
////
//////==============================================================================
////// Funcoes
//////==============================================================================
////
////public: 
////    
////    [[nodiscard]]
////    bool EvaluateCoordinates ();   
////
////protected:    
////    
////    [[nodiscard]]
////    virtual bool MainCoordinate (VecReal&)  = 0;   
////
////    [[nodiscard]]
////    bool EvaluateCentreDx ();   
////    
////    [[nodiscard]]
////    bool EvaluateFaceDx ();   
////
////std::unique_ptr< CartesianGrid1D<TypePattern> > ptrCartesianGrid1D; // Smart pointer para evitar vazamentos de mem�ria
////friend struct TraitsPatternGrid<TypePattern>; // Trait tem acesso ao construtor privado de Base    
////    
//////==============================================================================
////// Dados da classe
//////==============================================================================
////
////
////protected:
////    
////LUInt               nVol          = 0;              // Numero de coordenadas
////Real                startX        = 0;              // Coordenada inicial
////Real                endX          = 0;              // Coordenada final
////
////VecReal             faceX{};                        // Coordenadas das faces dos volumes
////VecReal             centreX{};                      // Coordenadas dos centros dos volumes
////VecReal             centreDX{};                     // Distancia entre as coordenadas dos centros dos volumes
////VecReal             faceDX{};                       // Distancia entre as coordenadas das faces dos volumes
////
////
////
////};
////
////MVF_NAMESPACE_CLOSE
////
////#include <MVF++/Grid/Grid1D/CartesianGrid1D.hpp>                  // CartesianGrid1D 
////
////#endif