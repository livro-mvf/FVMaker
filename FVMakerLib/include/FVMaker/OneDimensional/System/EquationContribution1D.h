// ----------------------------------------------------------------------------
// File: EquationContribution1D.h
// Project: FVMaker
// Description: Defines additive 1D equation contributions in book convention.
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

class EquationContribution1D final {
public:
    explicit EquationContribution1D(Size size);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "EquationContribution1D",
            "fvm.1d.system.EquationContribution1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] DenseVector& aw() noexcept;
    [[nodiscard]] const DenseVector& aw() const noexcept;

    [[nodiscard]] DenseVector& ap() noexcept;
    [[nodiscard]] const DenseVector& ap() const noexcept;

    [[nodiscard]] DenseVector& ae() noexcept;
    [[nodiscard]] const DenseVector& ae() const noexcept;

    [[nodiscard]] DenseVector& bp() noexcept;
    [[nodiscard]] const DenseVector& bp() const noexcept;

    EquationContribution1D& operator+=(const EquationContribution1D& other);

private:
    DenseVector aw_;
    DenseVector ap_;
    DenseVector ae_;
    DenseVector bp_;

    void validate() const;
};

[[nodiscard]] EquationContribution1D operator+(
    EquationContribution1D lhs,
    const EquationContribution1D& rhs
);

[[nodiscard]] TridiagonalSystem1D to_tridiagonal_system(
    const EquationContribution1D& contribution
);

}  // namespace fvm
