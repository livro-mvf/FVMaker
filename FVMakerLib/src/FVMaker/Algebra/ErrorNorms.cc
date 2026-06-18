// ----------------------------------------------------------------------------
// File: ErrorNorms.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements algebraic error norms for dense vectors.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <algorithm>
#include <cmath>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>

namespace fvm {

Real VectorNorms::evaluate(const DenseVector& vector, NormType type) noexcept {
    switch (type) {
        case NormType::infinity:
            return infinity(vector);
        case NormType::l1:
            return l1(vector);
        case NormType::l2:
            return l2(vector);
    }

    return infinity(vector);
}

Real VectorNorms::infinity(const DenseVector& vector) noexcept {
    Real norm{};

    for (Real value : vector.values()) {
        norm = std::max(norm, std::abs(value));
    }

    return norm;
}

Real VectorNorms::l1(const DenseVector& vector) noexcept {
    Real norm{};

    for (Real value : vector.values()) {
        norm += std::abs(value);
    }

    return norm;
}

Real VectorNorms::l2(const DenseVector& vector) noexcept {
    Real sum{};

    for (Real value : vector.values()) {
        sum += value * value;
    }

    return std::sqrt(sum);
}

Real norm_infinity(const DenseVector& vector) noexcept {
    return VectorNorms::infinity(vector);
}

Real norm_l1(const DenseVector& vector) noexcept {
    return VectorNorms::l1(vector);
}

Real norm_l2(const DenseVector& vector) noexcept {
    return VectorNorms::l2(vector);
}

}  // namespace fvm
