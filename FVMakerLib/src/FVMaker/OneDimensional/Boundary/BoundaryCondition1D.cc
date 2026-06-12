// ----------------------------------------------------------------------------
// File: BoundaryCondition1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements 1D linear boundary conditions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>

namespace fvm {

namespace {

[[nodiscard]] StoredBoundaryFunction1D constant_boundary_value(Real value) {
    return [value](Real, Real) {
        return value;
    };
}

void require_finite(Real value) {
    require(
        std::isfinite(value),
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );
}

void require_function(const StoredBoundaryFunction1D& function) {
    require(
        static_cast<bool>(function),
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );
}

}  // namespace

BoundaryCondition1D::BoundaryCondition1D(
    Real alpha,
    Real beta,
    Real gamma
)
    : alpha_(constant_boundary_value(alpha)),
      beta_(constant_boundary_value(beta)),
      gamma_(constant_boundary_value(gamma)) {
    require_finite(alpha);
    require_finite(beta);
    require_finite(gamma);
    validate_at_reference_point();
}

BoundaryCondition1D::BoundaryCondition1D(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
)
    : alpha_(std::move(alpha)),
      beta_(std::move(beta)),
      gamma_(std::move(gamma)) {
    require_function(alpha_);
    require_function(beta_);
    require_function(gamma_);
    validate_at_reference_point();
}

Real BoundaryCondition1D::alpha(Real position, Real time) const {
    return alpha_(position, time);
}

Real BoundaryCondition1D::beta(Real position, Real time) const {
    return beta_(position, time);
}

Real BoundaryCondition1D::gamma(Real position, Real time) const {
    return gamma_(position, time);
}

void BoundaryCondition1D::validate_at_reference_point() const {
    const Real alpha_value = alpha_(Real{}, Real{});
    const Real beta_value = beta_(Real{}, Real{});
    const Real gamma_value = gamma_(Real{}, Real{});

    require_finite(alpha_value);
    require_finite(beta_value);
    require_finite(gamma_value);

    require(
        std::abs(alpha_value) > Real{} || std::abs(beta_value) > Real{},
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );
}

BoundarySet1D::BoundarySet1D(
    BoundaryCondition1D left,
    BoundaryCondition1D right
)
    : left_(std::move(left)), right_(std::move(right)) {}

const BoundaryCondition1D& BoundarySet1D::left() const noexcept {
    return left_;
}

const BoundaryCondition1D& BoundarySet1D::right() const noexcept {
    return right_;
}

const BoundaryCondition1D& BoundarySet1D::at(BoundarySide1D side) const noexcept {
    return side == BoundarySide1D::left ? left_ : right_;
}

BoundaryCondition1D linear_boundary_1d(Real alpha, Real beta, Real gamma) {
    return BoundaryCondition1D{alpha, beta, gamma};
}

BoundaryCondition1D linear_boundary_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D{
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    };
}

BoundaryCondition1D dirichlet_1d(Real value) {
    return BoundaryCondition1D{Real{1.0}, Real{}, value};
}

BoundaryCondition1D dirichlet_1d(StoredBoundaryFunction1D value) {
    return BoundaryCondition1D{
        constant_boundary_value(Real{1.0}),
        constant_boundary_value(Real{}),
        std::move(value)
    };
}

BoundaryCondition1D neumann_1d(Real derivative) {
    return BoundaryCondition1D{Real{}, Real{1.0}, derivative};
}

BoundaryCondition1D neumann_1d(StoredBoundaryFunction1D derivative) {
    return BoundaryCondition1D{
        constant_boundary_value(Real{}),
        constant_boundary_value(Real{1.0}),
        std::move(derivative)
    };
}

BoundaryCondition1D robin_1d(Real alpha, Real beta, Real gamma) {
    return BoundaryCondition1D{alpha, beta, gamma};
}

BoundaryCondition1D robin_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D{
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    };
}

}  // namespace fvm
