#pragma once

//==============================================================================
// FVMAKER includes
//==============================================================================

#include <FVMaker/BoundaryCondition/BoundaryCondition.h>

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

class Neumann : public BoundaryCondition{

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:
    
    Neumann()                noexcept = default;
    Neumann(const Neumann&)   noexcept = default;
    virtual ~Neumann()       noexcept = default;
    Neumann (const Real& _gamma) : BoundaryCondition(0,1, _gamma) {};
    
    Neumann(Neumann&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Neumann& operator=(const Neumann&) = delete;
    Neumann& operator=(Neumann&&) = delete;
    
};

FVMAKER_NAMESPACE_CLOSE
