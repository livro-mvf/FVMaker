// ----------------------------------------------------------------------------
// File: Advection1D.cc
// Project: FVMaker
// Version: 0.1.0
// Description: Implements a provisional 1D advection helper.
// Author: FVMaker Team
// License: GPL-3.0-or-later
// ----------------------------------------------------------------------------

#include <utility>

#include <FVMaker/OneDimensional/Advection/Advection1D.h>

namespace fvm {

Advection1D::Advection1D(FaceFlux1D face_flux)
    : face_flux_(std::move(face_flux)) {}

const FaceFlux1D& Advection1D::face_flux() const noexcept {
    return face_flux_;
}

}  // namespace fvm
