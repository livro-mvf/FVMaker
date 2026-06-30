// ============================================================================
// Arquivo: tst_DiffusionOperator1D.cc
// Projeto: FVMaker
// Descricao: Testa operadores diferenciais difusivos 1D.
// ============================================================================

#include <cmath>
#include <string_view>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Operator/DiffusionOperator1D.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid(Size cells) {
    static fvgrid::Axis1D axis_8 = fvgrid::uniform_axis_1d(
        fvgrid::NVol{8},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    static fvgrid::Axis1D axis_12 = fvgrid::uniform_axis_1d(
        fvgrid::NVol{12},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    return cells == Size{8} ? GridView1D{axis_8} : GridView1D{axis_12};
}

[[nodiscard]] DenseVector values_at_centers(
    const GridView1D& grid,
    Real (*function)(Real)
) {
    DenseVector values{grid.num_volumes()};

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        values[i] = function(grid.centers()[i]);
    }

    return values;
}

[[nodiscard]] Real square(Real x) {
    return x * x;
}

[[nodiscard]] Real linear(Real x) {
    return x;
}

}  // namespace

TEST(DiffusionOperator1D, StoresClassIdentity) {
    EXPECT_EQ(
        DiffusionOperator1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        DiffusionOperator1D::id().class_name(),
        std::string_view{"DiffusionOperator1D"}
    );
    EXPECT_EQ(
        DiffusionOperator1D::id().class_id(),
        std::string_view{"fvm.1d.operator.DiffusionOperator1D"}
    );
}

TEST(DiffusionOperator1D, ComputesSecondDerivativeWithNeumannBoundaries) {
    const GridView1D grid = make_grid(Size{12});
    const DenseVector phi = values_at_centers(grid, square);
    const BoundarySet1D boundaries{neumann_1d(0.0), neumann_1d(2.0)};

    const DenseVector d2phi = second_derivative_1d(grid, phi, boundaries);

    for (const Real value : d2phi.values()) {
        EXPECT_NEAR(value, 2.0, 1.0e-12);
    }
}

TEST(DiffusionOperator1D, AppliesConstantDiffusionCoefficient) {
    const GridView1D grid = make_grid(Size{12});
    const DenseVector phi = values_at_centers(grid, square);
    const BoundarySet1D boundaries{neumann_1d(0.0), neumann_1d(2.0)};

    const DenseVector result = diffusion_operator_1d(
        grid,
        phi,
        3.0,
        boundaries
    );

    for (const Real value : result.values()) {
        EXPECT_NEAR(value, 6.0, 1.0e-12);
    }
}

TEST(DiffusionOperator1D, AppliesPositionDependentCoefficient) {
    const GridView1D grid = make_grid(Size{12});
    const DenseVector phi = values_at_centers(grid, square);
    const BoundarySet1D boundaries{neumann_1d(0.0), neumann_1d(2.0)};

    const DenseVector result = diffusion_operator_1d(
        grid,
        phi,
        [](Real x) { return Real{1.0} + x; },
        boundaries
    );

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        EXPECT_NEAR(result[i], 2.0 + 4.0 * grid.centers()[i], 1.0e-12);
    }
}

TEST(DiffusionOperator1D, InterpolatesPhiToFacesUsingBoundaryConditions) {
    const GridView1D grid = make_grid(Size{8});
    const DenseVector phi = values_at_centers(grid, linear);
    const BoundarySet1D boundaries{neumann_1d(1.0), neumann_1d(1.0)};

    const DenseVector face_phi = interpolate_phi_to_faces_1d(
        grid,
        phi,
        boundaries
    );

    for (Size face = 0; face < grid.num_faces(); ++face) {
        EXPECT_NEAR(face_phi[face], grid.faces()[face], 1.0e-12);
    }
}

TEST(DiffusionOperator1D, AppliesCoefficientDependingOnPositionAndPhi) {
    const GridView1D grid = make_grid(Size{8});
    const DenseVector phi = values_at_centers(grid, linear);
    const BoundarySet1D boundaries{neumann_1d(1.0), neumann_1d(1.0)};

    const DenseVector result = diffusion_operator_1d(
        grid,
        phi,
        [](Real x, Real phi_value) { return Real{1.0} + x + phi_value; },
        boundaries
    );

    for (const Real value : result.values()) {
        EXPECT_NEAR(value, 2.0, 1.0e-12);
    }
}

TEST(DiffusionOperator1D, EvaluatesDiffusionTerm) {
    const GridView1D grid = make_grid(Size{12});
    const DenseVector phi = values_at_centers(grid, square);
    const BoundarySet1D boundaries{neumann_1d(0.0), neumann_1d(2.0)};
    const DiffusionTerm1D term{3.0};

    const DenseVector result = diffusion_operator_1d(
        grid,
        phi,
        term,
        boundaries
    );

    for (const Real value : result.values()) {
        EXPECT_NEAR(value, 6.0, 1.0e-12);
    }
}

TEST(DiffusionOperator1D, LaplacianAliasMatchesSecondDerivative) {
    const GridView1D grid = make_grid(Size{12});
    const DenseVector phi = values_at_centers(grid, square);
    const BoundarySet1D boundaries{neumann_1d(0.0), neumann_1d(2.0)};

    const DenseVector second_derivative = second_derivative_1d(
        grid,
        phi,
        boundaries
    );
    const DenseVector laplacian = laplacian_1d(grid, phi, boundaries);

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        EXPECT_DOUBLE_EQ(laplacian[i], second_derivative[i]);
    }
}

}  // namespace fvm