// ----------------------------------------------------------------------------
// File: tst_Field1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests scalar fields stored on one-dimensional control volumes.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Field/Field1D.h>
#include <FVMaker/OneDimensional/Field/FieldView1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{2.0},
        fvgrid::XInit{0.0}
    );

    return GridView1D{axis};
}

}  // namespace

TEST(Field1D, StoresClassIdentity) {
    EXPECT_EQ(Field1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Field1D::id().class_name(), std::string_view{"Field1D"});
    EXPECT_EQ(Field1D::id().class_id(), std::string_view{"fvm.1d.field.Field1D"});

    EXPECT_EQ(FieldView1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(FieldView1D::id().class_name(), std::string_view{"FieldView1D"});
    EXPECT_EQ(
        FieldView1D::id().class_id(),
        std::string_view{"fvm.1d.field.FieldView1D"}
    );
}

TEST(Field1D, BuildsZeroFieldAssociatedWithFVGridMakerMesh) {
    const GridView1D grid = make_grid();
    const Field1D field{grid, "phi"};

    EXPECT_EQ(field.name(), std::string_view{"phi"});
    EXPECT_EQ(field.size(), grid.num_volumes());
    EXPECT_FALSE(field.empty());
    EXPECT_EQ(field.grid().num_volumes(), grid.num_volumes());

    for (Real value : field.values()) {
        EXPECT_DOUBLE_EQ(value, 0.0);
    }
}

TEST(Field1D, InitializesUniformValues) {
    const Field1D field{make_grid(), "temperature", 3.5};

    for (Real value : field.values()) {
        EXPECT_DOUBLE_EQ(value, 3.5);
    }
}

TEST(Field1D, InitializesFromVector) {
    const Field1D field{
        make_grid(),
        "phi",
        std::vector<Real>{1.0, 2.0, 3.0, 4.0}
    };

    ASSERT_EQ(field.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(field.values()[0], 1.0);
    EXPECT_DOUBLE_EQ(field.values()[1], 2.0);
    EXPECT_DOUBLE_EQ(field.values()[2], 3.0);
    EXPECT_DOUBLE_EQ(field.values()[3], 4.0);
}

TEST(Field1D, InitializesFromFunctionOfCellCenter) {
    const Field1D field = Field1D::from_function(
        make_grid(),
        "phi",
        [](Real x) {
            return x * x;
        }
    );

    ASSERT_EQ(field.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(field.values()[0], 0.25 * 0.25);
    EXPECT_DOUBLE_EQ(field.values()[1], 0.75 * 0.75);
    EXPECT_DOUBLE_EQ(field.values()[2], 1.25 * 1.25);
    EXPECT_DOUBLE_EQ(field.values()[3], 1.75 * 1.75);
}

TEST(Field1D, AssignsUniformFunctionAndVectorValues) {
    Field1D field{make_grid(), "phi", 1.0};

    field.fill(2.0);
    EXPECT_DOUBLE_EQ(field[0], 2.0);
    EXPECT_DOUBLE_EQ(field[3], 2.0);

    field.assign_function([](Real x) {
        return x + 1.0;
    });
    EXPECT_DOUBLE_EQ(field[0], 1.25);
    EXPECT_DOUBLE_EQ(field[3], 2.75);

    field.assign_values({4.0, 3.0, 2.0, 1.0});
    EXPECT_DOUBLE_EQ(field[0], 4.0);
    EXPECT_DOUBLE_EQ(field[3], 1.0);
}

TEST(Field1D, ViewMutatesUnderlyingValues) {
    Field1D field{make_grid(), "phi", 1.0};
    FieldView1D view = field.view();

    EXPECT_EQ(view.name(), std::string_view{"phi"});
    EXPECT_EQ(view.size(), field.size());
    EXPECT_EQ(view.grid().num_volumes(), field.grid().num_volumes());

    view[2] = 9.0;

    EXPECT_DOUBLE_EQ(field[2], 9.0);
    EXPECT_EQ(view.values().data(), field.values().data());
}

TEST(Field1D, KeepsHistoryWhenRequested) {
    Field1D field{make_grid(), "phi", 1.0, true};

    ASSERT_TRUE(field.history_enabled());
    ASSERT_EQ(field.history_size(), static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(field.history_step(0)[0], 1.0);

    field[0] = 5.0;
    field.save_state();

    ASSERT_EQ(field.history_size(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(field.history_step(0)[0], 1.0);
    EXPECT_DOUBLE_EQ(field.history_step(1)[0], 5.0);

    field.clear_history();
    EXPECT_EQ(field.history_size(), static_cast<Size>(0));
}

TEST(Field1D, DoesNotKeepHistoryByDefault) {
    Field1D field{make_grid(), "phi", 1.0};

    EXPECT_FALSE(field.history_enabled());
    EXPECT_EQ(field.history_size(), static_cast<Size>(0));

    field.save_state();

    EXPECT_EQ(field.history_size(), static_cast<Size>(0));
}

TEST(Field1D, RejectsVectorWithWrongSize) {
    try {
        const Field1D field{
            make_grid(),
            "phi",
            std::vector<Real>{1.0, 2.0, 3.0}
        };
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidFieldSize.code);
        EXPECT_EQ(exception.record().category, std::string_view{"Field"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Field1D"});
        return;
    }

    FAIL() << "Field1D did not reject a value vector with the wrong size.";
}

TEST(Field1D, RejectsHistoryIndexOutOfRange) {
    const Field1D field{make_grid(), "phi", 1.0, true};

    EXPECT_THROW(
        static_cast<void>(field.history_step(1)),
        FVMException
    );
}

}  // namespace fvm
