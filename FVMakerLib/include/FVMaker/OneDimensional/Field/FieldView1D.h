// ----------------------------------------------------------------------------
// File: FieldView1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a non-owning mutable view over one-dimensional field data.
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
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class FieldView1D final {
public:
    FieldView1D(GridView1D grid, std::string_view name, std::span<Real> values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "FieldView1D",
            "fvm.1d.field.FieldView1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const GridView1D& grid() const noexcept;
    [[nodiscard]] std::string_view name() const noexcept;
    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::span<Real> values() noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;

    [[nodiscard]] Real& operator[](Size index) noexcept;
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

private:
    GridView1D grid_;
    std::string_view name_;
    std::span<Real> values_;

    void validate() const;
};

}  // namespace fvm
