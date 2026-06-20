// ----------------------------------------------------------------------------
// File: tst_IterativeSolvers1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests 1D iterative solvers for tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------
#include <array>

#include <string_view>
#include <vector>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Solver/BiCG.h>
#include <FVMaker/OneDimensional/Solver/BiCGSTAB.h>
#include <FVMaker/OneDimensional/Solver/ConjugateGradient.h>
#include <FVMaker/OneDimensional/Solver/GaussSeidel.h>
#include <FVMaker/OneDimensional/Solver/Jacobi.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] TridiagonalSystem1D make_spd_system() {
    return TridiagonalSystem1D{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };
}

[[nodiscard]] TridiagonalSystem1D make_nonsymmetric_system() {
    return TridiagonalSystem1D{
        std::vector<Real>{-1.0, -2.0},
        std::vector<Real>{4.0, 5.0, 6.0},
        std::vector<Real>{-2.0, -1.0},
        DenseVector{std::vector<Real>{2.0, 4.0, 8.0}}
    };
}

[[nodiscard]] TridiagonalSystem1D make_boundary_system() {
    return TridiagonalSystem1D{
        {-1.0, -1.0, -1.0, -1.0},
        {3.0, 2.0, 2.0, 2.0, 3.0},
        {-1.0, -1.0, -1.0, -1.0},
        DenseVector{std::vector<Real>{0.0, 0.0, 0.0, 0.0, 20.0}}
    };
}

void expect_unit_solution(const SolveResult& result) {
    ASSERT_TRUE(result.converged);
    ASSERT_EQ(result.solution.size(), static_cast<Size>(3));
    EXPECT_NEAR(result.solution[0], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[1], 1.0, 1.0e-8);
    EXPECT_NEAR(result.solution[2], 1.0, 1.0e-8);
    EXPECT_LE(result.residual_norm, 1.0e-8);
}

void expect_nonsymmetric_solution(const SolveResult& result) {
    ASSERT_TRUE(result.converged);
    ASSERT_EQ(result.solution.size(), static_cast<Size>(3));
    EXPECT_NEAR(result.solution[0], 1.2, 1.0e-8);
    EXPECT_NEAR(result.solution[1], 1.4, 1.0e-8);
    EXPECT_NEAR(result.solution[2], 1.8, 1.0e-8);
    EXPECT_LE(result.residual_norm, 1.0e-8);
}

}  // namespace

TEST(IterativeSolvers1D, JacobiStoresClassIdentity) {
    EXPECT_EQ(Jacobi::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Jacobi::id().class_name(), std::string_view{"Jacobi"});
    EXPECT_EQ(Jacobi::id().class_id(), std::string_view{"fvm.1d.solver.Jacobi"});
}

TEST(IterativeSolvers1D, GaussSeidelStoresClassIdentity) {
    EXPECT_EQ(GaussSeidel::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(GaussSeidel::id().class_name(), std::string_view{"GaussSeidel"});
    EXPECT_EQ(
        GaussSeidel::id().class_id(),
        std::string_view{"fvm.1d.solver.GaussSeidel"}
    );
}

TEST(IterativeSolvers1D, ConjugateGradientStoresClassIdentity) {
    EXPECT_EQ(
        ConjugateGradient::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        ConjugateGradient::id().class_name(),
        std::string_view{"ConjugateGradient"}
    );
    EXPECT_EQ(
        ConjugateGradient::id().class_id(),
        std::string_view{"fvm.1d.solver.ConjugateGradient"}
    );
}

TEST(IterativeSolvers1D, BiCGStoresClassIdentity) {
    EXPECT_EQ(BiCG::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(BiCG::id().class_name(), std::string_view{"BiCG"});
    EXPECT_EQ(BiCG::id().class_id(), std::string_view{"fvm.1d.solver.BiCG"});
}

TEST(IterativeSolvers1D, BiCGSTABStoresClassIdentity) {
    EXPECT_EQ(BiCGSTAB::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(BiCGSTAB::id().class_name(), std::string_view{"BiCGSTAB"});
    EXPECT_EQ(
        BiCGSTAB::id().class_id(),
        std::string_view{"fvm.1d.solver.BiCGSTAB"}
    );
}

TEST(IterativeSolvers1D, JacobiSolvesKnownSystem) {
    const SolveResult result = Jacobi::solve(
        make_spd_system(),
        IterativeSolverOptions{.tolerance = 1.0e-10, .max_iterations = 1000}
    );

    expect_unit_solution(result);
}

TEST(IterativeSolvers1D, GaussSeidelSolvesKnownSystem) {
    const SolveResult result = GaussSeidel::solve(
        make_spd_system(),
        IterativeSolverOptions{.tolerance = 1.0e-10, .max_iterations = 1000}
    );

    expect_unit_solution(result);
}

TEST(IterativeSolvers1D, GaussSeidelSupportsBackwardSweep) {
    const SolveResult result = GaussSeidel::solve(
        make_spd_system(),
        IterativeSolverOptions{
            .tolerance = 1.0e-10,
            .max_iterations = 1000,
            .gauss_seidel_sweep = GaussSeidelSweep::backward
        }
    );

    expect_unit_solution(result);
}

TEST(IterativeSolvers1D, GaussSeidelSupportsHybridSweep) {
    const SolveResult result = GaussSeidel::solve(
        make_spd_system(),
        IterativeSolverOptions{
            .tolerance = 1.0e-10,
            .max_iterations = 1000,
            .gauss_seidel_sweep = GaussSeidelSweep::hybrid
        }
    );

    expect_unit_solution(result);
}

TEST(IterativeSolvers1D, BoundaryRowsAreUpdatedByAllSweepDirections) {
    const DenseVector exact{std::vector<Real>{1.0, 3.0, 5.0, 7.0, 9.0}};
    const auto check = [&exact](const SolveResult& result) {
        ASSERT_TRUE(result.converged);
        for (Size i = 0; i < exact.size(); ++i)
            EXPECT_NEAR(result.solution[i], exact[i], 1.0e-8);
    };
    check(Jacobi::solve(make_boundary_system()));
    for (const auto direction : std::array{GaussSeidelSweep::forward,
                                           GaussSeidelSweep::backward,
                                           GaussSeidelSweep::hybrid}) {
        check(GaussSeidel::solve(make_boundary_system(),
            IterativeSolverOptions{.gauss_seidel_sweep = direction}));
    }
}

TEST(IterativeSolvers1D, JacobiUsesCustomStopCriteria) {
    const SolveResult result = Jacobi::solve(
        make_spd_system(),
        IterativeSolverOptions{
            .tolerance = 1.0e-30,
            .max_iterations = 1000,
            .gauss_seidel_sweep = GaussSeidelSweep::hybrid,
            .stop_criteria = StopCriteria::correction_absolute(1.0e-10)
        }
    );

    expect_unit_solution(result);
    EXPECT_EQ(result.stop_criterion, StopCriterionKind::correction_absolute);
}

TEST(IterativeSolvers1D, ConjugateGradientSolvesKnownSPDSystem) {
    const SolveResult result = ConjugateGradient::solve(
        make_spd_system(),
        IterativeSolverOptions{.tolerance = 1.0e-12, .max_iterations = 10}
    );

    expect_unit_solution(result);
    EXPECT_LE(result.iterations, static_cast<Size>(3));
}

TEST(IterativeSolvers1D, BiCGSolvesNonSymmetricSystem) {
    const SolveResult result = BiCG::solve(
        make_nonsymmetric_system(),
        IterativeSolverOptions{.tolerance = 1.0e-12, .max_iterations = 20}
    );

    expect_nonsymmetric_solution(result);
}

TEST(IterativeSolvers1D, BiCGSTABSolvesNonSymmetricSystem) {
    const SolveResult result = BiCGSTAB::solve(
        make_nonsymmetric_system(),
        IterativeSolverOptions{.tolerance = 1.0e-12, .max_iterations = 20}
    );

    expect_nonsymmetric_solution(result);
}

TEST(IterativeSolvers1D, ReportsNonConvergenceWithoutThrowing) {
    const SolveResult result = Jacobi::solve(
        make_spd_system(),
        IterativeSolverOptions{.tolerance = 1.0e-30, .max_iterations = 1}
    );

    EXPECT_FALSE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(1));
    EXPECT_GT(result.residual_norm, 0.0);
}

TEST(IterativeSolvers1D, RejectsInvalidTolerance) {
    try {
        static_cast<void>(
            Jacobi::solve(
                make_spd_system(),
                IterativeSolverOptions{
                    .tolerance = 0.0,
                    .max_iterations = 10
                }
            )
        );
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidTolerance.code);
        EXPECT_EQ(exception.record().category, std::string_view{"Core"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Jacobi"});
        return;
    }

    FAIL() << "Jacobi did not reject an invalid tolerance.";
}


TEST(IterativeSolvers1D, GaussSeidelAcceptsInitialGuess) {
    const TridiagonalSystem1D system = make_spd_system();
    DenseVector initial_guess{3, 1.0};

    const SolveResult result = GaussSeidel::solve(
        system,
        std::move(initial_guess)
    );

    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(0));
    EXPECT_DOUBLE_EQ(result.initial_residual_norm, 0.0);
    expect_unit_solution(result);
}

TEST(IterativeSolvers1D, GaussSeidelRejectsWrongInitialGuessSize) {
    const TridiagonalSystem1D system = make_spd_system();

    EXPECT_THROW(
        static_cast<void>(
            GaussSeidel::solve(system, DenseVector{2})
        ),
        FVMException
    );
}

}  // namespace fvm
