// ----------------------------------------------------------------------------
// File: FVMaker.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements root FVMaker library helpers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker.h>

namespace fvm {

const char* version() noexcept {
    return kVersionString.data();
}

}  // namespace fvm
