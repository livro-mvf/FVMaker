// ----------------------------------------------------------------------------
// File: ex_ResidualNorm1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates A X - B residual and norms for a 1D system.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

int main() {
    const fvm::TridiagonalSystem1D system{
        std::vector<fvm::Real>{-1.0, -1.0},
        std::vector<fvm::Real>{2.0, 2.0, 2.0},
        std::vector<fvm::Real>{-1.0, -1.0},
        fvm::DenseVector{std::vector<fvm::Real>{1.0, 0.0, 1.0}}
    };

    const fvm::DenseVector solution{
        std::vector<fvm::Real>{1.0, 0.5, 1.0}
    };

    const fvm::DenseVector residual =
        fvm::algebraic_residual(system, solution);

    std::cout << "Residual r = A X - B\n";
    std::cout << "||r||inf = " << fvm::norm_infinity(residual) << '\n';
    std::cout << "||r||1   = " << fvm::norm_l1(residual) << '\n';
    std::cout << "||r||2   = " << fvm::norm_l2(residual) << '\n';

    return 0;
}
