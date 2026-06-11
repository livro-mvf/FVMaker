// ----------------------------------------------------------------------------
// File: GridView1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a non-owning view over an FVGridMaker Axis1D.
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
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridMetrics1D.h>

namespace fvm {

class GridView1D final {
public:
    explicit GridView1D(const fvgrid::Axis1D& axis);

    explicit GridView1D(const fvgrid::Axis1D* axis);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "GridView1D",
            "fvm.1d.grid.GridView1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_volumes() const noexcept;
    [[nodiscard]] Size num_cells() const noexcept;
    [[nodiscard]] Size num_faces() const noexcept;

    [[nodiscard]] std::span<const Real> faces() const noexcept;
    [[nodiscard]] std::span<const Real> centers() const noexcept;
    [[nodiscard]] std::span<const Real> dx_faces() const noexcept;
    [[nodiscard]] std::span<const Real> dx_centers() const noexcept;
    [[nodiscard]] std::span<const Real> cell_lengths() const noexcept;

    [[nodiscard]] Real xmin() const noexcept;
    [[nodiscard]] Real xmax() const noexcept;
    [[nodiscard]] Real length() const noexcept;

    [[nodiscard]] std::string_view pattern_name() const noexcept;
    [[nodiscard]] GridMetrics1D metrics() const noexcept;

private:
    const fvgrid::Axis1D* axis_{nullptr};

    void validate() const;
};

}  // namespace fvm
