#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <iostream>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/AbstractCoefficient .h>              ///< Definições da classe base de coeficientes; 


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
class Coefficient1D : public AbstractCoefficient<T> {

//==============================================================================
// Construtores e destrutora
//==============================================================================
    
public:
    

    Coefficient1D(const Coefficient1D&) noexcept = default;
    virtual ~Coefficient1D() noexcept override = default;
    Coefficient1D(Coefficient1D&&) = delete;    
    
    explicit Coefficient1D (const T& _grid, const Real& = 1.0) : AbstractCoefficient<T>(_grid) {
        this->coeff_.assign(this->grid_.size(), 1.0);
        
        std::cout << "Aqui: " << sizeof(this->coeff_[0]) << "\n\n\n";
    };
};


FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Coefficient/Coefficient1D.hpp>

        