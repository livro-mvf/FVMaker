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

class Robin : public BoundaryCondition{

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:
    
    Robin()                noexcept = default;
    Robin(const Robin&)   noexcept = default;
    virtual ~Robin()       noexcept = default;
    Robin   ( const Real&   _alpha
            , const Real&   _beta
            , const Real&   _gamma
            ) : BoundaryCondition( _alpha, _beta, _gamma)
    {};
    
    
    Robin(Robin&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Robin& operator=(const Robin&) = delete;
    Robin& operator=(Robin&&) = delete;
    
};

FVMAKER_NAMESPACE_CLOSE
