#include <FVMaker.h>

#include <gtest/gtest.h>

TEST(FVMakerMinimal, DefaultConstructible) {
    const fvm::FVMaker fvmaker;
    (void)fvmaker;

    SUCCEED();
}
