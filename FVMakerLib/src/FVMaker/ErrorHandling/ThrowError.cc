// ----------------------------------------------------------------------------
// File: ThrowError.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements helper functions for throwing FVMaker exceptions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ThrowError.h>

namespace fvm {

[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    throw_error(
        descriptor.code,
        std::string{descriptor.message},
        descriptor.category,
        source,
        location
    );
}

[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    throw FVMException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .category = category,
            .source = source,
            .location = location,
        }
    };
}

void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(descriptor, source, location);
    }
}

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(code, std::move(message), category, source, location);
    }
}

}  // namespace fvm
