// ----------------------------------------------------------------------------
// File: tst_DenseVector.cc
// Project: FVMaker
// Description: Tests dense vector algebra helpers.
// ----------------------------------------------------------------------------

#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(DenseVector, AddsAndSubtractsVectors) {
    const DenseVector a{std::vector<Real>{1.0, 2.0, 3.0}};
    const DenseVector b{std::vector<Real>{0.5, -1.0, 4.0}};

    const DenseVector sum = a + b;
    const DenseVector difference = a - b;

    ASSERT_EQ(sum.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(sum[0], 1.5);
    EXPECT_DOUBLE_EQ(sum[1], 1.0);
    EXPECT_DOUBLE_EQ(sum[2], 7.0);

    ASSERT_EQ(difference.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(difference[0], 0.5);
    EXPECT_DOUBLE_EQ(difference[1], 3.0);
    EXPECT_DOUBLE_EQ(difference[2], -1.0);
}

TEST(DenseVector, ScalesVectorOnBothSides) {
    const DenseVector values{std::vector<Real>{1.0, -2.0, 4.0}};

    const DenseVector left_scaled = 2.0 * values;
    const DenseVector right_scaled = values * -0.5;
    const DenseVector negated = -values;

    EXPECT_DOUBLE_EQ(left_scaled[0], 2.0);
    EXPECT_DOUBLE_EQ(left_scaled[1], -4.0);
    EXPECT_DOUBLE_EQ(left_scaled[2], 8.0);

    EXPECT_DOUBLE_EQ(right_scaled[0], -0.5);
    EXPECT_DOUBLE_EQ(right_scaled[1], 1.0);
    EXPECT_DOUBLE_EQ(right_scaled[2], -2.0);

    EXPECT_DOUBLE_EQ(negated[0], -1.0);
    EXPECT_DOUBLE_EQ(negated[1], 2.0);
    EXPECT_DOUBLE_EQ(negated[2], -4.0);
}

TEST(DenseVector, SupportsInPlaceOperations) {
    DenseVector values{std::vector<Real>{1.0, 2.0, 3.0}};
    const DenseVector increment{std::vector<Real>{4.0, 5.0, 6.0}};

    values += increment;
    values -= DenseVector{std::vector<Real>{1.0, 1.0, 1.0}};
    values *= 0.5;

    EXPECT_DOUBLE_EQ(values[0], 2.0);
    EXPECT_DOUBLE_EQ(values[1], 3.0);
    EXPECT_DOUBLE_EQ(values[2], 4.0);
}

TEST(DenseVector, RejectsOperationsWithDifferentSizes) {
    const DenseVector a{std::vector<Real>{1.0, 2.0}};
    const DenseVector b{std::vector<Real>{1.0, 2.0, 3.0}};

    try {
        static_cast<void>(a + b);
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSystemSize.code);
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"DenseVector"});
        return;
    }

    FAIL() << "DenseVector addition did not reject incompatible sizes.";
}

}  // namespace fvm
