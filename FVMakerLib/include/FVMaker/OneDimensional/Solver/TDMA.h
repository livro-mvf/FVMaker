// ----------------------------------------------------------------------------
// File: TDMA.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the TDMA solver for 1D tridiagonal systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>
#include <FVMaker/Solver/SolveResult.h>

namespace fvm {

struct TDMAForwardSweep final {
    DenseVector t;
    DenseVector q;
};

class TDMA final {
public:
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "TDMA",
            "fvm.1d.solver.TDMA"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static TDMAForwardSweep forward_sweep(
        const TridiagonalSystem1D& system
    );

    [[nodiscard]] static SolveResult solve(const TridiagonalSystem1D& system);
};

}  // namespace fvm
