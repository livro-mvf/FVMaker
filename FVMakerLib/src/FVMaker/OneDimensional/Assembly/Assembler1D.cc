// ----------------------------------------------------------------------------
// File: Assembler1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements assembly functions for minimal steady 1D equations.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

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
