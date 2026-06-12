// ----------------------------------------------------------------------------
// File: PentadiagonalSystem1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a pentadiagonal linear system for 1D finite volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <span>
#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class PentadiagonalSystem1D final {
public:
    explicit PentadiagonalSystem1D(Size size);

    PentadiagonalSystem1D(
        std::vector<Real> lower_second,
        std::vector<Real> lower_first,
        std::vector<Real> diagonal,
        std::vector<Real> upper_first,
        std::vector<Real> upper_second,
        DenseVector rhs
    );

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "PentadiagonalSystem1D",
            "fvm.1d.system.PentadiagonalSystem1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] std::span<Real> lower_second() noexcept;
    [[nodiscard]] std::span<const Real> lower_second() const noexcept;

    [[nodiscard]] std::span<Real> lower_first() noexcept;
    [[nodiscard]] std::span<const Real> lower_first() const noexcept;

    [[nodiscard]] std::span<Real> diagonal() noexcept;
    [[nodiscard]] std::span<const Real> diagonal() const noexcept;

    [[nodiscard]] std::span<Real> upper_first() noexcept;
    [[nodiscard]] std::span<const Real> upper_first() const noexcept;

    [[nodiscard]] std::span<Real> upper_second() noexcept;
    [[nodiscard]] std::span<const Real> upper_second() const noexcept;

    [[nodiscard]] DenseVector& rhs() noexcept;
    [[nodiscard]] const DenseVector& rhs() const noexcept;

    void set_row(
        Size row,
        Real lower_second,
        Real lower_first,
        Real diagonal,
        Real upper_first,
        Real upper_second,
        Real rhs
    );

private:
    std::vector<Real> lower_second_;
    std::vector<Real> lower_first_;
    std::vector<Real> diagonal_;
    std::vector<Real> upper_first_;
    std::vector<Real> upper_second_;
    DenseVector rhs_;

    void validate() const;
};

}  // namespace fvm
