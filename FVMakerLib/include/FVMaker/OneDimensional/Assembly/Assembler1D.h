// ----------------------------------------------------------------------------
// File: Assembler1D.h
// Project: FVMaker
// Version: 0.1.0
// Description: Defines assembly functions for minimal steady 1D equations.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#pragma once

#include <FVMaker/OneDimensional/Equation/Equation1D.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

namespace fvm {

[[nodiscard]] TridiagonalSystem1D assemble_laplacian_1d(
    const GridView1D& grid,
    const Laplacian1D& laplacian,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] TridiagonalSystem1D assemble_steady_1d(
    const Equation1D& equation,
    Real time = Real{}
);

}  // namespace fvm
