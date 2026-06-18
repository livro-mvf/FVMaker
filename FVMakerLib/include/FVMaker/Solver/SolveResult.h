// ----------------------------------------------------------------------------
// File: SolveResult.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines result data returned by solve operations.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/Solver/StopCriteria.h>

namespace fvm {

struct SolveResult final {
    DenseVector solution;
    DenseVector residual;
    bool converged{false};
    Size iterations{};
    Real residual_norm{};
    Real initial_residual_norm{};
    Real relative_residual_norm{};
    Real correction_norm{};
    Real requested_tolerance{};
    bool reached_absolute_tolerance{false};
    bool reached_relative_tolerance{false};
    StopCriterionKind stop_criterion{StopCriterionKind::max_iterations};
    Real stop_value{};
    Real stop_tolerance{};
    bool reached_iteration_limit{false};
};

}  // namespace fvm
