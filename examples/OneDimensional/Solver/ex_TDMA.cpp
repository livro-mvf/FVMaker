// ----------------------------------------------------------------------------
// File: ex_TDMA.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates solving a 1D tridiagonal system with TDMA.
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
#include <FVMaker/OneDimensional/Solver/TDMA.h>

int main() {
    const fvm::TridiagonalSystem1D system{
        std::vector<fvm::Real>{-1.0, -1.0},
        std::vector<fvm::Real>{2.0, 2.0, 2.0},
        std::vector<fvm::Real>{-1.0, -1.0},
        fvm::DenseVector{std::vector<fvm::Real>{1.0, 0.0, 1.0}}
    };

    const fvm::SolveResult result = fvm::TDMA::solve(system);

    std::cout << "TDMA solution\n";
    std::cout << "converged  : " << result.converged << '\n';
    std::cout << "iterations : " << result.iterations << '\n';

    for (fvm::Size i = 0; i < result.solution.size(); ++i) {
        std::cout << "x[" << i << "] = " << result.solution[i] << '\n';
    }

    return 0;
}
