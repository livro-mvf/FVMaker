// ----------------------------------------------------------------------------
// File: tst_GhostBoundary1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests ghost-cell values induced by 1D boundary conditions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(GhostBoundary1D, ComputesDirichletGhostValue) {
    const BoundaryCondition1D condition = dirichlet_1d(3.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::left,
            0.0,
            2.0,
            0.25
        ),
        4.0
    );
}

TEST(GhostBoundary1D, LinearizesDirichletGhostValue) {
    const BoundaryCondition1D condition = dirichlet_1d(3.0);

    const GhostCellLinearization1D linearization = first_ghost_linearization(
        condition,
        BoundarySide1D::left,
        0.0,
        0.25
    );

    EXPECT_DOUBLE_EQ(linearization.constant, 6.0);
    EXPECT_DOUBLE_EQ(linearization.interior_coefficient, -1.0);
}

TEST(GhostBoundary1D, ComputesLeftNeumannGhostValueUsingDxDerivative) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::left,
            0.0,
            10.0,
            0.5
        ),
        8.0
    );
}

TEST(GhostBoundary1D, ComputesRightNeumannGhostValueUsingDxDerivative) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::right,
            1.0,
            10.0,
            0.5
        ),
        12.0
    );
}

TEST(GhostBoundary1D, LinearizesLeftNeumannGhostValue) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    const GhostCellLinearization1D linearization = first_ghost_linearization(
        condition,
        BoundarySide1D::left,
        0.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(linearization.constant, -2.0);
    EXPECT_DOUBLE_EQ(linearization.interior_coefficient, 1.0);
}

TEST(GhostBoundary1D, ComputesLeftRobinGhostValue) {
    const BoundaryCondition1D condition = robin_1d(1.0, 2.0, 5.0);

    const Real ghost = first_ghost_value(
        condition,
        BoundarySide1D::left,
        0.0,
        3.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(ghost, 2.4285714285714284);
}

TEST(GhostBoundary1D, ComputesRightRobinGhostValue) {
    const BoundaryCondition1D condition = robin_1d(1.0, 2.0, 5.0);

    const Real ghost = first_ghost_value(
        condition,
        BoundarySide1D::right,
        1.0,
        3.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(ghost, 3.4444444444444446);
}

TEST(GhostBoundary1D, UsesBoundarySetSide) {
    const BoundarySet1D boundaries{dirichlet_1d(4.0), neumann_1d(1.0)};

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            boundaries,
            BoundarySide1D::left,
            0.0,
            2.0,
            0.5
        ),
        6.0
    );

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            boundaries,
            BoundarySide1D::right,
            1.0,
            2.0,
            0.5
        ),
        2.5
    );
}

TEST(GhostBoundary1D, RejectsInvalidDistance) {
    try {
        static_cast<void>(
            first_ghost_value(
                dirichlet_1d(1.0),
                BoundarySide1D::left,
                0.0,
                2.0,
                0.0
            )
        );
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        return;
    }

    FAIL() << "first_ghost_value did not reject an invalid distance.";
}

}  // namespace fvm
