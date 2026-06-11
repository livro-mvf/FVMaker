// ----------------------------------------------------------------------------
// File: ex_ErrorHandling.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Demonstrates basic FVMaker error handling.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <iostream>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

int main() {
    const fvm::ID example_id{
        "Examples",
        "Ex_ErrorHandling",
        "fvm.examples.Ex_ErrorHandling"
    };

    try {
        fvm::require(false, fvm::error_catalog::kMissingMesh, example_id);
    } catch (const fvm::FVMException& exception) {
        const fvm::ErrorRecord& record = exception.record();

        std::cout << "Caught FVMException\n";
        std::cout << "code       : " << record.code << '\n';
        std::cout << "category   : " << record.category << '\n';
        std::cout << "module     : " << record.source.module() << '\n';
        std::cout << "class name : " << record.source.class_name() << '\n';
        std::cout << "class id   : " << record.source.class_id() << '\n';
        std::cout << "message    : " << record.message << '\n';
        std::cout << "what       : " << exception.what() << '\n';

        return 0;
    }

    return 1;
}
