// ----------------------------------------------------------------------------
// File: tst_FakeBlocks.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Tests provisional callable placeholders for roadmap blocks.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <string_view>

#include <FVMaker/Roadmap/FakeBlocks.h>

#include <gtest/gtest.h>

namespace fvm::roadmap {

TEST(FakeBlocks, ExposesAllRemainingBlocks) {
    constexpr auto blocks = remaining_blocks();

    static_assert(blocks.size() == 15);
    EXPECT_EQ(blocks.front().block, 15);
    EXPECT_EQ(blocks.back().block, 29);
    EXPECT_EQ(blocks.front().name, std::string_view{"Transient1D/Ddt1D"});
    EXPECT_EQ(blocks.back().name, std::string_view{"PerformanceProfiling"});
}

TEST(FakeBlocks, AllRemainingBlocksAreCallableAndProvisional) {
    EXPECT_TRUE(all_remaining_blocks_callable());

    for (const FakeBlockResult block : remaining_blocks()) {
        EXPECT_TRUE(block.callable);
        EXPECT_TRUE(block.provisional);
    }
}

TEST(FakeBlocks, CallsRepresentativeRoutines) {
    EXPECT_EQ(time_schemes_1d().block, 16);
    EXPECT_EQ(grid_view_2d().block, 18);
    EXPECT_EQ(advection_2d().block, 26);
    EXPECT_EQ(yaml_examples().block, 28);
}

}  // namespace fvm::roadmap
