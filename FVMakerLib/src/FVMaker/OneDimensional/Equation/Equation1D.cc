// ----------------------------------------------------------------------------
// File: Equation1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a minimal steady 1D equation container.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Equation/Equation1D.h>

namespace fvm {

Equation1D::Equation1D(
    GridView1D grid,
    Laplacian1D laplacian,
    LinearizedSource1D source,
    BoundarySet1D boundaries
)
    : grid_(grid),
      laplacian_(laplacian),
      source_(std::move(source)),
      boundaries_(std::move(boundaries)) {
    validate();
}

const GridView1D& Equation1D::grid() const noexcept {
    return grid_;
}

const Laplacian1D& Equation1D::laplacian() const noexcept {
    return laplacian_;
}

const LinearizedSource1D& Equation1D::source() const noexcept {
    return source_;
}

const BoundarySet1D& Equation1D::boundaries() const noexcept {
    return boundaries_;
}

void Equation1D::validate() const {
    require(
        source_.size() == grid_.num_volumes(),
        error_catalog::kIncompatibleTerms,
        Equation1D::id()
    );
}

}  // namespace fvm
