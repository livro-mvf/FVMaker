// ----------------------------------------------------------------------------
// File: GhostBoundary1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines ghost-cell values induced by 1D boundary conditions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

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
