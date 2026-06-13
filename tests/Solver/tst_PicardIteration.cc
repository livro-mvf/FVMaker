// ----------------------------------------------------------------------------
// File: tst_PicardIteration.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the generic Picard fixed-point iteration helper.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/Solver/PicardIteration.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(PicardOptions, StoresClassIdentity) {
    EXPECT_EQ(PicardOptions::id().module(), std::string_view{"Solver"});
    EXPECT_EQ(PicardOptions::id().class_name(), std::string_view{"PicardOptions"});
    EXPECT_EQ(
        PicardOptions::id().class_id(),
        std::string_view{"fvm.solver.PicardOptions"}
    );
}

TEST(PicardOptions, RejectsInvalidRelaxationFactor) {
    const PicardOptions options{
        .absolute_tolerance = 1.0e-10,
        .relative_tolerance = 1.0e-8,
        .max_iterations = 10,
        .relaxation_factor = 0.0
    };

    EXPECT_THROW(options.validate(), FVMException);
}

TEST(PicardIteration, RelaxesCandidateSolution) {
    const DenseVector previous{std::vector<Real>{0.0, 2.0}};
    const DenseVector candidate{std::vector<Real>{2.0, 4.0}};

    const DenseVector relaxed = relax_picard_update(previous, candidate, 0.25);

    ASSERT_EQ(relaxed.size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(relaxed[0], 0.5);
    EXPECT_DOUBLE_EQ(relaxed[1], 2.5);
}

TEST(PicardIteration, ConvergesSimpleFixedPointProblem) {
    const PicardResult result = picard_iteration(
        DenseVector{std::vector<Real>{0.0}},
        [](const DenseVector& phi) {
            return DenseVector{std::vector<Real>{
                0.5 * phi[0] + 1.0
            }};
        },
        PicardOptions{
            .absolute_tolerance = 1.0e-8,
            .relative_tolerance = 0.0,
            .max_iterations = 100,
            .relaxation_factor = 1.0
        }
    );

    EXPECT_TRUE(result.converged);
    EXPECT_LT(result.iterations, static_cast<Size>(40));
    EXPECT_NEAR(result.solution[0], 2.0, 1.0e-7);
    EXPECT_LE(result.correction_norm, 1.0e-8);
}

TEST(PicardIteration, ReportsNonConvergence) {
    const PicardResult result = picard_iteration(
        DenseVector{std::vector<Real>{0.0}},
        [](const DenseVector& phi) {
            return DenseVector{std::vector<Real>{phi[0] + 1.0}};
        },
        PicardOptions{
            .absolute_tolerance = 1.0e-12,
            .relative_tolerance = 0.0,
            .max_iterations = 3,
            .relaxation_factor = 1.0
        }
    );

    EXPECT_FALSE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(result.solution[0], 3.0);
}

TEST(PicardIteration, RejectsMismatchedUpdateSize) {
    EXPECT_THROW(
        static_cast<void>(picard_iteration(
            DenseVector{std::vector<Real>{0.0}},
            [](const DenseVector&) {
                return DenseVector{std::vector<Real>{1.0, 2.0}};
            }
        )),
        FVMException
    );
}

}  // namespace fvm
