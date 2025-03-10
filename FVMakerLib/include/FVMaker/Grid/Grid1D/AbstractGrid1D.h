#pragma once

//==============================================================================
// Name        : AbstractGrid1D.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definição da classe AbstractGrid1D, que representa uma grade unidimensional
//               na biblioteca FVMaker.
//
// Copyright   : Copyright (C) 2025 João Flávio Vasconcellos
//               (jflavio at iprj.uerj.br)
//
// License     : Este programa é software livre: você pode redistribuí-lo e/ou
//               modificá-lo sob os termos da Licença Pública Geral GNU, conforme
//               publicada pela Free Software Foundation, versão 3 da licença,
//               ou (a seu critério) qualquer versão posterior.
//
//               Este programa é distribuído na esperança de que seja útil,
//               mas SEM QUALQUER GARANTIA; sem mesmo a garantia implícita de
//               COMERCIABILIDADE ou ADEQUAÇÃO A UM DETERMINADO PROPÓSITO. Consulte
//               a Licença Pública Geral GNU para mais detalhes.
//
//               Você deve ter recebido uma cópia da Licença Pública Geral GNU
//               junto com este programa. Caso contrário, veja <https://www.gnu.org/licenses/>.
//==============================================================================

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Grid/AbstractGrid.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>
GRID_NAMESPACE_OPEN

/**
 * @class AbstractGrid1D
 * @brief Representa uma grade unidimensional.
 *
 * A classe AbstractGrid1D herda de Grid e é usada para representar grades
 * unidimensionais em simulações numéricas usando volumes finitos.
 */




template<typename TypePattern>
class AbstractGrid1D : public AbstractGrid <TypePattern> {
    
  
    
//==============================================================================
// Construtores e destrutora
//==============================================================================


public:
    
    AbstractGrid1D() noexcept = default;
    AbstractGrid1D(const AbstractGrid1D&) noexcept = default;
    virtual ~AbstractGrid1D() noexcept = default;
    AbstractGrid1D (const int&, const Real&, const Real& = 0);
    
    AbstractGrid1D(AbstractGrid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    AbstractGrid1D& operator=(const AbstractGrid1D&) = delete;
    AbstractGrid1D& operator=(AbstractGrid1D&&) = delete;
    
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const AbstractGrid1D<U>&);
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] virtual std::unique_ptr<AbstractGrid1D>Clone() const = 0;
    [[nodiscard]] virtual bool GeraFaces() {return false;};
    [[nodiscard]] virtual bool GeraCentros() {return false;};
    [[nodiscard]] virtual bool empty () const override {return xFace_.empty() || xCentro_.empty();};

//==============================================================================
// Funções 
//==============================================================================
    
public:
        
    void InitVector ();
    
    [[nodiscard]] bool CalculaCentros(const Real&);
    [[nodiscard]] bool CalculaFaces(const Real&);
    [[nodiscard]] bool CalculaDistancias();

//==============================================================================
// Funções inline
//==============================================================================

public:
        
    [[nodiscard]] inline size_t size () const {return nVol_;};
    [[nodiscard]] inline size_t NVol () const {return nVol_;};
    [[nodiscard]] inline Real Length () const {return length_;};
    [[nodiscard]] inline Real XInit () const {return xIni_;};
    [[nodiscard]] inline Real XEnd () const {return xIni_ + length_;};

    
    [[nodiscard]] inline const VecReal& FaceCoordinate() const {return xFace_;}  // Retorna ref para o original  
    [[nodiscard]] inline const VecReal& CentreCoordinate() const {return xCentro_;}  // Retorna ref para o original  
    [[nodiscard]] inline const VecReal& DFaceCoordinate() const {return dxFace_;}  // Retorna ref para o original  
    [[nodiscard]] inline const VecReal& DCentreCoordinate() const {return dxCentro_;}  // Retorna ref para o original  
    
//    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCFaceCoordinate() const {return std::make_unique<const VecReal>(xFace_);};
//    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCCentreCoordinate() const {return std::make_unique<const VecReal>(xCentro_);};
//    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCDXFace() const {return std::make_unique<const VecReal>(dxFace_);};
//    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCDXCentre() const {return std::make_unique<const VecReal>(dxCentro_);};
//
//    [[nodiscard]] inline std::unique_ptr<VecReal> PtrUFaceCoordinate() {return std::make_unique<VecReal>(xFace_);};
//    [[nodiscard]] inline std::unique_ptr<VecReal> PtrUCentreCoordinate() {return std::make_unique<VecReal>(xCentro_);};
//
//    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCFaceCoordinate() const {return std::make_shared<const VecReal>(xFace_);};
//    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCCentreCoordinate() const {return std::make_shared<const VecReal>(xCentro_);};
//    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCDXFace() const {return std::make_shared<const VecReal>(dxFace_);};
//    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCDXCentre() const {return std::make_shared<const VecReal>(dxCentro_);};
//
//    [[nodiscard]] inline std::shared_ptr<VecReal> PtrSFaceCoordinate() {return std::make_shared<VecReal>(xFace_);};
//    [[nodiscard]] inline std::shared_ptr<VecReal> PtrSCentreCoordinate() {return std::make_shared<VecReal>(xCentro_);};

    [[nodiscard]] inline VecReal* AddressxFace() {return &xFace_;};
    [[nodiscard]] inline const VecReal* AddressxFace() const {return &xFace_;};
    

//==============================================================================
// Variaveis da classe
//==============================================================================
    
protected:

    VecReal                                     xFace_;
    VecReal                                     xCentro_;
    VecReal                                     dxFace_;
    VecReal                                     dxCentro_;
    Real                                        xIni_ = 0;
    Real                                        length_ = 0;
    int                                         nVol_ = 0;
    std::shared_ptr<TypePattern>                typePattern_  = nullptr;
    
};

template<typename TypePattern>
using SharedGrid1D = std::shared_ptr<AbstractGrid1D<TypePattern>>;
template<typename TypePattern>
using SharedConstGrid1D = std::shared_ptr<AbstractGrid1D<TypePattern> const>;
template<typename TypePattern>
using UniqueGrid1D = std::unique_ptr<AbstractGrid1D<TypePattern>>;
template<typename TypePattern>
using UniqueConstGrid1D = std::unique_ptr<AbstractGrid1D<TypePattern> const>;


GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/AbstractGrid1D.hpp>