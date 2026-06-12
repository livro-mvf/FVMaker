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

[[nodiscard]] Real norm_infinity(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l1(const DenseVector& vector) noexcept;
[[nodiscard]] Real norm_l2(const DenseVector& vector) noexcept;

}  // namespace fvm
