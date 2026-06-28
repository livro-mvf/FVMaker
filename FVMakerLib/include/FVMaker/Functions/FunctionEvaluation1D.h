// ============================================================================
// Arquivo: FunctionEvaluation1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Function Evaluation 1 D no contexto de Functions.
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

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

template <ScalarFunction1D Function>
[[nodiscard]] DenseVector evaluate_at_centers(
    const GridView1D& grid,
    Function&& function
) {
    DenseVector values{grid.num_volumes()};
    const auto centers = grid.centers();

    for (Size i = 0; i < values.size(); ++i) {
        values[i] = static_cast<Real>(std::invoke(function, centers[i]));
    }

    return values;
}

template <ScalarFunction1D Function>
[[nodiscard]] DenseVector evaluate_at_faces(
    const GridView1D& grid,
    Function&& function
) {
    DenseVector values{grid.num_faces()};
    const auto faces = grid.faces();

    for (Size i = 0; i < values.size(); ++i) {
        values[i] = static_cast<Real>(std::invoke(function, faces[i]));
    }

    return values;
}

}  // namespace fvm
