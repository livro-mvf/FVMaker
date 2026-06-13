// ----------------------------------------------------------------------------
// File: ex_Nonlinear1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Runs the preliminary Picard path for a nonlinear 1D problem.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Solver/NonlinearSolve1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::PicardProblem1D problem{
        grid,
        fvm::nonlinear_coefficient_1d(
            [](fvm::Real phi, fvm::Real) {
                return fvm::Real{1.0} + phi * phi;
            }
        ),
        fvm::zero_source_1d(grid.num_volumes()),
        fvm::BoundarySet1D{
            fvm::dirichlet_1d(0.0),
            fvm::dirichlet_1d(0.0)
        }
    };

    const fvm::DenseVector initial_solution{grid.num_volumes(), 1.0};
    const fvm::PicardResult result = fvm::picard_solve_1d(
        problem,
        initial_solution,
        fvm::PicardOptions{
            .absolute_tolerance = 1.0e-12,
            .relative_tolerance = 0.0,
            .max_iterations = 20,
            .relaxation_factor = 0.8
        }
    );

    std::cout << std::scientific << std::setprecision(8);
    std::cout << "Preliminary nonlinear 1D Picard example\n";
    std::cout << "=======================================\n";
    std::cout << "Equation path: d/dx(Gamma(phi,x) dphi/dx) = 0\n";
    std::cout << "Gamma(phi,x) = 1 + phi^2\n";
    std::cout << "Boundary conditions: phi(0) = phi(1) = 0\n";
    std::cout << "Status: provisional fake example for plumbing only\n\n";
    std::cout << "converged                = " << std::boolalpha
              << result.converged << '\n';
    std::cout << "picard iterations        = " << result.iterations << '\n';
    std::cout << "correction infinity      = "
              << result.correction_norm << '\n';
    std::cout << "relative correction      = "
              << result.relative_correction_norm << '\n';
    std::cout << "requested tolerance      = "
              << result.requested_tolerance << "\n\n";

    std::cout << std::setw(5) << "i"
              << std::setw(16) << "x"
              << std::setw(18) << "phi" << '\n';

    for (fvm::Size i = 0; i < grid.num_volumes(); ++i) {
        std::cout << std::setw(5) << i
                  << std::setw(16) << grid.centers()[i]
                  << std::setw(18) << result.solution[i] << '\n';
    }

    return result.converged ? 0 : 1;
}
