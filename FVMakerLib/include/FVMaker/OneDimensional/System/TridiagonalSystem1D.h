// ----------------------------------------------------------------------------
// File: TridiagonalSystem1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a tridiagonal linear system for 1D finite volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <span>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class TridiagonalSystem1D final {
public:
    explicit TridiagonalSystem1D(Size size);

    TridiagonalSystem1D(
        std::vector<Real> lower,
        std::vector<Real> diagonal,
        std::vector<Real> upper,
        DenseVector rhs
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "TridiagonalSystem1D",
            "fvm.1d.system.TridiagonalSystem1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] std::span<Real> lower() noexcept;
    [[nodiscard]] std::span<const Real> lower() const noexcept;

    [[nodiscard]] std::span<Real> diagonal() noexcept;
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    [[nodiscard]] std::span<Real> upper() noexcept;
    [[nodiscard]] std::span<const Real> upper() const noexcept;

    [[nodiscard]] DenseVector& rhs() noexcept;
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    void set_row(
        Size row,
        Real lower,
        Real diagonal,
        Real upper,
        Real rhs
    );

private:
    std::vector<Real> lower_;
    std::vector<Real> diagonal_;
    std::vector<Real> upper_;
    DenseVector rhs_;

    void validate() const;
};

void multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

void multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x,
    DenseVector& y
);

[[nodiscard]] DenseVector multiply(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

[[nodiscard]] DenseVector multiply_transpose(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

[[nodiscard]] DenseVector operator*(
    const TridiagonalSystem1D& system,
    const DenseVector& x
);

}  // namespace fvm
