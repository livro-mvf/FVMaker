// ----------------------------------------------------------------------------
// File: tst_Functions2D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests two-dimensional function concepts.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <functional>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Functions/FunctionTypes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] Real free_scalar_2d(Real x, Real y) {
    return x + y;
}

[[nodiscard]] VectorValue2D free_vector_2d(Real x, Real y) {
    return VectorValue2D{x, y};
}

struct ScalarFunctor2D final {
    [[nodiscard]] Real operator()(Real x, Real y) const {
        return x * y;
    }
};

struct VectorFunctor2D final {
    [[nodiscard]] VectorValue2D operator()(Real x, Real y) const {
        return VectorValue2D{x + 1.0, y + 2.0};
    }
};

}  // namespace

TEST(Functions2D, AcceptsScalarFunctions) {
    static_assert(ScalarFunction2D<decltype(free_scalar_2d)>);
    static_assert(ScalarFunction2D<ScalarFunctor2D>);
    static_assert(ScalarFunction2D<std::function<Real(Real, Real)>>);

    const auto lambda = [](Real x, Real y) {
        return x - y;
    };

    static_assert(ScalarFunction2D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_scalar_2d(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0), -1.0);
    EXPECT_DOUBLE_EQ(ScalarFunctor2D{}(2.0, 3.0), 6.0);
}

TEST(Functions2D, AcceptsVectorFunctions) {
    static_assert(VectorFunction2D<decltype(free_vector_2d)>);
    static_assert(VectorFunction2D<VectorFunctor2D>);
    static_assert(VectorFunction2D<std::function<VectorValue2D(Real, Real)>>);

    const auto lambda = [](Real x, Real y) {
        return VectorValue2D{x - y, x + y};
    };

    static_assert(VectorFunction2D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_vector_2d(2.0, 3.0)[0], 2.0);
    EXPECT_DOUBLE_EQ(free_vector_2d(2.0, 3.0)[1], 3.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0)[0], -1.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0)[1], 5.0);
    EXPECT_DOUBLE_EQ(VectorFunctor2D{}(2.0, 3.0)[0], 3.0);
    EXPECT_DOUBLE_EQ(VectorFunctor2D{}(2.0, 3.0)[1], 5.0);
}

}  // namespace fvm
