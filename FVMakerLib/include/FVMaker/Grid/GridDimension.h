#pragma once

//==============================================================================
// Nome        : GridDimension.h
// Autor       : João Flávio Vieira de Vasconcellos
// Versão      : 1.3
// Descrição   : Definição de traits para dimensionamento de grids 1D.
//               Permite identificar automaticamente a dimensão de uma malha 1D
//               e restringir a utilização de Diffusion<Grid> apenas para malhas suportadas.
//==============================================================================

//==============================================================================
//  C++ includes
//==============================================================================
#include <type_traits>

//==============================================================================
//  FVMAKER includes
//==============================================================================
#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>
#include <FVMaker/Grid/Grid1D/AbstractGrid1D.h>
#include <FVMaker/Grid/GridPattern/CellCentered.h>
using fvm::grd::CellCentered;

//==============================================================================
// Abertura do namespace FVMaker
//==============================================================================
FVMAKER_NAMESPACE_OPEN

//==============================================================================
// 📌 Trait para identificar a dimensão de um grid 1D
//==============================================================================
template <typename T>
struct GridDim {
    using DataType = Real;
    static constexpr int value =
        std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ? 1 :  // Para UniformGrid1D
        std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T> ? 1 : -1;  // Para RandomGrid1D<TypePattern>
};

//==============================================================================
//  Concept para restringir a Diffusion<T> a grids 1D
//==============================================================================

template<typename T>
concept Is1DGrid =
    std::is_base_of_v<fvm::grd::AbstractGrid1D<CellCentered>, T> ||  // Para UniformGrid1D
    std::is_base_of_v<fvm::grd::AbstractGrid1D<typename T::DataType>, T>; // Para RandomGrid1D<TypePattern>

FVMAKER_NAMESPACE_CLOSE
