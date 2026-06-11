// ----------------------------------------------------------------------------
// File: tst_StrongTypes.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the strongly typed wrappers used by the FVMaker core.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <type_traits>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/StrongTypes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(StrongReal, StoresRealValue) {
    const StrongReal<Real, ToleranceTag> value{1.25};

    EXPECT_DOUBLE_EQ(value.value(), 1.25);
}

TEST(StrongInteger, StoresIntegerValue) {
    const StrongInteger<Size, MaxIterationsTag> value{42};

    EXPECT_EQ(value.value(), static_cast<Size>(42));
}

TEST(StrongTypes, ToleranceUsesReal) {
    const Tolerance tolerance{1.0e-10};

    static_assert(std::is_same_v<Tolerance::value_type, Real>);
    static_assert(std::is_same_v<Tolerance::tag_type, ToleranceTag>);
    EXPECT_DOUBLE_EQ(tolerance.value(), 1.0e-10);
}

TEST(StrongTypes, MaxIterationsUsesSize) {
    const MaxIterations max_iterations{100};

    static_assert(std::is_same_v<MaxIterations::value_type, Size>);
    static_assert(std::is_same_v<MaxIterations::tag_type, MaxIterationsTag>);
    EXPECT_EQ(max_iterations.value(), static_cast<Size>(100));
}

TEST(StrongTypes, TimeTypesUseReal) {
    const Time time{0.5};
    const TimeStep time_step{0.01};
    const InitialTime initial_time{0.0};
    const FinalTime final_time{1.0};

    static_assert(std::is_same_v<Time::value_type, Real>);
    static_assert(std::is_same_v<TimeStep::value_type, Real>);
    static_assert(std::is_same_v<InitialTime::value_type, Real>);
    static_assert(std::is_same_v<FinalTime::value_type, Real>);

    EXPECT_DOUBLE_EQ(time.value(), 0.5);
    EXPECT_DOUBLE_EQ(time_step.value(), 0.01);
    EXPECT_DOUBLE_EQ(initial_time.value(), 0.0);
    EXPECT_DOUBLE_EQ(final_time.value(), 1.0);
}

TEST(StrongTypes, RelaxationFactorUsesReal) {
    const RelaxationFactor relaxation_factor{0.7};

    static_assert(std::is_same_v<RelaxationFactor::value_type, Real>);
    static_assert(std::is_same_v<RelaxationFactor::tag_type, RelaxationFactorTag>);
    EXPECT_DOUBLE_EQ(relaxation_factor.value(), 0.7);
}

TEST(StrongTypes, DifferentStrongTypesAreDifferentTypes) {
    static_assert(!std::is_same_v<Tolerance, TimeStep>);
    static_assert(!std::is_same_v<Time, InitialTime>);
    static_assert(!std::is_same_v<InitialTime, FinalTime>);
    static_assert(!std::is_same_v<MaxIterations, StrongInteger<Size, TimeStepTag>>);

    SUCCEED();
}

}  // namespace fvm
