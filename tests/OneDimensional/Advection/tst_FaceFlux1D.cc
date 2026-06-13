// ----------------------------------------------------------------------------
// File: tst_FaceFlux1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests provisional 1D face flux helpers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>

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

TEST(FaceFlux1D, StoresClassIdentity) {
    EXPECT_EQ(FaceFlux1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(FaceFlux1D::id().class_name(), std::string_view{"FaceFlux1D"});
    EXPECT_EQ(
        FaceFlux1D::id().class_id(),
        std::string_view{"fvm.1d.advection.FaceFlux1D"}
    );
}

TEST(FaceFlux1D, BuildsUniformFluxOnFaces) {
    const GridView1D grid = make_grid();
    const FaceFlux1D flux = uniform_face_flux_1d(grid, 2.0);

    ASSERT_EQ(flux.size(), grid.num_faces());
    EXPECT_DOUBLE_EQ(flux.value(0), 2.0);
    EXPECT_DOUBLE_EQ(flux.value(3), 2.0);
}

TEST(FaceFlux1D, RejectsWrongVectorSize) {
    const GridView1D grid = make_grid();

    EXPECT_THROW(
        static_cast<void>(vector_face_flux_1d(
            grid,
            DenseVector{std::vector<Real>{1.0, 2.0}}
        )),
        FVMException
    );
}

TEST(FaceFlux1D, ComputesFacePecletNumber) {
    EXPECT_DOUBLE_EQ(face_peclet_1d(4.0, 2.0), 2.0);
    EXPECT_THROW(static_cast<void>(face_peclet_1d(4.0, 0.0)), FVMException);
}

}  // namespace fvm
