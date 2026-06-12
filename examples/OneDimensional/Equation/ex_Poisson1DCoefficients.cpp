// ----------------------------------------------------------------------------
// File: ex_Poisson1DCoefficients.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Prints coefficients for d2phi/dx2 = f(x) on a 1D grid.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <iomanip>
#include <iostream>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace {

[[nodiscard]] fvm::Real source(fvm::Real x) {
    constexpr fvm::Real pi = 3.141592653589793238462643383279502884;
    return -pi * pi * std::sin(pi * x);
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvm::GridView1D grid{axis};
    const fvm::BoundarySet1D boundaries{
        fvm::dirichlet_1d(0.0),
        fvm::dirichlet_1d(0.0)
    };

    const fvm::Equation1D equation{
        grid,
        fvm::Laplacian1D{},
        fvm::function_source_1d(grid, source),
        boundaries
    };
    const fvm::TridiagonalSystem1D system = fvm::assemble_steady_1d(equation);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Coeficientes para d2phi/dx2 = f(x)\n";
    std::cout << "==================================\n";
    std::cout << "Malha FVGridMaker uniforme: N = " << grid.num_volumes()
              << ", dx = " << grid.cell_lengths()[0] << "\n\n";

    std::cout << std::setw(5) << "i"
              << std::setw(12) << "x_i"
              << std::setw(14) << "a_W"
              << std::setw(14) << "a_P"
              << std::setw(14) << "a_E"
              << std::setw(14) << "b"
              << '\n';

    for (fvm::Size row = 0; row < system.size(); ++row) {
        const fvm::Real lower =
            row > 0 ? system.lower()[row - 1] : fvm::Real{};
        const fvm::Real upper =
            row + 1 < system.size() ? system.upper()[row] : fvm::Real{};

        std::cout << std::setw(5) << row
                  << std::setw(12) << grid.centers()[row]
                  << std::setw(14) << lower
                  << std::setw(14) << system.diagonal()[row]
                  << std::setw(14) << upper
                  << std::setw(14) << system.rhs()[row]
                  << '\n';
    }

    return 0;
}
