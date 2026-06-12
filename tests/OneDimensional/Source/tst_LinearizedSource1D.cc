// ----------------------------------------------------------------------------
// File: tst_LinearizedSource1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests 1D linearized source terms.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(LinearizedSource1D, StoresClassIdentity) {
    EXPECT_EQ(
        LinearizedSource1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        LinearizedSource1D::id().class_name(),
        std::string_view{"LinearizedSource1D"}
    );
    EXPECT_EQ(
        LinearizedSource1D::id().class_id(),
        std::string_view{"fvm.1d.source.LinearizedSource1D"}
    );
}

TEST(LinearizedSource1D, BuildsZeroSource) {
    const LinearizedSource1D source = zero_source_1d(3);

    EXPECT_EQ(source.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(source.source_constant()[0], 0.0);
    EXPECT_DOUBLE_EQ(source.source_linear()[2], 0.0);
}

TEST(LinearizedSource1D, BuildsUniformSourceWithOptionalSP) {
    const LinearizedSource1D explicit_source = uniform_source_1d(3, 2.0);
    const LinearizedSource1D linearized_source = uniform_source_1d(3, 2.0, -0.5);

    EXPECT_DOUBLE_EQ(explicit_source.source_constant()[1], 2.0);
    EXPECT_DOUBLE_EQ(explicit_source.source_linear()[1], 0.0);
    EXPECT_DOUBLE_EQ(linearized_source.source_constant()[1], 2.0);
    EXPECT_DOUBLE_EQ(linearized_source.source_linear()[1], -0.5);
}

TEST(LinearizedSource1D, BuildsFromVectors) {
    const LinearizedSource1D source = vector_source_1d(
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
        DenseVector{std::vector<Real>{0.1, 0.2, 0.3}}
    );

    EXPECT_EQ(source.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(source.source_constant()[2], 3.0);
    EXPECT_DOUBLE_EQ(source.source_linear()[2], 0.3);
}

TEST(LinearizedSource1D, BuildsFromFunctionsAtGridCenters) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{2},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    const LinearizedSource1D source = function_source_1d(
        grid,
        [](Real x) {
            return x;
        },
        [](Real x) {
            return Real{2.0} * x;
        }
    );

    EXPECT_DOUBLE_EQ(source.source_constant()[0], 0.25);
    EXPECT_DOUBLE_EQ(source.source_constant()[1], 0.75);
    EXPECT_DOUBLE_EQ(source.source_linear()[0], 0.5);
    EXPECT_DOUBLE_EQ(source.source_linear()[1], 1.5);
}

TEST(LinearizedSource1D, BuildsFromTimeDependentFunctionsAtGridCenters) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{2},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    const LinearizedSource1D source = function_source_1d(
        grid,
        [](Real x, Real time) {
            return x + time;
        },
        [](Real x, Real time) {
            return x - time;
        },
        Real{2.0}
    );

    EXPECT_DOUBLE_EQ(source.source_constant()[0], 2.25);
    EXPECT_DOUBLE_EQ(source.source_constant()[1], 2.75);
    EXPECT_DOUBLE_EQ(source.source_linear()[0], -1.75);
    EXPECT_DOUBLE_EQ(source.source_linear()[1], -1.25);
}

TEST(LinearizedSource1D, EvaluatesBPAsSCPlusSPPhiP) {
    const LinearizedSource1D source = vector_source_1d(
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
        DenseVector{std::vector<Real>{0.5, -1.0, 2.0}}
    );
    const DenseVector phi{std::vector<Real>{2.0, 3.0, 4.0}};

    const DenseVector result = source.evaluate(phi);

    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], -1.0);
    EXPECT_DOUBLE_EQ(result[2], 11.0);
}

TEST(LinearizedSource1D, AppliesContributionToTridiagonalSystem) {
    TridiagonalSystem1D system{3};
    system.set_row(0, 0.0, 10.0, -1.0, 0.0);
    system.set_row(1, -1.0, 10.0, -1.0, 0.0);
    system.set_row(2, -1.0, 10.0, 0.0, 0.0);

    const LinearizedSource1D source = vector_source_1d(
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
        DenseVector{std::vector<Real>{0.5, -1.0, 2.0}}
    );

    apply_source_to_system(system, source);

    EXPECT_DOUBLE_EQ(system.rhs()[0], 1.0);
    EXPECT_DOUBLE_EQ(system.rhs()[1], 2.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 3.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[0], 9.5);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], 11.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[2], 8.0);
}

TEST(LinearizedSource1D, RejectsIncompatibleVectors) {
    try {
        const LinearizedSource1D source{
            DenseVector{std::vector<Real>{1.0, 2.0}},
            DenseVector{std::vector<Real>{0.0}}
        };
        (void)source;
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSource.code);
        EXPECT_EQ(exception.record().category, std::string_view{"Source"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"LinearizedSource1D"}
        );
        return;
    }

    FAIL() << "LinearizedSource1D did not reject incompatible vectors.";
}

}  // namespace fvm
