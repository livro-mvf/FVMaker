// ----------------------------------------------------------------------------
// File: tst_NonlinearSolve1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests preliminary nonlinear 1D solve helpers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Solver/NonlinearSolve1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{6},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    return GridView1D{axis};
}

[[nodiscard]] PicardProblem1D make_zero_boundary_problem() {
    const GridView1D grid = make_grid();

    return PicardProblem1D{
        grid,
        nonlinear_coefficient_1d(
            [](Real phi, Real) {
                return Real{1.0} + phi * phi;
            }
        ),
        zero_source_1d(grid.num_volumes()),
        BoundarySet1D{dirichlet_1d(0.0), dirichlet_1d(0.0)}
    };
}

}  // namespace

TEST(PicardProblem1D, StoresClassIdentity) {
    EXPECT_EQ(PicardProblem1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(PicardProblem1D::id().class_name(), std::string_view{"PicardProblem1D"});
    EXPECT_EQ(
        PicardProblem1D::id().class_id(),
        std::string_view{"fvm.1d.solver.PicardProblem1D"}
    );
}

TEST(PicardSolve1D, ConvergesZeroSourceZeroBoundaryProblem) {
    const GridView1D grid = make_grid();
    const PicardProblem1D problem = make_zero_boundary_problem();

    const PicardResult result = picard_solve_1d(
        problem,
        DenseVector{grid.num_volumes(), 1.0},
        PicardOptions{
            .absolute_tolerance = 1.0e-12,
            .relative_tolerance = 0.0,
            .max_iterations = 10,
            .relaxation_factor = 1.0
        }
    );

    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(2));

    for (const Real value : result.solution.values()) {
        EXPECT_NEAR(value, 0.0, 1.0e-12);
    }
}

TEST(PicardSolve1D, RejectsInvalidInitialSolutionSize) {
    const PicardProblem1D problem = make_zero_boundary_problem();

    EXPECT_THROW(
        static_cast<void>(picard_solve_1d(
            problem,
            DenseVector{std::vector<Real>{1.0, 2.0}}
        )),
        FVMException
    );
}

TEST(NewtonSolve1DFake, ReportsNotImplemented) {
    const GridView1D grid = make_grid();
    const PicardProblem1D problem = make_zero_boundary_problem();

    try {
        static_cast<void>(newton_solve_1d_fake(
            problem,
            DenseVector{grid.num_volumes(), 1.0}
        ));
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kNotImplemented.code);
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"NewtonSolve1D"}
        );
        return;
    }

    FAIL() << "newton_solve_1d_fake did not report NotImplemented.";
}

}  // namespace fvm
