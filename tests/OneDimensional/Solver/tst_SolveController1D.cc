// ----------------------------------------------------------------------------
// File: tst_SolveController1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests steady 1D solve controller functions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>
#include <FVMaker/OneDimensional/Solver/SolveController1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

constexpr Real pi = 3.141592653589793238462643383279502884;

[[nodiscard]] TridiagonalSystem1D make_spd_system() {
    return TridiagonalSystem1D{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };
}

[[nodiscard]] Real phi_exact(Real x) {
    return std::sin(pi * x);
}

[[nodiscard]] Real source(Real x) {
    return -pi * pi * std::sin(pi * x);
}

}  // namespace

TEST(SolveController1D, ComputesRhsNormAndRelativeResidual) {
    const TridiagonalSystem1D system = make_spd_system();

    EXPECT_DOUBLE_EQ(rhs_norm_infinity(system), 1.0);
    EXPECT_DOUBLE_EQ(relative_residual_norm(0.25, 2.0), 0.125);
    EXPECT_DOUBLE_EQ(relative_residual_norm(0.25, 0.0), 0.25);
}

TEST(SolveController1D, SolvesKnownSystemWithTDMA) {
    const SolveResult result = solve_steady_system_1d<TDMA>(
        make_spd_system(),
        SteadyState{
            .absolute_tolerance = 1.0e-12,
            .relative_tolerance = 1.0e-12,
            .max_iterations = 100
        }
    );

    ASSERT_TRUE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(result.solution[0], 1.0);
    EXPECT_DOUBLE_EQ(result.solution[1], 1.0);
    EXPECT_DOUBLE_EQ(result.solution[2], 1.0);
    EXPECT_LE(result.residual_norm, 1.0e-12);
    EXPECT_LE(result.relative_residual_norm, 1.0e-12);
    EXPECT_TRUE(result.reached_absolute_tolerance);
}

TEST(SolveController1D, SolvesKnownSystemWithIterativeSolver) {
    const SolveResult result = solve_steady_system_1d<GaussSeidel>(
        make_spd_system(),
        SteadyState{
            .absolute_tolerance = 1.0e-10,
            .relative_tolerance = 1.0e-10,
            .max_iterations = 1000
        }
    );

    ASSERT_TRUE(result.converged);
    EXPECT_NEAR(result.solution[0], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[1], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[2], 1.0, 1.0e-8);
    EXPECT_LE(result.iterations, static_cast<Size>(1000));
    EXPECT_DOUBLE_EQ(result.requested_tolerance, 1.0e-10);
}

TEST(SolveController1D, SolvesKnownSystemWithRuntimeSolverSelection) {
    const SolveResult result = solve_steady_system_1d(
        make_spd_system(),
        LinearSolverOptions1D{
            .solver = LinearSolverKind1D::bicgstab,
            .absolute_tolerance = 1.0e-12,
            .relative_tolerance = 1.0e-12,
            .max_iterations = 100
        }
    );

    ASSERT_TRUE(result.converged);
    EXPECT_NEAR(result.solution[0], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[1], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[2], 1.0, 1.0e-8);
    EXPECT_EQ(name(LinearSolverKind1D::bicgstab), std::string_view{"BiCGSTAB"});
}

TEST(SolveController1D, ReportsNonConvergenceFromIterativeSolver) {
    const SolveResult result = solve_steady_system_1d<GaussSeidel>(
        make_spd_system(),
        SteadyState{
            .absolute_tolerance = 1.0e-30,
            .relative_tolerance = 0.0,
            .max_iterations = 1
        }
    );

    EXPECT_FALSE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(1));
    EXPECT_GT(result.residual_norm, 0.0);
}

TEST(SolveController1D, SolvesSteadyEquationWithTDMA) {
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

    const SolveResult result = solve_steady_1d(equation);

    DenseVector error{grid.num_volumes()};

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        error[i] = result.solution[i] - phi_exact(grid.centers()[i]);
    }

    EXPECT_TRUE(result.converged);
    EXPECT_LT(norm_infinity(error), 6.0e-4);
    EXPECT_LT(result.residual_norm, 1.0e-10);
}

}  // namespace fvm
