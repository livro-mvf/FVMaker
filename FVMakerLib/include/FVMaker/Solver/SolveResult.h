// ----------------------------------------------------------------------------
// File: SolveResult.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a minimal result object for linear solvers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

struct SolveResult final {
    DenseVector solution;
    bool converged{false};
    Size iterations{};
    Real residual_norm{};
};

}  // namespace fvm
