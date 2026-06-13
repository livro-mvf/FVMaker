// ----------------------------------------------------------------------------
// File: ex_SolveSteady1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Solves a steady 1D equation through the solve controller.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <iomanip>
#include <iostream>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/SolveController1D.h>

namespace {

constexpr fvm::Real pi = 3.141592653589793238462643383279502884;

[[nodiscard]] fvm::Real phi_exact(fvm::Real x) {
    return std::sin(pi * x);
}

[[nodiscard]] fvm::Real source(fvm::Real x) {
    return -pi * pi * std::sin(pi * x);
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{40},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::Equation1D equation{
        grid,
        fvm::Laplacian1D{},
        fvm::function_source_1d(grid, source),
        fvm::BoundarySet1D{fvm::dirichlet_1d(0.0), fvm::dirichlet_1d(0.0)}
    };

    const fvm::SteadyState control{
        .absolute_tolerance = 1.0e-10,
        .relative_tolerance = 1.0e-10,
        .max_iterations = 1000
    };

    const fvm::SolveResult result = fvm::solve_steady_1d(equation, control);

    fvm::DenseVector error{grid.num_volumes()};

    for (fvm::Size i = 0; i < grid.num_volumes(); ++i) {
        error[i] = result.solution[i] - phi_exact(grid.centers()[i]);
    }

    std::cout << std::scientific << std::setprecision(8);
    std::cout << "Steady 1D solve controller\n";
    std::cout << "==========================\n";
    std::cout << "N                      = " << grid.num_volumes() << '\n';
    std::cout << "converged              = " << std::boolalpha
              << result.converged << '\n';
    std::cout << "iterations             = " << result.iterations << '\n';
    std::cout << "residual infinity      = " << result.residual_norm << '\n';
    std::cout << "relative residual      = "
              << result.relative_residual_norm << '\n';
    std::cout << "requested tolerance    = "
              << result.requested_tolerance << '\n';
    std::cout << "solution error infinity = "
              << fvm::norm_infinity(error) << '\n';

    return result.converged && fvm::norm_infinity(error) < fvm::Real{6.0e-4}
        ? 0
        : 1;
}
