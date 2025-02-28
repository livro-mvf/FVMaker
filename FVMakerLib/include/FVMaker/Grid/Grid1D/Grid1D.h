#pragma once

//==============================================================================
// Name        : Grid1D.h
// Author      : João Flávio Vieira de Vasconcellos
// Version     : 1.1
// Description : Definição da classe Grid1D, que representa uma grade unidimensional
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

#include <FVMaker/Grid/Grid.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Misc/namespace.h>
GRID_NAMESPACE_OPEN

/**
 * @class Grid1D
 * @brief Representa uma grade unidimensional.
 *
 * A classe Grid1D herda de Grid e é usada para representar grades
 * unidimensionais em simulações numéricas usando volumes finitos.
 */

template<typename TypePattern>
class Grid1D : public Grid <TypePattern> {
    
//==============================================================================
// Construtores e destrutora
//==============================================================================

public:

    using    DataType = Real;
    
public:
    
    Grid1D() noexcept = default;
    Grid1D(const Grid1D&) noexcept = default;
    virtual ~Grid1D() noexcept = default;
    Grid1D (const int&, const Real&, const Real& = 0);
    
    Grid1D(Grid1D&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Grid1D& operator=(const Grid1D&) = delete;
    Grid1D& operator=(Grid1D&&) = delete;
    
    friend std::ostream& operator<< <> (std::ostream&, const Grid1D<TypePattern>&);
    
//==============================================================================
// Funções Virtuais
//==============================================================================
    
public:
    
    [[nodiscard]] virtual std::unique_ptr<Grid1D>Clone() const = 0;
    [[nodiscard]] virtual bool GeraFaces() {return false;};
    [[nodiscard]] virtual bool GeraCentros() {return false;};
    virtual int Dimension () const {return 1;};
    
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
        
    [[nodiscard]] inline size_t size () const {return nVol;};
    [[nodiscard]] inline size_t NVol () const {return nVol;};
    [[nodiscard]] inline Real Length () const {return length;};
    [[nodiscard]] inline Real XInit () const {return xIni;};
    [[nodiscard]] inline Real XEnd () const {return xIni + length;};
    [[nodiscard]] inline bool empty () const {return size() == 0;};
    [[nodiscard]] inline bool GridSetup() const {return gridSetup;};

    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCFaceCoordinate() {return std::make_unique<const VecReal>(xFace);};
    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCCentreCoordinate() {return std::make_unique<const VecReal>(xCentro);};
    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCDXFace() {return std::make_unique<const VecReal>(dxFace);};
    [[nodiscard]] inline std::unique_ptr<const VecReal> PtrUCDXCentre() {return std::make_unique<const VecReal>(dxCentro);};

    [[nodiscard]] inline std::unique_ptr<VecReal> PtrUFaceCoordinate() {return std::make_unique<VecReal>(xFace);};
    [[nodiscard]] inline std::unique_ptr<VecReal> PtrUCentreCoordinate() {return std::make_unique<VecReal>(xCentro);};

    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCFaceCoordinate() {return std::make_shared<const VecReal>(xFace);};
    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCCentreCoordinate() {return std::make_shared<const VecReal>(xCentro);};
    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCDXFace() {return std::make_shared<const VecReal>(dxFace);};
    [[nodiscard]] inline std::shared_ptr<const VecReal> PtrSCDXCentre() {return std::make_shared<const VecReal>(dxCentro);};

    [[nodiscard]] inline std::shared_ptr<VecReal> PtrSFaceCoordinate() {return std::make_shared<VecReal>(xFace);};
    [[nodiscard]] inline std::shared_ptr<VecReal> PtrSCentreCoordinate() {return std::make_shared<VecReal>(xCentro);};

    [[nodiscard]] inline VecReal* AddressxFace() {return &xFace;};
    [[nodiscard]] inline const VecReal* AddressxFace() const {return &xFace;};
    

//==============================================================================
// Variaveis da classe
//==============================================================================
    
protected:

    VecReal                         xFace;
    VecReal                         xCentro;
    VecReal                         dxFace;
    VecReal                         dxCentro;
    Real                            xIni = 0;
    Real                            length = 0;
    int                             nVol = 0;
    bool                            gridSetup = false;
    std::shared_ptr<TypePattern>    typePattern  = nullptr;
    
};

template<typename TypePattern>
using SharedGrid1D = std::shared_ptr<Grid1D<TypePattern>>;
template<typename TypePattern>
using SharedConstGrid1D = std::shared_ptr<Grid1D<TypePattern> const>;
template<typename TypePattern>
using UniqueGrid1D = std::unique_ptr<Grid1D<TypePattern>>;
template<typename TypePattern>
using UniqueConstGrid1D = std::unique_ptr<Grid1D<TypePattern> const>;


GRID_NAMESPACE_CLOSE

#include <FVMaker/Grid/Grid1D/Grid1D.hpp>