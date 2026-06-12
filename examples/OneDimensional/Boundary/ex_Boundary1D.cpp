// ----------------------------------------------------------------------------
// File: ex_Boundary1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates 1D boundary conditions and ghost-cell values.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>

#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

int main() {
    using fvm::BoundarySet1D;
    using fvm::BoundarySide1D;
    using fvm::Real;

    const BoundarySet1D boundaries{
        fvm::dirichlet_1d(1.0),
        fvm::neumann_1d(0.0)
    };

    const Real dx = 0.25;
    const Real left_interior_value = 0.75;
    const Real right_interior_value = 2.0;

    const Real left_ghost = fvm::first_ghost_value(
        boundaries,
        BoundarySide1D::left,
        0.0,
        left_interior_value,
        dx
    );

    const Real right_ghost = fvm::first_ghost_value(
        boundaries,
        BoundarySide1D::right,
        1.0,
        right_interior_value,
        dx
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Boundary1D\n";
    std::cout << "==========\n";
    std::cout << "left interior value  = " << left_interior_value << '\n';
    std::cout << "left ghost value     = " << left_ghost << '\n';
    std::cout << "right interior value = " << right_interior_value << '\n';
    std::cout << "right ghost value    = " << right_ghost << '\n';

    return (left_ghost == Real{1.25} && right_ghost == right_interior_value)
        ? 0
        : 1;
}
