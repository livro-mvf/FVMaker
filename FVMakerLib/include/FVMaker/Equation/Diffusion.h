#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/AbstractCoefficient.h>               ///< Definições da classe base de coeficientes; 
#include <FVMaker/Equation/Equation.h>                              ///< Definições da Equation; 
#include <FVMaker/Grid/GridDimension.h>
using fvm::grd::AbstractGrid;
using fvm::GridDim;


FVMAKER_NAMESPACE_OPEN

/**
 * @brief Classe genérica para representar uma função matemática definida
 * sobre os centros dos volumes de uma grid.
 *
 * A classe assume que o grid fornece um método (ex.: PtrSCCentreCoordinate())
 * que retorna um ponteiro para um container dos centros dos volumes.
 * Cada centro é convertido para um std::vector<Real> contendo as coordenadas,
 * possibilitando usar a mesma interface para grid 1D, 2D, 3D, etc.
 */
        
template <typename T>
requires Is1DGrid<T>        
class Diffusion : public Equation <T>
{

//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    
    Diffusion(const Diffusion&) noexcept = default;
    virtual ~Diffusion() noexcept override = default;
    Diffusion(Diffusion&&) = delete;    

    Diffusion   (   const T& _grid
                ,   const AbstractCoefficient<T>    _coeff
                ,   const BoundaryConditions<T>     _bc
                ,   const Function<T>*              _funcao = nullptr  
                );
 
    
//==============================================================================
// Funções
//==============================================================================
public  :
        
    [[nodiscard]] bool ComputeCoefficient();
    
private:    
    [[nodiscard]] bool ComputeCoefficient1D();
    
//==============================================================================
// dados da classe
//==============================================================================
    
    
};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Equation/Diffusion.hpp>                              ///< Definições da Equation; 
