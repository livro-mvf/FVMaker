// ----------------------------------------------------------------------------
// File: tst_TridiagonalSystem1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests the 1D tridiagonal linear system container.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(DenseVector, StoresContiguousValues) {
    DenseVector vector{std::vector<Real>{1.0, 2.0, 3.0}};

    EXPECT_EQ(vector.size(), static_cast<Size>(3));
    EXPECT_FALSE(vector.empty());
    EXPECT_DOUBLE_EQ(vector[0], 1.0);
    EXPECT_DOUBLE_EQ(vector[1], 2.0);
    EXPECT_DOUBLE_EQ(vector[2], 3.0);
    EXPECT_EQ(vector.values().data(), &vector[0]);
}

TEST(TridiagonalSystem1D, StoresClassIdentity) {
    EXPECT_EQ(
        TridiagonalSystem1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        TridiagonalSystem1D::id().class_name(),
        std::string_view{"TridiagonalSystem1D"}
    );
    EXPECT_EQ(
        TridiagonalSystem1D::id().class_id(),
        std::string_view{"fvm.1d.system.TridiagonalSystem1D"}
    );
}

TEST(TridiagonalSystem1D, BuildsZeroSystemWithRequestedSize) {
    TridiagonalSystem1D system{4};

    EXPECT_EQ(system.size(), static_cast<Size>(4));
    EXPECT_EQ(system.lower().size(), static_cast<Size>(3));
    EXPECT_EQ(system.diagonal().size(), static_cast<Size>(4));
    EXPECT_EQ(system.upper().size(), static_cast<Size>(3));
    EXPECT_EQ(system.rhs().size(), static_cast<Size>(4));
}

TEST(TridiagonalSystem1D, BuildsFromCoefficientVectors) {
    const TridiagonalSystem1D system{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };

    EXPECT_EQ(system.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(system.lower()[0], -1.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], 2.0);
    EXPECT_DOUBLE_EQ(system.upper()[1], -1.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 1.0);
}

TEST(TridiagonalSystem1D, SetRowWritesInteriorAndBoundaryRows) {
    TridiagonalSystem1D system{3};

    system.set_row(0, 99.0, 2.0, -1.0, 1.0);
    system.set_row(1, -3.0, 4.0, -5.0, 6.0);
    system.set_row(2, -7.0, 8.0, 99.0, 9.0);

    EXPECT_DOUBLE_EQ(system.diagonal()[0], 2.0);
    EXPECT_DOUBLE_EQ(system.upper()[0], -1.0);
    EXPECT_DOUBLE_EQ(system.rhs()[0], 1.0);

    EXPECT_DOUBLE_EQ(system.lower()[0], -3.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], 4.0);
    EXPECT_DOUBLE_EQ(system.upper()[1], -5.0);
    EXPECT_DOUBLE_EQ(system.rhs()[1], 6.0);

    EXPECT_DOUBLE_EQ(system.lower()[1], -7.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[2], 8.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 9.0);
}

TEST(TridiagonalSystem1D, RejectsZeroSize) {
    EXPECT_THROW(
        static_cast<void>(TridiagonalSystem1D{0}),
        FVMException
    );
}

TEST(TridiagonalSystem1D, RejectsIncompatibleCoefficientSizes) {
    try {
        const TridiagonalSystem1D system{
            std::vector<Real>{1.0},
            std::vector<Real>{2.0, 2.0, 2.0},
            std::vector<Real>{1.0, 1.0},
            DenseVector{3}
        };
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSystemSize.code);
        EXPECT_EQ(exception.record().category, std::string_view{"System"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"TridiagonalSystem1D"}
        );
        return;
    }

    FAIL() << "TridiagonalSystem1D did not reject incompatible sizes.";
}

TEST(TridiagonalSystem1D, RejectsRowOutOfRange) {
    TridiagonalSystem1D system{2};

    EXPECT_THROW(
        system.set_row(2, 0.0, 1.0, 0.0, 1.0),
        FVMException
    );
}

}  // namespace fvm
