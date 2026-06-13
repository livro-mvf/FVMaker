// ----------------------------------------------------------------------------
// File: NonlinearSolve1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements preliminary nonlinear 1D solve helpers.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Solver/NonlinearSolve1D.h>

namespace fvm {

PicardProblem1D::PicardProblem1D(
    GridView1D grid,
    NonlinearCoefficient1D coefficient,
    LinearizedSource1D source,
    BoundarySet1D boundaries
)
    : grid_(grid),
      coefficient_(std::move(coefficient)),
      source_(std::move(source)),
      boundaries_(std::move(boundaries)) {
    validate();
}

const GridView1D& PicardProblem1D::grid() const noexcept {
    return grid_;
}

const NonlinearCoefficient1D& PicardProblem1D::coefficient() const noexcept {
    return coefficient_;
}

const LinearizedSource1D& PicardProblem1D::source() const noexcept {
    return source_;
}

const BoundarySet1D& PicardProblem1D::boundaries() const noexcept {
    return boundaries_;
}

void PicardProblem1D::validate() const {
    require(
        source_.size() == grid_.num_volumes(),
        error_catalog::kIncompatibleTerms,
        PicardProblem1D::id()
    );
}

}  // namespace fvm
