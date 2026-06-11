// ----------------------------------------------------------------------------
// File: ThrowError.h
// Project: FVMaker
// Version: 0.1.0
// Description: Declares helper functions for throwing FVMaker exceptions.
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
#include <FVMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVMaker/ErrorHandling/FVMException.h>

namespace fvm {

[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

}  // namespace fvm
