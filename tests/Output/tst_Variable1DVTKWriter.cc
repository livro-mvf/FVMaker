// ----------------------------------------------------------------------------
// File: tst_Variable1DVTKWriter.cc
// Project: FVMaker
// Description: Tests legacy VTK export for Variable1D.
// ----------------------------------------------------------------------------

#include <sstream>
#include <string>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>
#include <FVMaker/Output/VTK/Variable1DVTKWriter.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(Variable1DVTKWriter, WritesLegacyRectilinearGridWithVariableStates) {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    Variable1D variable{grid, "phi", std::vector<Real>{1.0, 2.0, 3.0}};
    variable.assign_analytic([](Real) {
        return 1.0;
    });

    std::ostringstream output;
    Variable1DVTKWriter::write(variable, output);

    const std::string text = output.str();
    EXPECT_NE(text.find("DATASET RECTILINEAR_GRID"), std::string::npos);
    EXPECT_NE(text.find("DIMENSIONS 4 2 2"), std::string::npos);
    EXPECT_NE(text.find("CELL_DATA 3"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_ant double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_iter double 1"), std::string::npos);
    EXPECT_NE(text.find("SCALARS phi_analit double 1"), std::string::npos);
}

}  // namespace fvm
