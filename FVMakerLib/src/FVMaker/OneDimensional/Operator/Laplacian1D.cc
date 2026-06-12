// ----------------------------------------------------------------------------
// File: Laplacian1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements the constant-coefficient 1D Laplacian operator.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>

namespace fvm {

Laplacian1D::Laplacian1D(Real coefficient)
    : coefficient_(coefficient) {
    validate();
}

Real Laplacian1D::coefficient() const noexcept {
    return coefficient_;
}

void Laplacian1D::validate() const {
    require(
        std::isfinite(coefficient_),
        error_catalog::kInvalidCoefficient,
        Laplacian1D::id()
    );
}

}  // namespace fvm
