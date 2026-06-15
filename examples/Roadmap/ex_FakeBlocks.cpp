// ----------------------------------------------------------------------------
// File: ex_FakeBlocks.cpp
// Project: FVMaker
// Version: 0.1.0
// Description: Calls provisional roadmap placeholders for blocks 15 to 29.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <iostream>

#include <FVMaker/Roadmap/FakeBlocks.h>

int main() {
    std::cout << "Provisional roadmap calls\n";
    std::cout << "=========================\n";

    for (const fvm::roadmap::FakeBlockResult block :
         fvm::roadmap::remaining_blocks()) {
        std::cout << "Block " << block.block << ": " << block.name
                  << " callable=" << std::boolalpha << block.callable
                  << " provisional=" << block.provisional << '\n';
    }

    return fvm::roadmap::all_remaining_blocks_callable() ? 0 : 1;
}
