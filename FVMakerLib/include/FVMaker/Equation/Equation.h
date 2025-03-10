#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================
#include <iostream>

//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Grid/AbstractGrid.h>     // Definições da Grid; 

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
class Equation {

public:
        
    Equation(const Equation&) noexcept = default;
    virtual ~Equation() noexcept = default;
    Equation(Equation&&) = delete; 
    Equation (const T& _grid, const AbstractCoefficient<T> _coeff) :
        grid_(_grid)
    {
        
    try {
        if (_coeff.empty())  throw fvm::FVMakerException(ErrorCode::UndefiniedCoefficient);
        
    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
    
        this->coeffPtr_ = std::make_shared<Coefficient1D<T>>(_coeff);    
//        std::cout << "Vou imprimir o coeff na equation\n";
//            auto    ptrCoeffic = this->coeffPtr_->VectorCoeff();
//
//            for (auto xx : *ptrCoeffic) {
//                std::cout << "coeff = " << xx << "\n";
//            }
//            
//            exit(EXIT_SUCCESS);
    
    };
    

    
    template <typename U>
    friend std::ostream& operator<< (std::ostream&, const Equation<U>&);
    
//==============================================================================
// Variaveis da classe
//==============================================================================
    
protected :    
    
    SharedAbstractCoefficient<T>        coeffPtr_;   
    const T&                            grid_;  ///< Referência para a grid usada
    
    VecReal                             aP_, aE_, aW_, aN_, aS_, aF_, aB_, BP_, SP_, SC_;
    
};

FVMAKER_NAMESPACE_CLOSE

#include <FVMaker/Equation/Equation.hpp>