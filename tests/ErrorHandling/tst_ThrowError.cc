// ----------------------------------------------------------------------------
// File: tst_ThrowError.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests helper functions for throwing FVMaker exceptions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] constexpr ID test_id() noexcept {
    return ID{
        "ErrorHandling",
        "ThrowErrorTest",
        "fvm.test.ThrowErrorTest"
    };
}

}  // namespace

TEST(ThrowError, ThrowErrorThrowsFVMException) {
    EXPECT_THROW(
        throw_error("FVM.TEST.THROW", "throw test message", "Test", test_id()),
        FVMException
    );
}

TEST(ThrowError, ThrowErrorAcceptsDescriptor) {
    EXPECT_THROW(
        throw_error(error_catalog::kInvalidArgument, test_id()),
        FVMException
    );
}

TEST(ThrowError, ThrowErrorStoresRecordFields) {
    try {
        throw_error("FVM.TEST.RECORD", "record test message", "Test", test_id());
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, std::string_view{"FVM.TEST.RECORD"});
        EXPECT_EQ(exception.record().message, "record test message");
        EXPECT_EQ(exception.record().category, std::string_view{"Test"});
        EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"ThrowErrorTest"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvm.test.ThrowErrorTest"}
        );
        return;
    }

    FAIL() << "throw_error did not throw FVMException.";
}

TEST(ThrowError, ThrowErrorStoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    try {
        throw_error(
            "FVM.TEST.LOCATION",
            "location test message",
            "Test",
            test_id(),
            location
        );
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().location.line(), location.line());
        EXPECT_STREQ(exception.record().location.file_name(), location.file_name());
        return;
    }

    FAIL() << "throw_error did not throw FVMException.";
}

TEST(Require, DoesNothingWhenConditionIsTrue) {
    EXPECT_NO_THROW(
        require(true, "FVM.TEST.REQUIRE", "require test message", "Test", test_id())
    );
}

TEST(Require, ThrowsWhenConditionIsFalse) {
    EXPECT_THROW(
        require(false, "FVM.TEST.REQUIRE", "require test message", "Test", test_id()),
        FVMException
    );
}

TEST(Require, AcceptsDescriptor) {
    EXPECT_THROW(
        require(false, error_catalog::kMissingMesh, test_id()),
        FVMException
    );
}

TEST(Require, StoresRecordFieldsWhenConditionIsFalse) {
    try {
        require(
            false,
            "FVM.TEST.REQUIRE.RECORD",
            "require record message",
            "Test",
            test_id()
        );
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            std::string_view{"FVM.TEST.REQUIRE.RECORD"}
        );
        EXPECT_EQ(exception.record().message, "require record message");
        EXPECT_EQ(exception.record().category, std::string_view{"Test"});
        EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"ThrowErrorTest"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvm.test.ThrowErrorTest"}
        );
        return;
    }

    FAIL() << "require did not throw FVMException.";
}

}  // namespace fvm
