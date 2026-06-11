// ----------------------------------------------------------------------------
// File: FVMException.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the base exception type used by FVMaker.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <exception>
#include <string>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorRecord.h>

namespace fvm {

class FVMException final : public std::exception {
public:
    explicit FVMException(ErrorRecord record);

    [[nodiscard]] const char* what() const noexcept override;

    [[nodiscard]] const ErrorRecord& record() const noexcept;

private:
    ErrorRecord record_;
    std::string formatted_message_;
};

}  // namespace fvm
