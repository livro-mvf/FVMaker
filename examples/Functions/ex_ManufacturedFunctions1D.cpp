// ----------------------------------------------------------------------------
// File: ex_ManufacturedFunctions1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates manufactured 1D functions for MMS-style tests.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Functions/ManufacturedProblem1D.h>
#include <FVMaker/Functions/ManufacturedSolution1D.h>
#include <FVMaker/Functions/VariableCoefficient1D.h>

int main() {
    const fvm::ManufacturedSolution1D solution{
        .phi =
            [](fvm::Real x) {
                return x * x;
            },
        .dphi_dx =
            [](fvm::Real x) {
                return 2.0 * x;
            },
        .d2phi_dx2 =
            [](fvm::Real) {
                return 2.0;
            },
    };

    const fvm::VariableCoefficient1D coefficient{
        .gamma =
            [](fvm::Real x) {
                return 1.0 + x;
            },
        .dgamma_dx =
            [](fvm::Real) {
                return 1.0;
            },
    };

    fvm::ManufacturedProblem1D problem{solution};
    problem.add_coefficient("gamma", coefficient);
    problem.add_coefficient(
        "sigma",
        fvm::VariableCoefficient1D{
            .gamma =
                [](fvm::Real x) {
                    return 2.0 + x * x;
                },
            .dgamma_dx =
                [](fvm::Real x) {
                    return 2.0 * x;
                },
        }
    );

    const fvm::StoredScalarFunction1D source = [solution, coefficient](fvm::Real x) {
        return coefficient.value(x) * solution.second_derivative(x) +
               coefficient.derivative(x) * solution.first_derivative(x);
    };

    const fvm::Real x = 0.5;

    std::cout << "Manufactured functions 1D\n";
    std::cout << "x          : " << x << '\n';
    std::cout << "phi        : " << solution.value(x) << '\n';
    std::cout << "dphi_dx    : " << solution.first_derivative(x) << '\n';
    std::cout << "d2phi_dx2  : " << solution.second_derivative(x) << '\n';
    std::cout << "gamma      : " << coefficient.value(x) << '\n';
    std::cout << "dgamma_dx  : " << coefficient.derivative(x) << '\n';
    std::cout << "sigma      : " << problem.coefficient("sigma").value(x) << '\n';
    std::cout << "source     : " << source(x) << '\n';

    return 0;
}
