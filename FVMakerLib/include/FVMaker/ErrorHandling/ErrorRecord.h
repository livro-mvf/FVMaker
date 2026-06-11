// ----------------------------------------------------------------------------
// File: ErrorRecord.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the structured error record used by FVMaker.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

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

namespace fvm {

struct ErrorRecord final {
    std::string_view code;
    std::string message;
    std::string_view category;
    ID source;
    std::source_location location = std::source_location::current();
};

}  // namespace fvm
