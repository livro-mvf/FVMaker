// ----------------------------------------------------------------------------
// File: tst_ErrorRecord.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the structured error record used by FVMaker.
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
#include <FVMaker/ErrorHandling/ErrorRecord.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorRecord, StoresCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVM.TEST.ERROR",
        .message = "test error message",
        .category = "Test",
        .source =
            ID{"ErrorHandling", "ErrorRecordTest", "fvm.test.ErrorRecordTest"},
    };

    EXPECT_EQ(record.code, std::string_view{"FVM.TEST.ERROR"});
    EXPECT_EQ(record.message, "test error message");
    EXPECT_EQ(record.category, std::string_view{"Test"});
    EXPECT_EQ(record.source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(record.source.class_name(), std::string_view{"ErrorRecordTest"});
    EXPECT_EQ(
        record.source.class_id(),
        std::string_view{"fvm.test.ErrorRecordTest"}
    );
}

TEST(ErrorRecord, StoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVM.TEST.LOCATION",
        .message = "location test",
        .category = "Test",
        .source =
            ID{"ErrorHandling", "ErrorRecordTest", "fvm.test.ErrorRecordTest"},
        .location = location,
    };

    EXPECT_EQ(record.location.line(), location.line());
    EXPECT_STREQ(record.location.file_name(), location.file_name());
}

}  // namespace fvm
