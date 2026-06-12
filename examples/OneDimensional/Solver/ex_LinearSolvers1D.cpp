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
#include <string_view>
#include <vector>

#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>

namespace {

[[nodiscard]] fvm::TridiagonalSystem1D make_system() {
    return fvm::TridiagonalSystem1D{
        std::vector<fvm::Real>{-1.0, -1.0},
        std::vector<fvm::Real>{2.0, 2.0, 2.0},
        std::vector<fvm::Real>{-1.0, -1.0},
        fvm::DenseVector{std::vector<fvm::Real>{1.0, 0.0, 1.0}}
    };
}

void print_result(std::string_view name, const fvm::SolveResult& result) {
    std::cout << std::setw(18) << name
              << std::setw(12) << (result.converged ? "yes" : "no")
              << std::setw(12) << result.iterations
              << std::setw(18) << result.residual_norm
              << std::setw(14) << result.solution[0]
              << std::setw(14) << result.solution[1]
              << std::setw(14) << result.solution[2]
              << '\n';
}

}  // namespace

int main() {
    const fvm::TridiagonalSystem1D system = make_system();
    const fvm::IterativeSolverOptions options{
        .tolerance = 1.0e-10,
        .max_iterations = 1000
    };

    const fvm::SolveResult tdma = fvm::TDMA::solve(system);
    const fvm::SolveResult jacobi = fvm::Jacobi::solve(system, options);
    const fvm::SolveResult gauss_seidel =
        fvm::GaussSeidel::solve(system, options);
    const fvm::SolveResult conjugate_gradient =
        fvm::ConjugateGradient::solve(system, options);

    std::cout << std::fixed << std::setprecision(8);
    std::cout << "Linear solvers 1D\n";
    std::cout << "=================\n";
    std::cout << std::setw(18) << "solver"
              << std::setw(12) << "converged"
              << std::setw(12) << "iterations"
              << std::setw(18) << "residual"
              << std::setw(14) << "x0"
              << std::setw(14) << "x1"
              << std::setw(14) << "x2"
              << '\n';

    print_result("TDMA", tdma);
    print_result("Jacobi", jacobi);
    print_result("GaussSeidel", gauss_seidel);
    print_result("CG", conjugate_gradient);

    return tdma.converged
        && jacobi.converged
        && gauss_seidel.converged
        && conjugate_gradient.converged
        ? 0
        : 1;
}
