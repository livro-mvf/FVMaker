// ----------------------------------------------------------------------------
// File: ErrorDescriptor.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines immutable descriptors for FVMaker error codes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvm {

struct ErrorDescriptor final {
    std::string_view code;
    std::string_view message;
    std::string_view category;
};

}  // namespace fvm
