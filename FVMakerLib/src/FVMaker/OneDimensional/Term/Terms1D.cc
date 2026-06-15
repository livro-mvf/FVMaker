// ----------------------------------------------------------------------------
// File: Terms1D.cc
// Project: FVMaker
// Description: Implements additive 1D finite-volume equation terms.
// ----------------------------------------------------------------------------

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>

namespace fvm {

namespace {

[[nodiscard]] Real west_boundary_distance(const GridView1D& grid) {
    return grid.centers()[0] - grid.faces()[0];
}

[[nodiscard]] Real east_boundary_distance(const GridView1D& grid) {
    return grid.faces()[grid.num_faces() - 1]
        - grid.centers()[grid.num_volumes() - 1];
}

}  // namespace

DiffusionTerm1D::DiffusionTerm1D(DiffusionCoefficient1D coefficient)
    : coefficient_(std::move(coefficient)) {}

DiffusionTerm1D::DiffusionTerm1D(Real coefficient)
    : coefficient_(DiffusionCoefficient1D{DenseVector{1, coefficient}}) {}

const DiffusionCoefficient1D& DiffusionTerm1D::coefficient() const noexcept {
    return coefficient_;
}

AdvectionTerm1D::AdvectionTerm1D(FaceFlux1D face_flux)
    : face_flux_(std::move(face_flux)) {}

const FaceFlux1D& AdvectionTerm1D::face_flux() const noexcept {
    return face_flux_;
}

SourceTerm1D::SourceTerm1D(LinearizedSource1D source)
    : source_(std::move(source)) {}

const LinearizedSource1D& SourceTerm1D::source() const noexcept {
    return source_;
}

BoundaryTerm1D::BoundaryTerm1D(
    BoundarySet1D boundaries,
    DiffusionCoefficient1D coefficient
)
    : boundaries_(std::move(boundaries)),
      coefficient_(std::move(coefficient)) {}

BoundaryTerm1D::BoundaryTerm1D(BoundarySet1D boundaries, Real coefficient)
    : boundaries_(std::move(boundaries)),
      coefficient_(DiffusionCoefficient1D{DenseVector{1, coefficient}}) {}

const BoundarySet1D& BoundaryTerm1D::boundaries() const noexcept {
    return boundaries_;
}

const DiffusionCoefficient1D& BoundaryTerm1D::coefficient() const noexcept {
    return coefficient_;
}

EquationContribution1D assemble(
    const DiffusionTerm1D& term,
    const GridView1D& grid
) {
    EquationContribution1D contribution{grid.num_volumes()};

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        const Real gamma = term.coefficient().num_faces() == 1
            ? term.coefficient().face_value(0)
            : term.coefficient().face_value(face);

        const Real conductance = gamma / grid.dx_centers()[face];
        const Size west = face - 1;
        const Size east = face;

        contribution.ae()[west] += conductance;
        contribution.ap()[west] += conductance;

        contribution.aw()[east] += conductance;
        contribution.ap()[east] += conductance;
    }

    return contribution;
}

EquationContribution1D assemble(
    const AdvectionTerm1D& term,
    const GridView1D& grid
) {
    require(
        term.face_flux().size() == grid.num_faces(),
        error_catalog::kInvalidArgument,
        AdvectionTerm1D::id()
    );

    EquationContribution1D contribution{grid.num_volumes()};

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        const Real flux = term.face_flux().value(face);
        const Size west = face - 1;
        const Size east = face;

        if (flux >= Real{}) {
            contribution.ap()[west] += flux;
            contribution.aw()[east] += flux;
        } else {
            const Real magnitude = -flux;
            contribution.ae()[west] += magnitude;
            contribution.ap()[east] += magnitude;
        }
    }

    return contribution;
}

EquationContribution1D assemble(
    const SourceTerm1D& term,
    const GridView1D& grid
) {
    require(
        term.source().size() == grid.num_volumes(),
        error_catalog::kInvalidSource,
        SourceTerm1D::id()
    );

    EquationContribution1D contribution{grid.num_volumes()};

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        const Real volume = grid.cell_lengths()[i];

        contribution.ap()[i] -= term.source().source_linear()[i] * volume;
        contribution.bp()[i] += term.source().source_constant()[i] * volume;
    }

    return contribution;
}

EquationContribution1D assemble(
    const BoundaryTerm1D& term,
    const GridView1D& grid
) {
    EquationContribution1D contribution{grid.num_volumes()};

    const Real west_gamma = term.coefficient().num_faces() == 1
        ? term.coefficient().face_value(0)
        : term.coefficient().face_value(0);
    const Real east_gamma = term.coefficient().num_faces() == 1
        ? term.coefficient().face_value(0)
        : term.coefficient().face_value(grid.num_faces() - 1);

    const Real dw = west_boundary_distance(grid);
    const Real left_alpha = term.boundaries().left().alpha(grid.xmin());
    const Real left_beta = term.boundaries().left().beta(grid.xmin());
    const Real left_gamma = term.boundaries().left().gamma(grid.xmin());
    const Real left_denominator = left_alpha * dw - left_beta;

    require(
        std::abs(left_denominator) > Real{},
        error_catalog::kInvalidBoundaryCondition,
        BoundaryTerm1D::id()
    );

    contribution.ap()[0] += west_gamma * left_alpha / left_denominator;
    contribution.bp()[0] += west_gamma * left_gamma / left_denominator;

    const Size last = grid.num_volumes() - 1;
    const Real de = east_boundary_distance(grid);
    const Real right_alpha = term.boundaries().right().alpha(grid.xmax());
    const Real right_beta = term.boundaries().right().beta(grid.xmax());
    const Real right_gamma = term.boundaries().right().gamma(grid.xmax());
    const Real right_denominator = right_alpha * de + right_beta;

    require(
        std::abs(right_denominator) > Real{},
        error_catalog::kInvalidBoundaryCondition,
        BoundaryTerm1D::id()
    );

    contribution.ap()[last] += east_gamma * right_alpha / right_denominator;
    contribution.bp()[last] += east_gamma * right_gamma / right_denominator;

    return contribution;
}

TridiagonalSystem1D assemble_equation(
    const EquationContribution1D& contribution
) {
    return to_tridiagonal_system(contribution);
}

}  // namespace fvm
