// ----------------------------------------------------------------------------
// File: DenseVector.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a contiguous dense vector for small algebra kernels.
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
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class DenseVector final {
public:
    DenseVector() = default;
    explicit DenseVector(Size size);
    DenseVector(Size size, Real value);
    explicit DenseVector(std::vector<Real> values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Algebra", "DenseVector", "fvm.algebra.DenseVector"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::span<Real> values() noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;

    [[nodiscard]] Real& operator[](Size index) noexcept;
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

private:
    std::vector<Real> values_;
};

}  // namespace fvm
