// ----------------------------------------------------------------------------
// File: LinearSolverOptions1D.h
// Project: FVMaker
// Description: Defines runtime selection options for 1D linear solvers.
// ----------------------------------------------------------------------------

#pragma once

#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/Solver/SteadyState.h>

namespace fvm {

enum class LinearSolverKind1D {
    tdma,
    jacobi,
    gauss_seidel,
    conjugate_gradient,
    bicg,
    bicgstab,
};

[[nodiscard]] std::string_view name(LinearSolverKind1D kind) noexcept;

class LinearSolverOptions1D final {
public:
    LinearSolverKind1D solver{LinearSolverKind1D::tdma};
    Real absolute_tolerance{1.0e-10};
    Real relative_tolerance{1.0e-8};
    Size max_iterations{10000};

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "LinearSolverOptions1D",
            "fvm.1d.solver.LinearSolverOptions1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] SteadyState steady_state() const;
    void validate() const;
};

}  // namespace fvm
