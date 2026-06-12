// ----------------------------------------------------------------------------
// File: Laplacian1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the constant-coefficient 1D Laplacian operator.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class Laplacian1D final {
public:
    explicit Laplacian1D(Real coefficient = Real{1.0});

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Laplacian1D",
            "fvm.1d.operator.Laplacian1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Real coefficient() const noexcept;

private:
    Real coefficient_{1.0};

    void validate() const;
};

}  // namespace fvm
