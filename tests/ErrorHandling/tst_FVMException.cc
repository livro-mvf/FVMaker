// ----------------------------------------------------------------------------
// File: tst_FVMException.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the base exception type used by FVMaker.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/ErrorHandling/FVMException.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(FVMException, StoresErrorRecord) {
    const ErrorRecord record{
        .code = "FVM.TEST.EXCEPTION",
        .message = "exception test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
    };

    const FVMException exception{record};

    EXPECT_EQ(exception.record().code, std::string_view{"FVM.TEST.EXCEPTION"});
    EXPECT_EQ(exception.record().message, "exception test message");
    EXPECT_EQ(exception.record().category, std::string_view{"Test"});
    EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(
        exception.record().source.class_name(),
        std::string_view{"FVMExceptionTest"}
    );
    EXPECT_EQ(
        exception.record().source.class_id(),
        std::string_view{"fvm.test.FVMExceptionTest"}
    );
}

TEST(FVMException, WhatContainsCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVM.TEST.WHAT",
        .message = "what test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
    };

    const FVMException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find("FVM.TEST.WHAT"), std::string::npos);
    EXPECT_NE(what.find("what test message"), std::string::npos);
    EXPECT_NE(what.find("Test"), std::string::npos);
    EXPECT_NE(what.find("ErrorHandling"), std::string::npos);
    EXPECT_NE(what.find("FVMExceptionTest"), std::string::npos);
    EXPECT_NE(what.find("fvm.test.FVMExceptionTest"), std::string::npos);
}

TEST(FVMException, WhatContainsSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVM.TEST.LOCATION",
        .message = "location test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
        .location = location,
    };

    const FVMException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find(location.file_name()), std::string::npos);
}

}  // namespace fvm
