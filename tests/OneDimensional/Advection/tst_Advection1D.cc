// ----------------------------------------------------------------------------
// File: tst_Advection1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests provisional 1D advection helpers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Advection/Advection1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    return GridView1D{axis};
}

}  // namespace

TEST(Advection1D, StoresClassIdentity) {
    EXPECT_EQ(Advection1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Advection1D::id().class_name(), std::string_view{"Advection1D"});
    EXPECT_EQ(
        Advection1D::id().class_id(),
        std::string_view{"fvm.1d.advection.Advection1D"}
    );
}

TEST(Advection1D, ComputesAdvectiveFaceValuesWithChosenScheme) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 3.0, 5.0}};
    const Advection1D advection{uniform_face_flux_1d(grid, 1.0)};

    const DenseVector faces = advective_face_values_1d(
        grid,
        phi,
        advection,
        Upwind1D{}
    );

    EXPECT_DOUBLE_EQ(faces[0], 1.0);
    EXPECT_DOUBLE_EQ(faces[1], 1.0);
    EXPECT_DOUBLE_EQ(faces[2], 3.0);
    EXPECT_DOUBLE_EQ(faces[3], 5.0);
}

}  // namespace fvm
