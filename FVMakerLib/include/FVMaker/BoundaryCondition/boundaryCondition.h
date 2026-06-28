// ============================================================================
// Arquivo: boundaryCondition.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara boundary Condition no contexto de BoundaryCondition.
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
#include <utility> // std::forward

//==============================================================================
// FVMAKER includes
//==============================================================================
#include <FVMaker/BoundaryCondition/bcTrait.h>
#include <FVMaker/Grid/gridDimension.h>

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


template<typename TypePattern>
class BoundaryConditions {
    
public:
    
    static constexpr int Dim = GridDim<TypePattern>::value;
    using Trait   = BCTrait<Dim>;
    using Face    = typename Trait::FaceType;  // Tipo da face
    static constexpr int N = Trait::numFaces;  // Numero de faces
    std::array<Face, N> faces;
};

template<typename TypePattern, typename... Args>
BoundaryConditions<TypePattern> makeBC(Args&&... args)
{
    static constexpr int Dim = GridDim<TypePattern>::value;
    static_assert(sizeof...(Args) == BCTrait<Dim>::numFaces,
                  "Numero de argumentos nao corresponde a dimensão da malha");

    BoundaryConditions<TypePattern> bc;
    bc.faces = { std::forward<Args>(args)... };

    return bc;
}

FVMAKER_NAMESPACE_CLOSE
