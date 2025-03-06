#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/AbstractGrid.h>              ///< Definições da Grid; 
using fvm::grd::AbstractGrid;

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
class AbstractCoefficient {
    public :
using DataType = typename T::DataType;

//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    

    AbstractCoefficient(const AbstractCoefficient&) noexcept = default;
    virtual ~AbstractCoefficient() noexcept = default;
    explicit AbstractCoefficient (const T& _grid) : grid_(_grid) {
        coeff_.reserve(this->grid_.NVol());
        coeff_.resize(this->grid_.NVol());
    };

//==============================================================================
// Variaveis da classe
//==============================================================================
  
protected:
    
const T&                    grid_;  ///< Referência para a grid usada
std::vector<DataType>       coeff_; ///< Vetor para armazenar os coeficientes 
};


FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Coefficient/AbstractCoefficient.hpp>
        