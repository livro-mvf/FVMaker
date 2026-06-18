#pragma once

//==============================================================================
// FVMAKER includes
//==============================================================================
#include <FVMaker/BoundaryCondition/bc.h>

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

class Dirichlet : public BC{

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:
    
    Dirichlet()                noexcept = default;
    Dirichlet(const Dirichlet&)   noexcept = default;
    virtual ~Dirichlet()       noexcept = default;
    Dirichlet (const Real& _gamma) : BC(1,0, _gamma) {};
    
    Dirichlet(Dirichlet&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    Dirichlet& operator=(const Dirichlet&) = delete;
    Dirichlet& operator=(Dirichlet&&) = delete;
    
};

FVMAKER_NAMESPACE_CLOSE
