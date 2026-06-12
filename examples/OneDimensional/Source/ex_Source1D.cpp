// ----------------------------------------------------------------------------
// File: ex_Source1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates a 1D source term S = S_C + S_P phi.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <vector>

#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

int main() {
    using fvm::DenseVector;
    using fvm::LinearizedSource1D;
    using fvm::Real;

    const LinearizedSource1D source = fvm::vector_source_1d(
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
        DenseVector{std::vector<Real>{0.5, -1.0, 2.0}}
    );
    const DenseVector phi{std::vector<Real>{2.0, 3.0, 4.0}};
    const DenseVector b = source.evaluate(phi);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Linearized source 1D\n";
    std::cout << "====================\n";
    std::cout << "B_P = S_C + S_P phi_P\n\n";

    for (fvm::Size i = 0; i < source.size(); ++i) {
        std::cout << "i = " << i
                  << ", S_C = " << source.source_constant()[i]
                  << ", S_P = " << source.source_linear()[i]
                  << ", phi_P = " << phi[i]
                  << ", B_P = " << b[i]
                  << '\n';
    }

    return b[0] == Real{2.0} && b[1] == Real{-1.0} && b[2] == Real{11.0}
        ? 0
        : 1;
}
