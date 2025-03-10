#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

//==============================================================================
// FVMAKER includes
//==============================================================================

#include <FVMaker/Misc/namespace.h>
#include <FVMaker/Misc/type.h>

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

class BC {

//==============================================================================
// Construtores e destrutora
//==============================================================================

public:
    
    BC()                noexcept = default;
    BC(const BC&)   noexcept = default;
    virtual ~BC()       noexcept = default;
    BC   ( const Real&   _alpha
                        , const Real&   _beta
                        , const Real&   _gamma
                        ) : alpha_(_alpha), beta_(_beta), gamma_(_gamma)
    {};
    
    BC(BC&&) = delete;
    
//==============================================================================
// Sobrecarga de operadores
//==============================================================================
    
public:
    
    BC& operator=(const BC&) noexcept = default;
    BC& operator=(BC&&) = delete;
    
//==============================================================================
// Funções inline
//==============================================================================
    
public:

    [[nodiscard]] inline Real Alpha() const {return alpha_;};
    [[nodiscard]] inline Real Beta() const {return beta_;};
    [[nodiscard]] inline Real Gamma() const {return gamma_;};
    
//==============================================================================
// Variaveis da classe
//==============================================================================
    
private:

    Real    alpha_ = 0;
    Real    beta_ = 0;
    Real    gamma_ = 0;
    
};    

FVMAKER_NAMESPACE_CLOSE
