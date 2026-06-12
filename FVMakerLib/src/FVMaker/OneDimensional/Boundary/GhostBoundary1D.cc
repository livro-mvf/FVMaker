// ----------------------------------------------------------------------------
// File: GhostBoundary1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements ghost-cell values induced by 1D boundary conditions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <cmath>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

namespace fvm {

namespace {

void validate_common(
    Real boundary_position,
    Real center_to_center_distance
) {
    require(
        std::isfinite(boundary_position),
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );

    require(
        std::isfinite(center_to_center_distance)
            && center_to_center_distance > Real{},
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );
}

}  // namespace

GhostCellLinearization1D first_ghost_linearization(
    const BoundaryCondition1D& condition,
    BoundarySide1D side,
    Real boundary_position,
    Real center_to_center_distance,
    Real time
) {
    validate_common(boundary_position, center_to_center_distance);

    const Real alpha = condition.alpha(boundary_position, time);
    const Real beta = condition.beta(boundary_position, time);
    const Real gamma = condition.gamma(boundary_position, time);
    const Real half_alpha = Real{0.5} * alpha;
    const Real beta_over_distance = beta / center_to_center_distance;

    require(
        std::isfinite(alpha) && std::isfinite(beta) && std::isfinite(gamma),
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );

    if (side == BoundarySide1D::left) {
        const Real denominator = half_alpha - beta_over_distance;
        require(
            std::abs(denominator) > Real{},
            error_catalog::kInvalidBoundaryCondition,
            BoundaryCondition1D::id()
        );

        return GhostCellLinearization1D{
            .constant = gamma / denominator,
            .interior_coefficient =
                -(half_alpha + beta_over_distance) / denominator,
        };
    }

    const Real denominator = half_alpha + beta_over_distance;
    require(
        std::abs(denominator) > Real{},
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );

    return GhostCellLinearization1D{
        .constant = gamma / denominator,
        .interior_coefficient =
            -(half_alpha - beta_over_distance) / denominator,
    };
}

GhostCellLinearization1D first_ghost_linearization(
    const BoundarySet1D& boundaries,
    BoundarySide1D side,
    Real boundary_position,
    Real center_to_center_distance,
    Real time
) {
    return first_ghost_linearization(
        boundaries.at(side),
        side,
        boundary_position,
        center_to_center_distance,
        time
    );
}

Real first_ghost_value(
    const BoundaryCondition1D& condition,
    BoundarySide1D side,
    Real boundary_position,
    Real interior_value,
    Real center_to_center_distance,
    Real time
) {
    require(
        std::isfinite(interior_value),
        error_catalog::kInvalidBoundaryCondition,
        BoundaryCondition1D::id()
    );

    const GhostCellLinearization1D linearization = first_ghost_linearization(
        condition,
        side,
        boundary_position,
        center_to_center_distance,
        time
    );

    return linearization.constant
        + linearization.interior_coefficient * interior_value;
}

Real first_ghost_value(
    const BoundarySet1D& boundaries,
    BoundarySide1D side,
    Real boundary_position,
    Real interior_value,
    Real center_to_center_distance,
    Real time
) {
    return first_ghost_value(
        boundaries.at(side),
        side,
        boundary_position,
        interior_value,
        center_to_center_distance,
        time
    );
}

}  // namespace fvm
