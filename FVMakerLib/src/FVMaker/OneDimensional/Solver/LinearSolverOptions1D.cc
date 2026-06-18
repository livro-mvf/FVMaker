// ----------------------------------------------------------------------------
// File: LinearSolverOptions1D.cc
// Project: FVMaker
// Description: Implements runtime 1D linear solver options.
// ----------------------------------------------------------------------------

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/LinearSolverOptions1D.h>

namespace fvm {

std::string_view name(LinearSolverKind1D kind) noexcept {
    switch (kind) {
        case LinearSolverKind1D::tdma:
            return "TDMA";
        case LinearSolverKind1D::jacobi:
            return "Jacobi";
        case LinearSolverKind1D::gauss_seidel:
            return "Gauss-Seidel";
        case LinearSolverKind1D::conjugate_gradient:
            return "Conjugate Gradient";
        case LinearSolverKind1D::bicg:
            return "BiCG";
        case LinearSolverKind1D::bicgstab:
            return "BiCGSTAB";
    }

    return "unknown";
}

SteadyState LinearSolverOptions1D::steady_state() const {
    return SteadyState{
        .absolute_tolerance = absolute_tolerance,
        .relative_tolerance = relative_tolerance,
        .max_iterations = max_iterations
    };
}

void LinearSolverOptions1D::validate() const {
    steady_state().validate();

    require(
        solver == LinearSolverKind1D::tdma
            || solver == LinearSolverKind1D::jacobi
            || solver == LinearSolverKind1D::gauss_seidel
            || solver == LinearSolverKind1D::conjugate_gradient
            || solver == LinearSolverKind1D::bicg
            || solver == LinearSolverKind1D::bicgstab,
        error_catalog::kInvalidArgument,
        id()
    );
}

}  // namespace fvm
