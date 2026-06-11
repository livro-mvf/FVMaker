// ----------------------------------------------------------------------------
// File: tst_ID.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the lightweight immutable identity type used by FVMaker
//              classes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ID, DefaultIdentityIsUnknown) {
    constexpr ID id;

    EXPECT_EQ(id.module(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_name(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvm.unknown.Unknown"});
}

TEST(ID, StoresModuleClassNameAndClassId) {
    constexpr ID id{
        "OneDimensional",
        "Field1D",
        "fvm.1d.field.Field1D"
    };

    EXPECT_EQ(id.module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(id.class_name(), std::string_view{"Field1D"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvm.1d.field.Field1D"});
}

TEST(ID, IsCopyConstructible) {
    constexpr ID original{
        "Core",
        "ID",
        "fvm.core.ID"
    };

    constexpr ID copy{original};

    EXPECT_EQ(copy.module(), original.module());
    EXPECT_EQ(copy.class_name(), original.class_name());
    EXPECT_EQ(copy.class_id(), original.class_id());
}

TEST(ID, IsMoveConstructible) {
    ID original{
        "Core",
        "ID",
        "fvm.core.ID"
    };

    const ID moved{std::move(original)};

    EXPECT_EQ(moved.module(), std::string_view{"Core"});
    EXPECT_EQ(moved.class_name(), std::string_view{"ID"});
    EXPECT_EQ(moved.class_id(), std::string_view{"fvm.core.ID"});
}

}  // namespace fvm
