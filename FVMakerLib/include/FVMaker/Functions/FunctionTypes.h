// ----------------------------------------------------------------------------
// File: FunctionTypes.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines scalar and vector function concepts used by FVMaker.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <array>
#include <concepts>
#include <functional>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>

namespace fvm {

using VectorValue1D = std::array<Real, 1>;
using VectorValue2D = std::array<Real, 2>;

template <class Function>
concept ScalarFunction1D =
    std::invocable<Function, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real>, Real>;

template <class Function>
concept ScalarFunction1DTime =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, Real>;

template <class Function>
concept ScalarFunction2D =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, Real>;

template <class Function>
concept ScalarFunction2DTime =
    std::invocable<Function, Real, Real, Real> &&
    std::convertible_to<
        std::invoke_result_t<Function, Real, Real, Real>,
        Real
    >;

template <class Function>
concept VectorFunction1D =
    std::invocable<Function, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real>, VectorValue1D>;

template <class Function>
concept VectorFunction2D =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, VectorValue2D>;

using StoredScalarFunction1D = std::function<Real(Real)>;
using StoredScalarFunction1DTime = std::function<Real(Real, Real)>;
using StoredScalarFunction2D = std::function<Real(Real, Real)>;
using StoredScalarFunction2DTime = std::function<Real(Real, Real, Real)>;
using StoredVectorFunction1D = std::function<VectorValue1D(Real)>;
using StoredVectorFunction2D = std::function<VectorValue2D(Real, Real)>;

}  // namespace fvm
