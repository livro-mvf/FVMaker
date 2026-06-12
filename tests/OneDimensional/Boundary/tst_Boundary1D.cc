// ----------------------------------------------------------------------------
// File: tst_Boundary1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests 1D linear boundary conditions.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(Boundary1D, BoundaryConditionStoresClassIdentity) {
    EXPECT_EQ(
        BoundaryCondition1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        BoundaryCondition1D::id().class_name(),
        std::string_view{"BoundaryCondition1D"}
    );
    EXPECT_EQ(
        BoundaryCondition1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.BoundaryCondition1D"}
    );
}

TEST(Boundary1D, BoundarySetStoresClassIdentity) {
    EXPECT_EQ(BoundarySet1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(BoundarySet1D::id().class_name(), std::string_view{"BoundarySet1D"});
    EXPECT_EQ(
        BoundarySet1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.BoundarySet1D"}
    );
}

TEST(Boundary1D, StoresGeneralLinearBoundaryCondition) {
    const BoundaryCondition1D condition = linear_boundary_1d(2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 2.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 3.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 4.0);
}

TEST(Boundary1D, StoresFunctionalLinearBoundaryCondition) {
    const BoundaryCondition1D condition = linear_boundary_1d(
        StoredBoundaryFunction1D{[](Real x, Real) {
            return Real{1.0} + x;
        }},
        StoredBoundaryFunction1D{[](Real, Real time) {
            return Real{2.0} + time;
        }},
        StoredBoundaryFunction1D{[](Real x, Real time) {
            return x + time;
        }}
    );

    EXPECT_DOUBLE_EQ(condition.alpha(3.0, 4.0), 4.0);
    EXPECT_DOUBLE_EQ(condition.beta(3.0, 4.0), 6.0);
    EXPECT_DOUBLE_EQ(condition.gamma(3.0, 4.0), 7.0);
}

TEST(Boundary1D, DirichletIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = dirichlet_1d(2.5);

    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 1.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 0.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 2.5);
}

TEST(Boundary1D, NeumannIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = neumann_1d(-1.25);

    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 0.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 1.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), -1.25);
}

TEST(Boundary1D, RobinIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = robin_1d(2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 2.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 3.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 4.0);
}

TEST(Boundary1D, BoundarySetStoresLeftAndRightConditions) {
    const BoundarySet1D boundaries{dirichlet_1d(1.0), neumann_1d(0.0)};

    EXPECT_DOUBLE_EQ(boundaries.left().alpha(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.left().gamma(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.right().beta(1.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.right().gamma(1.0), 0.0);

    EXPECT_DOUBLE_EQ(boundaries.at(BoundarySide1D::left).gamma(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.at(BoundarySide1D::right).gamma(1.0), 0.0);
}

TEST(Boundary1D, RejectsInvalidLinearBoundaryCoefficients) {
    try {
        const BoundaryCondition1D condition = linear_boundary_1d(0.0, 0.0, 1.0);
        (void)condition;
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"BoundaryCondition1D"}
        );
        return;
    }

    FAIL() << "BoundaryCondition1D did not reject alpha=0 and beta=0.";
}

}  // namespace fvm
