// ----------------------------------------------------------------------------
// File: ex_PentadiagonalAndGhostCells1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates optional 1D ghost cells and pentadiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <vector>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>

int main() {
    using fvm::DenseVector;
    using fvm::GhostCells1D;
    using fvm::PentadiagonalSystem1D;
    using fvm::Real;

    const GhostCells1D ghost_cells{2, 2};

    const PentadiagonalSystem1D system{
        std::vector<Real>{-0.5, -0.5, -0.5},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{4.0, 4.0, 4.0, 4.0, 4.0},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{-0.5, -0.5, -0.5},
        DenseVector{std::vector<Real>{2.5, 1.5, 1.0, 1.5, 2.5}}
    };

    const DenseVector solution{std::vector<Real>{1.0, 1.0, 1.0, 1.0, 1.0}};
    const DenseVector residual = fvm::algebraic_residual(system, solution);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Ghost cells 1D\n";
    std::cout << "==============\n";
    std::cout << "left                  = " << ghost_cells.left() << '\n';
    std::cout << "right                 = " << ghost_cells.right() << '\n';
    std::cout << "physical cells        = " << system.size() << '\n';
    std::cout << "storage cells         = "
              << ghost_cells.storage_size(system.size()) << '\n';
    std::cout << "first physical index  = "
              << ghost_cells.first_physical_index() << "\n\n";

    std::cout << "Pentadiagonal residual\n";
    std::cout << "======================\n";
    std::cout << "||A x - b||_inf       = "
              << fvm::norm_infinity(residual) << '\n';

    return fvm::norm_infinity(residual) == Real{} ? 0 : 1;
}
