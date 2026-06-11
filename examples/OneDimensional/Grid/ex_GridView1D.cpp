// ----------------------------------------------------------------------------
// File: ex_GridView1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates consuming a one-dimensional FVGridMaker axis.
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
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvm::GridView1D grid{axis};
    const fvm::GridMetrics1D metrics = grid.metrics();

    std::cout << "FVM GridView1D over FVGridMaker Axis1D\n";
    std::cout << "volumes : " << metrics.num_volumes << '\n';
    std::cout << "faces   : " << metrics.num_faces << '\n';
    std::cout << "xmin    : " << metrics.xmin << '\n';
    std::cout << "xmax    : " << metrics.xmax << '\n';
    std::cout << "length  : " << metrics.length << '\n';
    std::cout << "dx[0]   : " << metrics.cell_lengths.front() << '\n';

    return 0;
}
