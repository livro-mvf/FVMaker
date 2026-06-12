// ----------------------------------------------------------------------------
// File: FunctionEvaluation1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines helpers for evaluating 1D functions on structured grids.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

template <ScalarFunction1D Function>
[[nodiscard]] DenseVector evaluate_at_centers(
    const GridView1D& grid,
    Function&& function
) {
    DenseVector values{grid.num_volumes()};
    const auto centers = grid.centers();

    for (Size i = 0; i < values.size(); ++i) {
        values[i] = static_cast<Real>(std::invoke(function, centers[i]));
    }

    return values;
}

template <ScalarFunction1D Function>
[[nodiscard]] DenseVector evaluate_at_faces(
    const GridView1D& grid,
    Function&& function
) {
    DenseVector values{grid.num_faces()};
    const auto faces = grid.faces();

    for (Size i = 0; i < values.size(); ++i) {
        values[i] = static_cast<Real>(std::invoke(function, faces[i]));
    }

    return values;
}

}  // namespace fvm
