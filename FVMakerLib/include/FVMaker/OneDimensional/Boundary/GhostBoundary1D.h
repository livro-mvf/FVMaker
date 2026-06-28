// ============================================================================
// Arquivo: GhostBoundary1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Ghost Boundary 1 D no contexto de OneDimensional / Boundary.
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

#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>

namespace fvm {

struct GhostCellLinearization1D final {
    Real constant{};
    Real interior_coefficient{};
};

[[nodiscard]] GhostCellLinearization1D first_ghost_linearization(
    const BoundaryCondition1D& condition,
    BoundarySide1D side,
    Real boundary_position,
    Real center_to_center_distance,
    Real time = Real{}
);

[[nodiscard]] GhostCellLinearization1D first_ghost_linearization(
    const BoundarySet1D& boundaries,
    BoundarySide1D side,
    Real boundary_position,
    Real center_to_center_distance,
    Real time = Real{}
);

[[nodiscard]] Real first_ghost_value(
    const BoundaryCondition1D& condition,
    BoundarySide1D side,
    Real boundary_position,
    Real interior_value,
    Real center_to_center_distance,
    Real time = Real{}
);

[[nodiscard]] Real first_ghost_value(
    const BoundarySet1D& boundaries,
    BoundarySide1D side,
    Real boundary_position,
    Real interior_value,
    Real center_to_center_distance,
    Real time = Real{}
);

}  // namespace fvm
