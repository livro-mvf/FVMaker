// ----------------------------------------------------------------------------
// File: tst_ErrorNorms.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests algebraic error norms for dense vectors.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorNorms, EmptyVectorHasZeroNorms) {
    const DenseVector vector;

    EXPECT_DOUBLE_EQ(norm_infinity(vector), 0.0);
    EXPECT_DOUBLE_EQ(norm_l1(vector), 0.0);
    EXPECT_DOUBLE_EQ(norm_l2(vector), 0.0);
}

TEST(ErrorNorms, ComputesInfinityNorm) {
    const DenseVector vector{std::vector<Real>{-2.0, 3.5, -1.0}};

    EXPECT_DOUBLE_EQ(norm_infinity(vector), 3.5);
    EXPECT_DOUBLE_EQ(VectorNorms::infinity(vector), 3.5);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::infinity), 3.5);
}

TEST(ErrorNorms, ComputesL1Norm) {
    const DenseVector vector{std::vector<Real>{-2.0, 3.5, -1.0}};

    EXPECT_DOUBLE_EQ(norm_l1(vector), 6.5);
    EXPECT_DOUBLE_EQ(VectorNorms::l1(vector), 6.5);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::l1), 6.5);
}

TEST(ErrorNorms, ComputesL2Norm) {
    const DenseVector vector{std::vector<Real>{3.0, 4.0}};

    EXPECT_DOUBLE_EQ(norm_l2(vector), 5.0);
    EXPECT_DOUBLE_EQ(VectorNorms::l2(vector), 5.0);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::l2), 5.0);
}

}  // namespace fvm
