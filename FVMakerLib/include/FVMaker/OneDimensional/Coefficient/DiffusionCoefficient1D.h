// ============================================================================
// Arquivo: DiffusionCoefficient1D.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Diffusion Coefficient 1 D no contexto de OneDimensional / Coefficient.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#pragma once

#include <concepts>
#include <span>
#include <string_view>
#include <utility>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Functions/FunctionTypes.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {
struct FaceInterpolationData1D final {
    Real west_value{};
    Real east_value{};
    Real west_distance{};
    Real east_distance{};
};

struct ArithmeticFaceInterpolation1D final {
    [[nodiscard]] Real operator()(FaceInterpolationData1D data) const noexcept {
        const Real distance = data.west_distance + data.east_distance;
        return (data.east_distance * data.west_value
              + data.west_distance * data.east_value) / distance;
    }
};

struct HarmonicFaceInterpolation1D final {
    [[nodiscard]] Real operator()(FaceInterpolationData1D data) const noexcept {
        const Real distance = data.west_distance + data.east_distance;
        return distance / (data.west_distance / data.west_value
                         + data.east_distance / data.east_value);
    }
};

template <class Interpolator>
concept FaceInterpolator1D = requires(
    const Interpolator& interpolation,
    FaceInterpolationData1D data
) {
    { interpolation(data) } -> std::convertible_to<Real>;
};

class DiffusionCoefficient1D final {
public:
    explicit DiffusionCoefficient1D(DenseVector face_values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{
            "OneDimensional",
            "DiffusionCoefficient1D",
            "fvm.1d.coefficient.DiffusionCoefficient1D"
        };
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size num_faces() const noexcept;
    [[nodiscard]] std::span<const Real> face_values() const noexcept;
    [[nodiscard]] Real face_value(Size face) const noexcept;

private:
    DenseVector face_values_;

    void validate() const;
};

[[nodiscard]] DiffusionCoefficient1D uniform_coefficient_1d(
    const GridView1D& grid,
    Real coefficient
);

template <ScalarFunction1D Function>
[[nodiscard]] DiffusionCoefficient1D function_coefficient_1d(
    const GridView1D& grid,
    Function function
) {
    DenseVector face_values{grid.num_faces()};

    for (Size face = 0; face < grid.num_faces(); ++face) {
        face_values[face] = function(grid.faces()[face]);
    }

    return DiffusionCoefficient1D{std::move(face_values)};
}

[[nodiscard]] DiffusionCoefficient1D face_coefficient_1d(
    const GridView1D& grid,
    DenseVector face_values
);

template <FaceInterpolator1D Interpolator>
[[nodiscard]] DiffusionCoefficient1D interpolated_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values,
    Interpolator interpolation
) {
    require(
        cell_values.size() == grid.num_volumes(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    DenseVector face_values{grid.num_faces()};
    face_values[0] = cell_values[0];
    face_values[grid.num_faces() - 1] = cell_values[grid.num_volumes() - 1];

    for (Size face = 1; face + 1 < grid.num_faces(); ++face) {
        const Size west = face - 1;
        const Size east = face;
        const FaceInterpolationData1D data{
            .west_value = cell_values[west],
            .east_value = cell_values[east],
            .west_distance = grid.faces()[face] - grid.centers()[west],
            .east_distance = grid.centers()[east] - grid.faces()[face]
        };
        face_values[face] = static_cast<Real>(interpolation(data));
    }

    return DiffusionCoefficient1D{std::move(face_values)};
}

[[nodiscard]] DiffusionCoefficient1D arithmetic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

[[nodiscard]] DiffusionCoefficient1D harmonic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

[[nodiscard]] DiffusionCoefficient1D field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
);

}  // namespace fvm
