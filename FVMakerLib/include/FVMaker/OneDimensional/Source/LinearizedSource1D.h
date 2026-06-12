// ----------------------------------------------------------------------------
// File: LinearizedSource1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a 1D source term S = S_C + S_P phi.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <span>
#include <string_view>
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

class LinearizedSource1D final {
public:
    explicit LinearizedSource1D(Size size);
    LinearizedSource1D(DenseVector source_constant, DenseVector source_linear);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "LinearizedSource1D",
            "fvm.1d.source.LinearizedSource1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] DenseVector& source_constant() noexcept;
    [[nodiscard]] const DenseVector& source_constant() const noexcept;

    [[nodiscard]] DenseVector& source_linear() noexcept;
    [[nodiscard]] const DenseVector& source_linear() const noexcept;

    [[nodiscard]] DenseVector evaluate(const DenseVector& phi) const;

private:
    DenseVector source_constant_;
    DenseVector source_linear_;

    void validate() const;
};

[[nodiscard]] LinearizedSource1D zero_source_1d(Size size);

[[nodiscard]] LinearizedSource1D uniform_source_1d(
    Size size,
    Real source_constant,
    Real source_linear = Real{}
);

[[nodiscard]] LinearizedSource1D vector_source_1d(
    DenseVector source_constant,
    DenseVector source_linear
);

[[nodiscard]] LinearizedSource1D explicit_vector_source_1d(
    DenseVector source_constant
);

template <class Function>
concept SourceFunction1D =
    ScalarFunction1D<Function> || ScalarFunction1DTime<Function>;

template <SourceFunction1D Function>
[[nodiscard]] Real evaluate_source_function_1d(
    Function&& function,
    Real x,
    Real time
) {
    if constexpr (ScalarFunction1DTime<Function>) {
        return std::forward<Function>(function)(x, time);
    } else {
        return std::forward<Function>(function)(x);
    }
}

template <SourceFunction1D SourceConstantFunction>
[[nodiscard]] LinearizedSource1D function_source_1d(
    const GridView1D& grid,
    SourceConstantFunction source_constant,
    Real time = Real{}
) {
    DenseVector source_constant_values{grid.num_volumes()};
    DenseVector source_linear_values{grid.num_volumes()};

    const std::span<const Real> centers = grid.centers();

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        source_constant_values[i] = evaluate_source_function_1d(
            source_constant,
            centers[i],
            time
        );
    }

    return LinearizedSource1D{
        std::move(source_constant_values),
        std::move(source_linear_values)
    };
}

template <
    SourceFunction1D SourceConstantFunction,
    SourceFunction1D SourceLinearFunction
>
[[nodiscard]] LinearizedSource1D function_source_1d(
    const GridView1D& grid,
    SourceConstantFunction source_constant,
    SourceLinearFunction source_linear,
    Real time = Real{}
) {
    DenseVector source_constant_values{grid.num_volumes()};
    DenseVector source_linear_values{grid.num_volumes()};

    const std::span<const Real> centers = grid.centers();

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        source_constant_values[i] = evaluate_source_function_1d(
            source_constant,
            centers[i],
            time
        );
        source_linear_values[i] = evaluate_source_function_1d(
            source_linear,
            centers[i],
            time
        );
    }

    return LinearizedSource1D{
        std::move(source_constant_values),
        std::move(source_linear_values)
    };
}

void apply_source_to_system(
    TridiagonalSystem1D& system,
    const LinearizedSource1D& source
);

}  // namespace fvm
