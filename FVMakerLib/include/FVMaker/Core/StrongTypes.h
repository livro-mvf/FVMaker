// ----------------------------------------------------------------------------
// File: StrongTypes.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines strongly typed wrappers for scalar and integer
//              configuration values used by the FVMaker core.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <concepts>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>

namespace fvm {

template <std::floating_point T, class Tag>
class StrongReal final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongReal(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

template <std::integral T, class Tag>
class StrongInteger final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongInteger(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

struct ToleranceTag final {};
struct MaxIterationsTag final {};
struct TimeTag final {};
struct TimeStepTag final {};
struct InitialTimeTag final {};
struct FinalTimeTag final {};
struct RelaxationFactorTag final {};

using Tolerance = StrongReal<Real, ToleranceTag>;
using MaxIterations = StrongInteger<Size, MaxIterationsTag>;
using Time = StrongReal<Real, TimeTag>;
using TimeStep = StrongReal<Real, TimeStepTag>;
using InitialTime = StrongReal<Real, InitialTimeTag>;
using FinalTime = StrongReal<Real, FinalTimeTag>;
using RelaxationFactor = StrongReal<Real, RelaxationFactorTag>;

}  // namespace fvm
