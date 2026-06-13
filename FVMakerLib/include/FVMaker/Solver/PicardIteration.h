// ----------------------------------------------------------------------------
// File: PicardIteration.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a small generic Picard fixed-point iteration helper.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <cmath>
#include <functional>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

namespace fvm {

struct PicardOptions final {
    Real absolute_tolerance{1.0e-10};
    Real relative_tolerance{1.0e-8};
    Size max_iterations{100};
    Real relaxation_factor{1.0};

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "PicardOptions", "fvm.solver.PicardOptions"};
    }

    void validate() const {
        require(
            std::isfinite(absolute_tolerance) && absolute_tolerance > Real{},
            error_catalog::kInvalidTolerance,
            id()
        );

        require(
            std::isfinite(relative_tolerance) && relative_tolerance >= Real{},
            error_catalog::kInvalidTolerance,
            id()
        );

        require(
            max_iterations > 0,
            error_catalog::kInvalidArgument,
            id()
        );

        require(
            std::isfinite(relaxation_factor)
                && relaxation_factor > Real{}
                && relaxation_factor <= Real{1},
            error_catalog::kInvalidArgument,
            id()
        );
    }
};

struct PicardResult final {
    DenseVector solution;
    bool converged{false};
    Size iterations{};
    Real correction_norm{};
    Real initial_correction_norm{};
    Real relative_correction_norm{};
    Real requested_tolerance{};
};

[[nodiscard]] inline DenseVector relax_picard_update(
    const DenseVector& previous,
    const DenseVector& candidate,
    Real relaxation_factor
) {
    require(
        previous.size() == candidate.size(),
        error_catalog::kInvalidArgument,
        PicardOptions::id()
    );

    DenseVector relaxed{previous.size()};

    for (Size i = 0; i < previous.size(); ++i) {
        relaxed[i] = relaxation_factor * candidate[i]
            + (Real{1} - relaxation_factor) * previous[i];
    }

    return relaxed;
}

[[nodiscard]] inline Real picard_correction_norm(
    const DenseVector& previous,
    const DenseVector& current
) {
    require(
        previous.size() == current.size(),
        error_catalog::kInvalidArgument,
        PicardOptions::id()
    );

    Real norm{};

    for (Size i = 0; i < previous.size(); ++i) {
        norm = std::max(norm, std::abs(current[i] - previous[i]));
    }

    return norm;
}

template <class Update>
[[nodiscard]] PicardResult picard_iteration(
    DenseVector initial_solution,
    Update&& update,
    const PicardOptions& options = {}
) {
    options.validate();

    PicardResult result;
    result.solution = initial_solution;

    for (Size iteration = 1; iteration <= options.max_iterations; ++iteration) {
        DenseVector candidate = std::invoke(update, result.solution);
        DenseVector next_solution = relax_picard_update(
            result.solution,
            candidate,
            options.relaxation_factor
        );

        const Real correction_norm = picard_correction_norm(
            result.solution,
            next_solution
        );

        if (iteration == 1) {
            result.initial_correction_norm = correction_norm;
            result.requested_tolerance = std::max(
                options.absolute_tolerance,
                options.relative_tolerance * result.initial_correction_norm
            );
        }

        result.solution = next_solution;
        result.iterations = iteration;
        result.correction_norm = correction_norm;
        result.relative_correction_norm =
            result.initial_correction_norm > Real{}
                ? correction_norm / result.initial_correction_norm
                : correction_norm;

        if (correction_norm <= result.requested_tolerance) {
            result.converged = true;
            break;
        }
    }

    return result;
}

}  // namespace fvm
