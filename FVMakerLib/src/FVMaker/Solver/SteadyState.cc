// ----------------------------------------------------------------------------
// File: SteadyState.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements steady-state solve control options.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <algorithm>
#include <cmath>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Solver/SteadyState.h>

namespace fvm {

Real SteadyState::effective_tolerance(Real reference_norm) const {
    validate();

    require(
        std::isfinite(reference_norm) && reference_norm >= Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    return std::max(absolute_tolerance, relative_tolerance * reference_norm);
}

void SteadyState::validate() const {
    require(
        std::isfinite(absolute_tolerance) && absolute_tolerance > Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    require(
        std::isfinite(relative_tolerance) && relative_tolerance >= Real{},
        error_catalog::kInvalidTolerance,
        SteadyState::id()
    );

    require(
        max_iterations > 0,
        error_catalog::kInvalidArgument,
        SteadyState::id()
    );
}

}  // namespace fvm
