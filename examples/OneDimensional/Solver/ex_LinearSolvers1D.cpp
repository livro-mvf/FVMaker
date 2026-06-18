// ----------------------------------------------------------------------------
// File: ex_LinearSolvers1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Compares direct and iterative 1D linear solvers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>
#include <FVMaker/OneDimensional/Solver/SolveController1D.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>
#include <FVMaker/Output/VTK/Variable1DVTKWriter.h>

namespace {

[[nodiscard]] fvm::TridiagonalSystem1D make_system() {
    return fvm::TridiagonalSystem1D{
        std::vector<fvm::Real>{-1.0, -1.0},
        std::vector<fvm::Real>{2.0, 2.0, 2.0},
        std::vector<fvm::Real>{-1.0, -1.0},
        fvm::DenseVector{std::vector<fvm::Real>{1.0, 0.0, 1.0}}
    };
}

void print_result(
    fvm::LinearSolverKind1D solver,
    const fvm::SolveResult& result,
    const fvm::VariableDifferenceNorms1D& error
) {
    std::cout << std::setw(18) << fvm::name(solver)
              << std::setw(12) << (result.converged ? "yes" : "no")
              << std::setw(12) << result.iterations
              << std::setw(18) << result.residual_norm
              << std::setw(18) << error.infinity
              << std::setw(14) << result.solution[0]
              << std::setw(14) << result.solution[1]
              << std::setw(14) << result.solution[2]
              << '\n';
}

[[nodiscard]] std::string vtk_filename(fvm::LinearSolverKind1D solver) {
    switch (solver) {
        case fvm::LinearSolverKind1D::tdma:
            return "ex_solver_tdma.vtk";
        case fvm::LinearSolverKind1D::jacobi:
            return "ex_solver_jacobi.vtk";
        case fvm::LinearSolverKind1D::gauss_seidel:
            return "ex_solver_gauss_seidel.vtk";
        case fvm::LinearSolverKind1D::conjugate_gradient:
            return "ex_solver_conjugate_gradient.vtk";
        case fvm::LinearSolverKind1D::bicg:
            return "ex_solver_bicg.vtk";
        case fvm::LinearSolverKind1D::bicgstab:
            return "ex_solver_bicgstab.vtk";
    }

    return "ex_solver_unknown.vtk";
}

}  // namespace

int main() {
    const fvm::TridiagonalSystem1D system = make_system();
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::LinearSolverKind1D solvers[]{
        fvm::LinearSolverKind1D::tdma,
        fvm::LinearSolverKind1D::jacobi,
        fvm::LinearSolverKind1D::gauss_seidel,
        fvm::LinearSolverKind1D::conjugate_gradient,
        fvm::LinearSolverKind1D::bicg,
        fvm::LinearSolverKind1D::bicgstab
    };

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Linear solvers 1D\n";
    std::cout << "=================\n";
    std::cout << std::setw(18) << "solver"
              << std::setw(12) << "converged"
              << std::setw(12) << "iterations"
              << std::setw(18) << "residual"
              << std::setw(18) << "error_inf"
              << std::setw(14) << "x0"
              << std::setw(14) << "x1"
              << std::setw(14) << "x2"
              << '\n';

    bool all_converged = true;
    bool printed_variable = false;

    for (fvm::LinearSolverKind1D solver : solvers) {
        const fvm::SolveResult result = fvm::solve_steady_system_1d(
            system,
            fvm::LinearSolverOptions1D{
                .solver = solver,
                .absolute_tolerance = 1.0e-10,
                .relative_tolerance = 1.0e-10,
                .max_iterations = 1000
            }
        );

        fvm::Variable1D phi{grid, "phi"};
        phi.update_phi(result);
        phi.assign_analytic([](fvm::Real) {
            return 1.0;
        });

        print_result(solver, result, phi.norms_to_analytic());
        fvm::Variable1DVTKWriter::write(phi, vtk_filename(solver));

        if (!printed_variable) {
            std::cout << "\nVariable1D impressa com operator<<\n";
            std::cout << "=================================\n";
            std::cout << phi << "\n\n";

            const fvm::VariableDifferenceNorms1D analytic =
                phi.norms_to_analytic();
            std::cout << "Normas de phi - phi_analit para "
                      << fvm::name(solver) << '\n';
            std::cout << "  infinita = " << analytic.infinity << '\n';
            std::cout << "  L1       = " << analytic.l1 << '\n';
            std::cout << "  L2       = " << analytic.l2 << "\n\n";

            printed_variable = true;
        }

        all_converged = all_converged && result.converged;
    }

    std::cout << "\nArquivos VTK gerados: ex_solver_*.vtk\n";

    return all_converged ? 0 : 1;
}
