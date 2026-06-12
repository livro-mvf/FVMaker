// ----------------------------------------------------------------------------
// File: tst_AlgebraicResidual1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests A X - B residual evaluation for 1D systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] TridiagonalSystem1D make_known_system() {
    return TridiagonalSystem1D{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };
}

}  // namespace

TEST(AlgebraicResidual1D, ComputesAxMinusBForExactSolution) {
    const TridiagonalSystem1D system = make_known_system();
    const DenseVector solution{std::vector<Real>{1.0, 1.0, 1.0}};

    const DenseVector residual = algebraic_residual(system, solution);

    ASSERT_EQ(residual.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(residual[0], 0.0);
    EXPECT_DOUBLE_EQ(residual[1], 0.0);
    EXPECT_DOUBLE_EQ(residual[2], 0.0);
    EXPECT_DOUBLE_EQ(norm_infinity(residual), 0.0);
}

TEST(AlgebraicResidual1D, ComputesAxMinusBForApproximateSolution) {
    const TridiagonalSystem1D system = make_known_system();
    const DenseVector solution{std::vector<Real>{1.0, 0.5, 1.0}};

    const DenseVector residual = algebraic_residual(system, solution);

    ASSERT_EQ(residual.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(residual[0], 0.5);
    EXPECT_DOUBLE_EQ(residual[1], -1.0);
    EXPECT_DOUBLE_EQ(residual[2], 0.5);
    EXPECT_DOUBLE_EQ(norm_infinity(residual), 1.0);
    EXPECT_DOUBLE_EQ(norm_l1(residual), 2.0);
}

TEST(AlgebraicResidual1D, RejectsSolutionWithWrongSize) {
    const TridiagonalSystem1D system = make_known_system();
    const DenseVector solution{std::vector<Real>{1.0, 1.0}};

    try {
        static_cast<void>(algebraic_residual(system, solution));
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSystemSize.code);
        EXPECT_EQ(exception.record().category, std::string_view{"System"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"TridiagonalSystem1D"}
        );
        return;
    }

    FAIL() << "algebraic_residual did not reject an incompatible solution size.";
}

TEST(AlgebraicResidual1D, ComputesAxMinusBForPentadiagonalSystem) {
    const PentadiagonalSystem1D system{
        std::vector<Real>{-0.5, -0.5, -0.5},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{4.0, 4.0, 4.0, 4.0, 4.0},
        std::vector<Real>{-1.0, -1.0, -1.0, -1.0},
        std::vector<Real>{-0.5, -0.5, -0.5},
        DenseVector{std::vector<Real>{2.5, 1.5, 1.0, 1.5, 2.5}}
    };
    const DenseVector solution{std::vector<Real>{1.0, 1.0, 1.0, 1.0, 1.0}};

    const DenseVector residual = algebraic_residual(system, solution);

    ASSERT_EQ(residual.size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(residual[0], 0.0);
    EXPECT_DOUBLE_EQ(residual[1], 0.0);
    EXPECT_DOUBLE_EQ(residual[2], 0.0);
    EXPECT_DOUBLE_EQ(residual[3], 0.0);
    EXPECT_DOUBLE_EQ(residual[4], 0.0);
    EXPECT_DOUBLE_EQ(norm_infinity(residual), 0.0);
}

TEST(AlgebraicResidual1D, TDMAStoresResidualAndInfinityNormInSolveResult) {
    const TridiagonalSystem1D system = make_known_system();
    const SolveResult result = TDMA::solve(system);

    ASSERT_EQ(result.residual.size(), system.size());
    EXPECT_NEAR(result.residual[0], 0.0, 1.0e-14);
    EXPECT_NEAR(result.residual[1], 0.0, 1.0e-14);
    EXPECT_NEAR(result.residual[2], 0.0, 1.0e-14);
    EXPECT_DOUBLE_EQ(result.residual_norm, norm_infinity(result.residual));
}

}  // namespace fvm
