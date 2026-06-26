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
    : BoundaryCondition1D{
          BoundaryConditionKind1D::linear,
          constant_boundary_value(alpha),
          constant_boundary_value(beta),
          constant_boundary_value(gamma)
      } {
    require_finite(alpha);
    require_finite(beta);
    require_finite(gamma);
}

BoundaryCondition1D::BoundaryCondition1D(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
)
    : BoundaryCondition1D{
          BoundaryConditionKind1D::linear,
          std::move(alpha),
          std::move(beta),
          std::move(gamma)
      } {}

BoundaryCondition1D::BoundaryCondition1D(
    BoundaryConditionKind1D kind,
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
)
    : kind_(kind),
      alpha_(std::move(alpha)),
      beta_(std::move(beta)),
      gamma_(std::move(gamma)) {
    require_function(alpha_);
    require_function(beta_);
    require_function(gamma_);
    validate_at_reference_point();
}

BoundaryCondition1D BoundaryCondition1D::linear(
    Real alpha,
    Real beta,
    Real gamma
) {
    return BoundaryCondition1D{alpha, beta, gamma};
}

BoundaryCondition1D BoundaryCondition1D::linear(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D{
        BoundaryConditionKind1D::linear,
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    };
}

BoundaryCondition1D BoundaryCondition1D::dirichlet(Real value) {
    require_finite(value);

    return BoundaryCondition1D{
        BoundaryConditionKind1D::dirichlet,
        constant_boundary_value(Real{1.0}),
        constant_boundary_value(Real{}),
        constant_boundary_value(value)
    };
}

BoundaryCondition1D BoundaryCondition1D::dirichlet(
    StoredBoundaryFunction1D value
) {
    return BoundaryCondition1D{
        BoundaryConditionKind1D::dirichlet,
        constant_boundary_value(Real{1.0}),
        constant_boundary_value(Real{}),
        std::move(value)
    };
}

BoundaryCondition1D BoundaryCondition1D::neumann(Real derivative) {
    require_finite(derivative);

    return BoundaryCondition1D{
        BoundaryConditionKind1D::neumann,
        constant_boundary_value(Real{}),
        constant_boundary_value(Real{1.0}),
        constant_boundary_value(derivative)
    };
}

BoundaryCondition1D BoundaryCondition1D::neumann(
    StoredBoundaryFunction1D derivative
) {
    return BoundaryCondition1D{
        BoundaryConditionKind1D::neumann,
        constant_boundary_value(Real{}),
        constant_boundary_value(Real{1.0}),
        std::move(derivative)
    };
}

BoundaryCondition1D BoundaryCondition1D::robin(
    Real alpha,
    Real beta,
    Real gamma
) {
    require_finite(alpha);
    require_finite(beta);
    require_finite(gamma);

    return BoundaryCondition1D{
        BoundaryConditionKind1D::robin,
        constant_boundary_value(alpha),
        constant_boundary_value(beta),
        constant_boundary_value(gamma)
    };
}

BoundaryCondition1D BoundaryCondition1D::robin(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D{
        BoundaryConditionKind1D::robin,
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    };
}

BoundaryConditionKind1D BoundaryCondition1D::kind() const noexcept {
    return kind_;
}

std::string_view BoundaryCondition1D::kind_name() const noexcept {
    switch (kind_) {
        case BoundaryConditionKind1D::dirichlet:
            return "Dirichlet";
        case BoundaryConditionKind1D::neumann:
            return "Neumann";
        case BoundaryConditionKind1D::robin:
            return "Robin";
        case BoundaryConditionKind1D::linear:
            return "Linear";
    }

    return "Unknown";
}

bool BoundaryCondition1D::is_dirichlet() const noexcept {
    return kind_ == BoundaryConditionKind1D::dirichlet;
}

bool BoundaryCondition1D::is_neumann() const noexcept {
    return kind_ == BoundaryConditionKind1D::neumann;
}

bool BoundaryCondition1D::is_robin() const noexcept {
    return kind_ == BoundaryConditionKind1D::robin;
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
    return BoundaryCondition1D::linear(alpha, beta, gamma);
}

BoundaryCondition1D linear_boundary_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D::linear(
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    );
}

BoundaryCondition1D dirichlet_1d(Real value) {
    return BoundaryCondition1D::dirichlet(value);
}

BoundaryCondition1D dirichlet_1d(StoredBoundaryFunction1D value) {
    return BoundaryCondition1D::dirichlet(std::move(value));
}

BoundaryCondition1D neumann_1d(Real derivative) {
    return BoundaryCondition1D::neumann(derivative);
}

BoundaryCondition1D neumann_1d(StoredBoundaryFunction1D derivative) {
    return BoundaryCondition1D::neumann(std::move(derivative));
}

BoundaryCondition1D robin_1d(Real alpha, Real beta, Real gamma) {
    return BoundaryCondition1D::robin(alpha, beta, gamma);
}

BoundaryCondition1D robin_1d(
    StoredBoundaryFunction1D alpha,
    StoredBoundaryFunction1D beta,
    StoredBoundaryFunction1D gamma
) {
    return BoundaryCondition1D::robin(
        std::move(alpha),
        std::move(beta),
        std::move(gamma)
    );
}

}  // namespace fvm

std::ostream& operator<<(std::ostream& os, const fvm::BoundaryCondition1D& bc) {
    os << static_cast<std::string>(bc.kind_name())
       << " { alpha=" << bc.alpha(0.0)
       << ", beta=" << bc.beta(0.0)
       << ", gamma=" << bc.gamma(0.0)
       << " }";
    return os;
}
