// ----------------------------------------------------------------------------
// File: IterativeSolverOptions.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines common options for iterative linear solvers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <FVMaker/Core/Types.h>
#include <FVMaker/Solver/StopCriteria.h>

namespace fvm {

enum class GaussSeidelSweep {
    forward,
    backward,
    hybrid,
};

struct IterativeSolverOptions final {
    Real tolerance{1.0e-10};
    Size max_iterations{10000};
    GaussSeidelSweep gauss_seidel_sweep{GaussSeidelSweep::hybrid};
    StopCriteria stop_criteria{};
};

}  // namespace fvm
