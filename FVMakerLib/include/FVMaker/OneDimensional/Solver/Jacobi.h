// ----------------------------------------------------------------------------
// File: Jacobi.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the Jacobi solver for 1D tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/Solver/IterativeSolverOptions.h>
#include <FVMaker/Solver/SolveResult.h>

namespace fvm {

class Jacobi final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "Jacobi", "fvm.1d.solver.Jacobi"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static SolveResult solve(
        const TridiagonalSystem1D& system,
        IterativeSolverOptions options = {}
    );
};

}  // namespace fvm
