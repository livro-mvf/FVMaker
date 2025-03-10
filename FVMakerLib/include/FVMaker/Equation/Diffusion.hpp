#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/AbstractCoefficient.h>               ///< Definições da classe base de coeficientes; 
#include <FVMaker/Equation/Equation.h>                              ///< Definições da Equation; 
using fvm::grd::AbstractGrid;

FVMAKER_NAMESPACE_OPEN

        
template <typename T>
requires Is1DGrid<T>
Diffusion<T> ::Diffusion    (   const T& _grid
                            ,   const AbstractCoefficient<T> _coeff
                            ) 
                            : Equation<T> (_grid, _coeff){
};
            
        
template <typename T>
requires Is1DGrid<T>
bool Diffusion<T> :: ComputeCoefficient() {
    
    try {
        if (this->coeffPtr_->empty()) {
            throw fvm::FVMakerException(ErrorCode::UndefiniedCoefficient);
        }

        switch (GridDim<T>::value) {
            case 1:
                return ComputeCoefficient1D();
            case 2:
            case 3:    
            default:
                throw fvm::FVMakerException(ErrorCode::InvalidGridDimension);
        }

    } catch (const fvm::FVMakerException& e) {
        std::cerr << "\n\n";
        PrintLine(std::cerr);
        std::cerr << "Exceção capturada: " << e.what() << "\n";
        PrintLine(std::cerr);
        std::cerr << "\n\n";
        exit(EXIT_FAILURE);
    }
    return true;
}


template <typename T>
requires Is1DGrid<T>
bool Diffusion<T>::ComputeCoefficient1D() 
{
    const std::size_t N = this->grid_.NVol();

    this->aP_.resize(N, 0.0);
    this->aE_.resize(N, 0.0);
    this->aW_.resize(N, 0.0);
    this->BP_.resize(N, 0.0);
    this->SP_.resize(N, 0.0);
    this->SC_.resize(N, 0.0);

    auto& aP = this->aP_;
    auto& aE = this->aE_;
    auto& aW = this->aW_;

    const auto& xf    = this->grid_.FaceCoordinate();    // vetor de faces
    const auto& xc    = this->grid_.CentreCoordinate();  // vetor de centros
    const auto& dxc   = this->grid_.DCentreCoordinate();          // comprimento dos volumes
    const auto& coeff = *this->coeffPtr_->VectorCoeff();       // coeficientes
    
    std::vector<std::size_t> indices(N > 3 ? N - 2 : 0);
    std::iota(indices.begin(), indices.end(), 1);
    
auto CalculaCoeficientes = [&](const std::size_t& i)
    {
//
//      Faces
//
        Real xw = xf[i];     // face "W" (entre W e P)
        Real xe = xf[i+1];   // face "E" (entre P e E)
//
//      Centros
//
        Real xW = xc[i-1];   // centro da célula W
        Real xP = xc[i];     // centro da célula P
        Real xE = xc[i+1];   // centro da célula E

//
//      Tamanhos dos volumes
//
        Real deltaxw = dxc[i];
        Real deltaxe = dxc[i+1];
//
//      Coeficientes do termo da equação diferencial
//
        Real gammaW = coeff[i-1];
        Real gammaP = coeff[i];
        Real gammaE = coeff[i+1];

//
//      Variáveis auxiliares
//
        Real denow  = gammaW * (xP - xw) + gammaP * (xw - xW);
        Real denoe  = gammaE * (xe - xP) + gammaP * (xE - xe);

//
//      Gammas nas faces
//
        Real gammaw = gammaW * gammaP * deltaxw / denow;
        Real gammae = gammaE * gammaP * deltaxe / denoe;

//
//      Coeficientes do sistema de equações lineares
//
        aW[i] = gammaw / deltaxw;
        aE[i] = gammae / deltaxe;
        aP[i] = aW[i] + aE[i];
        
    };
            
    std::for_each   (   std::execution::par
                    ,   indices.begin()
                    ,   indices.end()
                    ,   CalculaCoeficientes
                    );

    return true;
}


FVMAKER_NAMESPACE_CLOSE
