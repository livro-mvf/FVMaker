// ----------------------------------------------------------------------------
// File: Equation1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a minimal steady 1D equation container.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

namespace fvm {

class Equation1D final {
public:
    Equation1D(
        GridView1D grid,
        Laplacian1D laplacian,
        LinearizedSource1D source,
        BoundarySet1D boundaries
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "Equation1D",
            "fvm.1d.equation.Equation1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] const GridView1D& grid() const noexcept;
    [[nodiscard]] const Laplacian1D& laplacian() const noexcept;
    [[nodiscard]] const LinearizedSource1D& source() const noexcept;
    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;

private:
    GridView1D grid_;
    Laplacian1D laplacian_;
    LinearizedSource1D source_;
    BoundarySet1D boundaries_;

    void validate() const;
};

}  // namespace fvm
