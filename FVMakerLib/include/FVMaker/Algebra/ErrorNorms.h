// ----------------------------------------------------------------------------
// File: ErrorNorms.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines algebraic error norms for dense vectors.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

enum class NormType {
    infinity,
    l1,
    l2,
    rms,
};

class VectorNorms final {
public:
    [[nodiscard]] static Real evaluate(
        const DenseVector& vector,
        NormType type
    ) noexcept;

    [[nodiscard]] static Real infinity(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real l1(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real l2(const DenseVector& vector) noexcept;
    [[nodiscard]] static Real rms(const DenseVector& vector) noexcept;
};

[[nodiscard]] Real norm_infinity(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l1(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l2(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_rms(const DenseVector& vector) noexcept;

}  // namespace fvm
