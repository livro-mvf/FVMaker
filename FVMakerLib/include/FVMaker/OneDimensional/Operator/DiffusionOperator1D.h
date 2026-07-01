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

// Monta o operador de difusao para uma malha e um coeficiente 1D.
struct DiffusionOperator1D final {
    // Retorna o identificador estavel desta classe na biblioteca.
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

// Realiza a operacao interpolate phi to faces 1d definida por esta interface.
[[nodiscard]] DenseVector interpolate_phi_to_faces_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao diffusion operator 1d definida por esta interface.
[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionCoefficient1D& coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao diffusion operator 1d definida por esta interface.
[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const DiffusionTerm1D& term,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao diffusion operator 1d definida por esta interface.
[[nodiscard]] DenseVector diffusion_operator_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    Real coefficient,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao second derivative 1d definida por esta interface.
[[nodiscard]] DenseVector second_derivative_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao laplacian 1d definida por esta interface.
[[nodiscard]] DenseVector laplacian_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Real time = Real{}
);

// Realiza a operacao diffusion operator 1d definida por esta interface.
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

// Realiza a operacao nonlinear function coefficient 1d definida por esta interface.
template <NonlinearCoefficientFunction1D Function>
[[nodiscard]] DiffusionCoefficient1D nonlinear_function_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& phi,
    const BoundarySet1D& boundaries,
    Function&& coefficient,
    Real time = Real{}
) {
    // Realiza a operacao interpolate phi to faces 1d definida por esta interface.
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

        // Realiza a operacao require definida por esta interface.
        require(
            // Informa se a condicao isfinite e verdadeira.
            std::isfinite(face_values[face]),
            error_catalog::kInvalidCoefficient,
            DiffusionOperator1D::id()
        );
    }

    return face_coefficient_1d(grid, std::move(face_values));
}

// Realiza a operacao diffusion operator 1d definida por esta interface.
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