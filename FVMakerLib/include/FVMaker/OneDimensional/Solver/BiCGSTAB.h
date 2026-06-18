// ----------------------------------------------------------------------------
// File: BiCGSTAB.h
// Project: FVMaker
// Description: Defines the stabilized BiConjugate Gradient solver for 1D systems.
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/Solver/IterativeSolverOptions.h>
#include <FVMaker/Solver/SolveResult.h>

namespace fvm {

class BiCGSTAB final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "BiCGSTAB", "fvm.1d.solver.BiCGSTAB"};
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
