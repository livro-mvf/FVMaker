// ----------------------------------------------------------------------------
// File: tst_GhostCells1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the 1D ghost-cell layout.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(GhostCells1D, StoresClassIdentity) {
    EXPECT_EQ(GhostCells1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(GhostCells1D::id().class_name(), std::string_view{"GhostCells1D"});
    EXPECT_EQ(
        GhostCells1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.GhostCells1D"}
    );
}

TEST(GhostCells1D, DefaultHasNoGhostCells) {
    const GhostCells1D ghost_cells;

    EXPECT_EQ(ghost_cells.left(), static_cast<Size>(0));
    EXPECT_EQ(ghost_cells.right(), static_cast<Size>(0));
    EXPECT_EQ(ghost_cells.total(), static_cast<Size>(0));
    EXPECT_TRUE(ghost_cells.empty());
    EXPECT_EQ(ghost_cells.storage_size(5), static_cast<Size>(5));
    EXPECT_EQ(ghost_cells.storage_index(3), static_cast<Size>(3));
}

TEST(GhostCells1D, AcceptsUpToTwoGhostCellsPerSide) {
    const GhostCells1D ghost_cells{2, 1};

    EXPECT_EQ(GhostCells1D::max_per_side(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.left(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.right(), static_cast<Size>(1));
    EXPECT_EQ(ghost_cells.total(), static_cast<Size>(3));
    EXPECT_FALSE(ghost_cells.empty());
    EXPECT_EQ(ghost_cells.first_physical_index(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.storage_size(10), static_cast<Size>(13));
    EXPECT_EQ(ghost_cells.storage_index(0), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.storage_index(9), static_cast<Size>(11));
}

TEST(GhostCells1D, RejectsMoreThanTwoGhostCellsPerSide) {
    try {
        const GhostCells1D ghost_cells{3, 0};
        (void)ghost_cells;
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"GhostCells1D"}
        );
        return;
    }

    FAIL() << "GhostCells1D did not reject more than two ghost cells.";
}

}  // namespace fvm
