// ----------------------------------------------------------------------------
// File: GridMetrics1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines read-only one-dimensional grid metrics used by FVMaker.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <span>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>

namespace fvm {

struct GridMetrics1D final {
    Size num_volumes{};
    Size num_faces{};
    std::span<const Real> faces;
    std::span<const Real> centers;
    std::span<const Real> dx_faces;
    std::span<const Real> dx_centers;
    std::span<const Real> cell_lengths;
    Real xmin{};
    Real xmax{};
    Real length{};
    std::string_view pattern_name;
};

}  // namespace fvm
