// ============================================================================
// Arquivo: DiffusionOperator1D.h
// Projeto: FVMaker
// Descricao: Operadores diferenciais difusivos 1D aplicados a campos discretos.
// ============================================================================

#pragma once

#include <cmath>
#include <concepts>
#include <functional>
#include <string_view>
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>

namespace fvm {

struct DiffusionOperator1D final {
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "DiffusionOperator1D",
            "fvm.1d.operator.DiffusionOperator1D"
        };
    }
};

template <class Function>
concept NonlinearCoefficientFunction1D =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, Real>;

[[nodiscard]] DenseVector interpolate_phi_to_faces_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionCoefficient1D& coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionTerm1D& term,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    Real coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] DenseVector second_derivative_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

[[nodiscard]] DenseVector laplacian_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

template <ScalarFunction1D Function>
[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    Function&& coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
) {
    return diffusion_operator_1d(
        grid,
        phi,
        function_coefficient_1d(grid, std::forward<Function>(coefficient)),
        boundaries,
        time
    );
}

template <NonlinearCoefficientFunction1D Function>
[[nodiscard]] DiffusionCoefficient1D nonlinear_function_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Function&& coefficient,
    Real time = Real{}
) {
    const DenseVector face_phi = interpolate_phi_to_faces_1d(
        grid,
        phi,
        boundaries,
        time
    );

    DenseVector face_values{grid.num_faces()};

    for (Size face = 0; face < grid.num_faces(); ++face) {
        face_values[face] = static_cast<Real>(std::invoke(
            coefficient,
            grid.faces()[face],
            face_phi[face]
        ));

        require(
            std::isfinite(face_values[face]),
            error_catalog::kInvalidCoefficient,
            DiffusionOperator1D::id()
        );
    }

    return face_coefficient_1d(grid, std::move(face_values));
}

template <NonlinearCoefficientFunction1D Function>
[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    Function&& coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
) {
    return diffusion_operator_1d(
        grid,
        phi,
        nonlinear_function_coefficient_1d(
            grid,
            phi,
            boundaries,
            std::forward<Function>(coefficient),
            time
        ),
        boundaries,
        time
    );
}

}  // namespace fvm