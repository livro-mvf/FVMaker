// ----------------------------------------------------------------------------
// File: Laplacian1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines the 1D conservative Laplacian operator.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class Laplacian1D final {
public:
    explicit Laplacian1D(Real coefficient = Real{1.0});
    explicit Laplacian1D(DiffusionCoefficient1D coefficient);

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
    [[nodiscard]] bool has_variable_coefficient() const noexcept;
    [[nodiscard]] Real face_coefficient(const GridView1D& grid, Size face) const;

private:
    Real coefficient_{1.0};
    DenseVector face_coefficients_;

    void validate() const;
};

}  // namespace fvm
