// ----------------------------------------------------------------------------
// File: tst_Equation1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the minimal steady 1D equation container.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(Equation1D, StoresClassIdentity) {
    EXPECT_EQ(Equation1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Equation1D::id().class_name(), std::string_view{"Equation1D"});
    EXPECT_EQ(
        Equation1D::id().class_id(),
        std::string_view{"fvm.1d.equation.Equation1D"}
    );
}

TEST(Equation1D, StoresEquationParts) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    const Equation1D equation{
        grid,
        Laplacian1D{2.0},
        uniform_source_1d(grid.num_volumes(), 1.0),
        BoundarySet1D{dirichlet_1d(0.0), dirichlet_1d(1.0)}
    };

    EXPECT_EQ(equation.grid().num_volumes(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(equation.laplacian().coefficient(), 2.0);
    EXPECT_EQ(equation.source().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(equation.boundaries().right().gamma(1.0), 1.0);
}

TEST(Equation1D, RejectsSourceWithWrongSize) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    try {
        const Equation1D equation{
            grid,
            Laplacian1D{},
            uniform_source_1d(2, 1.0),
            BoundarySet1D{dirichlet_1d(0.0), dirichlet_1d(0.0)}
        };
        (void)equation;
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kIncompatibleTerms.code);
        EXPECT_EQ(exception.record().category, std::string_view{"Term"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"Equation1D"}
        );
        return;
    }

    FAIL() << "Equation1D did not reject an incompatible source size.";
}

}  // namespace fvm
