// ----------------------------------------------------------------------------
// File: ex_Advection1D.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Compares provisional 1D face interpolation schemes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iomanip>
#include <iostream>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Advection/Advection1D.h>

namespace {

void print_faces(
    const char* name,
    const fvm::GridView1D& grid,
    const fvm::DenseVector& values
) {
    std::cout << name << '\n';

    for (fvm::Size face = 0; face < grid.num_faces(); ++face) {
        std::cout << "  face " << std::setw(2) << face
                  << " x = " << std::setw(12) << grid.faces()[face]
                  << " phi_f = " << std::setw(12) << values[face] << '\n';
    }
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::DenseVector phi{std::vector<fvm::Real>{1.0, 2.0, 4.0, 8.0}};
    const fvm::Advection1D advection{fvm::uniform_face_flux_1d(grid, 1.0)};

    const fvm::DenseVector central = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::CentralDifference1D{}
    );
    const fvm::DenseVector upwind = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::Upwind1D{}
    );
    const fvm::DenseVector power_law = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::PowerLaw1D{}
    );
    const fvm::DenseVector exponential = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::Exponential1D{}
    );
    const fvm::DenseVector quick = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::QUICK1D{}
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Preliminary 1D advection interpolation example\n";
    std::cout << "==============================================\n";
    std::cout << "Status: provisional fake example for face-value plumbing only\n\n";

    print_faces("CentralDifference1D", grid, central);
    print_faces("Upwind1D", grid, upwind);
    print_faces("PowerLaw1D fake", grid, power_law);
    print_faces("Exponential1D fake", grid, exponential);
    print_faces("QUICK1D fake", grid, quick);

    return 0;
}
