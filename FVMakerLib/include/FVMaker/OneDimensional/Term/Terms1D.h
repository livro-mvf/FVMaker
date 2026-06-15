// ----------------------------------------------------------------------------
// File: Terms1D.h
// Project: FVMaker
// Description: Defines additive 1D finite-volume equation terms.
// ----------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <string_view>

#include <FVMaker/Core/ID.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/OneDimensional/System/EquationContribution1D.h>

namespace fvm {

class DiffusionTerm1D final {
public:
    explicit DiffusionTerm1D(DiffusionCoefficient1D coefficient);
    explicit DiffusionTerm1D(Real coefficient);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "DiffusionTerm1D", "fvm.1d.term.DiffusionTerm1D"};
    }

    [[nodiscard]] const DiffusionCoefficient1D& coefficient() const noexcept;

private:
    DiffusionCoefficient1D coefficient_;
};

class AdvectionTerm1D final {
public:
    explicit AdvectionTerm1D(FaceFlux1D face_flux);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "AdvectionTerm1D", "fvm.1d.term.AdvectionTerm1D"};
    }

    [[nodiscard]] const FaceFlux1D& face_flux() const noexcept;

private:
    FaceFlux1D face_flux_;
};

class SourceTerm1D final {
public:
    explicit SourceTerm1D(LinearizedSource1D source);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "SourceTerm1D", "fvm.1d.term.SourceTerm1D"};
    }

    [[nodiscard]] const LinearizedSource1D& source() const noexcept;

private:
    LinearizedSource1D source_;
};

class BoundaryTerm1D final {
public:
    BoundaryTerm1D(BoundarySet1D boundaries, DiffusionCoefficient1D coefficient);
    BoundaryTerm1D(BoundarySet1D boundaries, Real coefficient = Real{1.0});

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"OneDimensional", "BoundaryTerm1D", "fvm.1d.term.BoundaryTerm1D"};
    }

    [[nodiscard]] const BoundarySet1D& boundaries() const noexcept;
    [[nodiscard]] const DiffusionCoefficient1D& coefficient() const noexcept;

private:
    BoundarySet1D boundaries_;
    DiffusionCoefficient1D coefficient_;
};

[[nodiscard]] EquationContribution1D assemble(
    const DiffusionTerm1D& term,
    const GridView1D& grid
);

[[nodiscard]] EquationContribution1D assemble(
    const AdvectionTerm1D& term,
    const GridView1D& grid
);

[[nodiscard]] EquationContribution1D assemble(
    const SourceTerm1D& term,
    const GridView1D& grid
);

[[nodiscard]] EquationContribution1D assemble(
    const BoundaryTerm1D& term,
    const GridView1D& grid
);

[[nodiscard]] TridiagonalSystem1D assemble_equation(
    const EquationContribution1D& contribution
);

}  // namespace fvm
