// ----------------------------------------------------------------------------
// File: NonlinearCoefficient1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines a generic 1D coefficient Gamma(phi, x).
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <functional>
#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

class NonlinearCoefficient1D final {
public:
    using Function = std::function<Real(Real phi, Real x)>;

    explicit NonlinearCoefficient1D(Function function);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "NonlinearCoefficient1D",
            "fvm.1d.coefficient.NonlinearCoefficient1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] DenseVector cell_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

    [[nodiscard]] DiffusionCoefficient1D arithmetic_face_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

    [[nodiscard]] DiffusionCoefficient1D harmonic_face_values(
        const GridView1D& grid,
        const DenseVector& phi
    ) const;

private:
    Function function_;
};

template <class Function>
[[nodiscard]] NonlinearCoefficient1D nonlinear_coefficient_1d(
    Function function
) {
    return NonlinearCoefficient1D{
        NonlinearCoefficient1D::Function{function}
    };
}

}  // namespace fvm
