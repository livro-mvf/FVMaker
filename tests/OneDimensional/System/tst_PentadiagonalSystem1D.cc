// ----------------------------------------------------------------------------
// File: tst_PentadiagonalSystem1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the 1D pentadiagonal linear system container.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/System/PentadiagonalSystem1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(PentadiagonalSystem1D, StoresClassIdentity) {
    EXPECT_EQ(
        PentadiagonalSystem1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        PentadiagonalSystem1D::id().class_name(),
        std::string_view{"PentadiagonalSystem1D"}
    );
    EXPECT_EQ(
        PentadiagonalSystem1D::id().class_id(),
        std::string_view{"fvm.1d.system.PentadiagonalSystem1D"}
    );
}

TEST(PentadiagonalSystem1D, BuildsZeroSystemWithRequestedSize) {
    PentadiagonalSystem1D system{5};

    EXPECT_EQ(system.size(), static_cast<Size>(5));
    EXPECT_EQ(system.lower_second().size(), static_cast<Size>(3));
    EXPECT_EQ(system.lower_first().size(), static_cast<Size>(4));
    EXPECT_EQ(system.diagonal().size(), static_cast<Size>(5));
    EXPECT_EQ(system.upper_first().size(), static_cast<Size>(4));
    EXPECT_EQ(system.upper_second().size(), static_cast<Size>(3));
    EXPECT_EQ(system.rhs().size(), static_cast<Size>(5));
}

TEST(PentadiagonalSystem1D, BuildsSmallSystemsWithEmptyOuterBands) {
    PentadiagonalSystem1D system{1};

    EXPECT_EQ(system.size(), static_cast<Size>(1));
    EXPECT_TRUE(system.lower_second().empty());
    EXPECT_TRUE(system.lower_first().empty());
    EXPECT_EQ(system.diagonal().size(), static_cast<Size>(1));
    EXPECT_TRUE(system.upper_first().empty());
    EXPECT_TRUE(system.upper_second().empty());
}

TEST(PentadiagonalSystem1D, BuildsFromCoefficientVectors) {
    const PentadiagonalSystem1D system{
        std::vector<Real>{-0.5, -0.5},
        std::vector<Real>{-1.0, -1.0, -1.0},
        std::vector<Real>{4.0, 4.0, 4.0, 4.0},
        std::vector<Real>{-1.0, -1.0, -1.0},
        std::vector<Real>{-0.5, -0.5},
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0, 4.0}}
    };

    EXPECT_EQ(system.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(system.lower_second()[1], -0.5);
    EXPECT_DOUBLE_EQ(system.lower_first()[2], -1.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[3], 4.0);
    EXPECT_DOUBLE_EQ(system.upper_first()[0], -1.0);
    EXPECT_DOUBLE_EQ(system.upper_second()[1], -0.5);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 3.0);
}

TEST(PentadiagonalSystem1D, SetRowWritesInteriorAndBoundaryRows) {
    PentadiagonalSystem1D system{5};

    system.set_row(0, 99.0, 98.0, 1.0, 2.0, 3.0, 4.0);
    system.set_row(2, -1.0, -2.0, 5.0, -3.0, -4.0, 6.0);
    system.set_row(4, 7.0, 8.0, 9.0, 97.0, 96.0, 10.0);

    EXPECT_DOUBLE_EQ(system.diagonal()[0], 1.0);
    EXPECT_DOUBLE_EQ(system.upper_first()[0], 2.0);
    EXPECT_DOUBLE_EQ(system.upper_second()[0], 3.0);
    EXPECT_DOUBLE_EQ(system.rhs()[0], 4.0);

    EXPECT_DOUBLE_EQ(system.lower_second()[0], -1.0);
    EXPECT_DOUBLE_EQ(system.lower_first()[1], -2.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[2], 5.0);
    EXPECT_DOUBLE_EQ(system.upper_first()[2], -3.0);
    EXPECT_DOUBLE_EQ(system.upper_second()[2], -4.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 6.0);

    EXPECT_DOUBLE_EQ(system.lower_second()[2], 7.0);
    EXPECT_DOUBLE_EQ(system.lower_first()[3], 8.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[4], 9.0);
    EXPECT_DOUBLE_EQ(system.rhs()[4], 10.0);
}

TEST(PentadiagonalSystem1D, RejectsZeroSize) {
    EXPECT_THROW(static_cast<void>(PentadiagonalSystem1D{0}), FVMException);
}

TEST(PentadiagonalSystem1D, RejectsIncompatibleCoefficientSizes) {
    try {
        const PentadiagonalSystem1D system{
            std::vector<Real>{1.0},
            std::vector<Real>{1.0, 1.0, 1.0},
            std::vector<Real>{2.0, 2.0, 2.0},
            std::vector<Real>{1.0, 1.0},
            std::vector<Real>{1.0},
            DenseVector{3}
        };
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSystemSize.code);
        EXPECT_EQ(exception.record().category, std::string_view{"System"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"PentadiagonalSystem1D"}
        );
        return;
    }

    FAIL() << "PentadiagonalSystem1D did not reject incompatible sizes.";
}

TEST(PentadiagonalSystem1D, RejectsRowOutOfRange) {
    PentadiagonalSystem1D system{2};

    EXPECT_THROW(
        system.set_row(2, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0),
        FVMException
    );
}

}  // namespace fvm
