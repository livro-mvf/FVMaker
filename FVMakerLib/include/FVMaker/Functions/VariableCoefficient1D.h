// ----------------------------------------------------------------------------
// File: VariableCoefficient1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a stored one-dimensional variable coefficient package.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>
#include <FVMaker/Functions/FunctionTypes.h>

namespace fvm {

struct VariableCoefficient1D final {
    StoredScalarFunction1D gamma;
    StoredScalarFunction1D dgamma_dx;

    [[nodiscard]] Real value(Real x) const {
        return gamma(x);
    }

    [[nodiscard]] Real derivative(Real x) const {
        return dgamma_dx(x);
    }
};

}  // namespace fvm
