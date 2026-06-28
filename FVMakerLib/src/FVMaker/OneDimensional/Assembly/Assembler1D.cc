// ============================================================================
// Arquivo: Assembler1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Assembler 1 D no contexto de OneDimensional / Assembly.
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

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

namespace fvm {

namespace {

[[nodiscard]] Real west_distance(const GridView1D& grid, Size row) {
    if (row == 0) {
        return Real{2.0} * grid.dx_centers()[0];
    }

    return grid.dx_centers()[row];
}

[[nodiscard]] Real east_distance(const GridView1D& grid, Size row) {
    if (row + 1 == grid.num_volumes()) {
        return Real{2.0} * grid.dx_centers()[grid.num_volumes()];
    }

    return grid.dx_centers()[row + 1];
}

}  // namespace

TridiagonalSystem1D assemble_laplacian_1d(
    const GridView1D& grid,
    const Laplacian1D& laplacian,
    const BoundarySet1D& boundaries,
    Real time
) {
    require(
        grid.num_volumes() > 0,
        error_catalog::kInvalidMeshDimension,
        Equation1D::id()
    );

    TridiagonalSystem1D system{grid.num_volumes()};

    for (Size row = 0; row < grid.num_volumes(); ++row) {
        const Real volume = grid.cell_lengths()[row];
        const Real west = laplacian.face_coefficient(grid, row)
            / (west_distance(grid, row) * volume);
        const Real east = laplacian.face_coefficient(grid, row + 1)
            / (east_distance(grid, row) * volume);

        Real lower = row > 0 ? west : Real{};
        Real upper = row + 1 < grid.num_volumes() ? east : Real{};
        Real diagonal = -(west + east);
        Real rhs = Real{};

        if (row == 0) {
            const GhostCellLinearization1D ghost = first_ghost_linearization(
                boundaries,
                BoundarySide1D::left,
                grid.xmin(),
                west_distance(grid, row),
                time
            );

            diagonal += west * ghost.interior_coefficient;
            rhs -= west * ghost.constant;
        }

        if (row + 1 == grid.num_volumes()) {
            const GhostCellLinearization1D ghost = first_ghost_linearization(
                boundaries,
                BoundarySide1D::right,
                grid.xmax(),
                east_distance(grid, row),
                time
            );

            diagonal += east * ghost.interior_coefficient;
            rhs -= east * ghost.constant;
        }

        system.set_row(row, lower, diagonal, upper, rhs);
    }

    return system;
}

TridiagonalSystem1D assemble_steady_1d(
    const Equation1D& equation,
    Real time
) {
    TridiagonalSystem1D system = assemble_laplacian_1d(
        equation.grid(),
        equation.laplacian(),
        equation.boundaries(),
        time
    );

    apply_source_to_system(system, equation.source());

    return system;
}

}  // namespace fvm
