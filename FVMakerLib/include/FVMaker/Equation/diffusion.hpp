// ============================================================================
// Arquivo: diffusion.hpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara diffusion no contexto de Equation.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================


//==============================================================================
// Incluições do FVMAKER
//==============================================================================

#include <FVMaker/Coefficient/abstractCoefficient.h>               ///< Definições da classe base de coeficientes; 
#include <FVMaker/Equation/equation.h>                              ///< Definições da Equation; 
using fvm::grd::AbstractGrid;

FVMAKER_NAMESPACE_OPEN

        
template <typename T>
requires Is1DGrid<T>
Diffusion<T> ::Diffusion    (   const T& _grid
                            ,   const AbstractCoefficient<T> _coeff
                            ,   const BoundaryConditions<T> _bc
                            ,   const Function<T>*          _funcao
                            ) 
                            : Equation<T> (_grid, _coeff, _bc, _funcao){
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
    
//    std::cout << this->bc_.faces[0].Alpha() << "  ";
//    std::cout << this->bc_.faces[0].Beta() << "  ";
//    std::cout << this->bc_.faces[0].Gamma() << "\n";
//
//    std::cout << this->bc_.faces[1].Alpha() << "  ";
//    std::cout << this->bc_.faces[1].Beta() << "  ";
//    std::cout << this->bc_.faces[1].Gamma() << "\n";
    
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

    const auto& xf    = this->grid_.GetFace();    // vetor de faces
    const auto& xc    = this->grid_.GetCentre();  // vetor de centros
    const auto& dxc   = this->grid_.GetDCentre();          // comprimento dos volumes
    const auto& coeff = *this->coeffPtr_->VectorCoeff();       // coeficientes
    
    std::vector<std::size_t> indices(N > 3 ? N - 2 : 0);
    std::iota(indices.begin(), indices.end(), 1);
    
    Real    xw, xe;
    Real    xW, xP, xE;
    Real    deltaxw, deltaxe;
    Real    gammaW, gammaP, gammaE;
    Real    denow, denoe;
    Real    gammaw, gammae;
    
auto CalculaCoeficientes = [&](const std::size_t& i)
    {
//
//      Faces
//
        xw = xf[i];     // face "W" (entre W e P)
        xe = xf[i+1];   // face "E" (entre P e E)
//
//      Centros
//
        xW = xc[i-1];   // centro da célula W
        xP = xc[i];     // centro da célula P
        xE = xc[i+1];   // centro da célula E

//
//      Tamanhos dos volumes
//
        deltaxw = dxc[i];
        deltaxe = dxc[i+1];
//
//      Coeficientes do termo da equação diferencial
//
        gammaW = coeff[i-1];
        gammaP = coeff[i];
        gammaE = coeff[i+1];

//
//      Variáveis auxiliares
//
        denow  = gammaW * (xP - xw) + gammaP * (xw - xW);
        denoe  = gammaE * (xe - xP) + gammaP * (xE - xe);

//
//      Gammas nas faces
//
        gammaw = gammaW * gammaP * deltaxw / denow;
        gammae = gammaE * gammaP * deltaxe / denoe;

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

//
//      Primeiro volume    
//
        int i = 0;
        denoe  = coeff[i+1] * (xf[i+1] - xc[i]) + coeff[i] * (xc[i+1] - xf[i+1]);
        gammaw = coeff[i];
        gammae = coeff[i+1] * coeff[i] * dxc[i+1] / denoe;
        aE[i] = gammae / deltaxe;
        denow = 1.0 / (this->bc_.faces[0].Alpha() * dxc[i] - this->bc_.faces[0].Beta());
        aP[i] = aE[i] + gammaw * this->bc_.faces[0].Alpha() * denow;

//
//      Ultimo volume volume    
//
        i = N - 1;
        xw   = xf[i];     // face "W" (entre W e P)
        denow  = coeff[i-1] * (xc[i] - xf[i]) + coeff[i] * (xf[i] - xc[i-1]);
        gammaw = coeff[i-1] * coeff[i] * dxc[i] / denow;
        gammae = coeff[i];
        aW[i] = gammaw / deltaxw;
        denoe = 1.0 / (this->bc_.faces[1].Alpha() * dxc[i+1] + this->bc_.faces[1].Beta());
        aP[i] = aW[i] + gammae * this->bc_.faces[1].Alpha() * denoe;
        
    return true;
}


FVMAKER_NAMESPACE_CLOSE
