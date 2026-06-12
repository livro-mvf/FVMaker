// ----------------------------------------------------------------------------
// File: ManufacturedSolution1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a stored one-dimensional manufactured solution package.
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

struct ManufacturedSolution1D final {
    StoredScalarFunction1D phi;
    StoredScalarFunction1D dphi_dx;
    StoredScalarFunction1D d2phi_dx2;

    [[nodiscard]] Real value(Real x) const {
        return phi(x);
    }

    [[nodiscard]] Real first_derivative(Real x) const {
        return dphi_dx(x);
    }

    [[nodiscard]] Real second_derivative(Real x) const {
        return d2phi_dx2(x);
    }
};

}  // namespace fvm
