// ----------------------------------------------------------------------------
// File: SteadyState.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines steady-state solve control options.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class SteadyState final {
public:
    Real absolute_tolerance{1.0e-10};
    Real relative_tolerance{1.0e-8};
    Size max_iterations{10000};

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "SteadyState", "fvm.solver.SteadyState"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Real effective_tolerance(Real reference_norm) const;
    void validate() const;
};

}  // namespace fvm
