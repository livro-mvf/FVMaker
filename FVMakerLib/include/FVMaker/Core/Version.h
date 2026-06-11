// ----------------------------------------------------------------------------
// File: Version.h
// Project: FVMaker
// Version: 0.1.0
// Description: Declares version information for the FVMaker core.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>

namespace fvm {

inline constexpr int kVersionMajor = 0;
inline constexpr int kVersionMinor = 1;
inline constexpr int kVersionPatch = 0;

inline constexpr std::string_view kVersionString = "0.1.0";

[[nodiscard]] constexpr int version_major() noexcept {
    return kVersionMajor;
}

[[nodiscard]] constexpr int version_minor() noexcept {
    return kVersionMinor;
}

[[nodiscard]] constexpr int version_patch() noexcept {
    return kVersionPatch;
}

[[nodiscard]] constexpr std::string_view version_string() noexcept {
    return kVersionString;
}

}  // namespace fvm
