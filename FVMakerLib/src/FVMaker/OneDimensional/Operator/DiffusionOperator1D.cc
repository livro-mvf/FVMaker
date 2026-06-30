// ============================================================================
// Arquivo: DiffusionOperator1D.cc
// Projeto: FVMaker
// Descricao: Implementa operadores diferenciais difusivos 1D.
// ============================================================================

#include <cmath>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>
#include <FVMaker/OneDimensional/Operator/DiffusionOperator1D.h>

namespace fvm {

namespace {

void validate_phi_size(const GridView1D& grid, const DenseVector& phi) {
    require(
        phi.size() == grid.num_volumes(),
        error_catalog::kInvalidFieldSize,
        DiffusionOperator1D::id()
    );

    for (const Real value : phi.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidFieldSize,
            DiffusionOperator1D::id()
        );
    }
}

void validate_coefficient_size(
    const GridView1D& grid,
    const DiffusionCoefficient1D& coefficient
) {
    require(
        coefficient.num_faces() == Size{1}
            || coefficient.num_faces() == grid.num_faces(),
        error_catalog::kInvalidCoefficient,
        DiffusionOperator1D::id()
    );
}

[[nodiscard]] Real coefficient_at_face(
    const DiffusionCoefficient1D& coefficient,
    Size face
) noexcept {
    return coefficient.num_faces() == Size{1}
        ? coefficient.face_value(Size{0})
        : coefficient.face_value(face);
}

[[nodiscard]] Real west_center_distance(const GridView1D& grid, Size cell) {
    return cell == Size{0}
        ? Real{2.0} * grid.dx_centers()[0]
        : grid.dx_centers()[cell];
}

[[nodiscard]] Real east_center_distance(const GridView1D& grid, Size cell) {
    return cell + Size{1} == grid.num_volumes()
        ? Real{2.0} * grid.dx_centers()[grid.num_volumes()]
        : grid.dx_centers()[cell + Size{1}];
}

[[nodiscard]] Real west_value(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time,
    Size cell
) {
    if (cell > Size{0}) {
        return phi[cell - Size{1}];
    }

    return first_ghost_value(
        boundaries,
        BoundarySide1D::left,
        grid.xmin(),
        phi[cell],
        west_center_distance(grid, cell),
        time
    );
}

[[nodiscard]] Real east_value(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time,
    Size cell
) {
    if (cell + Size{1} < grid.num_volumes()) {
        return phi[cell + Size{1}];
    }

    return first_ghost_value(
        boundaries,
        BoundarySide1D::right,
        grid.xmax(),
        phi[cell],
        east_center_distance(grid, cell),
        time
    );
}

}  // namespace

DenseVector interpolate_phi_to_faces_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time
) {
    validate_phi_size(grid, phi);

    DenseVector face_values{grid.num_faces()};

    const Real left_ghost = first_ghost_value(
        boundaries,
        BoundarySide1D::left,
        grid.xmin(),
        phi[0],
        west_center_distance(grid, Size{0}),
        time
    );
    face_values[0] = Real{0.5} * (left_ghost + phi[0]);

    for (Size face = 1; face + Size{1} < grid.num_faces(); ++face) {
        const Size west = face - Size{1};
        const Size east = face;
        const Real west_distance = grid.faces()[face] - grid.centers()[west];
        const Real east_distance = grid.centers()[east] - grid.faces()[face];
        const Real distance = west_distance + east_distance;

        require(
            distance > Real{},
            error_catalog::kIncompatibleMesh,
            DiffusionOperator1D::id()
        );

        face_values[face] =
            (east_distance * phi[west] + west_distance * phi[east]) / distance;
    }

    const Size last_cell = grid.num_volumes() - Size{1};
    const Size last_face = grid.num_faces() - Size{1};
    const Real right_ghost = first_ghost_value(
        boundaries,
        BoundarySide1D::right,
        grid.xmax(),
        phi[last_cell],
        east_center_distance(grid, last_cell),
        time
    );
    face_values[last_face] = Real{0.5} * (phi[last_cell] + right_ghost);

    return face_values;
}

DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionCoefficient1D& coefficient,
    const BoundarySet1D& boundaries,
    Real time
) {
    validate_phi_size(grid, phi);
    validate_coefficient_size(grid, coefficient);

    DenseVector result{grid.num_volumes()};

    for (Size cell = 0; cell < grid.num_volumes(); ++cell) {
        const Real volume = grid.cell_lengths()[cell];
        const Real west_distance = west_center_distance(grid, cell);
        const Real east_distance = east_center_distance(grid, cell);
        const Real phi_w = west_value(grid, phi, boundaries, time, cell);
        const Real phi_e = east_value(grid, phi, boundaries, time, cell);
        const Real gamma_w = coefficient_at_face(coefficient, cell);
        const Real gamma_e = coefficient_at_face(coefficient, cell + Size{1});

        result[cell] = (
            gamma_w * (phi_w - phi[cell]) / west_distance
          + gamma_e * (phi_e - phi[cell]) / east_distance
        ) / volume;

        require(
            std::isfinite(result[cell]),
            error_catalog::kAssemblyFailed,
            DiffusionOperator1D::id()
        );
    }

    return result;
}

DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionTerm1D& term,
    const BoundarySet1D& boundaries,
    Real time
) {
    return diffusion_operator_1d(
        grid,
        phi,
        term.coefficient(),
        boundaries,
        time
    );
}

DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    Real coefficient,
    const BoundarySet1D& boundaries,
    Real time
) {
    return diffusion_operator_1d(
        grid,
        phi,
        DiffusionCoefficient1D{DenseVector{Size{1}, coefficient}},
        boundaries,
        time
    );
}

DenseVector second_derivative_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time
) {
    return diffusion_operator_1d(grid, phi, Real{1.0}, boundaries, time);
}

DenseVector laplacian_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time
) {
    return second_derivative_1d(grid, phi, boundaries, time);
}

}  // namespace fvm