// ----------------------------------------------------------------------------
// File: AlgebraicResidual1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines A X - B residual evaluation for 1D systems.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

[[nodiscard]] DenseVector algebraic_residual(
    const TridiagonalSystem1D& system,
    const DenseVector& solution
);

}  // namespace fvm
