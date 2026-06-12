// ----------------------------------------------------------------------------
// File: GhostCells1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the ghost-cell layout for 1D finite-volume problems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class GhostCells1D final {
public:
    GhostCells1D() = default;
    GhostCells1D(Size left, Size right);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "GhostCells1D",
            "fvm.1d.boundary.GhostCells1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static constexpr Size max_per_side() noexcept {
        return 2;
    }

    [[nodiscard]] Size left() const noexcept;
    [[nodiscard]] Size right() const noexcept;
    [[nodiscard]] Size total() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] Size first_physical_index() const noexcept;
    [[nodiscard]] Size storage_size(Size physical_size) const noexcept;
    [[nodiscard]] Size storage_index(Size physical_index) const noexcept;

private:
    Size left_{0};
    Size right_{0};

    void validate() const;
};

}  // namespace fvm
