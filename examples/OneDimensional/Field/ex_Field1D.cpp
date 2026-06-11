// ----------------------------------------------------------------------------
// File: ex_Field1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates creating a 1D field on an FVGridMaker axis.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/OneDimensional/Field/Field1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvm::GridView1D grid{axis};
    const fvm::Field1D phi = fvm::Field1D::from_function(
        grid,
        "phi",
        [](fvm::Real x) {
            return x * x;
        }
    );

    std::cout << "FVM Field1D over FVGridMaker Axis1D\n";
    std::cout << "name    : " << phi.name() << '\n';
    std::cout << "volumes : " << phi.size() << '\n';

    for (fvm::Size i = 0; i < phi.size(); ++i) {
        std::cout << "phi[" << i << "] = " << phi.values()[i] << '\n';
    }

    return 0;
}
