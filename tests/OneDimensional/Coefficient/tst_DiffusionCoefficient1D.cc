// ----------------------------------------------------------------------------
// File: tst_DiffusionCoefficient1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests 1D diffusion coefficients stored on faces.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------
#include <cmath>

#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Custom1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/Coordinates1D.h>
#include <FVGridMaker/OneDimensional/GridPattern1D/VolumeCentered1D.h>

#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>

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

TEST(DiffusionCoefficient1D, StoresClassIdentity) {
    EXPECT_EQ(
        DiffusionCoefficient1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        DiffusionCoefficient1D::id().class_name(),
        std::string_view{"DiffusionCoefficient1D"}
    );
    EXPECT_EQ(
        DiffusionCoefficient1D::id().class_id(),
        std::string_view{"fvm.1d.coefficient.DiffusionCoefficient1D"}
    );
}

TEST(DiffusionCoefficient1D, CreatesUniformFaceValues) {
    const GridView1D grid = make_grid();
    const DiffusionCoefficient1D coefficient = uniform_coefficient_1d(grid, 2.5);

    ASSERT_EQ(coefficient.num_faces(), grid.num_faces());

    for (const Real value : coefficient.face_values()) {
        EXPECT_DOUBLE_EQ(value, 2.5);
    }
}

TEST(DiffusionCoefficient1D, EvaluatesFunctionOnFaces) {
    const GridView1D grid = make_grid();
    const DiffusionCoefficient1D coefficient = function_coefficient_1d(
        grid,
        [](Real x) { return Real{1.0} + x; }
    );

    ASSERT_EQ(coefficient.num_faces(), grid.num_faces());
    EXPECT_DOUBLE_EQ(coefficient.face_value(0), 1.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(1), 4.0 / 3.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(2), 5.0 / 3.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(3), 2.0);
}

TEST(DiffusionCoefficient1D, CreatesArithmeticFaceInterpolationFromCellValues) {
    const GridView1D grid = make_grid();
    const DenseVector cell_values{std::vector<Real>{1.0, 3.0, 5.0}};
    const DiffusionCoefficient1D coefficient = field_coefficient_1d(
        grid,
        cell_values
    );

    EXPECT_DOUBLE_EQ(coefficient.face_value(0), 1.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(1), 2.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(2), 4.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(3), 5.0);
}

TEST(DiffusionCoefficient1D, CreatesHarmonicFaceInterpolationFromCellValues) {
    const GridView1D grid = make_grid();
    const DenseVector cell_values{std::vector<Real>{1.0, 3.0, 6.0}};
    const DiffusionCoefficient1D coefficient = harmonic_field_coefficient_1d(
        grid,
        cell_values
    );

    EXPECT_DOUBLE_EQ(coefficient.face_value(0), 1.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(1), 1.5);
    EXPECT_DOUBLE_EQ(coefficient.face_value(2), 4.0);
    EXPECT_DOUBLE_EQ(coefficient.face_value(3), 6.0);
}

TEST(DiffusionCoefficient1D, WeightsInterpolationByFacePosition) {
    const fvgrid::Axis1D axis = fvgrid::Custom1D::make(
        fvgrid::Coordinates1D::faces({0.0, 0.2, 1.0}),
        fvgrid::VolumeCentered1D{}
    );
    const GridView1D grid{axis};
    const DenseVector cells{std::vector<Real>{1.0, 10.0}};

    const auto arithmetic = arithmetic_field_coefficient_1d(grid, cells);
    const auto harmonic = harmonic_field_coefficient_1d(grid, cells);
    EXPECT_NEAR(arithmetic.face_value(1), 2.8, 1.0e-14);
    EXPECT_NEAR(harmonic.face_value(1), 25.0 / 7.0, 1.0e-14);
}

TEST(DiffusionCoefficient1D, AcceptsUserDefinedInterpolationPolicy) {
    const GridView1D grid = make_grid();
    const DenseVector cells{std::vector<Real>{1.0, 4.0, 9.0}};
    const auto geometric = interpolated_field_coefficient_1d(
        grid, cells, [](FaceInterpolationData1D data) {
            return std::sqrt(data.west_value * data.east_value);
        }
    );
    EXPECT_DOUBLE_EQ(geometric.face_value(1), 2.0);
    EXPECT_DOUBLE_EQ(geometric.face_value(2), 6.0);
}

}  // namespace fvm
