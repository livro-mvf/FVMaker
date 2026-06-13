// ----------------------------------------------------------------------------
// File: tst_SteadyState.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests steady-state solve control options.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/Solver/SteadyState.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(SteadyState, StoresClassIdentity) {
    EXPECT_EQ(SteadyState::id().module(), std::string_view{"Solver"});
    EXPECT_EQ(SteadyState::id().class_name(), std::string_view{"SteadyState"});
    EXPECT_EQ(
        SteadyState::id().class_id(),
        std::string_view{"fvm.solver.SteadyState"}
    );
}

TEST(SteadyState, ComputesEffectiveTolerance) {
    const SteadyState control{
        .absolute_tolerance = 1.0e-10,
        .relative_tolerance = 1.0e-4,
        .max_iterations = 100
    };

    EXPECT_DOUBLE_EQ(control.effective_tolerance(1000.0), 0.1);
    EXPECT_DOUBLE_EQ(control.effective_tolerance(1.0e-12), 1.0e-10);
}

TEST(SteadyState, RejectsInvalidAbsoluteTolerance) {
    const SteadyState control{
        .absolute_tolerance = 0.0,
        .relative_tolerance = 1.0e-4,
        .max_iterations = 100
    };

    try {
        control.validate();
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidTolerance.code);
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"SteadyState"});
        return;
    }

    FAIL() << "SteadyState did not reject an invalid absolute tolerance.";
}

TEST(SteadyState, RejectsInvalidMaxIterations) {
    const SteadyState control{
        .absolute_tolerance = 1.0e-10,
        .relative_tolerance = 1.0e-4,
        .max_iterations = 0
    };

    EXPECT_THROW(control.validate(), FVMException);
}

}  // namespace fvm
