#pragma once

//==============================================================================
// Includes da biblioteca padrão do C++
//==============================================================================

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

template<int Dim>
struct BCTrait;

template<>
struct BCTrait<1> {
    static constexpr int numFaces = 2;
    using FaceType = BC;
};

template<>
struct BCTrait<2> {
    static constexpr int numFaces = 4;
    using FaceType = std::vector<BC>;
};

template<>
struct BCTrait<3> {
    static constexpr int numFaces = 6;
    using FaceType = std::vector<BC>;
};

FVMAKER_NAMESPACE_CLOSE
