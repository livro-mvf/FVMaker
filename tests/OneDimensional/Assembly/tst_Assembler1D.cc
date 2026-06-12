// ----------------------------------------------------------------------------
// File: tst_Assembler1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests assembly of the first steady 1D Poisson problem.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

constexpr Real pi = 3.141592653589793238462643383279502884;

[[nodiscard]] Real phi_exact(Real x) {
    return std::sin(pi * x);
}

[[nodiscard]] Real source(Real x) {
    return -pi * pi * std::sin(pi * x);
}

}  // namespace

TEST(Assembler1D, AppliesLinearizedSourceToLaplacianSystem) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const Equation1D equation{
        grid,
        Laplacian1D{},
        vector_source_1d(
            DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
            DenseVector{std::vector<Real>{0.5, -1.0, 2.0}}
        ),
        BoundarySet1D{dirichlet_1d(0.0), dirichlet_1d(0.0)}
    };

    const TridiagonalSystem1D system = assemble_steady_1d(equation);

    EXPECT_DOUBLE_EQ(system.rhs()[0], 1.0);
    EXPECT_DOUBLE_EQ(system.rhs()[1], 2.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 3.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[0], -27.5);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], -17.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[2], -29.0);
}

TEST(Assembler1D, SolvesPoissonProblemWithManufacturedSolution) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{40},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const Equation1D equation{
        grid,
        Laplacian1D{},
        function_source_1d(grid, source),
        BoundarySet1D{dirichlet_1d(0.0), dirichlet_1d(0.0)}
    };

    const TridiagonalSystem1D system = assemble_steady_1d(equation);
    const SolveResult result = TDMA::solve(system);

    DenseVector error{grid.num_volumes()};

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        error[i] = result.solution[i] - phi_exact(grid.centers()[i]);
    }

    EXPECT_TRUE(result.converged);
    EXPECT_LT(norm_infinity(error), 6.0e-4);
    EXPECT_LT(result.residual_norm, 1.0e-10);
}

}  // namespace fvm
